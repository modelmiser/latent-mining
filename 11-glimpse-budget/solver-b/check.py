#!/usr/bin/env python3
"""check.py FILE -- independent checker for witness files (written separately from gen.cpp).
Vertices 0..n-1, root 0, children of v are 2v+1, 2v+2. Sets are Python frozensets.

LOSING file  (header 'LOSING MODE h k', then lines 'c j Shex'):
  proves the cop cannot win MODE(h,k). Let L be the listed states (cop at c about to move,
  robber somewhere in S, j glimpses unspent). Checked:
   (a) every opening leads into L: for each start c, (c, V-c, k) in L, and if k>=1 some glimpse
       answer P at c has (c, P, k-1) in L;
   (b) for every state in L and every cop move c' (stay or step): S-{c'} is non-empty (no forced
       capture), and with S2 = N[S-{c'}]-{c'}: (c',S2,j) in L, and if j>=1 some answer P of a
       glimpse at c' has (c',P,j-1) in L.
  So whatever the cop does, some robber consistent with everything he has seen is still free.
STRATEGY file (header 'STRATEGY MODE h k'): a cop strategy; checked by following every glimpse answer
  and tracking the exact set of robber positions consistent with the history; every branch must
  end with that set empty, with at most k glimpses, and without revisiting (cycle => reject).
"""
import sys
sys.setrecursionlimit(100000)
def main(fn):
    lines=[l.split() for l in open(fn) if l.strip()]
    kind,mode,h,k=lines[0][0],lines[0][1],int(lines[0][2]),int(lines[0][3])
    n=2**(h+1)-1
    nb={v:[] for v in range(n)}
    for v in range(1,n):
        p=(v-1)//2; nb[v].append(p); nb[p].append(v)
    V=frozenset(range(n))
    def closed(S):
        R=set(S)
        for v in S: R.update(nb[v])
        return R
    def side(c,b):   # component of T-c containing neighbour b
        seen={b}; st=[b]
        while st:
            v=st.pop()
            for w in nb[v]:
                if w!=c and w not in seen: seen.add(w); st.append(w)
        return seen
    def answers(c,S):  # dict label -> part
        if mode=='E': return {v:frozenset([v]) for v in S}
        out={}
        for b in nb[c]:
            P=frozenset(S & side(c,b))
            if P: out[b]=P
        return out
    def after(c2,S):  # cop steps to c2; returns (S1,S2)
        S1=frozenset(S)-{c2}
        return S1, frozenset(closed(S1)-{c2})
    if kind=='LOSING':
        L=set()
        for t in lines[1:]:
            c,j,x=int(t[0]),int(t[1]),int(t[2],16)
            S=frozenset(v for v in range(n) if x>>v&1)
            assert S and c not in S and 0<=j<=k, "bad state"
            L.add((c,j,S))
        for c in range(n):
            S=V-{c}
            if (c,k,S) not in L: return "REJECT: opening without glimpse from %d not covered"%c
            if k>=1 and not any((c,k-1,P) in L for P in answers(c,S).values()):
                return "REJECT: opening with glimpse from %d not covered"%c
        for (c,j,S) in L:
            for c2 in nb[c]+[c]:
                S1,S2=after(c2,S)
                if not S1: return "REJECT: forced capture from %r"%((c,j,sorted(S)),)
                if (c2,j,S2) not in L: return "REJECT: not closed (no glimpse) at %r move %d"%((c,j,sorted(S)),c2)
                if j>=1 and not any((c2,j-1,P) in L for P in answers(c2,S2).values()):
                    return "REJECT: not closed (glimpse) at %r move %d"%((c,j,sorted(S)),c2)
        return "OK: cop cannot win %s(%d,%d); %d states closed"%(mode,h,k,len(L))
    else:
        node={}
        start=None
        for t in lines[1:]:
            if t[0]=='START': start=t
            else: node[int(t[1])]=t
        done={}; onstack=set()
        def follow(spec,c,S,j):   # spec: tokens after the move; (c,S) at start of a round; returns rounds needed
            if spec[0]=='NOGL': return run(int(spec[1]),c,S,j)
            assert spec[0]=='GL' and j>=1, "glimpse without budget"
            tab=dict((int(a.split(':')[0]),int(a.split(':')[1])) for a in spec[1:])
            worst=0
            for lab,P in answers(c,S).items():
                assert lab in tab, "answer %d not handled"%lab
                worst=max(worst,run(tab[lab],c,P,j-1))
            return worst
        def run(i,c,S,j):         # node i: cop at c about to move
            key=(i,c,S,j)
            if key in done: return done[key]
            assert key not in onstack, "cycle"
            onstack.add(key)
            t=node[i]; assert t[2]=='MOVE'; c2=int(t[3]); assert c2==c or c2 in nb[c], "illegal move"
            S1,S2=after(c2,S)
            if not S1: r=1
            else:
                assert t[4]!='WIN', "claims capture but robber may be free"
                r=1+follow(t[4:],c2,S2,j)
            onstack.discard(key); done[key]=r; return r
        try:
            c0=int(start[1]); N=follow(start[2:],c0,V-{c0},k)
        except AssertionError as e:
            return "REJECT: %s"%e
        return "OK: cop wins %s(%d,%d) from start %d within %d rounds"%(mode,h,k,c0,N)
for fn in sys.argv[1:]:
    print(fn, main(fn))
