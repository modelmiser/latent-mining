#!/usr/bin/env python3
"""Direct, obvious implementation of the definition. No pruning except memo on exact (K, D_prev).
usage: slow.py n k"""
import sys
from itertools import combinations
from functools import lru_cache
sys.setrecursionlimit(100000)
n=int(sys.argv[1]); k=int(sys.argv[2])
links=[(i,(i+1)%n) for i in range(n)]+[(i,(i-1)%n) for i in range(n)]  # (src,dst)
FULL=(1<<n)-1
subsets=[]
for s in range(0,min(k,2*n)+1):
    for c in combinations(range(2*n),s):
        m=0
        for x in c: m|=1<<x
        subsets.append(m)
def step(K,D):
    new=list(K)
    for idx,(src,dst) in enumerate(links):
        if not (D>>idx)&1:
            new[dst]|=K[src]
    return tuple(new)
@lru_cache(maxsize=None)
def V(K,prev):
    best=0
    for D in subsets:
        if D&prev: continue
        K2=step(K,D)
        if any(x==FULL for x in K2): v=1
        else: v=1+V(K2,D)
        if v>best: best=v
    return best
print(n,k,V(tuple(1<<i for i in range(n)),0))
