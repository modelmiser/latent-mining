# print principal variations using slow direct solver with canonical memo-free lru (n<=6) 
import sys
from functools import lru_cache
n=int(sys.argv[1]); full=(1<<n)-1
@lru_cache(maxsize=None)
def val(K):
    if all(k==full for k in K): return 0
    return max(m for m in (amin(K,a) for a in range(n)) if m is not None)
def kids(K,a):
    for b in range(n):
        if b!=a and (not (K[b]>>a)&1 or not (K[a]>>b)&1):
            u=K[a]|K[b]; L=list(K); L[a]=u; L[b]=u; yield b,tuple(L)
def amin(K,a):
    o=[1+val(L) for b,L in kids(K,a)]
    return min(o) if o else None
K=tuple(1<<i for i in range(n))
def show(K): return ' '.join(''.join(str(j+1) for j in range(n) if k>>j&1) for k in K)
while val(K):
    v=val(K)
    A=[a+1 for a in range(n) if amin(K,a)==v]
    a=A[0]-1
    B=[(b+1) for b,L in kids(K,a) if 1+val(L)==v]
    allb=[(b+1,1+val(L)) for b,L in kids(K,a)]
    b=B[0]-1
    print(f"val={v} state [{show(K)}] optimal a: {A}; for a={a+1} replies (b,val): {allb}")
    K=dict(kids(K,a))[b]
