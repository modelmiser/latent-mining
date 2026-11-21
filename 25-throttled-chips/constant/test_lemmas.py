#!/usr/bin/env python3
"""Mechanical tests of the lemmas in proofs.txt (this folder).  All lags are integers in FIFTHS.

Strategy(theta): after the add, among vertices with eta >= 2 and lag >= theta, fire one of largest
lag (tie rule: leftmost / rightmost / random); else pass.  theta = 1 is Theorem 2's strategy;
theta = 6 is the lazy variant (same proof), which actually reaches lag >= 6 so the lemmas bite.

Checks (each can be switched to a deliberately WRONG variant with --wrong N, which must FAIL):
 D  fact (d): after-add max lag M >= max(6,theta)  =>  a fire happens and the fired vertex has lag M.
 L  Lemma 1: for a >= max(5, theta-1): in a round with M >= a+1, Phi_a(after round) <=
    Phi_a(before round); in a round with M < a+1 all lags after the round are < a+1.
    [--wrong 1: use a = theta-2.2 and a = theta-3 instead (round with M>=a+1 need not fire).]
 C  Lemma 2 chain: with tau_a = last round with M < a+1 and Z = {z: lag_z(tau_a) > a}:
    for every y with lag_y(t) > a:  lag_y(t) - a <= Phi_a(tau_a) < |Z| ;  a*|Z \\ [y-3,y+3]| < e_y(tau_a)
    unless that set is empty.   [--wrong 2: do not remove [y-3,y+3] from Z.]
 S  static count, any a > 0 on a grid, every round: a * #{z notin [y-3,y+3]: lag_z > a} < e_y or
    the set is empty.           [--wrong 3: window [y-4,y+4] instead of [y-2,y+2] for e_y.]
 T  Theorem A: after each round lag_y < 17 + e_y/10 and eta_y <= t/5 + 34; after each add
    eta_y <= t/5 + 35.2.        [--wrong 4: claim eta_y <= t/5 + 1 instead.]
"""
import sys, random, argparse

