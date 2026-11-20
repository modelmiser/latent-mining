# Direct implementation of the definition, no symmetry reduction, no pruning.
import sys
from functools import lru_cache
sys.setrecursionlimit(10000)
def solve(n):
    full=(1<<n)-1
    @lru_cache(maxsize=None)
    def val(K):
        if all(k==full for k in K): return 0
        best=-1
        for a in range(n):
            opts=[]
            for b in range(n):
                if b!=a and (not (K[b]>>a)&1 or not (K[a]>>b)&1):
                    u=K[a]|K[b]; L=list(K); L[a]=u; L[b]=u
                    opts.append(1+val(tuple(L)))
            if opts: best=max(best,min(opts))
        return best
    r=val(tuple(1<<i for i in range(n)))
    return r, val.cache_info().currsize
for n in range(2,int(sys.argv[1])+1):
    print(n,*solve(n),flush=True)
