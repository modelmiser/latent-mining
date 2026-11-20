#!/usr/bin/env python3
"""Model sanity check: over ALL plays (both players cooperating / both adversarial) compute the
shortest and the longest play.  Expected: shortest = 2n-4 for n>=4 (classical gossip theorem; 1 and 3
for n=2,3), longest <= n(n-1)/2.  Direct implementation, raw states, no symmetry."""
import sys
from functools import lru_cache
def run(n):
    full=(1<<n)-1
    @lru_cache(maxsize=None)
    def mm(K):
        if all(k==full for k in K): return (0,0)
        lo,hi=10**9,-1
        for a in range(n):
            for b in range(a+1,n):
                if not ((K[b]>>a)&1 and (K[a]>>b)&1):
                    L=list(K);u=K[a]|K[b];L[a]=u;L[b]=u
                    x,y=mm(tuple(L)); lo=min(lo,x+1); hi=max(hi,y+1)
        return (lo,hi)
    return mm(tuple(1<<i for i in range(n)))
for n in range(2,int(sys.argv[1])+1):
    lo,hi=run(n); print("n=%d shortest play=%d (2n-4=%d) longest play=%d (n(n-1)/2=%d)"%(n,lo,2*n-4,hi,n*(n-1)//2),flush=True)
