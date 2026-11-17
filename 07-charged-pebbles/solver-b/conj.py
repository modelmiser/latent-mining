#!/usr/bin/env python3
"""Test candidate invariants on the relaxed game (all states with V<=fmax).
J: a pebble at x with exactly j pebbles strictly below it has x <= 2^q - 2^(q-1-j) + V*2^(q-2)
Usage: conj.py q fmax N"""
import sys
from relaxed import solve
q, fmax, N = map(int, sys.argv[1:4])
V = solve(q, fmax, N)
bad = 0
tight = {}
mx = 0
for S, v in V.items():
    pos = [i for i in range(1, N + 1) if S >> i & 1]
    if pos:
        mx = max(mx, pos[-1])
    for j, x in enumerate(pos):
        b = 2 ** q - 2 ** (q - 1 - j) + v * 2 ** (q - 2) if q >= 2 else 1
        if x > b:
            bad += 1
            if bad < 10:
                print("J violated", v, pos, j, x, b)
        if x == b:
            tight[(v, j)] = tight.get((v, j), 0) + 1
print("q", q, "fmax", fmax, "states", len(V), "maxnode", mx, "N", N, "J violations", bad)
print("tight (V,j):count", sorted(tight.items()))
