#!/usr/bin/env python3
"""Sharpened upper bound (proofs.txt Theorem U).  dead(n,k,t)=True means: PROVED that no legal sequence
keeps all processes incomplete through round t.  UB2(n,k) = least dead t  (so F(n,k) <= UB2)."""
import sys
def ceil_div(a,b): return -(-a//b)
def dead(n,k,t):
    if 2*t >= 2*(n//2)*2: return True            # t >= 2*floor(n/2): trivial bound from the statement
    Dp = 2*t-n+1                                  # D' = D-1, D = 2t-n+2
    if t%2==0:
        H=t//2
        if Dp < H: return False                   # lemma not applicable
        s=Dp-H; w=2*H-Dp                          # w = n-t-1
        if w<=0: return True
        need = 2*(n + s*ceil_div(n,w))
        return need > k*t
    else:
        H1=(t-1)//2; H=H1+1
        if Dp < H: return False
        s=Dp-H1; w=2*H1-Dp                        # w = n-t-2
        best=None                                 # minimise the lower bound on |P| over 0<=B<=E<=k
        for E in range(0,k+1):
            for B in range(0,E+1):
                if w<=0:
                    if 2*n > E+B: continue        # infeasible
                    m=1
                else:
                    m=max(1,ceil_div(2*n-E-B,w))
                val=max(2*n-B+s*m, E)
                if best is None or val<best: best=val
        return best is None or best > k*t
def UB2(n,k):
    t=1
    while not dead(n,k,t): t+=1
    return t
def LB(n,k):
    m=min(k,n); return max(n//2, n-ceil_div(n,m))
if __name__=='__main__':
    N=int(sys.argv[1]); K=int(sys.argv[2])
    gaps=[]
    for n in range(3,N+1):
        for k in range(1,K+1):
            u=UB2(n,k); l=LB(n,k)
            if u!=l: gaps.append((n,k,l,u))
    print('pairs with LB != UB2 (n,k,LB,UB2), 3<=n<=%d, 1<=k<=%d: %d'%(N,K,len(gaps)))
    print([g for g in gaps if g[1]>=3 and g[1]<g[0]][:60])
    print('k<=2 gaps:',len([g for g in gaps if g[1]<=2]),' k>=n gaps:',[g for g in gaps if g[1]>=g[0]][:40])
