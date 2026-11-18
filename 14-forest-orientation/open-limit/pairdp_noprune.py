#!/usr/bin/env python3
"""Second implementation of the pair-type game, no pruning, different state encoding (Counter-as-tuple), for cross-check.
VARIANT 1 (deliberately false rules): Algorithm loses the two expensive options (via slot 2)."""
import sys
from functools import lru_cache
sys.setrecursionlimit(1000000)
VAR = int(sys.argv[3]) if len(sys.argv) > 3 else 0
def norm(d): return tuple(sorted((k, v) for k, v in d.items() if v))
@lru_cache(maxsize=None)
def V(st):
    d = dict(st)
    if sum(d.values()) <= 1: return 0
    keys = sorted(d)
    best = 0
    for i, t in enumerate(keys):
        for u in keys[i:]:
            if t == u and d[t] < 2: continue
            e = dict(d); e[t] -= 1; e[u] -= 1
            def succ(nt):
                f = dict(e); f[nt] = f.get(nt, 0) + 1; return V(norm(f))
            (p, q), (pp, qq) = t, u
            res = []
            res.append(p + 1 + max(succ(tuple(sorted((q, pp + 1)))), succ(u)))
            res.append(pp + 1 + max(succ(tuple(sorted((qq, p + 1)))), succ(t)))
            if VAR == 0:
                res.append(q + 1 + max(succ(tuple(sorted((p, pp + 1)))), succ(u)))
                res.append(qq + 1 + max(succ(tuple(sorted((pp, p + 1)))), succ(t)))
            best = max(best, min(res))
    return best
for n in range(int(sys.argv[1]), int(sys.argv[2]) + 1):
    print(n, V(norm({(-1, -1): n})), flush=True)
