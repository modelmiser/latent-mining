#!/usr/bin/env python3
"""Fast DP for N(k,n), using the normal form "probe every top after every change"
(Lemma 1 in proofs.txt).  State: stacks of flags (0 = U: could be current min,
1 = K: known not current min), bottom->top, plus u = number of unarrived items.
f(state) = max number of consistent completions beaten."""
import sys
from functools import lru_cache
sys.setrecursionlimit(1000000)

def make(k):
    @lru_cache(maxsize=None)
    def f(st, u):
        # probe an unknown top
        for i in range(k):
            s = st[i]
            if s and s[-1] == 0:
                # branch A: it is the min -> output, all flags reset to U
                a = tuple(tuple(0 for _ in (t[:-1] if j == i else t)) for j, t in enumerate(st))
                # branch B: it is not -> K
                b = st[:i] + (s[:-1] + (1,),) + st[i+1:]
                return f(a, u) + f(b, u)
        if u == 0 and all(len(s) == 0 for s in st):
            return 1
        best = 0
        if u > 0 and len(st[0]) < 2:
            best = max(best, f((st[0] + (0,),) + st[1:], u - 1))
        for i in range(k - 1):
            if st[i] and len(st[i+1]) < 2:
                n = list(st)
                n[i+1] = st[i+1] + (st[i][-1],)
                n[i] = st[i][:-1]
                best = max(best, f(tuple(n), u))
        return best
    return f

def N(k, n, cache={}):
    if k not in cache: cache[k] = make(k)
    return cache[k](tuple(() for _ in range(k)), n)

if __name__ == "__main__":
    kmax = int(sys.argv[1]); nmax = int(sys.argv[2])
    for k in range(1, kmax + 1):
        # fill cache bottom-up in u to limit recursion depth
        print(k, [N(k, n) for n in range(1, nmax + 1)], flush=True)
