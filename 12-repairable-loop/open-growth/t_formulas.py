#!/usr/bin/env python3
# (a) the local formulas for the change of k-window counts under one edit, checked on random loops for k=2..6 (for k=1 a doubling creates 1 window, not k-1=0: lemmas are stated for k>=2)
# (b) on all known repairable loops of length 2^n+2 (n=4,5): every isolated singleton has neighbour runs {n,2}
import random, itertools
from og import *
rng = random.Random(99)
def cnt(v, k):
    c = counts(v, k); return c
t = bad = 0
for _ in range(4000):
    L = rng.randint(14, 40); v = tuple(rng.randint(0, 1) for _ in range(L)); k = rng.randint(2, 6); c = rng.randrange(L)
    g = lambda i: v[i % L]
    # loss at c: destroyed = k windows through c; created = k-1 windows of v' containing both neighbours
    exp = dict(cnt(v, k))
    for i in range(k):
        f = tuple(g(c - k + 1 + i + j) for j in range(k)); exp[f] = exp.get(f, 0) - 1
    seq = [g(c - k + 1 + j) for j in range(k - 1)] + [g(c + 1 + j) for j in range(k - 1)]   # 2k-2 cells around the junction
    for i in range(k - 1):
        f = tuple(seq[i:i + k]); exp[f] = exp.get(f, 0) + 1
    act = cnt(loss(v, c), k); t += 1
    if {f: n for f, n in exp.items() if n} != act: bad += 1
    # doubling at c: destroyed = the k-2 windows containing c and c+1 that do not start at c (the window starting at c
    # survives as the window starting at the copy); created = k-1 windows containing both copies.  Net +1 window.
    exp = dict(cnt(v, k))
    for i in range(k - 2):
        f = tuple(g(c - k + 2 + i + j) for j in range(k)); exp[f] = exp.get(f, 0) - 1
    seq = [g(c - k + 2 + j) for j in range(k - 2)] + [g(c), g(c)] + [g(c + 1 + j) for j in range(k - 2)] if k >= 2 else []
    for i in range(k - 1):
        f = tuple(seq[i:i + k]); exp[f] = exp.get(f, 0) + 1
    act = cnt(dbl(v, c), k); t += 1
    if {f: n for f, n in exp.items() if n} != act: bad += 1
print(f"(a) window-count formulas for loss/doubling: tests={t} mismatches={bad}")
# deliberately false formula: pretend a loss creates k windows
t = bad = 0
for _ in range(300):
    L = rng.randint(14, 40); v = tuple(rng.randint(0, 1) for _ in range(L)); k = 3; c = rng.randrange(L)
    if sum(cnt(loss(v, c), k).values()) != sum(cnt(v, k).values()) - k + k: bad += 1
    t += 1
print(f"    FALSE variant 'a loss creates k windows': tests={t} mismatches={bad} (expected all)")
R = load_repairable()
for n in (4, 5):
    L = 2 ** n + 2; tot = viol = 0
    for s in R[n]:
        if len(s) != L: continue
        w = T(s); rr = runs(w)
        for j, (st, l, a) in enumerate(rr):
            if l == 1:
                p = rr[j - 1][1]; q = rr[(j + 1) % len(rr)][1]
                if p >= 2 and q >= 2:
                    tot += 1
                    if sorted((p, q)) != [2, n]: viol += 1
    print(f"(b) n={n}: isolated singletons in repairable loops of length 2^n+2: {tot}; with neighbour runs other than {{n,2}}: {viol}")
