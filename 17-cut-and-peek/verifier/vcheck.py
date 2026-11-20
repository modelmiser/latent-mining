#!/usr/bin/env python3
"""Referee checker, from the problem definition only.
Input: owner string (char '0' or 'H' = house, else player label), u0, u1 strings over 01 of length m
(house positions: card = u0 there, and u0 must equal u1 there), R list.
Returns (ok, reason)."""
import sys, itertools
from collections import Counter

def check(owner, u0, u1, R, verbose=False):
    m = len(owner)
    if not (len(u0) == m and len(u1) == m): return False, "length mismatch"
    if any(c not in '01' for c in u0+u1): return False, "bad card symbol"
    R = sorted(set(R))
    if not R: return False, "empty R"
    if any(q < 0 or q >= m for q in R): return False, "R out of range"
    house = [p for p in range(m) if owner[p] in '0H' ]
    labels = sorted(set(owner[p] for p in range(m) if owner[p] not in '0H'))
    n = len(labels)
    if n < 2: return False, "fewer than 2 players"
    for p in house:
        if u0[p] != u1[p]: return False, "house card depends on a bit at %d" % p
    idx = {c: i for i, c in enumerate(labels)}
    def row(x):
        return [ (u1[p] if x[idx[owner[p]]] else u0[p]) if owner[p] not in '0H' else u0[p] for p in range(m)]
    def M(x):
        w = row(x)
        c = Counter()
        for j in range(m):
            # card at p moves to p+j: new[q] = w[q-j]
            c[''.join(w[(q - j) % m] for q in R)] += 1
        assert sum(c.values()) == m
        return c
    xs = list(itertools.product((0, 1), repeat=n))
    ones = tuple([1]*n); zeros = tuple([0]*n)
    M0 = M(zeros)
    for x in xs:
        if x == ones: continue
        if M(x) != M0: return False, "privacy fails at x=%s" % (x,)
    M1 = M(ones)
    common = set(M1) & set(M0)
    if common: return False, "correctness fails: %d common words, e.g. %s" % (len(common), sorted(common)[0])
    if verbose:
        print("  n=%d m=%d |R|=%d  M(0) support %d, M(1) support %d" % (n, m, len(R), len(M0), len(M1)))
        print("  M(0):", dict(sorted(M0.items())))
        print("  M(1):", dict(sorted(M1.items())))
    return True, "valid"

def parse_a(path):
    d = {}
    for line in open(path):
        line = line.strip()
        if not line or line[0] == '#': continue
        k, v = line.split(None, 1); d[k] = v
    return d['owner'], d['u0'], d['u1'], [int(t) for t in d['R'].split(',')], int(d['n']), int(d['m'])

if __name__ == '__main__':
    for p in sys.argv[1:]:
        o, a, b, R, n, m = parse_a(p)
        assert len(o) == m, "m mismatch"
        ok, why = check(o, a, b, R, verbose=True)
        nn = len(set(o) - set('0H'))
        print(p, "declared n=%d found n=%d:" % (n, nn), ok, why)
