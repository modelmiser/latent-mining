#!/usr/bin/env python3
"""Mechanical small-case tests of the solvers' lemmas, using only chk.py (referee's definitional code)."""
import sys, random, itertools
from chk import Csets, g_of, reach, legal

def windows(w, tr, start=0):
    seen = set()
    for x in itertools.product((0, 1), repeat=w):
        q = start
        for b in x: q = tr[q][b]
        seen.add((q, x))
    st = list(seen)
    while st:
        q, x = st.pop()
        for b in (0, 1):
            y = (tr[q][b], x[1:] + (b,))
            if y not in seen: seen.add(y); st.append(y)
    return seen

def counter(K):  # states 0..K saturating
    return [(max(k - 1, 0), min(k + 1, K)) for k in range(K + 1)]

def split_end(w):
    """solver-a S_w from the prose: levels 0..T=w-2; B0,B1 bottom, T0,T1 top. Returns tr and names; start B0."""
    T = w - 2
    names = ['B0', 'B1'] + [('L', j) for j in range(1, T)] + ['T1', 'T0']
    idx = {nm: i for i, nm in enumerate(names)}
    def lvl(j, came):  # state for level j, came = 'up' (arrived by a 1) or 'down' or 'stay'
        if j == 0: return idx['B0'] if came == 'stay' else idx['B1']
        if j == T: return idx['T0'] if came == 'stay' else idx['T1']
        return idx[('L', j)]
    tr = []
    for nm in names:
        j = 0 if nm in ('B0', 'B1') else T if nm in ('T0', 'T1') else nm[1]
        z = lvl(0, 'stay') if j == 0 else lvl(j - 1, 'down')
        o = lvl(T, 'stay') if j == T else lvl(j + 1, 'up')
        tr.append((z, o))
    return tr, names

def bouncy(w):
    K = w
    tr = []
    for k in range(K + 1):
        z = 0 if k == 0 else K - 2 if k == K else k - 1
        o = 2 if k == 0 else K if k == K else k + 1
        tr.append((z, o))
    return tr

