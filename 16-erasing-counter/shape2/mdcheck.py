#!/usr/bin/env python3
"""Mechanical check of Theorem MD (PROOFS.md) on random trees whose subtrees S_i off the all-zero path
have pairwise disjoint data index sets.

For every cycle C (length >= 2) of every generated tree:
 Case A (no state with all Z bits 0): data bits constant on C and |C| <= 2^k.
 Case B: (1) every state has at most one 1 on Z;
         (2) a mode-i step changes the data only inside D_i, and there by g_i = f_{G_i};
         (3) the D_i-parts seen in mode i form one cycle C_i of g_i (length c_i);
         (4) at most one state of C_i is an overflow (step of G_i ends at an empty leaf), and every
             non-overflow mode-i state is followed by a home state;
         (5) the visited modes form a chain m_1..m_p (m_1 = mode written by the all-zero leaf,
             m_(j+1) = overflow target of m_j), and in every mode-m_j state the D-part of every
             earlier chain mode m_t equals g(e_t);
         (6) #mode-m_j states <= prod_{t>=j} c_(m_t), #home states <= prod_t c_(m_t);
         (7) |C| <= prod_t c_(m_t) + sum_j prod_{t>=j} c_(m_t).
usage: mdcheck.py NTREES SEED [MUT]
 MUT=1: data pools of different S_i overlap (hypothesis violated on purpose)
 MUT=2: bound (7) without the first product (wrong on purpose)
 MUT=3: bound (6)/(7) with prefix instead of suffix products (wrong on purpose)
"""
import random, sys
from sim import legal, step, cycles, L, depth, show, idx

NT = int(sys.argv[1]); SEED = int(sys.argv[2]); MUT = int(sys.argv[3]) if len(sys.argv) > 3 else 0
rng = random.Random(SEED)


def rand_sub(d, path, pool, pleaf, wz):
    """random tree over 'pool', leaves write nothing or an index of the path (biased)"""
    avail = [u for u in pool if u not in path]
    if d == 0 or not avail or rng.random() < pleaf:
        r = rng.random()
        if r < 0.15 or not path:
            return ('l', None)
        zs = [u for u in path if u in wz]
        if zs and r < 0.5:
            return ('l', rng.choice(zs))
        return ('l', rng.choice(path))
    u = rng.choice(avail)
    return ('n', u, rand_sub(d - 1, path + [u], pool, pleaf, wz), rand_sub(d - 1, path + [u], pool, pleaf, wz))


def counter(pool):
    """read-once counter (construction Q of Theorem RO) on a random shape over fresh indices of pool;
    returns (tree, all-ones leaf marker replaced later). Leaves: ('l', w)."""
    def q(d):
        if d == 0 or not pool or rng.random() < 0.3:
            return ('l', None)
        a = pool.pop()
        t1 = q(d - 1); t0 = q(d - 1)
        return ('n', a, set_all_ones(t0, a), t1)
    return q(2)


def set_all_ones(t, w):
    if t[0] == 'l':
        return ('l', w)
    return ('n', t[1], t[2], set_all_ones(t[3], w))


def gen():
    k = rng.randint(1, 4)
    Z = list(range(1, k + 1))
    nxt = k + 1
    pools = []
    budget = 11 - k
    for i in range(k):
        n = rng.randint(0, min(3, budget)); budget -= n
        pools.append(list(range(nxt, nxt + n))); nxt += n
    if MUT == 1:
        allp = sorted(set(sum(pools, [])))
        pools = [allp[:3] for _ in range(k)]
    S = []
    structured = rng.random() < 0.6
    for i in range(k):
        path = Z[:i + 1]
        later = Z[i + 1:]
        if structured:
            c = counter(list(pools[i]))
            tgt = rng.choice(Z + [None])
            if tgt is None or tgt <= Z[i]:
                s = set_all_ones(c, tgt)
            else:  # ascent: the overflow leaf becomes a node reading z_tgt
                s = set_all_ones(c, ('X', tgt))
                def fix(t):
                    if t[0] == 'l':
                        if isinstance(t[1], tuple):
                            return ('n', t[1][1], ('l', t[1][1]), ('l', None))
                        return t
                    return ('n', t[1], fix(t[2]), fix(t[3]))
                s = fix(s)
            if rng.random() < 0.3:  # perturb: replace by a random subtree sometimes
                s = rand_sub(3, path, pools[i] + later, 0.25, set(Z))
        else:
            s = rand_sub(3, path, pools[i] + later, 0.25, set(Z))
        S.append(s)
    lam = ('l', rng.choice(Z + Z + [None]))
    t = lam
    for i in range(k - 1, -1, -1):
        t = ('n', Z[i], t, S[i])
    return t


def prune(t, zset):
    if t[0] == 'l':
        return ('l', None) if t[1] in zset else t
    if t[1] in zset:
        return prune(t[2], zset)
    return ('n', t[1], prune(t[2], zset), prune(t[3], zset))


def prod(xs):
    p = 1
    for x in xs:
        p *= x
    return p


class Fail(Exception):
    pass


