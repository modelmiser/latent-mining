#!/usr/bin/env python3
"""Random test of  longest cycle <= MEASURE(T)  on 'chain machines': an all-zero path of k modes,
each S_i a read-once counter whose overflow leaf jumps to another mode (reading the target's z-index
first when the target lies deeper), the modes chained in a random order.
 POOLS=disjoint : the counters use disjoint indices (Theorem MD applies)
 POOLS=shared   : afterwards random data indices of different modes are identified and a few random
                  leaf/index edits are made (Theorem MD does not apply)
 MEASURE=L      : the shape function of Conjecture P1 (control: violations MUST be found)
 MEASURE=Lhat   : sorted variant (see lhat_search.py)
usage: lhat_rand.py NTREES SEED POOLS MEASURE
"""
import random, sys
from sim import show, legal, cycles, L, depth, idx

NT = int(sys.argv[1]); SEED = int(sys.argv[2]); POOLS = sys.argv[3]; MEAS = sys.argv[4]
rng = random.Random(SEED)
MAXI = 12


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


def set_all_ones(t, new):
    if t[0] == 'l':
        return new
    return ('n', t[1], t[2], set_all_ones(t[3], new))


def counter(fresh, d):
    if d == 0 or rng.random() < 0.35 or fresh[0] > MAXI:
        return ('l', None)
    a = fresh[0]; fresh[0] += 1
    t1 = counter(fresh, d - 1); t0 = counter(fresh, d - 1)
    return ('n', a, set_all_ones(t0, ('l', a)), t1)


def rename(t, u, v):
    if t[0] == 'l':
        return ('l', v if t[1] == u else t[1])
    return ('n', v if t[1] == u else t[1], rename(t[2], u, v), rename(t[3], u, v))


def edit(t, path):
    """one random edit somewhere below"""
    if t[0] == 'l':
        return ('l', rng.choice([None] + path) if path else None)
    r = rng.random()
    if r < 0.15:
        return ('n', rng.randint(1, MAXI), t[2], t[3])
    if r < 0.55:
        return ('n', t[1], edit(t[2], path + [t[1]]), t[3])
    return ('n', t[1], t[2], edit(t[3], path + [t[1]]))


def gen():
    k = rng.randint(2, 5)
    Z = list(range(1, k + 1))
    fresh = [k + 1]
    order = Z[:]; rng.shuffle(order)
    p = rng.randint(2, k); chain = order[:p]
    if rng.random() < 0.6:      # bias: descending chain that ends with one ascent from mode 1 or so
        last = max(chain); chain = sorted([m for m in chain if m != last], reverse=True) + [last]
    S = {}
    for i in Z:
        c = counter(fresh, rng.choice([0, 0, 1, 2, 2]))
        if i in chain and chain.index(i) + 1 < p:
            tgt = chain[chain.index(i) + 1]
            if tgt < i:
                ov = ('l', tgt)
            else:
                ov = ('n', tgt, ('l', tgt), ('l', None))
        else:
            ov = ('l', None)
        S[i] = set_all_ones(c, ov)
    if POOLS == 'shared':
        data = list(range(k + 1, fresh[0]))
        for _ in range(rng.randint(1, 3)):
            if len(data) >= 2:
                u, v = rng.sample(data, 2)
                i = rng.choice(Z)
                S[i] = rename(S[i], u, v)
    t = ('l', chain[0])
    for i in reversed(Z):
        t = ('n', i, t, S[i])
    if POOLS == 'shared':
        for _ in range(rng.randint(0, 2)):
            t = edit(t, [])
    return t


viol = 0; n = 0; best = (0, None); tight = 0
while n < NT:
    t = gen()
    if not legal(t) or len(idx(t)) > MAXI:
        continue
    n += 1
    cs, _ = cycles(t)
    m = max(len(c) for c in cs); l = measure(t)
    if m > l:
        viol += 1
        if viol <= 3:
            print('VIOLATION cycle %d > %s %d  depth %d  %s' % (m, MEAS, l, depth(t), show(t)))
    if m == l:
        tight += 1
    if m / l > best[0]:
        best = (m / l, 'cycle %d %s %d depth %d %s' % (m, MEAS, l, depth(t), show(t)))
print('lhat_rand NT=%d SEED=%d POOLS=%s MEASURE=%s' % (NT, SEED, POOLS, MEAS))
print('violations: %d   tight: %d   best ratio %.4f  %s' % (viol, tight, best[0], best[1]))
