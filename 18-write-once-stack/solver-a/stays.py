#!/usr/bin/env python3
"""Lists operation sequences in a witness where an operation that is NOT the t-th leaves the configuration unchanged."""
import sys, itertools
from check import load
n, t, Q, E = load(sys.argv[1])
found = 0
for L in range(1, t):  # op number L is not the last allowed one
    for seq in itertools.product(range(3), repeat=L):
        c = '0' * n; d = 0; ok = True; prev = c
        for op in seq:
            d += -1 if op == 2 else 1
            if d < 0: ok = False; break
            prev = c; c = E[c][op] if c in E else c
        if ok and prev == c:
            found += 1
            if found <= 3: print("stay: ops", [("PUSH0","PUSH1","POP")[o] for o in seq], "config", c, "(op %d of t=%d)" % (L, t))
print(sys.argv[1], "non-last operations that leave the configuration unchanged:", found)