def m(x): return min(x, 1 + (x + 1) // 2)
bad = 0
def fail(msg):
    global bad; bad += 1; print("FAIL", msg)

# ---- P1 (solver-a) per-state ranges, from EVERY start state
for w in range(3, 12):
    tr, names = split_end(w); T = w - 2; k = (w - 1) // 2
    g, sp, C = g_of(w, tr, 0)
    if g != k: fail("P1 g(S_%d)=%s" % (w, g))
    for start in range(len(tr)):
        C = Csets(w, tr, start)
        for i, nm in enumerate(names):
            if not C[i]: continue
            lo, hi = min(C[i]), max(C[i])
            if nm == 'B0': ok = hi <= k
            elif nm == 'B1': ok = lo >= 1 and hi <= w // 2
            elif nm == 'T0': ok = lo >= w - k
            elif nm == 'T1': ok = hi <= w - 1 and lo >= w - w // 2
            else:
                j = nm[1]; ok = lo >= m(j) and hi <= w - m(T - j)
                if w != 4 and hi - lo > k: ok = False
            if not ok: fail("P1 w=%d start=%d state=%s C=%s" % (w, start, nm, sorted(C[i])))
print("P1 ranges tested w=3..11, all start states")

# ---- Theorem A (solver-b) L(k) bounds for every K, every start
for w in range(1, 11):
    for K in range(1, w + 1):
        tr = counter(K)
        L = lambda k: min(k, -((-(k + w - K)) // 2))
        for start in range(K + 1):
            C = Csets(w, tr, start)
            for k in range(K + 1):
                if C[k] and not (min(C[k]) >= L(k) and max(C[k]) <= w - L(K - k)):
                    fail("ThmA w=%d K=%d k=%d C=%s" % (w, K, k, sorted(C[k])))
    K = w // 2 + 1
    if K <= w or w == 1:
        g, sp, C = g_of(w, counter(K))
        if max(x for x in sp if x is not None) > w // 2: fail("ThmA maxspread w=%d" % w)
    # author's claims
    g, sp, C = g_of(w, counter(w))
    if g != w // 2 or max(sp) != w // 2: fail("author counter w=%d g=%s sp=%s" % (w, g, sp))
    tro = [(0, min(i + 1, w)) for i in range(w + 1)]  # trailing ones up to w
    g, sp, C = g_of(w, tro)
    if g != w - 1 and w > 1: fail("author trailing-ones w=%d g=%s" % (w, g))
    if w == 1: print("trailing ones w=1 g=", g)
print("Theorem A + author's two machines tested w=1..10")

# ---- Theorem D (solver-b) ranges for B_w, every start
for w in range(4, 13, 2):
    t = w // 2; tr = bouncy(w)
    g, sp, C = g_of(w, tr)
    if g != t - 1: fail("ThmD g(B_%d)=%s" % (w, g))
    for start in range(w + 1):
        C = Csets(w, tr, start)
        for k in range(w + 1):
            if not C[k]: continue
            lo, hi = min(C[k]), max(C[k])
            if k == 0 and hi > t - 1: fail("ThmD w=%d start=%d state0 %s" % (w, start, sorted(C[k])))
            if k == w and lo < t + 1: fail("ThmD w=%d start=%d stateK %s" % (w, start, sorted(C[k])))
            if k % 2 == 1 and not (lo >= (k + 1) // 2 and hi <= t + (k - 1) // 2):
                fail("ThmD w=%d start=%d odd %d %s" % (w, start, k, sorted(C[k])))
    # same machine as solver-a's S_w?
    tra, names = split_end(w)
    # S_w order: B0,B1,L1..,T1,T0 ; bouncy order 0..w. map B0->0,B1->1, L j -> j+1, T1 -> w-1, T0 -> w
    perm = list(range(w + 1))
    if [(perm[a], perm[b]) for a, b in tra] != tr: fail("S_w != B_w for w=%d" % w)
print("Theorem D tested even w=4..12; S_w == B_w for even w")

# ---- Theorem B and P3 on random machines
random.seed(12345)
cnt = 0
for it in range(6000):
    w = random.randint(2, 7); n = random.randint(1, 7)
    tr = [(random.randrange(n), random.randrange(n)) for _ in range(n)]
    g, sp, C = g_of(w, tr)
    R = reach(tr)
    if g < w + 1 - len(R): fail("ThmB w=%d tr=%s g=%d reach=%d" % (w, tr, g, len(R)))
    s = g
    if s + 1 <= w:
        W = windows(w, tr)
        for bit in (0, 1):
            qs = {q for q, x in W if all(v == bit for v in x[w - s - 1:])}
            for q in qs:
                path = [q]
                for _ in range(w - s): path.append(tr[path[-1]][1 - bit])
                cnt += 1
                if len(set(path)) != len(path): fail("P3 w=%d tr=%s s=%d q=%d bit=%d" % (w, tr, s, q, bit))
print("Theorem B + P3 tested on 6000 random machines (%d P3 instances)" % cnt)

# ---- P2 / Theorem C: g >= 1 for w>=2 with ANY number of reachable states <= w+1 : covered by exhaustive tables.
# ---- raw labelled brute force for w<=3 (all (w+1)^(2(w+1)) machines), min g and min e
for w in (1, 2, 3):
    n = w + 1; hg = {}; he = {}
    for flat in itertools.product(range(n), repeat=2 * n):
        tr = [(flat[2 * i], flat[2 * i + 1]) for i in range(n)]
        g, sp, C = g_of(w, tr)
        e = max(x for x in sp if x is not None)
        hg[g] = hg.get(g, 0) + 1; he[e] = he.get(e, 0) + 1
    print("raw brute w=%d: g hist %s  e hist %s" % (w, sorted(hg.items()), sorted(he.items())))
print("FAILS:", bad)
