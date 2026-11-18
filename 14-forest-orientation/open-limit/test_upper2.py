#!/usr/bin/env python3
"""Real-tree test of the pair potential as an Algorithm potential:  G(C) = max over vertices v of g(slots(v)),
3g(p,q) = 3q + (4 if q==p, 2 if q==p+1, else 1).  For all pairs of oriented trees (sizes <= MAXM), all u, v:
   min over normal replies of 3*cost + 3G(new)  <=  3G(X) + 3G(Y) ?      (MODE selects variants of g)"""
import sys
from trees import *
MAXM = int(sys.argv[1]); MODE = int(sys.argv[2]) if len(sys.argv) > 2 else 0; H = 6
T = all_trees(MAXM)
def g3(p, q):
    d = q - p
    return 3 * q + (4 if d == 0 else 2 if d == 1 else 1)
def G3(m, out, extra=None):
    h = [None] * m
    def rec(v):
        if h[v] is not None: return h[v]
        vals = [rec(w) for w in out[v]]
        if extra and v == extra[0]: vals = vals + [extra[1]]
        h[v] = 0 if len(vals) < 2 else 1 + min(vals)
        return h[v]
    for v in range(m): rec(v)
    best = 0
    for v in range(m):
        vals = [h[w] for w in out[v]]
        if extra and v == extra[0]: vals = vals + [extra[1]]
        vals = sorted(vals + [-1] * (2 - len(vals)))
        best = max(best, g3(vals[0], vals[1]))
    return best, h
sigs = {}
for m in range(1, MAXM + 1):
    for out in T[m]:
        G, h = G3(m, out)
        for x in range(m):
            reps = set()
            for (cost, o2) in replies(m, out, x):
                reps.add((cost, tuple(G3(m, o2, (x, hv))[0] for hv in range(H))))
            sig = (h[x], G, tuple(sorted(reps)))
            if sig not in sigs: sigs[sig] = (m, out, x)
print('MAXM', MAXM, 'distinct signatures', len(sigs))
S = list(sigs.items()); bad = 0; tight = 0; worst = 0; shown = set()
for (sa, ea) in S:
    hx, GX, ra = sa
    for (sb, eb) in S:
        hy, GY, rb = sb
        best = min([3 * c + max(t[hy], GY) for (c, t) in ra] + [3 * c + max(t[hx], GX) for (c, t) in rb])
        rhs = GX + GY
        if best > rhs:
            bad += 1; worst = max(worst, best - rhs)
            key = (ea[0], eb[0])
            if len(shown) < 6 and key not in shown:
                shown.add(key); print('VIOLATION by %d/3:' % (best - rhs), ea, eb, 'G3X=%d G3Y=%d' % (GX, GY))
        elif best == rhs: tight += 1
print('pairs', len(S) ** 2, 'violations', bad, 'tight', tight, 'worst excess (thirds)', worst)
