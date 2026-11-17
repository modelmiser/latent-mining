#!/usr/bin/env python3
"""Slow direct implementation of the definition.  No normal form assumed.
Knowledge state = (stacks of arrival indices, #arrived, frozenset of arrival
orders consistent with every answer so far).  Sorter may attempt any move.
An attempt whose answer is the same for all consistent orders and changes
nothing is a no-op and is skipped (it can never help; state identical).
V = max number of consistent orders beaten."""
import sys
from itertools import permutations
from functools import lru_cache
sys.setrecursionlimit(100000)

def solve(k, n):
    perms = list(permutations(range(1, n + 1)))   # p[j] = value of j-th arrival
    @lru_cache(maxsize=None)
    def V(st, arrived, nout, S):
        # S: frozenset of indices into perms; nout outputs done (same for all in S)
        if nout == n:
            return len(S)
        best = 0
        if len(st[0]) < 2 and arrived < n:
            best = max(best, V((st[0] + (arrived,),) + st[1:], arrived + 1, nout, S))
        for i in range(k - 1):
            if st[i] and len(st[i+1]) < 2:
                l = list(st); l[i+1] = st[i+1] + (st[i][-1],); l[i] = st[i][:-1]
                best = max(best, V(tuple(l), arrived, nout, S))
        for i in range(k):
            if st[i]:
                j = st[i][-1]
                D = frozenset(x for x in S if perms[x][j] == nout + 1)
                if not D:
                    continue          # refused for all: nothing changes, no info
                R = S - D
                l = list(st); l[i] = st[i][:-1]
                v = V(tuple(l), arrived, nout + 1, D)
                if R:
                    v += V(st, arrived, nout, R)
                best = max(best, v)
        return best
    return V(tuple(() for _ in range(k)), 0, 0, frozenset(range(len(perms))))

if __name__ == "__main__":
    k = int(sys.argv[1]); n = int(sys.argv[2])
    print(k, n, solve(k, n), flush=True)
