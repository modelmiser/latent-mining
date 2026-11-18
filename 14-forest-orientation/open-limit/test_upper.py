#!/usr/bin/env python3
"""Test a candidate Algorithm potential G(C) = F(max type index over the vertices of C):
for ALL pairs of oriented trees X, Y (sizes <= MAXM), all u in X, v in Y:
   min over normal replies of  3*cost + F3(maxidx(new tree))  <=  F3(maxidx X) + F3(maxidx Y) ?"""
import sys
from trees import *
MAXM = int(sys.argv[1]); H = 6
T = all_trees(MAXM)
def maxidx(m, out, extra=None):
    """max type index; extra=(u,hv): u has an additional out-neighbour with h = hv"""
    h = [None] * m
    def rec(v):
        if h[v] is not None: return h[v]
        vals = [rec(w) for w in out[v]]
        if extra and v == extra[0]: vals = vals + [extra[1]]
        h[v] = 0 if len(vals) < 2 else 1 + min(vals)
        return h[v]
    best = 0
    for v in range(m):
        rec(v)
    for v in range(m):
        vals = [h[w] for w in out[v]]
        if extra and v == extra[0]: vals = vals + [extra[1]]
        vals = sorted(vals + [-1] * (2 - len(vals)))
        best = max(best, idx_of(vals[0], vals[1]))
    return best, h
sigs = {}
for m in range(1, MAXM + 1):
    for out in T[m]:
        G, h = maxidx(m, out)
        for x in range(m):
            reps = set()
            for (cost, o2) in replies(m, out, x):
                reps.add((cost, tuple(maxidx(m, o2, (x, hv))[0] for hv in range(H))))
            # prune dominated replies
            reps = sorted(reps)
            sig = (h[x], G, tuple(reps))
            if sig not in sigs: sigs[sig] = (m, out, x)
print('MAXM', MAXM, 'distinct signatures', len(sigs))
S = list(sigs.items())
bad = 0; tight = 0; worst = 0
for (sa, ea) in S:
    hx, GX, ra = sa
    for (sb, eb) in S:
        hy, GY, rb = sb
        best = min([3 * c + F3(max(t[hy], GY)) for (c, t) in ra] + [3 * c + F3(max(t[hx], GX)) for (c, t) in rb])
        rhs = F3(GX) + F3(GY)
        if best > rhs:
            bad += 1
            if best - rhs > worst or bad <= 5:
                worst = max(worst, best - rhs)
                print('VIOLATION by %d/3:' % (best - rhs), ea, eb, 'GX=%d GY=%d' % (GX, GY))
        elif best == rhs: tight += 1
print('pairs', len(S) ** 2, 'violations', bad, 'tight', tight, 'worst excess (thirds)', worst)
