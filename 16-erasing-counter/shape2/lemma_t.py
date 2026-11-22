#!/usr/bin/env python3
"""Numerical check of Lemma T (PROOFS.md section 5):
for r, 1 <= i <= r, distinct prefix modes u in {1..i} with 1 <= c_u <= W_(r-u), optionally a tail unit at
chain position tau with 2 <= c_T <= W_(r-i), 1 <= H_T <= c_T/2:
   X = prod w + sum_{prefix j} prod_{t>=j} w_t + (c_T - H_T) prod_{t>tau} w_t  <=  W_r,
(w = c_u for prefix units, H_T for the tail).  Exhaustive over chains for r <= 5 with every c at a few
values including its maximum; random for r <= 8.
usage: lemma_t.py SEED [MUT]   MUT=1: allow H_T up to c_T - 1 (wrong on purpose)
                               MUT=2: cap c_u by W_(r-u+1) (wrong on purpose)
"""
import sys, random, itertools
SEED = int(sys.argv[1]); MUT = int(sys.argv[2]) if len(sys.argv) > 2 else 0
rng = random.Random(SEED)
W = [1]
for _ in range(10):
    W.append(W[-1] * (W[-1] + 1))


def prod(xs):
    p = 1
    for x in xs:
        p *= x
    return p


def X(chain, c, cT, HT):
    w = [HT if u == 'T' else c[u] for u in chain]
    tot = prod(w)
    for j, u in enumerate(chain):
        tot += (cT - HT) * prod(w[j + 1:]) if u == 'T' else prod(w[j:])
    return tot


def cap(r, u):
    return W[r - u + 1] if MUT == 2 else W[r - u]


n = bad = 0; worst = 0.0; ex = None
# exhaustive part
for r in range(1, 6):
    for i in range(1, r + 1):
        for q in range(0, i + 1):
            for Q in itertools.permutations(range(1, i + 1), q):
                for tau in [None] + list(range(q + 1)):
                    chain = list(Q) if tau is None else list(Q[:tau]) + ['T'] + list(Q[tau:])
                    if not chain:
                        continue
                    opts = [sorted({1, max(1, cap(r, u) // 2), cap(r, u)}) for u in Q]
                    for cs in itertools.product(*opts):
                        c = dict(zip(Q, cs))
                        tl = [(1, 1)]
                        if tau is not None:
                            m = W[r - i]
                            if m < 2:
                                continue
                            tl = [(m, h) for h in sorted({1, (m - 1) if MUT == 1 else m // 2})] + [(2, 1)]
                        for cT, HT in tl:
                            x = X(chain, c, cT, HT); n += 1
                            if x > W[r]:
                                bad += 1
                                if ex is None:
                                    ex = (r, i, chain, c, cT, HT, x, W[r])
                            worst = max(worst, x / W[r])
nex = n
# random part
for _ in range(200000):
    r = rng.randint(1, 8); i = rng.randint(1, r)
    Q = rng.sample(range(1, i + 1), rng.randint(0, i))
    c = {u: (cap(r, u) if rng.random() < 0.6 else rng.randint(1, cap(r, u))) for u in Q}
    chain = list(Q); cT = HT = 1
    if rng.random() < 0.7 and W[r - i] >= 2:
        cT = W[r - i] if rng.random() < 0.6 else rng.randint(2, W[r - i])
        hmax = (cT - 1) if MUT == 1 else cT // 2
        HT = hmax if rng.random() < 0.6 else rng.randint(1, hmax)
        chain.insert(rng.randint(0, len(Q)), 'T')
    if not chain:
        continue
    x = X(chain, c, cT, HT); n += 1
    if x > W[r]:
        bad += 1
        if ex is None:
            ex = (r, i, chain, c, cT, HT, x, W[r])
    worst = max(worst, x / W[r])
print('lemma_t SEED=%d MUT=%d: exhaustive cases %d, random cases %d, violations %d, max X/W_r %.6f' % (SEED, MUT, nex, n - nex, bad, worst))
if ex:
    print('first violation (r, i, chain, c, c_T, H_T, X, W_r):', ex)
print('RESULT:', 'ALL CHECKS PASSED' if bad == 0 else 'CHECK FAILED')
