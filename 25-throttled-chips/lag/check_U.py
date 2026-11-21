#!/usr/bin/env python3
"""check_U.py -- brute-force test of Theorem U (PROOFS.md) and of fact (d) at threshold 3.
Plays the lag rule naively (full scan every round, exact integers, no heap) on a list and checks
after EVERY round, for EVERY k:   sum of the k largest lags <= k*(C + H_n - H_k),   C = 3.
Also checks: whenever the largest lag after an add is > 3, the fired vertex has that lag.
usage: check_U.py LISTFILE|random:SEED:LEN:WIDTH  TIE  [C]  [--rule=leftready]
  C (default 3) can be set to a wrong value (e.g. 0) to watch the check fail.
  --rule=leftready replaces the rule by "fire the leftmost ready vertex with lag >= 1" (a wrong
  rule, for which the theorem is not claimed) to watch the fact-(d) check fail.
"""
import sys, random
from fractions import Fraction as Fr
src, tie = sys.argv[1], sys.argv[2]
C = Fr(sys.argv[3]) if len(sys.argv) > 3 and not sys.argv[3].startswith('--') else Fr(3)
wrong = '--rule=leftready' in sys.argv
if src.startswith('random:'):
    _, seed, ln, wd = src.split(':'); rng = random.Random(int(seed))
    # biased random list: mostly near recent positions, so that lags pile up
    lst = []; wd = int(wd)
    for i in range(int(ln)):
        lst.append(5 + (rng.randrange(wd) if rng.random() < .7 or not lst else min(wd - 1, max(0, lst[-1] - 5 + rng.choice((-2, -1, 0, 1, 2))))))
else:
    lst = [int(x) for x in open(src)]
size = max(lst) + 4
k = [0] * size; F = [0] * size; eta = [0] * size
V = sorted({z for a in lst for z in (a - 1, a, a + 1)}); n = len(V)
Hn = [Fr(0)]
for i in range(1, n + 1): Hn.append(Hn[-1] + Fr(1, i))
bound = [None] + [kk * (C + Hn[n] - Hn[kk]) for kk in range(1, n + 1)]
def lev(x): return 3 * k[x] + k[x - 1] + k[x + 1] - 5 * F[x]
def keyf(x): return x if tie == 'L' else -x if tie == 'R' else (x * 2654435761 + 12345) % 4294967291
worst = Fr(-10**9); maxlag = 0; dchecks = 0
for t, a in enumerate(lst, 1):
    k[a] += 1; eta[a] += 1
    levs = {x: lev(x) for x in V}
    M = max(levs.values())
    ready = [x for x in V if eta[x] >= 2 and levs[x] >= 5]
    if ready:
        if wrong: x = min(ready)
        else:
            m = max(levs[x] for x in ready)
            x = min((y for y in ready if levs[y] == m), key=keyf)
        if M > 15:
            dchecks += 1
            if levs[x] != M:
                print(f"FAIL fact (d): round {t}, largest lag {M/5}, fired vertex has lag {levs[x]/5}"); sys.exit(1)
        F[x] += 1; eta[x] -= 2; eta[x - 1] += 1; eta[x + 1] += 1
    else:
        if M > 15: print(f"FAIL fact (d): round {t}, largest lag {M/5} but nothing fired"); sys.exit(1)
    srt = sorted((lev(x) for x in V), reverse=True)
    maxlag = max(maxlag, srt[0])
    s = 0
    for kk in range(1, n + 1):
        s += srt[kk - 1]
        slack = bound[kk] - Fr(s, 5)
        if -slack > worst: worst = -slack
        if slack < 0:
            print(f"FAIL Theorem U: round {t}, k={kk}: sum of {kk} largest lags {s/5} > bound {float(bound[kk]):.3f} (C={C}, n={n})"); sys.exit(1)
print(f"ok: {len(lst)} rounds, n={n}, tie {tie}, C={C}: all {n} prefix sums within the bound every round; "
      f"largest lag {maxlag/5} <= {float(C - 1 + Hn[n]):.3f}; closest approach (sum - bound) {float(worst):.3f}; fact (d) checked in {dchecks} rounds")
