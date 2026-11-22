#!/usr/bin/env python3
"""Search for a tree with a cycle longer than Lhat(T) (the candidate replacement for P1).

Lhat(leaf) = 1. For a tree with all-zero path z_1..z_k and subtrees S_1..S_k at the 1-children:
  lam_i = Lhat(S_i), sorted decreasingly mu_1 >= ... >= mu_k,
  Lhat(T) = mu_1...mu_k + sum_{n=1..k} mu_1...mu_n.
(L(T) is the same formula with the lam_i in path order instead of sorted, so Lhat >= L.)

Hill climbing on  longest cycle / Lhat  (MEASURE=Lhat)  or  longest cycle / L  (MEASURE=L, the
control: it must find violations, since P1 is false).
usage: lhat_search.py SEED ITERS MAXDEPTH MAXIDX MEASURE [start-tree]
"""
import random, sys
from sim import parse, show, legal, cycles, L, depth, idx, nodes

SEED = int(sys.argv[1]); ITERS = int(sys.argv[2]); MAXD = int(sys.argv[3]); MAXI = int(sys.argv[4])
MEAS = sys.argv[5]
rng = random.Random(SEED)


def Lhat(t):
    if t[0] == 'l':
        return 1
    lam = []
    while t[0] == 'n':
        lam.append(Lhat(t[3])); t = t[2]
    lam.sort(reverse=True)
    tot = 0; p = 1
    for x in lam:
        p *= x; tot += p
    return tot + p


measure = Lhat if MEAS == 'Lhat' else L


def rand_tree(d, path, pleaf):
    avail = [u for u in range(1, MAXI + 1) if u not in path]
    if d == 0 or not avail or rng.random() < pleaf:
        return ('l', rng.choice([None] + list(path)) if path else None)
    u = rng.choice(avail)
    return ('n', u, rand_tree(d - 1, path + [u], pleaf), rand_tree(d - 1, path + [u], pleaf))


def paths(t, pre=()):
    yield pre
    if t[0] == 'n':
        yield from paths(t[2], pre + (2,))
        yield from paths(t[3], pre + (3,))


def get(t, p):
    for c in p:
        t = t[c]
    return t


def put(t, p, new):
    if not p:
        return new
    l = list(t); l[p[0]] = put(t[p[0]], p[1:], new)
    return tuple(l)


def path_idx(t, p):
    out = []
    for c in p:
        out.append(t[1]); t = t[c]
    return out


def mutate(t):
    ps = list(paths(t))
    p = rng.choice(ps)
    sub = get(t, p); pi = path_idx(t, p)
    r = rng.random()
    if sub[0] == 'l':
        if r < 0.6:
            new = ('l', rng.choice([None] + pi) if pi else None)
        else:
            new = rand_tree(1, pi, 0.0)
    else:
        if r < 0.4:
            new = ('n', rng.randint(1, MAXI), sub[2], sub[3])
        elif r < 0.6:
            new = ('l', rng.choice([None] + pi) if pi else None)
        elif r < 0.8:
            new = ('n', sub[1], sub[3], sub[2])
        else:
            new = sub[rng.choice([2, 3])]
    return put(t, p, new)


def score(t):
    cs, _ = cycles(t)
    m = max(len(c) for c in cs)
    return m, measure(t)


t = parse(sys.argv[6]) if len(sys.argv) > 6 else rand_tree(MAXD, [], 0.2)
m, l = score(t)
best = (m / l + 1e-7 * m, m, l, t)
cur = best
viol = 0
for it in range(ITERS):
    if it % 4000 == 3999 and len(sys.argv) <= 6:      # restart
        t = rand_tree(MAXD, [], 0.2); m, l = score(t); cur = (m / l + 1e-7 * m, m, l, t)
    t2 = mutate(cur[3])
    if not legal(t2) or depth(t2) > MAXD or len(idx(t2)) > MAXI:
        continue
    m, l = score(t2)
    if m > l:
        viol += 1
        if viol <= 3:
            print('VIOLATION of cycle <= %s: cycle %d > %d  %s' % (MEAS, m, l, show(t2)))
    c2 = (m / l + 1e-7 * m, m, l, t2)
    if c2[0] >= cur[0] or rng.random() < 0.02:
        cur = c2
    if c2[0] > best[0]:
        best = c2
print('lhat_search SEED=%d ITERS=%d MAXD=%d MAXI=%d MEASURE=%s' % (SEED, ITERS, MAXD, MAXI, MEAS))
print('best ratio %.4f  cycle %d  %s %d  depth %d  tree %s' % (best[0], best[1], MEAS, best[2], depth(best[3]), show(best[3])))
print('violations found:', viol)
