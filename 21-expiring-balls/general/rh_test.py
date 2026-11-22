#!/usr/bin/env python3
"""Random test of the rule of Theorem L (PROOFS.md) in the ANY-REMOVAL game.
usage: rh_test.py n r steps seed [wrong]
Checks after every move: (a) (#rich bins) * r <= n-1; (b) every ball received by a rich bin
after its stamp has its other end in the set S of bins that were rich just after the stamp;
(c) load <= r + |S| - 1.  With 'wrong' the tie-break is reversed (earlier stamp receives):
the checks are then expected to fail."""
import sys, random
n, r, steps, seed = map(int, sys.argv[1:5]); wrong = len(sys.argv) > 5
rnd = random.Random(seed)
balls = []                      # (receiver, other, time)
stamp = {}                      # rich bin -> (time it became rich, frozenset S of rich bins then)
t = 0; maxload = 0; maxrich = 0
def load(x): return sum(1 for b in balls if b[0] == x)
def comp():
    rep = list(range(n))
    def root(x):
        while rep[x] != x: x = rep[x]
        return x
    for (a, b, _) in balls: rep[root(a)] = root(b)
    return root
for step in range(steps):
    root = comp()
    if balls and rnd.random() < 0.35:
        # removal of an arbitrary ball, biased towards balls of poor bins and old balls
        i = rnd.randrange(len(balls)) if rnd.random() < 0.5 else min(range(len(balls)), key=lambda j: (balls[j][0] in stamp, rnd.random()))
        x = balls[i][0]; del balls[i]
        if x in stamp and load(x) < r: del stamp[x]
    else:
        rich = list(stamp)
        pairs = [(u, v) for u in range(n) for v in range(u + 1, n) if root(u) != root(v)]
        if not pairs: continue
        rr = [p for p in pairs if p[0] in stamp and p[1] in stamp]
        u, v = rnd.choice(rr) if rr and rnd.random() < 0.8 else rnd.choice(pairs)
        pu, pv = u not in stamp, v not in stamp
        if pu or pv:
            w = u if pu else v
        else:
            later = u if stamp[u][0] > stamp[v][0] else v
            w = later if not wrong else (u + v - later)
        t += 1
        balls.append((w, u + v - w, t))
        if w not in stamp and load(w) >= r:
            stamp[w] = (t, frozenset(list(stamp) + [w]))
    # checks
    assert len(stamp) * r <= n - 1, 'check (a) failed'
    for x, (t0, S) in stamp.items():
        newer = [b for b in balls if b[0] == x and b[2] > t0]
        assert all(b[1] in S for b in newer), 'check (b) failed at step %d' % step
        assert load(x) <= r + len(S) - 1, 'check (c) failed'
    maxload = max([maxload] + [load(x) for x in range(n)]); maxrich = max(maxrich, len(stamp))
print('n=%d r=%d steps=%d seed=%d wrong=%s: all checks passed; max load seen %d, max rich bins %d, bound r+floor((n-1)/r)-1 = %d'
      % (n, r, steps, seed, wrong, maxload, maxrich, r + (n - 1) // r - 1))
