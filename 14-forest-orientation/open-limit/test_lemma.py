#!/usr/bin/env python3
"""Exhaustive test of the TYPE LEMMA on explicit oriented trees.
For every pair of oriented trees (X, Y) (up to isomorphism, sizes <= MAXM each), every x in X, y in Y,
with i = min(idx(x), idx(y))  (the larger type is downgraded), and every normal reply:
  i = 2k   : cost >= k   and the replying endpoint has index >= 2k+1 afterwards
  i = 2k+1 : (cost >= k and new index >= 2k+2)  or  (cost >= k+1 and new index >= 2k+1)
and in all cases  3*cost + F3(new index) >= 2*F3(i)   (potential inequality).
VARIANT selects a deliberately FALSE claim, to see the test fail:
  1: R+R always yields index >= 2k+2        2: A+A costs >= k+1
  3: the NON-replying endpoint gets the new type   4: F(R_k)=k+1 instead of k+2/3 (potential)"""
import sys
from trees import *
MAXM = int(sys.argv[1]); VARIANT = int(sys.argv[2]) if len(sys.argv) > 2 else 0
T = all_trees(MAXM)
items = []
for m in range(1, MAXM + 1):
    for out in T[m]:
        h = hvals(m, out)
        for x in range(m):
            p, q = slots(x, out, h)
            items.append((m, out, x, idx_of(p, q), h[x], replies(m, out, x)))
print('trees per size:', {m: len(T[m]) for m in T}, ' (tree,vertex) items:', len(items))
def F3v(i):
    if VARIANT == 4 and i % 2 == 1: return 3 * (i // 2) + 3
    return F3(i)
checked = 0; fails = 0; maxidx = 0
# reply at x in X; Y enters only through h(y) and idx(y).  We still loop over all (Y,y) summaries.
ysum = sorted(set((it[3], it[4]) for it in items) | set((iy, hy) for iy in range(0, 9) for hy in range(iy // 2, 6)))
print('distinct (idx(y), h(y)) summaries:', ysum)
for (m, out, x, ix, hx, reps) in items:
    maxidx = max(maxidx, ix)
    for (iy, hy) in ysum:
        i = min(ix, iy); k = i // 2
        for (cost, o2) in reps:
            # merged tree: add vertex m standing for y with h = hy: emulate by computing slots of x directly
            h2 = hvals(m, o2)
            assert len(o2[x]) <= 1
            vals = sorted([h2[w] for w in o2[x]] + [hy] + [-1] * (1 - len(o2[x])))
            ni = idx_of(vals[0], vals[1])
            if VARIANT == 3:
                ni = iy   # (false) pretend only y's old type is available
            checked += 1
            if i % 2 == 0:
                ok = cost >= (k + 1 if VARIANT == 2 else k) and ni >= i + 1
            else:
                ok = (cost >= k and ni >= i + 1) if VARIANT == 1 else \
                     ((cost >= k and ni >= i + 1) or (cost >= k + 1 and ni >= i))
            # potential inequality with the guaranteed (downgraded) new type
            if i % 2 == 0: g = i + 1
            else: g = i + 1 if (cost >= k and ni >= i + 1) else i
            if VARIANT == 3: g = min(g, ni) if ni >= i else i
            pot = 3 * cost + F3v(g) >= 2 * F3v(i)
            if not (ok and pot):
                fails += 1
                if fails <= 3: print('FAIL', m, out, 'x=%d' % x, 'ix=%d iy=%d hy=%d cost=%d ni=%d' % (ix, iy, hy, cost, ni))
print('variant', VARIANT, 'checked', checked, 'fails', fails, 'max index seen', maxidx)