def check_tree(T, stats):
    Z = []; S = []; t = T
    while t[0] == 'n':
        Z.append(t[1]); S.append(t[3]); t = t[2]
    lam = t; k = len(Z); zset = set(Z)
    D = [idx(s) - zset for s in S]
    disjoint = all(not (D[i] & D[j]) for i in range(k) for j in range(i))
    if MUT != 1 and not disjoint:
        raise Fail('generator produced overlapping data sets')
    G = [prune(s, zset) for s in S]
    for g in G:
        if not legal(g):
            raise Fail('G_i illegal')
    cs, ix = cycles(T)
    LT = L(T)
    for C in cs:
        n = len(C)
        if n < 2:
            continue
        stats['cycles'] += 1
        if n > LT:
            stats['P1viol'] += 1
        zc = [sum(s[z] for z in Z) for s in C]
        if min(zc) > 0:
            stats['caseA'] += 1
            for s in C:
                if any(s[u] != C[0][u] for u in ix if u not in zset):
                    raise Fail('case A: data changes')
            if n > 2 ** k:
                raise Fail('case A: longer than 2^k')
            continue
        if max(zc) > 1:
            raise Fail('case B: two ones on Z')
        mode = [0 if zc[t] == 0 else 1 + [s[z] for z in Z].index(1) for t, s in enumerate(C)]  # 1-based
        H = mode.count(0)
        part = lambda s, i: tuple(sorted((u, s[u]) for u in D[i - 1]))
        seen = {}      # mode -> list of D_i-parts in cyclic order
        ovf = {}       # mode -> set of overflow parts
        a = {}         # mode -> overflow target (0 = home)
        for t in range(n):
            s = C[t]; s2 = C[(t + 1) % n]; i = mode[t]
            if i == 0:
                if any(s[u] != s2[u] for u in ix if u not in zset):
                    raise Fail('home step changes data')
                continue
            xi = {u: s[u] for u in D[i - 1]}
            gi, leaf = step(G[i - 1], xi)
            for u in ix:
                if u in zset:
                    continue
                want = gi[u] if u in D[i - 1] else s[u]
                if s2[u] != want:
                    raise Fail('(2) mode step is not g_i on D_i / identity elsewhere')
            seen.setdefault(i, []).append(part(s, i))
            if leaf[1] is None:
                ovf.setdefault(i, set()).add(part(s, i))
                tgt = mode[(t + 1) % n]
                if a.setdefault(i, tgt) != tgt:
                    raise Fail('(4) two overflow targets')
            elif mode[(t + 1) % n] != 0:
                raise Fail('(4) non-overflow step not followed by home')
        c = {}
        for i, xs in seen.items():
            c[i] = len(set(xs))
            # (3): one cycle of g_i
            x0 = dict(xs[0]); x = x0
            for _ in range(c[i]):
                x = step(G[i - 1], x)[0]
            if x != x0 or len(xs) % c[i] != 0:
                raise Fail('(3) D_i-parts are not one cycle of g_i')
            if len(ovf.get(i, ())) > 1:
                raise Fail('(4) two overflow states')
        # (5) chain
        j0 = Z.index(lam[1]) + 1
        chain = [j0]
        while chain[-1] in a and a[chain[-1]] != 0 and a[chain[-1]] not in chain:
            chain.append(a[chain[-1]])
        if set(seen) != set(chain):
            raise Fail('(5) visited modes are not the chain')
        ge = {}
        for i in chain:
            if i in ovf:
                e = dict(next(iter(ovf[i])))
                ge[i] = tuple(sorted(step(G[i - 1], e)[0].items()))
        for t in range(n):
            i = mode[t]
            if i == 0:
                continue
            j = chain.index(i)
            for m in chain[:j]:
                if part(C[t], m) != ge[m]:
                    raise Fail('(5) earlier chain mode not at g(e)')
        p = len(chain)
        cc = [c[m] for m in chain]
        if MUT == 3:
            suf = [prod(cc[:j + 1]) for j in range(p)]
        else:
            suf = [prod(cc[j:]) for j in range(p)]
        for j, m in enumerate(chain):
            if len(seen[m]) > suf[j]:
                raise Fail('(6) too many mode-m_j states')
        if H > prod(cc):
            raise Fail('(6) too many home states')
        bound = (0 if MUT == 2 else prod(cc)) + sum(suf)
        if n > bound:
            raise Fail('(7) cycle longer than bound')
        stats['caseB'] += 1
        if n == bound:
            stats['tight'] += 1
        if p >= 2:
            stats['chain>=2'] += 1
        if p >= 3:
            stats['chain>=3'] += 1
        if any(chain[j + 1] > chain[j] for j in range(p - 1)):
            stats['ascent'] += 1
        stats['maxlen'] = max(stats['maxlen'], n)


stats = dict.fromkeys(['trees', 'cycles', 'caseA', 'caseB', 'tight', 'chain>=2', 'chain>=3', 'ascent',
                       'P1viol', 'maxlen', 'FAILS'], 0)
firstfail = None
for _ in range(NT):
    T = gen()
    assert legal(T)
    stats['trees'] += 1
    try:
        check_tree(T, stats)
    except Fail as e:
        stats['FAILS'] += 1
        if firstfail is None:
            firstfail = (str(e), show(T))
print('mdcheck NT=%d SEED=%d MUT=%d' % (NT, SEED, MUT))
print(stats)
if firstfail:
    print('first failure:', firstfail[0], '\n  tree', firstfail[1])
print('RESULT:', 'ALL CHECKS PASSED' if stats['FAILS'] == 0 else 'CHECK FAILED')
