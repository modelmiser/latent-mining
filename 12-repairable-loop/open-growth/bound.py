#!/usr/bin/env python3
# Theorem 2 lower bound on e = M(n) - 2^n, optimised over k (exact integer arithmetic).
from fractions import Fraction
import math
def lb(n, k):
    m = n - k
    badw = 2 * sum((k - d) * 2 ** (max(m, d) - 1) for d in range(2, k))
    num = 2 ** (n - 2) - 2 - badw - 2 * (k - 1) * (n - 1) - k * (k - 1) * (2 ** m + 1)
    den = k * (2 ** m + 1) + 2 * (k - 1)
    return math.ceil(Fraction(num, den))
print("n  best_k  e>=   (2^n/e)/n   simple bound (2^(n-3)-19n^2)/(19n)")
for n in range(7, 41):
    best = max(((lb(n, k), k) for k in range(3, n + 1)))
    e, k = best
    simple = math.ceil(Fraction(2 ** (n - 3) - 19 * n * n, 19 * n))
    assert lb(n, n - 4) >= simple or simple <= 0, (n, lb(n, n-4), simple)
    print(n, k, max(e, 0), (f"{2**n / e / n:.1f}" if e > 0 else "-"), max(simple, 0))
