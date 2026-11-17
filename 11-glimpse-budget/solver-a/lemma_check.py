#!/usr/bin/env python3
# Empirical checks of the lemmas used in proofs.txt (they are proved there; this is a sanity net).
import sys
from functools import lru_cache
def setup(h):
    n=2**(h+1)-1
    nb={v:[u for u in (v//2,2*v,2*v+1) if 1<=u<=n] for v in range(1,n+1)}
    dep={v:v.bit_length()-1 for v in range(1,n+1)}
    return n,nb,dep
def expand(S,c,nb):
    R=set(S)
    for v in S: R.update(nb[v])
    R.discard(c); return frozenset(R)
def sub(v,n):
    out=[];st=[v]
    while st:
        x=st.pop(); out.append(x)
        for y in (2*x,2*x+1):
            if y<=n: st.append(y)
    return out
# (1) W_full: full start on T_h, blind: every vertex of height>=2 other than the cop is contaminated
for h in range(3,9):
    n,nb,dep=setup(h); V=frozenset(range(1,n+1))
    seen=set((c,V-{c}) for c in V); st=list(seen)
    while st:
        c,S=st.pop()
        for v in V:
            if v!=c and h-dep[v]>=2: assert v in S,("W_full fails",h,c,v)
        for c2 in [c]+nb[c]:
            S1=S-{c2}; assert S1
            t=(c2,expand(S1,c2,nb))
            if t not in seen: seen.add(t); st.append(t)
    print("W_full ok h=%d blind states %d"%(h,len(seen)))
# (2) W_seed: seed S0={v,v1,v2}, cop outside X=subtree(v), height(v)=m>=3 ; timed check of (F) and (W)
for h in range(3,8):
    n,nb,dep=setup(h); cnt=0
    for v in [1<<d for d in range(0,h-2)]:          # one v per depth (symmetry), height m=h-dep[v]>=3
        m=h-dep[v]; X=set(sub(v,n))
        starts=[c for c in range(1,n+1) if c not in X]
        if v==1: continue                            # no outside vertex for the cop
        level=set((c,frozenset([v,2*v,2*v+1])) for c in starts)
        seenall=set()
        for t in range(0,3*h+8):
            for c,S in level:
                for z in X:
                    dz=dep[z]-dep[v]
                    if dz==t+1: assert z in S,("F fails",h,v,c,z,t)
                    if dz<=t+1 and h-dep[z]>=2 and z!=c: assert z in S,("W fails",h,v,c,z,t)
            nxt=set()
            for c,S in level:
                for c2 in [c]+nb[c]:
                    S1=S-{c2}; assert S1
                    nxt.add((c2,expand(S1,c2,nb)))
            cnt+=len(level); level=nxt
    print("W_seed ok h=%d (timed states examined %d)"%(h,cnt))
# (3) base of Lambda+_0 for m=2: on T_3, X=subtree(2), cop in {1} u subtree(3), S={2,4,5}: blind cop never wins
n,nb,dep=setup(3)
seen=set((c,frozenset([2,4,5])) for c in [1]+sub(3,n)); st=list(seen)
while st:
    c,S=st.pop()
    for c2 in [c]+nb[c]:
        S1=S-{c2}; assert S1,"base fails"
        t=(c2,expand(S1,c2,nb))
        if t not in seen: seen.add(t); st.append(t)
print("base Lambda+_0 (m=2) ok: closed blind set of %d states, never empty"%len(seen))
# (4) spot check of Lambda+_j with an independent recursive E-solver: seeds (c, N_X[v]) lose with j glimpses when m>=2j+2
sys.setrecursionlimit(100000)
def make_solver(h):
    n,nb,dep=setup(h)
    memo={}
    def win(j,c,S):
        if not S: return True
        key=(j,c,S)
        if key in memo: return memo[key]
        seen={(c,S)}; q=[(c,S)]; res=False; i=0
        while i<len(q) and not res:
            cc,SS=q[i]; i+=1
            if j>0:
                ok=True
                for v in SS:
                    if not any(c2==v or win(j-1,c2,expand(frozenset([v]),c2,nb)) for c2 in [cc]+nb[cc]): ok=False;break
                if ok: res=True;break
            for c2 in [cc]+nb[cc]:
                S1=SS-{c2}
                if not S1: res=True;break
                t=(c2,expand(S1,c2,nb))
                if t not in seen: seen.add(t); q.append(t)
        if not res:
            for cc,SS in seen: memo[(j,cc,SS)]=False
        memo[key]=res
        return res
    return n,nb,dep,win
for h in range(3,7):
    n,nb,dep,win=make_solver(h)
    for v in [1<<d for d in range(1,h-1)]:
        m=h-dep[v]; X=set(sub(v,n))
        for j in range(0,3):
            exp_lose = (m>=2*j+2)
            res=[win(j,c,frozenset([v,2*v,2*v+1])) for c in range(1,n+1) if c not in X]
            if exp_lose: assert not any(res),("Lambda+ fails",h,v,j)
            print("  h=%d m=%d j=%d: cop wins from %d of %d seed states %s"%(h,m,j,sum(res),len(res),"(lemma says 0)" if exp_lose else ""))
    V=frozenset(range(1,n+1))
    print(" full-start check h=%d: least k with a winning start ="%h, next(k for k in range(0,6) if any(win(k,c,V-{c}) for c in V)))
