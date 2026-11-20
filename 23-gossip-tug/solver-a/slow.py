#!/usr/bin/env python3
"""Slow, direct implementation of the Delayer/Hastener gossip game.
State: tuple K of n bitmasks (bit j of K[i] set <=> agent i knows secret j).
No symmetry reduction at all: memo on the raw state."""
import sys
from functools import lru_cache
sys.setrecursionlimit(10000)

def solve(n):
    full = (1 << n) - 1
    def legal(K, a, b):
        return a != b and (not (K[b] >> a) & 1 or not (K[a] >> b) & 1)
    @lru_cache(maxsize=None)
    def V(K):
        if all(k == full for k in K):
            return 0
        best = -1
        for a in range(n):
            worst = None
            for b in range(n):
                if legal(K, a, b):
                    L = list(K); u = K[a] | K[b]; L[a] = u; L[b] = u
                    v = 1 + V(tuple(L))
                    if worst is None or v < worst:
                        worst = v
            if worst is not None and worst > best:
                best = worst
        assert best >= 0, "non-terminal position without active agent"
        return best
    start = tuple(1 << i for i in range(n))
    v = V(start)
    return v, V.cache_info().currsize

if __name__ == "__main__":
    N = int(sys.argv[1]) if len(sys.argv) > 1 else 5
    for n in range(2, N + 1):
        v, states = solve(n)
        print(f"n={n} A(n)={v} raw_states={states}", flush=True)
