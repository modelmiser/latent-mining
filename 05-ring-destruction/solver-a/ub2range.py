#!/usr/bin/env python3
"""For all k<n<=N, 3<=k<=K: check that Theorem U kills t0 = n-ceil(n/k) (so F = n-ceil(n/k) with Theorem L),
and (sanity) that it does NOT kill t0-1 (it must not, since the gen.py strategy survives t0-1)."""
import sys
from ub2 import dead
N=int(sys.argv[1]); K=int(sys.argv[2]); bad=0; cnt=0
for k in range(3,K+1):
    for n in range(k+1,N+1):
        t0=n-(-(-n//k)); cnt+=1
        if not dead(n,k,t0) or (t0>=2 and dead(n,k,t0-1)): bad+=1; print('BAD',n,k)
print('checked',cnt,'pairs, bad',bad)
