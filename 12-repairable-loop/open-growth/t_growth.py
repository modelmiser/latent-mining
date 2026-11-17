#!/usr/bin/env python3
# Mechanical tests of the growth lemmas G2, G3, G4 (see proofs.txt).
import sys, random, itertools
from og import *
rng = random.Random(12345)
R = load_repairable()
print("repairable loops loaded:", {n: len(v) for n, v in R.items()})
def xvec(w, n, k):
    c = counts(w, k); b = 1 << (n - k)
    return {f: c.get(f, 0) - b for f in itertools.product((0, 1), repeat=k)}
def through_and_junction(w, s, k):
    m = len(w)
    F = [tuple(w[(s - k + 1 + i + j) % m] for j in range(k)) for i in range(k)]            # windows through s
    w1 = loss(w, s); m1 = m - 1
    # junction windows of w1: contain cells s-1 and s (old s+1); starts s-k+1 .. s-1  (k-1 windows)
    J = [tuple(w1[(s - k + 1 + i + j) % m1] for j in range(k)) for i in range(k - 1)]
    return F, J
def g3_stats(w, n, rhs_base=1, use_j=True):
    """returns (#tests, #violations) of  sum_{f in F_s} x(f) >= rhs_base - #junction windows with word in F_s"""
    tests = viol = 0
    for k in range(1, n + 1):
        x = xvec(w, n, k)
        for s in singletons(w):
            F, J = through_and_junction(w, s, k)
            Fs = set(F); jc = sum(1 for g in J if g in Fs) if use_j else 0
            tests += 1
            if sum(x[f] for f in Fs) < rhs_base - jc: viol += 1
    return tests, viol
# --- G2 and G3 on every known repairable loop
for n in sorted(R):
    g2t = g2v = 0; t3 = v3 = 0; t3a = v3a = 0; t3b = v3b = 0
    for s in R[n]:
        w = T(s)
        for k in range(1, n + 1):
            x = xvec(w, n, k); g2t += 1
            if sum(-v for v in x.values() if v < 0) > k - 1: g2v += 1
        a, b = g3_stats(w, n); t3 += a; v3 += b
        a, b = g3_stats(w, n, rhs_base=2); t3a += a; v3a += b
        a, b = g3_stats(w, n, use_j=False); t3b += a; v3b += b
    print(f"n={n}: G2 tests={g2t} violations={g2v} | G3 tests={t3} violations={v3} | FALSE variant 'rhs 2': violations={v3a} | FALSE variant 'ignore junction': violations={v3b}")
# --- G3 must FAIL on non-repairable loops (it is a real constraint)
for n in (4, 5, 6, 7):
    nb = 0; tot = 0
    for _ in range(20):
        D = random_debruijn(n, rng); tot += 1
        if g3_stats(D, n)[1] > 0: nb += 1
    print(f"n={n}: G3 violated on {nb} of {tot} random de Bruijn cycles (not repairable; expected > 0)")
# --- G4: number of k-bad singletons of ANY loop
def bad_count(w, k):
    nb = 0
    for s in singletons(w):
        F, J = through_and_junction(w, s, k); Fs = set(F)
        if any(g in Fs for g in J): nb += 1
    return nb
def g4_bound(n, k, L, distinct, variant=0):
    m = n - k
    tot = 0
    for d in range(2, k):
        e = max(m, d) - 1
        if variant == 1: e = min(m, d) - 1          # false variant: min instead of max
        tot += (k - d) * (2 ** e)
    rterm = 2 * (k - 1) * (L - distinct)
    if variant == 2: rterm = 0                       # false variant: drop the repeat term
    return 2 * tot + rterm
def g4_test(name, gen, trials):
    t = v = v1 = v2 = 0; worst = 0.0
    for _ in range(trials):
        n, w = gen()
        if len(w) <= 2 * n + 2: continue
        distinct = len(set(words(w, n)))
        for k in range(2, n + 1):
            bc = bad_count(w, k); t += 1
            b0 = g4_bound(n, k, len(w), distinct)
            if bc > b0: v += 1
            if b0 > 0: worst = max(worst, bc / b0)
            if bc > g4_bound(n, k, len(w), distinct, 1): v1 += 1
            if bc > g4_bound(n, k, len(w), distinct, 2): v2 += 1
    print(f"G4 on {name}: tests={t} violations={v} (max actual/bound={worst:.3f}) | FALSE 'min' variant violations={v1} | FALSE 'no repeat term' violations={v2}")
def gen_db():
    n = rng.choice((4, 5, 6, 7, 8, 9, 10)); return n, random_debruijn(n, rng)
def gen_db_edit():
    n = rng.choice((4, 5, 6, 7, 8, 9)); w = random_debruijn(n, rng)
    for _ in range(rng.randint(1, 6)):
        k = rng.randrange(len(w)); w = loss(w, k) if rng.random() < .5 else dbl(w, k)
    return n, w
def gen_rand():
    n = rng.choice((3, 4, 5, 6, 7)); L = rng.randint(2 * n + 3, 3 * 2 ** n)
    return n, tuple(rng.randint(0, 1) for _ in range(L))
def gen_periodic():
    n = rng.choice((3, 4, 5, 6, 7)); p = rng.randint(2, 9); blk = tuple(rng.randint(0, 1) for _ in range(p))
    return n, blk * rng.randint(4, 30)
def gen_rep():
    n = rng.choice(sorted(R)); return n, T(rng.choice(R[n]))
g4_test("random de Bruijn cycles n=4..10", gen_db, 60)
g4_test("de Bruijn + 1..6 random edits n=4..9", gen_db_edit, 80)
g4_test("random loops n=3..7", gen_rand, 150)
g4_test("periodic loops n=3..7", gen_periodic, 150)
g4_test("known repairable loops", gen_rep, 150)
# --- exhaustive G4 for all loops of length 9..15, n = 3,4
t = v = 0
for L in range(9, 16):
    for w in itertools.product((0, 1), repeat=L):
        if w[0] == 1: continue
        for n in (3, 4):
            if L <= 2 * n + 2: continue
            distinct = len(set(words(w, n)))
            for k in range(2, n + 1):
                t += 1
                if bad_count(w, k) > g4_bound(n, k, L, distinct): v += 1
print(f"G4 exhaustive, all loops of length 9..15, n=3,4: tests={t} violations={v}")
