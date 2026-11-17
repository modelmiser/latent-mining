#!/usr/bin/env python3
"""Tables + linear-recurrence fitting (exact rational Berlekamp-Massey by linear solve)."""
import sys
from fractions import Fraction
from fast import N
def fit(seq, maxord):
    # find smallest (start, order) such that seq[start:] satisfies an integer-ish recurrence
    for order in range(1, maxord + 1):
        for start in range(0, 12):
            s = seq[start:]
            if len(s) < 3 * order + 4: continue
            # solve order x order system
            M = [[Fraction(s[i + j]) for j in range(order)] + [Fraction(s[i + order])] for i in range(order)]
            ok = True
            for c in range(order):
                p = next((r for r in range(c, order) if M[r][c] != 0), None)
                if p is None: ok = False; break
                M[c], M[p] = M[p], M[c]
                M[c] = [x / M[c][c] for x in M[c]]
                for r in range(order):
                    if r != c and M[r][c] != 0:
                        M[r] = [x - M[r][c] * y for x, y in zip(M[r], M[c])]
            if not ok: continue
            co = [M[i][order] for i in range(order)]
            if all(sum(co[j] * s[i + j] for j in range(order)) == s[i + order] for i in range(len(s) - order)):
                return start, order, co
    return None
kmax = int(sys.argv[1]); nmax = int(sys.argv[2]); maxord = int(sys.argv[3])
for k in range(1, kmax + 1):
    seq = [N(k, n) for n in range(1, nmax + 1)]
    print("k=%d N(k,1..16):" % k, seq[:16])
    r = fit(seq, maxord)
    if r:
        st, o, co = r
        print("  recurrence valid from n>=%d (first n using it: %d), order %d, checked to n=%d:" % (st + 1, st + 1 + o, o, nmax))
        print("  a(n) = " + " + ".join("(%s)*a(n-%d)" % (co[o - j], j) for j in range(1, o + 1)))
    else:
        print("  no recurrence of order <= %d found on n<=%d" % (maxord, nmax))
    sys.stdout.flush()
