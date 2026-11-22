#!/usr/bin/env python3
"""Mechanical check of Theorem NC (PROOFS.md section 4): the per-cycle version of Theorem MD.

Random trees with heavy index sharing between the subtrees S_i. For every cycle C through a home
state: R_i = data indices at nodes visited by mode-i steps of C; ch_i = data indices whose value some
mode-i step of C changes. Hypothesis (NC): ch_i and R_j are disjoint for all visited i != j.
If (NC) holds, check: every mode has one successor mode over all its steps that do not go home
(the chain), and with y_i = restriction of the state to R_i, c_i = number of distinct y_i at mode-i
states:   #home <= prod c,  #mode-m_j <= prod_{t>=j} c_(m_t),  |C| <= prod c + sum_j prod_{t>=j} c_(m_t),
and the y_i seen in mode i are one cycle of the pruned tree G_i^R (built as in the proof).
usage: nccheck.py NTREES SEED [MUT]
 MUT=1: skip the (NC) test and apply the checks to every cycle through home (wrong on purpose)
 MUT=2: prefix instead of suffix products (wrong on purpose)
"""
import random, sys
from sim import legal, step, cycles, show, idx

NT = int(sys.argv[1]); SEED = int(sys.argv[2]); MUT = int(sys.argv[3]) if len(sys.argv) > 3 else 0
rng = random.Random(SEED)


def rand_sub(d, path, pool, pleaf, zs):
    avail = [u for u in pool if u not in path]
    if d == 0 or not avail or rng.random() < pleaf:
        r = rng.random()
        if r < 0.15:
            return ('l', None)
        zz = [u for u in path if u in zs]
        if r < 0.5:
            return ('l', rng.choice(zz))
        return ('l', rng.choice(path))
    u = rng.choice(avail)
    return ('n', u, rand_sub(d - 1, path + [u], pool, pleaf, zs), rand_sub(d - 1, path + [u], pool, pleaf, zs))


def gen():
    k = rng.randint(2, 4)
    Z = list(range(1, k + 1))
    data = list(range(k + 1, k + 1 + rng.randint(2, 7)))
    t = ('l', rng.choice(Z))
    for i in range(k - 1, -1, -1):
        pool = rng.sample(data, rng.randint(1, min(4, len(data)))) + Z[i + 1:]
        t = ('n', Z[i], t, rand_sub(3, Z[:i + 1], pool, 0.25, set(Z)))
    return t


def walk(t, s):
    vis = []
    while t[0] == 'n':
        vis.append(t[1]); t = t[3] if s[t[1]] else t[2]
    return vis, t


def prune_keep(t, keep):
    if t[0] == 'l':
        return t if t[1] in keep else ('l', None)
    if t[1] not in keep:
        return prune_keep(t[2], keep)
    return ('n', t[1], prune_keep(t[2], keep), prune_keep(t[3], keep))


def prod(xs):
    p = 1
    for x in xs:
        p *= x
    return p


class Fail(Exception):
    pass


st = dict.fromkeys(['trees', 'cyclesB', 'NC', 'NC_with_shared_read', 'NC_trees_not_MD', 'tight', 'chain>=2', 'FAILS'], 0)
first = None
for _ in range(NT):
    T = gen()
    assert legal(T)
    st['trees'] += 1
    Z = []; S = []; t = T
    while t[0] == 'n':
        Z.append(t[1]); S.append(t[3]); t = t[2]
    lam = t; k = len(Z); zset = set(Z)
    Dsets = [idx(s) - zset for s in S]
    md = all(not (Dsets[i] & Dsets[j]) for i in range(k) for j in range(i))
    cs, ix = cycles(T)
    try:
        for C in cs:
            n = len(C)
            zc = [sum(s[z] for z in Z) for s in C]
            if n < 2 or min(zc) > 0:
                continue
            st['cyclesB'] += 1
            assert max(zc) <= 1
            mode = [0 if zc[t] == 0 else 1 + [s[z] for z in Z].index(1) for t, s in enumerate(C)]
            R = {}; ch = {}; succ = {}
            for t in range(n):
                i = mode[t]
                if i == 0:
                    continue
                s = C[t]; s2 = C[(t + 1) % n]
                vis, leaf = walk(T, s)
                R.setdefault(i, set()).update(u for u in vis if u not in zset)
                ch.setdefault(i, set()).update(u for u in ix if u not in zset and s[u] != s2[u])
                if mode[(t + 1) % n] != 0:
                    succ.setdefault(i, set()).add(mode[(t + 1) % n])
            nc = all(not (ch[i] & R[j]) for i in R for j in R if i != j)
            if not nc and MUT != 1:
                continue
            if nc:
                st['NC'] += 1
                if any(R[i] & R[j] for i in R for j in R if i < j):
                    st['NC_with_shared_read'] += 1
                if not md:
                    st['NC_trees_not_MD'] += 1
            if any(len(v) > 1 for v in succ.values()):
                raise Fail('a mode has two successor modes')
            j0 = Z.index(lam[1]) + 1
            chain = [j0]
            while chain[-1] in succ and next(iter(succ[chain[-1]])) not in chain:
                chain.append(next(iter(succ[chain[-1]])))
            if set(chain) != set(R):
                raise Fail('visited modes are not the chain')
            c = {}
            for i in R:
                ys = [tuple(sorted((u, C[t][u]) for u in R[i])) for t in range(n) if mode[t] == i]
                c[i] = len(set(ys))
                G = prune_keep(S[i - 1], R[i])
                if not legal(G):
                    raise Fail('pruned tree illegal')
                for a, b in zip(ys, ys[1:] + ys[:1]):
                    if tuple(sorted(step(G, dict(a))[0].items())) != b:
                        raise Fail('y_i does not follow the pruned tree')
            cc = [c[m] for m in chain]; p = len(cc)
            suf = [prod(cc[:j + 1]) if MUT == 2 else prod(cc[j:]) for j in range(p)]
            for j, m in enumerate(chain):
                if mode.count(m) > suf[j]:
                    raise Fail('too many mode-m_j states')
            if mode.count(0) > prod(cc):
                raise Fail('too many home states')
            if n > prod(cc) + sum(suf):
                raise Fail('cycle longer than bound')
            if n == prod(cc) + sum(suf):
                st['tight'] += 1
            if p >= 2:
                st['chain>=2'] += 1
    except Fail as e:
        st['FAILS'] += 1
        if first is None:
            first = (str(e), show(T))
print('nccheck NT=%d SEED=%d MUT=%d' % (NT, SEED, MUT))
print(st)
if first:
    print('first failure:', first[0], '\n  tree', first[1])
print('RESULT:', 'ALL CHECKS PASSED' if st['FAILS'] == 0 else 'CHECK FAILED')
