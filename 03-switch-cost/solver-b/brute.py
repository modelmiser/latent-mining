#!/usr/bin/env python3
"""Independent brute-force minimax for tiny cases. UNSIMPLIFIED machine (run / idle / switch
as three distinct actions, 'previous tick was a switch tick' flag), absolute time, value computed
directly as minimax of max lateness (no threshold trick), promise checked from the full release list.
usage: brute.py P T promise(S|F) [onlinecost]"""
import sys
from functools import lru_cache
P=int(sys.argv[1]); T=int(sys.argv[2]); PROM=sys.argv[3]; ONCOST=int(sys.argv[4]) if len(sys.argv)>4 else 1
INF=10**6

def actions(jobs, A, flag, t, cost):
    """jobs: tuple of (r,d,rem). A: index of most recently run job or -1. yields (newjobs,newA,newflag,ran_index)"""
    yield jobs, A, False, None          # idle: A unchanged, flag cleared
    yield jobs, A, True, None           # switch tick
    for i,(r,d,rem) in enumerate(jobs):
        if rem==0 or r>t: continue
        if cost and A>=0 and A!=i and jobs[A][2]>0 and not flag: continue
        nj=jobs[:i]+((r,d,rem-1),)+jobs[i+1:]
        yield nj, i, False, i

@lru_cache(maxsize=None)
def hfeas(jobs, A, flag, t, cost):
    if all(j[2]==0 for j in jobs): return True
    if any(j[2]>0 and t+j[2]>j[1] for j in jobs): return False
    if t>=T: return False
    for nj,nA,nf,_ in actions(jobs,A,flag,t,cost):
        if hfeas(nj,nA,nf,t+1,cost): return True
    return False

@lru_cache(maxsize=None)
def feasible(rel):   # rel: sorted tuple of (r,p,d)
    jobs=tuple((r,d,p) for (r,p,d) in rel)
    return hfeas(jobs,-1,False,0, 1 if PROM=='S' else 0)

def canon(jobs, A):
    # sort jobs, track A.  Only identity of A matters if it is unfinished.
    tagged=sorted((j, i==A) for i,j in enumerate(jobs))
    nj=tuple(j for j,_ in tagged); nA=-1
    for i,(j,a) in enumerate(tagged):
        if a: nA=i
    return nj,nA

@lru_cache(maxsize=None)
def solo(jobs, A, flag, t, limit):
    """min achievable max lateness with no more releases; jobs (r,d,rem) all released"""
    if all(j[2]==0 for j in jobs): return 0
    if t>limit: return INF
    best=INF
    for nj,nA,nf,ran in actions(jobs,A,flag,t,ONCOST):
        late=0
        if ran is not None and nj[ran][2]==0: late=max(0,t+1-nj[ran][1])
        if late>=best: continue
        v=max(late, solo(nj,nA,nf,t+1,limit))
        if v<best: best=v
    return best

def typelist(t):
    return [(t,p,d) for p in range(1,P+1) for d in range(t+p,T+1)]

def releases(rel, t):
    """all feasible multisets added at tick t (as full new sorted release tuples)"""
    types=typelist(t); out=[]
    def rec(cur, idx):
        out.append(cur)
        for k in range(idx,len(types)):
            n=tuple(sorted(cur+(types[k],)))
            if feasible(n): rec(n,k)
    rec(rel,0)
    return out

@lru_cache(maxsize=None)
def val(rel, jobs, A, flag, t):
    """adversary to move at tick t. rel = all released (r,p,d); jobs = online (r,d,rem) for unfinished + finished kept out"""
    if t==T:
        tot=sum(j[2] for j in jobs)
        return solo(jobs,A,flag,t,T+2*tot+2)
    best=-1
    for nrel in releases(rel,t):
        added=list(nrel)
        for x in rel: added.remove(x)
        jl=list(jobs); 
        # keep A identity through re-sorting
        jl2=jl+[(r,d,p) for (r,p,d) in added]
        nj,nA=canon(tuple(jl2),A)
        # scheduler minimises
        sb=INF
        for mj,mA,mf,ran in actions(nj,nA,flag,t,ONCOST):
            late=0
            if ran is not None and mj[ran][2]==0: late=max(0,t+1-mj[ran][1])
            # drop finished jobs unless it is A (finished A == no A)
            if mA>=0 and mj[mA][2]==0:
                keep=tuple(j for j in mj if j[2]>0); cj,cA=canon(keep,-1)
            else:
                marker=mA; keepidx=[i for i,j in enumerate(mj) if j[2]>0]
                keep=tuple(mj[i] for i in keepidx); a=keepidx.index(marker) if marker in keepidx else -1
                cj,cA=canon(keep,a)
            v=max(late, val(nrel,cj,cA,mf,t+1))
            if v<sb: sb=v
        if sb>best: best=sb
    return best
sys.setrecursionlimit(100000)
print(f"brute P={P} T={T} promise={PROM} onlinecost={ONCOST} value={val((),(),-1,False,0)}")
