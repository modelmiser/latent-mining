#!/usr/bin/env python3
"""Local search on top of lhat_rand.py's generator (POOLS=shared): from each generated chain machine,
STEPS greedy random edits (leaf rewrites, index renames, subtree swaps) maximising cycle/MEASURE.
MEASURE=L is the control (must find violations of P1); MEASURE=Lhat is the test.
usage: lhat_climb.py NSTARTS SEED STEPS MEASURE MAXDEPTH
"""
import sys, random
NS = int(sys.argv[1]); SEED = int(sys.argv[2]); STEPS = int(sys.argv[3]); MEAS = sys.argv[4]; MAXD = int(sys.argv[5])
_argv = sys.argv
sys.argv = ['x', '0', str(SEED), 'shared', MEAS]
exec(open('lhat_rand.py').read().split("viol = 0; n = 0")[0])
sys.argv = _argv


def paths(t, pre=()):
    yield pre
    if t[0] == 'n':
        yield from paths(t[2], pre + (2,)); yield from paths(t[3], pre + (3,))


def get(t, p):
    for c in p:
        t = t[c]
    return t


def put(t, p, new):
    if not p:
        return new
    l = list(t); l[p[0]] = put(t[p[0]], p[1:], new)
    return tuple(l)


def pidx(t, p):
    out = []
    for c in p:
        out.append(t[1]); t = t[c]
    return out


def mutate(t):
    p = rng.choice(list(paths(t)))
    sub = get(t, p); pi = pidx(t, p); r = rng.random()
    if sub[0] == 'l':
        if r < 0.7 or not pi:
            new = ('l', rng.choice([None] + pi) if pi else None)
        else:
            u = rng.randint(1, MAXI)
            new = ('n', u, ('l', rng.choice([None, u] + pi)), ('l', rng.choice([None, u] + pi)))
    else:
        if r < 0.5:
            new = ('n', rng.randint(1, MAXI), sub[2], sub[3])
        elif r < 0.7:
            new = ('n', sub[1], sub[3], sub[2])
        elif r < 0.85:
            new = sub[rng.choice([2, 3])]
        else:
            new = ('l', rng.choice([None] + pi) if pi else None)
    return put(t, p, new)


def score(t):
    cs, _ = cycles(t)
    return max(len(c) for c in cs), measure(t)


viol = 0; best = (0, ''); seen = set()
for s in range(NS):
    while True:
        t = gen()
        if legal(t) and len(idx(t)) <= MAXI and depth(t) <= MAXD:
            break
    m, l = score(t); cur = (m / l, t)
    for _ in range(STEPS):
        t2 = mutate(cur[1])
        if not legal(t2) or depth(t2) > MAXD or len(idx(t2)) > MAXI:
            continue
        m, l = score(t2)
        if m > l and show(t2) not in seen:
            seen.add(show(t2)); viol += 1
            if viol <= 5:
                print('VIOLATION cycle %d > %s %d depth %d %s' % (m, MEAS, l, depth(t2), show(t2)))
        if m / l >= cur[0]:
            cur = (m / l, t2)
        if m / l > best[0]:
            best = (m / l, 'cycle %d %s %d depth %d %s' % (m, MEAS, l, depth(t2), show(t2)))
print('lhat_climb NS=%d SEED=%d STEPS=%d MEASURE=%s MAXD=%d' % (NS, SEED, STEPS, MEAS, MAXD))
print('distinct violating trees: %d   best ratio %.4f  %s' % (viol, best[0], best[1]))
