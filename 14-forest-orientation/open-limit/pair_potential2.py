#!/usr/bin/env python3
"""Search/check Algorithm potentials g(p,q) = p + 1 + gamma(q-p) for the pair-type abstract game."""
import sys
from fractions import Fraction as Fr
K = int(sys.argv[1]); gam = [Fr(x) for x in sys.argv[2].split(',')]
def g(t):
    p, q = t; d = q - p
    return Fr(p + 1) + (gam[d] if d < len(gam) else gam[-1])
def srt(a, b): return (a, b) if a <= b else (b, a)
types = [(p, q) for p in range(-1, K + 1) for q in range(p, K + 1)]
bad = 0; tight = 0; tot = 0; worst = 0
for t in types:
    for u in types:
        (p, q), (pp, qq) = t, u
        opts = [(p + 1, srt(q, pp + 1), u), (q + 1, srt(p, pp + 1), u),
                (pp + 1, srt(qq, p + 1), t), (qq + 1, srt(pp, p + 1), t)]
        best = min(c + max(g(nt), g(keep)) for (c, nt, keep) in opts)
        tot += 1
        if best > g(t) + g(u):
            bad += 1; worst = max(worst, best - g(t) - g(u))
            if bad <= 6: print('VIOLATION', t, u, best, g(t) + g(u))
        elif best == g(t) + g(u): tight += 1
print('K', K, 'gamma', [str(x) for x in gam], 'pairs', tot, 'violations', bad, 'worst', worst, 'tight', tight)
