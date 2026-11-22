#!/usr/bin/env python3
"""Exact arithmetic behind PROOFS.md B4 (Fractions).  Usage: arith.py [cap5] [--nospecial]   (cap5 = 24 in the proof)
For j = 0,1,2,...: n = 7 + j, lam_j = largest multiple of 1/5 that is <= H_n,
e_j = least integer e with 5e >= 6j, except e_2 = 4 and e_4 = 6 (PROOFS.md B3), unless --nospecial.
Claim: 2*lam_j - e_j/5 <= cap5/5 for every j >= 0.
Checked term by term for j <= 2000 and, for j >= 10, also via the real bound
g(j) = 2 H_{7+j} - 6j/25, which is decreasing (g(j+1)-g(j) = 2/(8+j) - 6/25 < 0)."""
import sys
from fractions import Fraction as Fr
a = [x for x in sys.argv[1:] if not x.startswith('--')]
cap = Fr(int(a[0]) if a else 24, 5); special = '--nospecial' not in sys.argv
Hn = sum(Fr(1, i) for i in range(1, 8)); ok = True; worst = None
for j in range(0, 2001):
    n = 7 + j
    if j: Hn += Fr(1, n)
    lam = Fr((5 * Hn).__floor__(), 5)
    e = -((-6 * j) // 5)
    if special and j == 2: e = 4
    if special and j == 4: e = 6
    val = 2 * lam - Fr(e, 5)
    if j <= 10: print(f"j={j} n={n} H_n={float(Hn):.5f} lam={lam} e_j={e} 2lam-e/5={val} = {float(val)}")
    if worst is None or val > worst[0]: worst = (val, j)
    if val > cap: ok = False; print("FAILS at j =", j, "value", val)
    g = 2 * Hn - Fr(6 * j, 25)
    if j == 10:
        print("g(10) =", float(g))
        if g > cap: ok = False; print("tail bound g(10) exceeds the cap")
    if j >= 10 and Fr(2, 8 + j) - Fr(6, 25) >= 0: ok = False
print("maximum over j <= 2000:", worst[0], "at j =", worst[1], "; cap", cap)
print("PASS" if ok else "FAIL"); sys.exit(0 if ok else 1)
