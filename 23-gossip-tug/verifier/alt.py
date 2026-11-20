# Referee side checks, direct from the definition (no symmetry):
#  shortest / longest play over ALL plays; and the game value if Delayer may only name NON-full agents (a neighbouring reading).
import sys
from functools import lru_cache
sys.setrecursionlimit(10000)
def run(n):
    full=(1<<n)-1
    def legal(K,a,b): return a!=b and (not (K[b]>>a)&1 or not (K[a]>>b)&1)
    def call(K,a,b):
        L=list(K); L[a]=L[b]=K[a]|K[b]; return tuple(L)
    @lru_cache(maxsize=None)
    def sl(K):
        if all(k==full for k in K): return (0,0)
        r=[sl(call(K,a,b)) for a in range(n) for b in range(a+1,n) if legal(K,a,b)]
        return (1+min(x[0] for x in r), 1+max(x[1] for x in r))
    @lru_cache(maxsize=None)
    def val(K):
        if all(k==full for k in K): return 0
        best=None
        for a in range(n):
            if K[a]==full: continue
            w=[1+val(call(K,a,b)) for b in range(n) if legal(K,a,b)]
            if w and (best is None or min(w)>best): best=min(w)
        return best
    st=tuple(1<<i for i in range(n))
    print("n=%d shortest,longest play=%s (2n-4=%d, n(n-1)/2=%d); value if Delayer may name only non-full agents=%d"%(n,sl(st),2*n-4,n*(n-1)//2,val(st)),flush=True)
for n in range(2,int(sys.argv[1])+1): run(n)
