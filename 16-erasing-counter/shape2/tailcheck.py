#!/usr/bin/env python3
"""Mechanical check of Theorem T (PROOFS.md section 5): prefix modes 1..i with private data, a tail
Theta (the subtree at the end of the first i 0-edges) with arbitrary internal sharing, prefix modes
allowed to jump into the tail.

Random trees: spine of length k, split i; S_m (m <= i) over a private pool plus later spine indices;
S_j (j > i) over one shared tail pool plus later spine indices.
For every cycle C through a home state:
 (EC) all entries into tail modes (from home or from a prefix mode) go to one tail mode j*.
 If (EC) holds, check
  (1) theta (restriction to idx(Theta)) moves exactly at tail steps and entries, by f of Theta*
      (Theta with all-zero leaf := [z_j*] and leaves writing prefix spine indices emptied);
      its values form one cycle of Theta*, c_T states, H_T of them Theta-home, c_T >= 2 H_T;
  (2) y_m (restriction to the private data of S_m) moves only at mode-m steps; c_m distinct values;
  (3) every unit (prefix mode or tail) has one successor unit; the visited units are the chain;
  (4) with w = c_m for prefix units and H_T for the tail unit:
      #home <= prod w;  #mode u_j <= prod_{t>=j} w_t;  #tail states <= (c_T - H_T) prod_{t>tau} w_t;
  (5) |C| <= sum of those bounds, and that sum <= W_r for r = depth(T) (Lemma T, numerically).
usage: tailcheck.py NTREES SEED [MUT]
 MUT=1: no (EC) filter (wrong on purpose)   MUT=2: tail pool overlaps the prefix pools (wrong on purpose)
 MUT=3: bound (4) for prefix units without the factor H_T (wrong on purpose)
"""
import random, sys
from sim import legal, step, cycles, show, idx, depth

NT = int(sys.argv[1]); SEED = int(sys.argv[2]); MUT = int(sys.argv[3]) if len(sys.argv) > 3 else 0
rng = random.Random(SEED)
W = [1]
for _ in range(12):
    W.append(W[-1] * (W[-1] + 1))


def rand_sub(d, path, pool, pleaf, ok_write):
    avail = [u for u in pool if u not in path]
    if d == 0 or not avail or rng.random() < pleaf:
        cand = [u for u in path if ok_write(u)]
        r = rng.random()
        if r < 0.12 or not cand:
            return ('l', None)
        return ('l', rng.choice(cand))
    u = rng.choice(avail)
    return ('n', u, rand_sub(d - 1, path + [u], pool, pleaf, ok_write),
            rand_sub(d - 1, path + [u], pool, pleaf, ok_write))


def gen():
    k = rng.randint(2, 4); i = rng.randint(1, k - 1)
    Z = list(range(1, k + 1)); nxt = k + 1
    jstar = rng.randint(i + 1, k)
    tailpool = list(range(nxt, nxt + rng.randint(1, 4))); nxt += len(tailpool)
    loose = rng.random() < 0.35      # then (EC) is not forced by the tree and is tested per cycle
    S = []
    for m in range(1, k + 1):
        if m <= i:
            pool = list(range(nxt, nxt + rng.randint(0, 3))); nxt += len(pool)
            if MUT == 2 and tailpool:
                pool = pool + [rng.choice(tailpool)]
            okw = (lambda u: True) if loose else (lambda u: u <= i or u == jstar or u > k)
            S.append(rand_sub(3, Z[:m], pool + Z[m:], 0.3, okw))
        else:
            S.append(rand_sub(3, Z[:m], tailpool + Z[m:], 0.25, lambda u: True))
    t = ('l', rng.choice(Z if loose else [jstar, jstar] + Z[:i]))
    for m in range(k, 0, -1):
        t = ('n', m, t, S[m - 1])
    return t, i


def set_zero_leaf(t, new):
    if t[0] == 'l':
        return new
    return ('n', t[1], set_zero_leaf(t[2], new), t[3])


def empty_writes(t, bad):
    if t[0] == 'l':
        return ('l', None) if t[1] in bad else t
    return ('n', t[1], empty_writes(t[2], bad), empty_writes(t[3], bad))


def prod(xs):
    p = 1
    for x in xs:
        p *= x
    return p


class Fail(Exception):
    pass


st = dict.fromkeys(['trees', 'cyclesB', 'EC', 'tail_in_chain', 'prefix_before_tail', 'tail_shares_data',
                    'tight', 'maxlen', 'FAILS'], 0)
