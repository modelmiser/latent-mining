#!/usr/bin/env python3
"""Direct, slow implementation of the definition.
Knowledge state = (physical config in arrival indices, #arrived, frozenset of arrival orders still consistent).
A perm p is a tuple: p[a] = value of the a-th arriving item (0-based a, values 1..n).
All consistent perms share the same number of outputs so far (outputs are observed).
value = max over attempts of sum over answers of value(next knowledge state).
Attempts whose legality is determined by counts are only tried if legal (an illegal one changes nothing and tells nothing).
OUT attempts are tried on every non-empty stack, split by the true answer; if an answer has no consistent perm it contributes 0;
an OUT that is refused for all consistent perms changes nothing and is skipped.
"""
import sys
from itertools import permutations
from functools import lru_cache
sys.setrecursionlimit(100000)

def solve(k, n):
    @lru_cache(maxsize=None)
    def val(st, arrived, m, perms):
        # m = smallest value not yet output
        if m == n + 1:
            return len(perms)
        best = 0
        if arrived < n and len(st[0]) < 2:
            s = list(st); s[0] = st[0] + (arrived,)
            best = max(best, val(tuple(s), arrived+1, m, perms))
        for i in range(k-1):
            if st[i] and len(st[i+1]) < 2:
                s = list(st); s[i+1] = st[i+1] + (st[i][-1],); s[i] = st[i][:-1]
                best = max(best, val(tuple(s), arrived, m, perms))
        for i in range(k):
            if st[i]:
                a = st[i][-1]
                yes = frozenset(p for p in perms if p[a] == m)
                no = perms - yes
                if not yes:
                    continue  # refused for sure, nothing changes
                s = list(st); s[i] = st[i][:-1]
                v = val(tuple(s), arrived, m+1, yes)
                if no:
                    v += val(st, arrived, m, no)
                best = max(best, v)
        return best
    return val(tuple(() for _ in range(k)), 0, 1, frozenset(permutations(range(1, n+1))))

if __name__ == '__main__':
    k = int(sys.argv[1]); nmax = int(sys.argv[2])
    for n in range(1, nmax+1):
        print(k, n, solve(k, n), flush=True)
