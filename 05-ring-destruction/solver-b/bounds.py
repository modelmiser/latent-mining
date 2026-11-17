#!/usr/bin/env python3
"""Tabulate: lower bound from the sawtooth construction (verified by check.py's simulator for every entry)
and upper bound from Theorem U (proofs.txt)."""
import sys
from construct import strategy
from check import check
def UB(n,k):
    # F-1 = Tmax ; T is 'possible' unless Theorem U excludes it. Also trivial bound F<=2*floor(n/2).
    best=0
    for T in range(1,2*(n//2)):
        h=(T+1)//2; m=2*T-n+2
        if m>2*h: continue                      # impossible outright (deficits <= h each)
        if m>=h+1 and k<n and 2*n*h>k*T*(2*h+1-m): continue
        best=T
    return best+1
def LB(n,k):
    r=strategy(n,k)
    if r is None: return n//2
    T,rounds=r
    lines=[str(t)+"".join(" %d>%d"%l for l in D) for t,D in enumerate(rounds,1)]
    res=check(n,k,lines)
    assert res==("OK",T),(n,k,res)
    return max(n//2,T+1)
if __name__=="__main__":
    N=int(sys.argv[1]) if len(sys.argv)>1 else 14
    for n in range(3,N+1):
        print(n," ".join("k=%d:[%d,%d]"%(k,LB(n,k),UB(n,k)) for k in range(1,9)))
