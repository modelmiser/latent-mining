#!/usr/bin/env python3
# Slow, direct, exhaustive solver: ALL states (c, S), S any subset of V\{c}; no reachability pruning.
# usage: slow.py G h kmax [fast_cache_dump ...]     vertices 0..n-1, parent of v>0 is (v-1)//2
import sys
G=sys.argv[1]; h=int(sys.argv[2]); kmax=int(sys.argv[3])
n=2**(h+1)-1
adj=[[] for _ in range(n)]
for v in range(1,n):
    p=(v-1)//2; adj[v].append(p); adj[p].append(v)
def expand(S,c):          # robber step: stay or move, never onto the cop
    R=set()
    for v in range(n):
        if S>>v&1:
            R.add(v)
            for u in adj[v]: R.add(u)
    R.discard(c)
    return sum(1<<v for v in R)
EXP={}
def ex(S,c):
    key=(S,c)
    if key not in EXP: EXP[key]=expand(S,c)
    return EXP[key]
def branch(c,u):          # vertices on u's side of c
    seen={u}; st=[u]
    while st:
        x=st.pop()
        for y in adj[x]:
            if y!=c and y not in seen: seen.add(y); st.append(y)
    return sum(1<<v for v in seen)
BR={(c,u):branch(c,u) for c in range(n) for u in adj[c]}
moves=[[c]+adj[c] for c in range(n)]
full=(1<<n)-1
win=[]   # win[j] : dict (c,S)->bool for all S subset of V\{c}
for j in range(kmax+1):
    W={}
    # glimpse goals
    for c in range(n):
        for S in range(1<<n):
            if S>>c&1: continue
            g=False
            if j>0 and S:
                g=True
                if G=='E':
                    answers=[1<<v for v in range(n) if S>>v&1]
                else:
                    answers=[S&BR[(c,u)] for u in adj[c] if S&BR[(c,u)]]
                for A in answers:
                    ok=False
                    for c2 in moves[c]:
                        A1=A&~(1<<c2)
                        if A1==0 or win[j-1][(c2,ex(A1,c2))]: ok=True;break
                    if not ok: g=False;break
            W[(c,S)]= (S==0) or g
    changed=True; sweeps=0
    while changed:
        changed=False; sweeps+=1
        for (c,S),w in W.items():
            if w: continue
            for c2 in moves[c]:
                S1=S&~(1<<c2)
                if S1==0 or W[(c2,ex(S1,c2))]:
                    W[(c,S)]=True; changed=True; break
    win.append(W)
    res=[c for c in range(n) if W[(c,full&~(1<<c))]]
    print("SLOW %s h=%d k=%d : %s winning starts(0-indexed)=%s sweeps=%d"%(G,h,j,"WIN" if res else "LOSE",res,sweeps)); sys.stdout.flush()
# compare with fast cache dumps: lines "j c S(hex, bit v = heap index v) win"
for fn in sys.argv[4:]:
    cnt=bad=0
    for line in open(fn):
        j,c,S,w=line.split(); j=int(j); c=int(c)-1; S=int(S,16)>>1; w=int(w)
        cnt+=1
        if win[j][(c,S)]!=bool(w): bad+=1
    print("compare",fn,"states",cnt,"mismatches",bad)
