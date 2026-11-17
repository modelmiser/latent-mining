#!/usr/bin/env python3
"""Slow, obvious reference: F(n,k) straight from the definition.
Knowledge = bitmask sets. Links numbered: 2*i = (i -> i+1), 2*i+1 = (i -> i-1).
No pruning at all except memoisation on the exact state (K tuple, last D).
usage: slow.py n k
"""
import sys
from itertools import combinations
from functools import lru_cache

def main():
    n = int(sys.argv[1]); k = int(sys.argv[2])
    full = (1 << n) - 1
    links = list(range(2 * n))
    moves = []
    for s in range(0, k + 1):
        for c in combinations(links, s):
            m = 0
            for l in c:
                m |= 1 << l
            moves.append(m)

    def step(K, D):
        new = list(K)
        for i in range(n):
            # i sends to i+1 on link 2i, to i-1 on link 2i+1
            if not (D >> (2 * i)) & 1:
                new[(i + 1) % n] |= K[i]
            if not (D >> (2 * i + 1)) & 1:
                new[(i - 1) % n] |= K[i]
        return tuple(new)

    sys.setrecursionlimit(10000)

    @lru_cache(maxsize=None)
    def V(K, last):
        # max over legal continuations of number of further rounds until first completion
        best = 0
        for D in moves:
            if D & last:
                continue
            K2 = step(K, D)
            if any(x == full for x in K2):
                v = 1
            else:
                v = 1 + V(K2, D)
            if v > best:
                best = v
        return best

    K0 = tuple(1 << i for i in range(n))
    print(n, k, V(K0, 0))

main()
