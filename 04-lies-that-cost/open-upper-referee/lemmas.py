#!/usr/bin/env python3
"""Referee's tests of the lemma statements as written.  Each test returns a failure count;
'BAD' variants are deliberately false and must return > 0."""
import sys, itertools
from functools import lru_cache
from math import ceil, log2

def clog2(m):
    c = 0
    while (1 << c) < m: c += 1
    return c

def P(n):
    q = 1
    while True:
        if n * (q + 1) + ((q - 1) if n % 2 else 0) <= 2 ** q: return q
        q += 1

# ---------- literal game from the definition (explicit lie sets L, rule (ii) checked directly) ----------
def literal_Q(n, k, qmax):
    """least q <= qmax with a winning questioner strategy, else None.  No state abstraction:
    history = list of (S, answer); V_t recomputed from the definition."""
    U = list(range(n))
    def alive_and_L(hist):
        # returns dict x -> L (forced by (i)), keeping only x satisfying (ii); ells computed on C_{s-1}
        ells = []; C = set(U); Ls = {x: [] for x in U}
        for s, (S, ans) in enumerate(hist, 1):
            ell = min(len(S & C), len(C - S)); ells.append(ell)
            newC = set()
            for x in C:
                lie = ((x in S) != ans)
                L = Ls[x] + ([s] if lie else [])
                ok = all(not any(u < v <= u + k * ells[u - 1] for v in L) for u in L)   # rule (ii)
                if ok: Ls[x] = L; newC.add(x)
            C = newC
        return C
    subsets = [frozenset(c) for r in range(n + 1) for c in itertools.combinations(U, r)]
    def win(hist, j):
        C = alive_and_L(hist)
        if len(C) == 1: return True
        if j == 0: return False
        for S in subsets:
            if not S <= C: continue          # elements outside C are irrelevant
            ok = True
            for ans in (True, False):
                h2 = hist + [(S, ans)]
                if len(alive_and_L(h2)) == 0: continue   # illegal answer
                if not win(h2, j - 1): ok = False; break
            if ok: return True
        return False
    for q in range(qmax + 1):
        if win([], q): return q
    return None

# ---------- labelled obligation game (solver-a P1) ----------
def step(obl, S, ans, k):
    """obl: tuple of obligations of alive candidates; S: set of indices; returns new tuple (alive only)"""
    m = len(obl); ell = min(len(S), m - len(S)); out = []
    for i, r in enumerate(obl):
        truthful = ((i in S) == ans)
        if truthful: out.append((i, max(r - 1, 0)))
        elif r == 0: out.append((i, k * ell))
    return out

def lemmaC_forced(obl, k, S, rounds=3):
    """Play Lemma C's strategy from labelled position obl with first-round set S (later rounds: all S tried
    recursively -> worst case over the questioner's free choice of S AND over the responder).
    Returns max number of questions until |C| = 1 (None if the round fails to confine to one side)."""
    m = len(obl)
    if m == 1: return 0
    worst = 0
    # enumerate responder behaviours over the round
    def rnd(pos, asked):
        # pos: list of (origindex, obligation). returns list of (final positions, questions asked)
        idx = [i for i, _ in pos]
        inS = [i for i in idx if i in S]; outS = [i for i in idx if i not in S]
        if asked == rounds or not inS or not outS:
            return [(pos, asked)]
        res = []
        cur = tuple(r for _, r in pos); curS = {t for t, (i, _) in enumerate(pos) if i in S}
        for ans in (True, False):
            nxt = step(cur, curS, ans, k)
            if not nxt: continue
            res += rnd([(pos[t][0], r) for t, r in nxt], asked + 1)
        return res
    for pos, asked in rnd(list(enumerate(obl)), 0):
        idx = [i for i, _ in pos]
        if any(i in S for i in idx) and any(i not in S for i in idx): return None   # majority argument failed
        if len(pos) > (m + 1) // 2: return None
        sub = lemmaC_worst(tuple(sorted(r for _, r in pos)), k, rounds)
        if sub is None: return None
        worst = max(worst, asked + sub)
    return worst

