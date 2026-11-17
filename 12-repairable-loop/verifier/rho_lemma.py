#!/usr/bin/env python3
# Referee's run-count lemma: a universal loop of length 2^n+e has T + 2a runs with 0 <= a <= floor(e/2), T = 2^(n-1).
import itertools
from ref import covers_all
from collections import Counter
for n in (2, 3, 4):
    T = 2 ** (n - 1)
    for e in range(0, 5):
        L = 2 ** n + e; c = Counter()
        for v in itertools.product((0, 1), repeat=L):
            if v[0] == 0 and covers_all(v, n): c[sum(1 for i in range(L) if v[i] != v[i - 1])] += 1
        ok = all(T <= r <= T + 2 * (e // 2) for r in c)
        print(f"n={n} length=2^n+{e}: run counts of universal loops {dict(sorted(c.items()))} within [T, T+2*floor(e/2)] = {ok}")
