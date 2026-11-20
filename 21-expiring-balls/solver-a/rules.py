#!/usr/bin/env python3
"""Experiment: which SIMPLE Rule strategies (lexicographic comparison of endpoint features)
keep every load < L on n bins against every FIFO Adversary?  Exhaustive over the states
reachable under the fixed rule.  Usage: rules.py n L [maxstates]
A rule is a list of (sign, feature); Rule gives the ball to the endpoint with the smaller
key; a full tie goes to the endpoint named first in normal form (smaller label).
"""
import sys
from itertools import permutations, product

n, L = int(sys.argv[1]), int(sys.argv[2])
LIMIT = int(sys.argv[3]) if len(sys.argv) > 3 else 400000
INF = 99


def norm(balls):
    m = {}
    out = []
    for w, o in balls:
        for x in (w, o):
            if x not in m:
                m[x] = len(m)
        out.append((m[w], m[o]))
    return tuple(out)


def feats(st, k):
    c = list(range(k + 2))
    for w, o in st:
        a, b = c[w], c[o]
        if a != b:
            c = [b if x == a else x for x in c]
    f = {}
    for v in range(k + 2):
        rec = [i for i, (w, o) in enumerate(st) if w == v]
        inc = [i for i, (w, o) in enumerate(st) if v in (w, o)]
        cmp_ = [i for i, (w, o) in enumerate(st) if c[w] == c[v]]
        f[v] = {
            'load': len(rec),
            'csize': c.count(c[v]),
            'oldrec': rec[0] - len(st) if rec else -INF,       # age rank of oldest received ball (more negative = older; none = oldest)
            'newrec': rec[-1] - len(st) if rec else -INF,
            'oldcomp': cmp_[0] - len(st) if cmp_ else -INF,
            'newcomp': cmp_[-1] - len(st) if cmp_ else -INF,
            'deg': len(inc),
        }
    return c, f


def safe(rule):
    seen = {()}
    stack = [()]
    while stack:
        st = stack.pop()
        if len(seen) > LIMIT:
            return None
        k = 1 + max([max(b) for b in st], default=-1)
        c, f = feats(st, k)
        nxt = []
        if st:
            nxt.append(norm(st[1:]))
        offers = [(a, b) for a in range(k) for b in range(a + 1, k) if c[a] != c[b]]
        if k < n:
            offers += [(a, k) for a in range(k)]
        if k + 2 <= n:
            offers.append((k, k + 1))
        for a, b in offers:
            ka = tuple(s * f[a][x] for s, x in rule)
            kb = tuple(s * f[b][x] for s, x in rule)
            w, o = (a, b) if ka <= kb else (b, a)
            t = norm(st + ((w, o),))
            cnt = sum(1 for (ww, oo) in t if ww == t[-1][0])
            if cnt >= L:
                return False
            nxt.append(t)
        for t in nxt:
            if t not in seen:
                seen.add(t)
                stack.append(t)
    return len(seen)


names = ['load', 'csize', 'oldrec', 'newrec', 'oldcomp', 'newcomp', 'deg']
good = []
for r in (1, 2, 3):
    for fs in permutations(names, r):
        for signs in product((1, -1), repeat=r):
            rule = list(zip(signs, fs))
            res = safe(rule)
            if res:
                # skip rules whose proper prefix is already safe
                if any(g == rule[:len(g)] for g in good):
                    continue
                good.append(rule)
                print('SAFE', n, L, ' then '.join(('min ' if s > 0 else 'max ') + x for s, x in rule), 'reachable_states=', res, flush=True)
print('done; safe minimal rules:', len(good))
