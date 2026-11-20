#!/usr/bin/env python3
"""Independent checker for a Rule safe set in the ANY-removal game.
  check_any.py n L file
Canonical form here is NOT the one of anyiso.cpp: every tree is encoded rooted at EVERY
one of its vertices and the smallest encoding is kept (no centre finding).
Checks: empty state present; every load < L; removing any single ball stays in the set;
for every pair of bins in different components at least one answer stays in the set.
"""
import sys
sys.setrecursionlimit(10000)
n, L, fn = int(sys.argv[1]), int(sys.argv[2]), sys.argv[3]


def key(balls):
    adj = {}
    for w, o in balls:
        adj.setdefault(w, []).append((o, 'R'))   # w received from the pair {w,o}
        adj.setdefault(o, []).append((w, 'G'))
    def enc(v, p):
        return '[' + ''.join(sorted(t + enc(u, v) for u, t in adj[v] if u != p)) + ']'
    seen = set()
    trees = []
    for v0 in adj:
        if v0 in seen:
            continue
        comp = [v0]
        seen.add(v0)
        for v in comp:
            for u, _ in adj[v]:
                if u not in seen:
                    seen.add(u)
                    comp.append(u)
        trees.append(min(enc(v, None) for v in comp))
    return tuple(sorted(trees))


def comps(balls):
    c = list(range(n))
    for w, o in balls:
        a, b = c[w], c[o]
        if a != b:
            c = [b if x == a else x for x in c]
    return c


states = []
for line in open(fn):
    if line.startswith('#'):
        continue
    line = line.strip()
    st = tuple(tuple(int(x) for x in b.split(':')) for b in line.split(' ')) if line else ()
    states.append(st)
S = set(key(st) for st in states)
try:
    assert () in S, 'empty state missing'
    for st in states:
        assert all(0 <= x < n for b in st for x in b) and all(w != o for w, o in st)
        c = comps(st)
        assert len(set(c)) == n - len(st), 'not a forest'
        ld = [0] * n
        for w, o in st:
            ld[w] += 1
        assert max(ld) < L, 'load L present'
        for j in range(len(st)):
            assert key(st[:j] + st[j + 1:]) in S, 'not closed under removal'
        for a in range(n):
            for b in range(a + 1, n):
                if c[a] != c[b]:
                    assert key(st + ((a, b),)) in S or key(st + ((b, a),)) in S, f'offer {a},{b} at {st}: no safe answer'
except AssertionError as e:
    print('REJECTED', fn, repr(e))
    sys.exit(1)
print(f'OK any-removal rule n={n} L={L} lines={len(states)} classes={len(S)} {fn}')