def run(lst, theta5, tie, rng, wrong, stats):
    k = {}; F = {}; eta = {}
    def lag(x): return 3*k.get(x,0) + k.get(x-1,0) + k.get(x+1,0) - 5*F.get(x,0)
    lo_a = max(25, theta5-5)
    grid = list(range(lo_a, lo_a+16))            # a in fifths, a >= max(5, theta-1)
    if wrong == 1: grid = [theta5-11, theta5-15]
    phi = {a: 0 for a in grid}
    tau = {a: (0, {}, dict()) for a in grid}     # (phi at tau, Z lags, k snapshot)
    L = {}
    for t, x in enumerate(lst, 1):
        k[x] = k.get(x,0)+1; eta[x] = eta.get(x,0)+1
        for z in (x-1, x, x+1): L[z] = lag(z)
        assert all(v >= 0 for v in L.values()), "fact (a)"
        M = max(L.values())
        stats['maxlag'] = max(stats['maxlag'], M)
        hx = max(eta.values())
        stats['maxexc'] = max(stats['maxexc'], 5*hx - t)
        lim = 5 if wrong == 4 else 176
        assert 5*hx - t <= lim, ("Theorem A after add", t, hx)
        cand = [z for z in L if eta.get(z,0) >= 2 and L[z] >= theta5]
        fired = None
        if cand:
            m = max(L[z] for z in cand)
            top = [z for z in cand if L[z] == m]
            fired = min(top) if tie == 'L' else max(top) if tie == 'R' else rng.choice(top)
        if M >= max(30, theta5):
            assert fired is not None and L[fired] == M, ("fact (d)", t, M)
            stats['d_used'] += 1
        before = {a: sum(v-a for v in L.values() if v > a) for a in grid}
        # before-round value of Phi_a is phi[a]; after add it is `before`
        if fired is not None:
            F[fired] = F.get(fired,0)+1; eta[fired] -= 2
            eta[fired-1] = eta.get(fired-1,0)+1; eta[fired+1] = eta.get(fired+1,0)+1
            L[fired] -= 5
        for a in grid:
            new = sum(v-a for v in L.values() if v > a)
            if M >= a+5:
                assert new <= phi[a], ("Lemma 1 monotone", t, a, M, phi[a], new)
                if phi[a] > 0: stats['L_used'] += 1
            else:
                assert all(v < a+5 for v in L.values()), ("Lemma 1 low round", t, a)
                tau[a] = (new, {z: v for z, v in L.items() if v > a}, dict(k))
            phi[a] = new
        # chain C
        for a in grid:
            pt, Z, ks = tau[a]
            for y, v in L.items():
                if v > a:
                    assert v - a <= pt, ("chain 1", t, a, y)
                    assert pt < 5*len(Z), ("chain 2", t, a, y)
                    Zp = [z for z in Z if (wrong == 2 or abs(z-y) >= 4)]
                    e = sum(c for p, c in ks.items() if abs(p-y) >= 3)
                    if Zp:
                        assert a*len(Zp) < 5*e, ("chain 3", t, a, y, len(Zp), e)
                        stats['C_used'] += 1
        # static S and theorem T (sampled sites: all sites with positive lag)
        hx = max(eta.values())
        stats['maxexc'] = max(stats['maxexc'], 5*hx - t)
        lim = 5 if wrong == 4 else 170
        assert 5*hx - t <= lim, ("Theorem A after round", t, hx)
        if t % 7 == 0 or t == len(lst):
            w = 4 if wrong == 3 else 2
            for y, v in L.items():
                e = sum(c for p, c in k.items() if abs(p-y) > w)
                assert 10*v < 850 + 5*e, ("Theorem A lag", t, y)       # lag < 17 + e/10
                for a in (1, 3, 5, 8, 12, 20):
                    n = sum(1 for z, u in L.items() if u > a and abs(z-y) >= 4)
                    if n:
                        assert a*n < 5*e, ("static S", t, y, a, n, e)
                        stats['S_used'] += 1

def lists(rng, n):
    for w in (1, 2, 3, 5, 9, 30):
        yield [rng.randrange(w) for _ in range(n)]
    yield [0]*n
    yield [i % 5 for i in range(n)]
    yield [(i*7) % 40 for i in range(n)]
    # clusters: a main site plus scattered outside adds
    yield [0 if rng.random() < 0.8 else rng.randrange(10, 60) for _ in range(n)]
    yield [rng.choice((0, 0, 1, 20, 21, 40)) for _ in range(n)]
    lst = []; p = 0
    for i in range(n):
        if rng.random() < 0.05: p = rng.randrange(12)
        lst.append(p)
    yield lst
    # spread first (builds lag by passes), then concentrate
    yield list(range(0, n//2)) + [n//4]*(n - n//2)
    yield list(range(0, n//2)) + [rng.randrange(n//4-2, n//4+3) for _ in range(n - n//2)]

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument('--wrong', type=int, default=0)
    ap.add_argument('--n', type=int, default=400)
    ap.add_argument('--reps', type=int, default=3)
    A = ap.parse_args()
    rng = random.Random(12345)
    for theta5 in (5, 30, 55):
        stats = dict(maxlag=0, maxexc=-10**9, d_used=0, L_used=0, C_used=0, S_used=0)
        nl = 0
        for rep in range(A.reps):
            for lst in lists(rng, A.n):
                for tie in 'LRX':
                    run(lst, theta5, tie, rng, A.wrong, stats); nl += 1
        print("theta=%g lists=%d maxlag=%.1f max(height-t/5)=%.1f exercised: (d) %d, Lemma1 %d, chain %d, static %d"
              % (theta5/5, nl, stats['maxlag']/5, stats['maxexc']/5, stats['d_used'], stats['L_used'],
                 stats['C_used'], stats['S_used']))
    print("ALL CHECKS PASSED (wrong=%d)" % A.wrong)
