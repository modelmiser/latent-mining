#!/usr/bin/env python3
"""Upper bound from the Diagonal-cost theorem (proofs.txt, Theorem U):
if D=2t-n+2 >= H+1 with H=floor((t+1)/2) and k*t*(2H-D+1) < 2*n*H then F(n,k) <= t.
UB(n,k) = least such t (also capped by trivial 2*floor(n/2))."""
import sys
def UB(n,k):
    for t in range(1,2*(n//2)+1):
        H=(t+1)//2; D=2*t-n+2
        if D>=H+1 and k*t*(2*H-D+1) < 2*n*H: return t
    return 2*(n//2)
if __name__=='__main__':
    N=int(sys.argv[1]); K=int(sys.argv[2])
    print('n\\k '+' '.join('%3d'%k for k in range(1,K+1)))
    for n in range(3,N+1):
        print('%3d '%n+' '.join('%3d'%UB(n,k) for k in range(1,K+1)), '  floor(n-n/k):',' '.join('%3d'%max(n//2,(n*(k-1))//k) for k in range(1,K+1)))
