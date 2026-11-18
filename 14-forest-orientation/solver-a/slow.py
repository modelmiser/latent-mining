#!/usr/bin/env python3
"""Slow, obvious reference: labelled positions, straight from the definition.
usage: slow.py NMAX CAP     (CAP = max number of edges a vertex may be tail of; problem has CAP=2)
State = tuple of arcs (tail, head) sorted. No symmetry reduction, no pruning:
Algorithm may pick ANY valid orientation of G+uv (all 2^(E+1) are tried)."""
import sys
from functools import lru_cache
from itertools import product

def solve(n, cap):
    sys.setrecursionlimit(10000)

    def comps(arcs):
        p = list(range(n))
        def f(x):
            while p[x] != x:
                p[x] = p[p[x]]
                x = p[x]
            return x
        for a, b in arcs:
            p[f(a)] = f(b)
        return [f(x) for x in range(n)]

    def valid(arcs):
        od = [0] * n
        for a, b in arcs:
            od[a] += 1
        return max(od) <= cap if od else True

    @lru_cache(maxsize=None)
    def V(arcs):
        if len(arcs) == n - 1:
            return 0
        c = comps(arcs)
        best = 0
        for u in range(n):
            for v in range(u + 1, n):
                if c[u] == c[v]:
                    continue
                edges = list(arcs) + [(u, v)]
                E = len(edges)
                val = None
                for flips in product((0, 1), repeat=E):
                    new = tuple(sorted((b, a) if f else (a, b) for (a, b), f in zip(edges, flips)))
                    if not valid(new):
                        continue
                    cost = sum(flips[:-1])   # the new edge is free in either direction
                    t = cost + V(new)
                    if val is None or t < val:
                        val = t
                assert val is not None
                if val > best:
                    best = val
        return best

    r = V(())
    return r, V.cache_info().currsize

if __name__ == "__main__":
    nmax = int(sys.argv[1]); cap = int(sys.argv[2])
    for n in range(1, nmax + 1):
        r, s = solve(n, cap)
        print(f"cap={cap} n={n} T={r} states={s}", flush=True)
