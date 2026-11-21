#!/usr/bin/env python3
"""forestcount.py n -- number of sequences of k oriented offers on n LABELLED bins that form a forest,
= (labelled forests on n vertices with k edges) * k! * 2^k, by a recurrence on the tree containing the
lowest-numbered vertex (Cayley: s^(s-2) trees on s vertices).  Independent of check.cpp; compare with
`check count n 2 -`, column labelled=."""
import sys
from math import comb, factorial
from functools import lru_cache
n = int(sys.argv[1])
@lru_cache(None)
def f(v, c):            # forests on v labelled vertices with exactly c trees
    if v == 0: return 1 if c == 0 else 0
    if c == 0: return 0
    return sum(comb(v - 1, s - 1) * (s ** (s - 2) if s > 1 else 1) * f(v - s, c - 1) for s in range(1, v + 1))
for k in range(n): print(f"n={n} k={k} labelled={f(n, n - k) * factorial(k) * 2 ** k}")
