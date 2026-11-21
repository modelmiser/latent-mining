#!/usr/bin/env python3
"""corollary.py -- evaluates Corollary U2 (PROOFS.md) exactly: the largest m with
m/5 + 26/5 + 2*H_{3m} < h, so that every list of length <= m loses and G(h) >= m+1."""
from fractions import Fraction as Fr
import sys
hs = [int(x) for x in sys.argv[1:]] or [6, 10, 20, 31, 32, 40, 59, 100, 300, 1000, 3000, 10000]
H = [Fr(0)]
def Hn(n):
    while len(H) <= n: H.append(H[-1] + Fr(1, len(H)))
    return H[n]
print("    h   U2: G(h) >=   2h-1   5h-176   5h-6 (upper)")
for h in hs:
    m = 0
    while Fr(m + 1, 5) + Fr(26, 5) + 2 * Hn(3 * (m + 1)) < h: m += 1
    print(f"{h:6d} {m+1:10d} {2*h-1:8d} {5*h-176:8d} {5*h-6:8d}")
