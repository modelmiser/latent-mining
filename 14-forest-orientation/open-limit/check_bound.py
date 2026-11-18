#!/usr/bin/env python3
"""Compare the closed-form bound of Theorem 1 with the exact worst case of the canonical strategy in the
A/R abstract game (absdp 'canon').  The bound must never exceed the strategy value.
COEF argument: 7/3 is the proved coefficient; a smaller one (e.g. 1/2) is a false variant that must fail."""
import sys, math
from fractions import Fraction as Fr
from absdp import solve
N = int(sys.argv[1]); coef = Fr(sys.argv[2]) if len(sys.argv) > 2 else Fr(7, 3)
bad = 0; mingap = None
for n in range(1, N + 1):
    v = solve(n, 'canon')
    b = n / 3 - float(coef) * math.log2(n + 1) + 5 / 6
    if b > v + 1e-9:
        bad += 1
        if bad <= 3: print('BOUND EXCEEDS STRATEGY VALUE at n=%d: bound %.3f value %d' % (n, b, v))
    gap = v - b
    if mingap is None or gap < mingap[0]: mingap = (gap, n)
    if n in (16, 64, 256, 1024) or n == N: print('n=%d value=%d n/3=%.2f conj=(n-1)/3-log4(n)=%.2f bound=%.2f' % (n, v, n / 3, (n - 1) / 3 - math.log(n, 4), b))
print('coef', coef, 'n<=%d' % N, 'violations', bad, 'min gap', mingap)
