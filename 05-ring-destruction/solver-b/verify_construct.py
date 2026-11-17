#!/usr/bin/env python3
"""For all 3<=n<=N, 1<=k<=Kmax: build the sawtooth strategy, verify it with check.py's simulator,
and compare with Theorem U's bound. Prints the max gap UB-LB over the range where Theorem L applies (k>=3,n>=max(5,k+1))."""
import sys
from bounds import LB,UB
from construct import strategy
N=int(sys.argv[1]); Kmax=int(sys.argv[2]); worst=0; cnt=0
for n in range(3,N+1):
    for k in range(1,Kmax+1):
        lb=LB(n,k)   # asserts simulator result
        if k>=3 and n>=max(5,k+1):
            assert strategy(n,k) is not None
            ub=UB(n,k); assert lb<=ub
            assert lb==n-(-(-n//k)) and ub<=lb+(1 if n%k else 0),(n,k,lb,ub)
            worst=max(worst,ub-lb); cnt+=1
print("verified",cnt,"instances with k>=3,n>=max(5,k+1); max (U bound - L bound) =",worst)
