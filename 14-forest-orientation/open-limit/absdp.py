#!/usr/bin/env python3
"""Abstract type game (a rigorous LOWER bound on T(n) given the Type Lemma).
State: multiset of type indices (index 2k = A_k, 2k+1 = R_k).  Start: n trees of index 0.
Adversary picks two trees with indices i<=j and plays the (i,i) rule (the j tree is downgraded):
  i=2k   : result index 2k+1, cost k
  i=2k+1 : Algorithm chooses  (index 2k+2, cost k)  or  (index 2k+1, cost k+1)
Value V(n) = max over Adversary, min over Algorithm of total cost.   T(n) >= V(n).
mode 'full': Adversary may pick any pair.  mode 'canon': lowest equal pair, else two highest."""
import sys
from functools import lru_cache
sys.setrecursionlimit(100000)

def outcomes(i):
    k = i // 2
    if i % 2 == 0:
        return [(i + 1, k)]
    return [(i + 2 - 1, k), (i, k + 1)]   # (2k+2, k) or (2k+1, k+1)

def solve(n, mode):
    @lru_cache(maxsize=None)
    def V(state):
        if len(state) <= 1:
            return 0
        best = -1
        if mode == 'full':
            seen = set()
            pairs = []
            L = len(state)
            for a in range(L):
                for b in range(a + 1, L):
                    key = (state[a], state[b])
                    if key in seen: continue
                    seen.add(key); pairs.append((a, b))
        else:
            pairs = None
            for a in range(len(state) - 1):
                if state[a] == state[a + 1]:
                    pairs = [(a, a + 1)]; break
            if pairs is None:
                pairs = [(len(state) - 2, len(state) - 1)]
        for a, b in pairs:
            i = state[a]
            rest = state[:a] + state[a + 1:b] + state[b + 1:]
            worst = None
            for (ni, c) in outcomes(i):
                v = c + V(tuple(sorted(rest + (ni,))))
                if worst is None or v < worst: worst = v
            if worst > best: best = worst
        return best
    return V(tuple([0] * n))

if __name__ == '__main__':
    mode = sys.argv[1]; lo = int(sys.argv[2]); hi = int(sys.argv[3])
    for n in range(lo, hi + 1):
        v = solve(n, mode)
        B = max(0, n // 4 - 1)
        print(n, v, 'B=%d' % B, '>B' if v > B else '', flush=True)