@lru_cache(maxsize=None)
def lemmaC_worst(obl, k, rounds=3):
    m = len(obl)
    if m == 1: return 0
    worst = 0
    seen = set()
    for S in itertools.combinations(range(m), m // 2):
        sig = tuple(sorted(obl[i] for i in S))
        if sig in seen: continue
        seen.add(sig)
        r = lemmaC_forced(obl, k, set(S), rounds)
        if r is None: return None
        worst = max(worst, r)
    return worst

def test_lemmaC(k, M, R, rounds=3, slack=0):
    fails = 0; n = 0
    for m in range(1, M + 1):
        for obl in itertools.combinations_with_replacement(range(R + 1), m):
            n += 1
            w = lemmaC_worst(obl, k, rounds)
            if w is None or w > 3 * clog2(m) - slack: fails += 1
    return n, fails

# ---------- Lemma S ----------
def lemmaS_split(a, b, j, variant=""):
    if a % 2 == 0:
        return a // 2, (b + 1) // 2
    a1 = (a + 1) // 2
    d = j - 1 if (b - (j - 1)) % 2 == 0 else j - 2
    if variant == "wrongd": d = j - 2 if (b - (j - 1)) % 2 == 0 else j - 1
    if d < 0 or d > b or (b - d) % 2: return None
    return a1, (b - d) // 2

def qk_outcome(a, b, j, a1, b1, ans, k):
    """true Q_k transition on the multiset: free=0, blocked=j (clipped). returns (free, blocked, other) at j-1"""
    a2, b2 = a - a1, b - b1
    ell = min(a1 + b1, a2 + b2)
    tf, tb, lf = (a1, b1, a2) if ans else (a2, b2, a1)
    ob = k * ell
    free, blocked, other = tf, tb, 0
    if ob == 0: free += lf
    elif ob >= j - 1: blocked += lf
    else: other += lf
    if j - 1 == 0:  # everything is 'blocked' = 'free' when no questions remain; report as stated
        pass
    return free, blocked, other

def test_lemmaS(J, k, variant=""):
    fails = 0; n = 0
    for j in range(1, J + 1):
        amax = (2 ** j) // (j + 1)
        for a in range(0, amax + 2):
            lo = max(0, a - 1) if variant != "nonice" else 0
            for b in range(lo, 2 ** j - a * (j + 1) + 1):
                m = a + b
                if m < 2: continue
                if variant != "nom" and m < 2 * j - 2: continue
                if variant == "nom" and m < j: continue
                n += 1
                sp = lemmaS_split(a, b, j, variant)
                if sp is None: fails += 1; continue
                a1, b1 = sp; a2, b2 = a - a1, b - b1
                if min(a1, b1, a2, b2) < 0: fails += 1; continue
                ell = min(a1 + b1, a2 + b2)
                good = (2 * ell >= j - 1) and ell >= 1
                for ans, (fa, fb) in ((True, (a1, b1 + a2)), (False, (a2, b2 + a1))):
                    f, bl, oth = qk_outcome(a, b, j, a1, b1, ans, k)
                    if j - 1 == 0:
                        if f + bl + oth != fa + fb: good = False
                    elif (f, bl, oth) != (fa, fb, 0): good = False
                    if fa + fb < 1: good = False
                    if fb < fa - 1: good = False
                    if fa * j + fb > 2 ** (j - 1): good = False
                if not good: fails += 1
    return n, fails

def test_lemmaS_big(k):
    """boundary sampling at large j (exact integers): m near 2j-2, w near 2^j, b near a-1"""
    fails = 0; n = 0
    for j in list(range(13, 80)) + [100, 200, 257]:
        cands = set()
        for a in list(range(0, 2 * j + 3)) + [2 ** j // (2 * j + 2) + t for t in range(-3, 4)] + [2 ** j // (j + 2) + t for t in range(-3, 2)]:
            if a < 0: continue
            for b in [a - 1, a, a + 1, a + 2, 2 * j - 2 - a, 2 * j - 1 - a, 2 * j - a, 2 ** j - a * (j + 1), 2 ** j - a * (j + 1) - 1, j - 1, j, j + 1]:
                if b >= max(0, a - 1) and a * (j + 1) + b <= 2 ** j and a + b >= max(2, 2 * j - 2): cands.add((a, b))
        for a, b in cands:
            n += 1
            a1, b1 = lemmaS_split(a, b, j) or (-1, -1)
            a2, b2 = a - a1, b - b1
            if min(a1, b1, a2, b2) < 0: fails += 1; continue
            ell = min(a1 + b1, a2 + b2); good = 2 * ell >= j - 1 and ell >= 1
            for ans, (fa, fb) in ((True, (a1, b1 + a2)), (False, (a2, b2 + a1))):
                if qk_outcome(a, b, j, a1, b1, ans, k) != (fa, fb, 0): good = False
                if fb < fa - 1 or fa * j + fb > 2 ** (j - 1) or fa + fb < 1: good = False
            if not good: fails += 1
    return n, fails

# ---------- Theorem 1 arithmetic / coverage ----------
def test_threshold(lo, hi):
    return hi - lo + 1, sum(1 for j in range(lo, hi + 1) if 3 * clog2(max(2 * j - 3, 1)) > j)

def test_cover(J):
    """every nice state with w<=2^j, m>=2, j>=1 falls in exactly one of: Lemma S range, T1 range, Lemma C range"""
    fails = 0; n = 0
    for j in range(1, J + 1):
        for m in range(2, 4 * j + 4):
            n += 1
            S = m >= 2 * j - 2; T = (m <= 2 * j - 3 and j <= 14); C = (m <= 2 * j - 3 and j >= 15 and 3 * clog2(m) <= j)
            if S + T + C != 1: fails += 1
    return n, fails

# ---------- Theorem 2 ----------
def test_thm2(N, k, variant=""):
    fails = 0; n = 0
    for nn in list(range(2, N + 1)) + [2 ** e + t for e in range(14, 120, 7) for t in (-2, -1, 0, 1, 2, 3)]:
        q = P(nn); n += 1
        if nn in (3, 5) and variant != "noexc":
            need = {3: 4, 5: 3}[nn]
            if k < need: continue      # sent to T2
        ell = nn // 2
        good = True
        if k * ell < q - 1: good = False                # liars blocked for good
        if nn % 2 == 0:
            if q > nn + 1: good = False
            outs = [(nn // 2, nn // 2)]
        else:
            if nn >= 7 and q > nn: good = False
            outs = [((nn + 1) // 2, (nn - 1) // 2), ((nn - 1) // 2, (nn + 1) // 2)]
        for a, b in outs:
            if b < a - 1: good = False
            lim = 2 ** (q - 1)
            wt = a * q + b
            if variant == "evenP" and nn % 2:   # BAD: use the even-n formula for q
                q2 = 1
                while nn * (q2 + 1) > 2 ** q2: q2 += 1
                wt = a * q2 + b; lim = 2 ** (q2 - 1)
            if wt > lim: good = False
        if not good: fails += 1
    return n, fails

def report(name, res, expect_fail=False):
    n, f = res
    ok = (f > 0) if expect_fail else (f == 0)
    print(f"{'OK  ' if ok else 'XXXX'} {name}: cases={n} failures={f}" + ("  (deliberately false: must fail)" if expect_fail else ""))
    return ok

if __name__ == "__main__":
    allok = True
    print("literal Q_k(n) from the definition:")
    for (n, k, qmax, exp) in [(2, 2, 4, 3), (3, 2, 5, 5), (3, 3, 5, 5), (4, 2, 5, 5), (2, 1, 6, None), (3, 1, 6, None), (2, 3, 4, 3), (4, 3, 5, 5)]:
        q = literal_Q(n, k, qmax); ok = (q == exp); allok &= ok
        print(f"{'OK  ' if ok else 'XXXX'} literal Q_{k}({n}) = {q} (searched to {qmax}); expected {exp}; P={P(n)}")
    for k in (2, 3, 5):
        allok &= report(f"Lemma C k={k}, m<=7, obligations 0..6, all S, all responders, bound 3ceil(log2 m)", test_lemmaC(k, 7, 6))
    allok &= report("Lemma C BAD k=1", test_lemmaC(1, 6, 3), True)
    allok &= report("Lemma C BAD bound 3ceil(log2 m)-1, k=2", test_lemmaC(2, 6, 3, slack=1), True)
    lemmaC_worst.cache_clear()
    allok &= report("Lemma C BAD two askings instead of three, k=2", test_lemmaC(2, 6, 3, rounds=2), True)
    lemmaC_worst.cache_clear()
    for k in (2, 3, 13):
        allok &= report(f"Lemma S k={k}, all nice states, j<=13", test_lemmaS(13, k))
        allok &= report(f"Lemma S k={k}, boundary samples j up to 257", test_lemmaS_big(k))
    allok &= report("Lemma S BAD k=1", test_lemmaS(11, 1), True)
    allok &= report("Lemma S BAD without niceness", test_lemmaS(11, 2, "nonice"), True)
    allok &= report("Lemma S BAD with m>=j instead of m>=2j-2", test_lemmaS(11, 2, "nom"), True)
    allok &= report("Lemma S BAD d parity swapped", test_lemmaS(11, 2, "wrongd"), True)
    allok &= report("Thm 1 threshold 3ceil(log2(2j-3)) <= j, j=15..200000", test_threshold(15, 200000))
    allok &= report("Thm 1 threshold BAD from j=14", test_threshold(14, 200000), True)
    allok &= report("Thm 1 three ranges cover each (m,j) exactly once, j<=3000", test_cover(3000))
    for k in (2, 3, 4, 9):
        allok &= report(f"Thm 2 first question k={k}, n<=200000 + samples to 2^119", test_thm2(200000, k))
    allok &= report("Thm 2 BAD k=2 without the exceptions n=3,5", test_thm2(100, 2, "noexc"), True)
    allok &= report("Thm 2 BAD k=1", test_thm2(100, 1), True)
    allok &= report("Thm 2 BAD even-n formula for odd n", test_thm2(5000, 2, "evenP"), True)
    print("ALL AS EXPECTED" if allok else "SOMETHING UNEXPECTED")
