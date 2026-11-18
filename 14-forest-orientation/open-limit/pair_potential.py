#!/usr/bin/env python3
"""Algorithm-side potential for the PAIR-TYPE abstract game:  g(p,q) = p + 2 - (2/3) 2^{-(q-p)}.
Check for all pairs t,t' with entries in [-1,K]: some option has
     cost + max(g(new), g(kept)) <= g(t) + g(t').
VARIANT 1 (false): g = F(idx) (the Adversary's A/R potential) ; 2 (false): g = p + 2 - (1/2) 2^{-(q-p)}."""
import sys
from fractions import Fraction as Fr
K = int(sys.argv[1]); VAR = int(sys.argv[2]) if len(sys.argv) > 2 else 0
def g(t):
    p, q = t
    if VAR == 1:
        return Fr(p + 1) + (Fr(1, 3) if q == p else Fr(2, 3))
    if VAR == 2:
        return Fr(p + 2) - Fr(1, 2) / 2 ** (q - p)
    return Fr(p + 2) - Fr(2, 3) / 2 ** (q - p)
def srt(a, b): return (a, b) if a <= b else (b, a)
types = [(p, q) for p in range(-1, K + 1) for q in range(p, K + 1)]
bad = 0; tight = 0; tot = 0
for t in types:
    for u in types:
        (p, q), (pp, qq) = t, u
        opts = [(p + 1, srt(q, pp + 1), u), (q + 1, srt(p, pp + 1), u),
                (pp + 1, srt(qq, p + 1), t), (qq + 1, srt(pp, p + 1), t)]
        best = min(c + max(g(nt), g(keep)) for (c, nt, keep) in opts)
        tot += 1
        if best > g(t) + g(u):
            bad += 1
            if bad <= 5: print('VIOLATION', t, u, float(best), float(g(t) + g(u)))
        elif best == g(t) + g(u): tight += 1
print('K', K, 'variant', VAR, 'pairs', tot, 'violations', bad, 'tight', tight)
