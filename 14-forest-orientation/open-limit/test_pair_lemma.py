#!/usr/bin/env python3
"""Exhaustive test of the PAIR LEMMA on explicit oriented trees (all trees up to MAXM vertices, every vertex x,
every normal reply at x, partner depth hy = 0..6).  With (P,Q) the sorted slot values of x before the reply:
   every reply satisfies   (cost >= P+1 and new slots >= sort(Q,hy))  or  (cost >= Q+1 and new slots >= sort(P,hy)).
Also checks h(o) is unchanged for the out-neighbour not on the flipped path.
VARIANT 1 (false): cost >= P+2 in the first alternative.  VARIANT 2 (false): second alternative removed."""
import sys
from trees import *
MAXM = int(sys.argv[1]); VAR = int(sys.argv[2]) if len(sys.argv) > 2 else 0
T = all_trees(MAXM); checked = fails = 0
for m in range(1, MAXM + 1):
    for out in T[m]:
        h = hvals(m, out)
        for x in range(m):
            P, Q = slots(x, out, h)
            for (cost, o2) in replies(m, out, x):
                h2 = hvals(m, o2)
                for w in o2[x]:
                    assert h2[w] == h[w]
                for hy in range(0, 7):
                    vals = sorted([h2[w] for w in o2[x]] + [hy] + [-1] * (1 - len(o2[x])))
                    a = sorted((Q, hy)); b = sorted((P, hy))
                    alt1 = cost >= P + (2 if VAR == 1 else 1) and vals[0] >= a[0] and vals[1] >= a[1]
                    alt2 = VAR != 2 and cost >= Q + 1 and vals[0] >= b[0] and vals[1] >= b[1]
                    checked += 1
                    if not (alt1 or alt2):
                        fails += 1
                        if fails <= 2: print('FAIL', m, out, x, (P, Q), cost, hy, vals)
print('MAXM', MAXM, 'variant', VAR, 'checked', checked, 'fails', fails)
