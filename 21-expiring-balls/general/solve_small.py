#!/usr/bin/env python3
"""Sanity test of the game model used by check_adv.py: an exhaustive solver that uses the
SAME position conventions (newest-first, renaming by first appearance, expiry = drop last,
offer legal iff different components) and recomputes g(n) for small n.
Expected from the repository: N(1)=2, N(2)=4, N(3)=7 (g(6)=2, g(7)=3).
usage: solve_small.py L n   -> prints whether Adversary can force load L on n bins."""
import sys
L = int(sys.argv[1]); n = int(sys.argv[2])

def renamed(balls):
    name = {}; out = []
    for (r, o) in balls:
        for x in (r, o):
            if x not in name: name[x] = len(name)
        out.append((name[r], name[o]))
    return tuple(out)

def roots(s, m):
    rep = list(range(m + 2))
    def root(x):
        while rep[x] != x: x = rep[x]
        return x
    for (r, o) in s:
        rep[root(r)] = root(o)
    return root

# enumerate all non-final positions reachable (forward), with their moves
pos = {}          # s -> (expire successor or None, list of offers [(t_u, t_v)]) ; t None = load L
todo = [()]
seen = {()}
while todo:
    s = todo.pop()
    m = 1 + max([max(b) for b in s], default=-1)
    root = roots(s, m)
    exp = renamed(s[:-1]) if s else None
    offers = []
    cand = [(a, b) for a in range(m) for b in range(a + 1, m) if root(a) != root(b)]
    if m + 1 <= n: cand += [(a, m) for a in range(m)]
    if m + 2 <= n: cand.append((m, m + 1))
    for (a, b) in cand:
        res = []
        for (r, o) in ((a, b), (b, a)):
            if 1 + sum(1 for (x, _) in s if x == r) >= L: res.append(None)
            else: res.append(renamed(((r, o),) + s))
        offers.append(tuple(res))
    pos[s] = (exp, offers)
    for t in ([exp] if exp is not None else []) + [t for o in offers for t in o if t is not None]:
        if t not in seen:
            seen.add(t); todo.append(t)
# backward fixpoint: W = positions from which Adversary forces load L
W = set()
changed = True
while changed:
    changed = False
    for s, (exp, offers) in pos.items():
        if s in W: continue
        if (exp is not None and exp in W) or any(all(t is None or t in W for t in o) for o in offers):
            W.add(s); changed = True
print('L=%d n=%d positions=%d adversary_wins_from_empty=%s' % (L, n, len(pos), () in W))