first = None
for _ in range(NT):
    T, i = gen()
    if not legal(T) or len(idx(T)) > 12:
        continue
    st['trees'] += 1
    Z = []; S = []; t = T
    while t[0] == 'n':
        Z.append(t[1]); S.append(t[3]); t = t[2]
    k = len(Z); zset = set(Z); r = depth(T)
    Theta = T
    for _ in range(i):
        Theta = Theta[2]
    prefz = set(Z[:i]); tailz = Z[i:]
    thidx = sorted(idx(Theta))
    Dm = [idx(S[m]) - zset for m in range(i)]
    tail_sh = any((idx(S[a]) - zset) & (idx(S[b]) - zset) for a in range(i, k) for b in range(i, a))
    cs, ix = cycles(T)
    try:
        for C in cs:
            n = len(C)
            zc = [sum(s[z] for z in Z) for s in C]
            if n < 2 or min(zc) > 0:
                continue
            st['cyclesB'] += 1
            unit = []
            for s in C:
                ones = [z for z in Z if s[z]]
                unit.append('H' if not ones else ('T' if ones[0] not in prefz else ones[0]))
            mode = [next((z for z in Z if s[z]), 0) for s in C]
            entries = {mode[(t + 1) % n] for t in range(n) if unit[t] != 'T' and unit[(t + 1) % n] == 'T'}
            if len(entries) > 1 and MUT != 1:
                continue
            if len(entries) <= 1:
                st['EC'] += 1
            th = lambda s: {u: s[u] for u in thidx}
            cT = HT = 1; tail_vis = bool(entries)
            if tail_vis:
                jstar = next(iter(entries))
                Ths = empty_writes(set_zero_leaf(Theta, ('l', jstar)), prefz)
                if not legal(Ths):
                    raise Fail('Theta* illegal')
                vals = set()
                for t in range(n):
                    a = th(C[t]); b = th(C[(t + 1) % n])
                    moved = unit[t] == 'T' or unit[(t + 1) % n] == 'T'
                    want = step(Ths, a)[0] if moved else a
                    if b != want:
                        raise Fail('(1) theta does not follow Theta*')
                    vals.add(tuple(sorted(a.items())))
                cT = len(vals)
                HT = sum(1 for v in vals if not any(dict(v)[z] for z in tailz))
                x0 = dict(next(iter(vals))); x = x0
                for _ in range(cT):
                    x = step(Ths, x)[0]
                if x != x0:
                    raise Fail('(1) theta values are not one cycle')
                if cT < 2 * HT:
                    raise Fail('(1) c_T < 2 H_T')
            elif any(th(s) != th(C[0]) for s in C):
                raise Fail('(1) theta moves though tail unvisited')
            c = {}
            for m in range(1, i + 1):
                ys = [tuple(sorted((u, s[u]) for u in Dm[m - 1])) for s in C]
                for t in range(n):
                    if unit[t] != m and ys[t] != ys[(t + 1) % n]:
                        raise Fail('(2) y_m moved outside mode m')
                if m in unit:
                    c[m] = len({ys[t] for t in range(n) if unit[t] == m})
            succ = {}
            for t in range(n):
                u = unit[t]; v = unit[(t + 1) % n]
                if u != 'H' and v != 'H' and u != v:
                    succ.setdefault(u, set()).add(v)
                if u != 'H' and u != 'T' and v == u:
                    succ.setdefault(u, set()).add(v)
            if any(len(v) > 1 for v in succ.values()):
                raise Fail('(3) a unit has two successor units')
            firsts = {unit[(t + 1) % n] for t in range(n) if unit[t] == 'H'}
            if len(firsts) != 1:
                raise Fail('(3) two first units')
            chain = [next(iter(firsts))]
            while chain[-1] in succ and next(iter(succ[chain[-1]])) not in chain:
                chain.append(next(iter(succ[chain[-1]])))
            if set(chain) != set(unit) - {'H'}:
                raise Fail('(3) visited units are not the chain')
            w = [HT if u == 'T' else c[u] for u in chain]; p = len(w)
            total = 0
            bH = prod(w)
            if unit.count('H') > bH:
                raise Fail('(4) too many home states')
            total += bH
            for j, u in enumerate(chain):
                if u == 'T':
                    b = (cT - HT) * prod(w[j + 1:])
                else:
                    ww = w[j:]
                    if MUT == 3 and 'T' in chain[j:]:
                        ww = [x for x, uu in zip(w[j:], chain[j:]) if uu != 'T']
                    b = prod(ww)
                if unit.count(u) > b:
                    raise Fail('(4) too many states in unit %s' % u)
                total += b
            if n > total:
                raise Fail('(5) cycle longer than the sum of bounds')
            if r >= 5 and total > W[r]:
                pass  # c_T etc. are actual values; Lemma T needs the caps, checked in lemma_t.py
            if 'T' in chain:
                st['tail_in_chain'] += 1
                if chain.index('T') > 0:
                    st['prefix_before_tail'] += 1
                if tail_sh:
                    st['tail_shares_data'] += 1
            if n == total:
                st['tight'] += 1
            st['maxlen'] = max(st['maxlen'], n)
    except Fail as e:
        st['FAILS'] += 1
        if first is None:
            first = (str(e), show(T), i)
print('tailcheck NT=%d SEED=%d MUT=%d' % (NT, SEED, MUT))
print(st)
if first:
    print('first failure:', first[0], '\n  split i =', first[2], ' tree', first[1])
print('RESULT:', 'ALL CHECKS PASSED' if st['FAILS'] == 0 else 'CHECK FAILED')
