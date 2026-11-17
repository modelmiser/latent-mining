#!/usr/bin/env python3
# Sub-lemma of G4: for k>=2, p in [-(k-1),-1], d in [1, -p]: the set Pi(p,d) of words on cells [p, p+k+d-1] with
#   w[c+d]=w[c] (c in [p,-1]),  w[c+d-1]=w[c] (c in [1,p+k]),  w[-1]!=w[0]!=w[1]
# has size <= 2^(d-1) (and is empty for d=1).  Also: for a singleton s (cell 0), "junction word J_p == through word W_u"
# (u=p+d) is EQUIVALENT to membership in Pi(p,d) -- checked by brute force over all contexts.
import itertools
def pi_size(k, p, d):
    lo, hi = p, p + k + d - 1; cnt = 0
    cells = list(range(lo, hi + 1))
    for bits in itertools.product((0, 1), repeat=len(cells)):
        w = dict(zip(cells, bits))
        if not (w[-1] != w[0] != w[1]): continue
        if all(w[c + d] == w[c] for c in range(p, 0)) and all(w[c + d - 1] == w[c] for c in range(1, p + k + 1)): cnt += 1
    return cnt
viol = viol_false = tests = 0; d1 = 0; mx = {}
for k in range(2, 9):
    for p in range(-(k - 1), 0):
        for d in range(1, -p + 1):
            sz = pi_size(k, p, d); tests += 1
            if d == 1 and sz != 0: d1 += 1
            if sz > 2 ** (d - 1): viol += 1
            if d >= 2 and sz > 2 ** (d - 2): viol_false += 1
            mx[d] = max(mx.get(d, 0), sz)
print(f"|Pi(p,d)| <= 2^(d-1): tests={tests} violations={viol}; d=1 nonempty: {d1}; FALSE variant 2^(d-2): violations={viol_false}; max size by d: {mx}")
# equivalence check: over all contexts of cells [-(2k-2) .. 2k-2] is too big; do k<=5 over cells [-(k-1)-? ..]
bad = tests = 0
for k in range(2, 6):
    lo, hi = -(k - 1), 2 * k - 2          # enough cells for every (p,d) with d>0
    cells = list(range(lo, hi + 1))
    for bits in itertools.product((0, 1), repeat=len(cells)):
        w = dict(zip(cells, bits))
        if not (w[-1] != w[0] != w[1]): continue
        for p in range(-(k - 1), 0):
            J = tuple(w[c] for c in range(p, p + k + 1) if c != 0)
            for u in range(p + 1, 1):
                d = u - p
                W = tuple(w[c] for c in range(u, u + k))
                eq = all(w[c + d] == w[c] for c in range(p, 0)) and all(w[c + d - 1] == w[c] for c in range(1, p + k + 1))
                tests += 1
                if (J == W) != eq: bad += 1
            # d = 0
            W = tuple(w[c] for c in range(p, p + k)); tests += 1
            if J == W: bad += 1
print(f"junction==through  <=>  equations (d>0), and never for d=0: tests={tests} mismatches={bad}")
