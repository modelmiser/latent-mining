#!/usr/bin/env python3
"""Referee tests, independent of the author's programs.
usage: ref.py q N [mutant]     configs = sorted tuples of nodes >= 1.
mutant in {none, W3, L1, L5, N, STEP, SHAPE, I}"""
import sys
from collections import deque
from fractions import Fraction
from itertools import combinations
q = int(sys.argv[1]); N = int(sys.argv[2]); MUT = sys.argv[3] if len(sys.argv) > 3 else "none"
U = Fraction(2) ** (q - 2)
fails = {}
counts = {}
def cnt(name): counts[name] = counts.get(name, 0) + 1
def fail(name, *info):
    fails.setdefault(name, [])
    if len(fails[name]) < 3: fails[name].append(info)
def gaps(S):
    p = 0; out = []
    for x in S:
        out.append((x, x - p)); p = x
    return out
def reducibles(S, qq):
    t = 2 ** (qq - len(S)) if qq >= len(S) else 0
    if MUT == "L1" and qq == q: t += 1
    return [x for x, g in gaps(S) if g <= t]
def red(S, qq):
    while True:
        r = reducibles(S, qq)
        if not r: return S
        S = tuple(y for y in S if y != r[-1])
def rm(S, x): return tuple(y for y in S if y != x)
def add(S, x): return tuple(sorted(S + (x,)))
def nbrs(S, qq):
    s = set(S); out = []
    if len(S) < qq:
        if 1 not in s: out.append(add(S, 1))
        for x in S:
            if x + 1 not in s: out.append(add(S, x + 1))
    for x in S:
        if x == 1 or x - 1 in s: out.append(rm(S, x))
    return out
label = {}; comps = []
def lab(S):
    """P/R component by plain BFS in the q-game (no window)."""
    if S in label: return label[S]
    i = len(comps); mem = [S]; label[S] = i; dq = deque([S])
    while dq:
        T = dq.popleft()
        for T2 in nbrs(T, q):
            if T2 not in label:
                label[T2] = i; mem.append(T2); dq.append(T2)
    comps.append(mem); return i
def play_sets(c):
    seen = {()}; dq = deque([()])
    while dq:
        T = dq.popleft()
        for T2 in nbrs(T, c):
            if T2 not in seen: seen.add(T2); dq.append(T2)
    return seen
# ---------------- Lemma W ----------------
PLAY = {c: play_sets(c) for c in range(0, q + 1)}
for c in range(1, q + 1):
    for S in PLAY[c]:
        if not S: continue
        cnt("W")
        if S[0] > 2 ** (c - 1): fail("W(i)", c, S)
        if S[-1] > 2 ** c - 1: fail("W(ii)", c, S)
        for l in range(1, len(S) + 1):
            b = 2 ** c - 2 ** (c - l) - (1 if MUT == "W3" and l >= 2 else 0)
            if S[l - 1] > b: fail("W(iii)", c, l, S)
# ---------------- universe ----------------
UNIV = [c for k in range(0, q + 1) for c in combinations(range(1, N + 1), k)]
def H(Q, Aid, shiftadj=0):
    k = len(Q)
    if k > q - 1: return False
    if lab(Q) == Aid: return True
    d = 2 ** (q - k - 1) + shiftadj
    for j, u2 in enumerate(Q):
        lo = Q[j - 1] if j > 0 else 0
        hi = Q[j + 1] if j + 1 < k else 10 ** 9
        for u in range(max(lo + 1, u2 - d, 1), min(hi - 1, u2 + d) + 1):
            if u == u2: continue
            if lab(add(rm(Q, u2), u)) == Aid: return True
    return False
def fmin(A):
    """least f with J(f)."""
    if not A: return 0
    m = len(A); r = q - m
    f = 1
    while not (A[0] <= (f + 2) * U and A[-1] <= (f + 3) * U - Fraction(2) ** (r - 1) - (1 if MUT == "I" else 0)): f += 1
    return f
red2lab = {}; lab2red = {}
for S in UNIV:
    A = red(S, q); lS = lab(S); Aid = lab(A)
    # L1, L2
    R = reducibles(S, q)
    for x in R:
        cnt("L1")
        if lab(rm(S, x)) != lS: fail("L1", S, x)
        for y in R:
            if y != x:
                cnt("L2")
                if y not in reducibles(rm(S, x), q): fail("L2", S, x, y)
    # L3
    if red2lab.setdefault(A, lS) != lS: fail("L3 red equal, comps differ", S, A)
    if lab2red.setdefault(lS, A) != A: fail("L3 comp equal, red differ", S, A)
    if not set(A) <= set(S): fail("A subset S", S, A)
    cnt("L3")
    # L4
    m = len(A); r = q - m; bnd = (0,) + A + (10 ** 9,)
    for i in range(m + 1):
        a = bnd[i]; T = tuple(x - a for x in S if bnd[i] < x < bnd[i + 1])
        cnt("L4")
        if T not in PLAY[r]: fail("L4 play", S, A, i)
        if T:
            if T[-1] > 2 ** r - 1 or 2 * T[0] > 2 ** r: fail("L4 bounds", S, A, i)
            for l in range(1, len(T) + 1):
                if T[l - 1] > 2 ** r - Fraction(2) ** (r - l): fail("L4 rank", S, A, i)
    # L5
    if A:
        cnt("L5")
        if any(x < A[0] and x > 2 ** (q - 1) - 1 - (1 if MUT == "L5" else 0) for x in S): fail("L5", S, A)
    # L6
    if len(S) <= q - 1:
        A2 = red(S, q - 1)
        if red(A2, q) != A: fail("L6(1)", S)
        for z in range(1, N + 3):
            if z in S: continue
            cnt("L6")
            if lab(add(S, z)) != lab(add(A2, z)): fail("L6(2)", S, z)
    # Lemma N (constructive Q0 of the proof), all anchor sets
    for z in S:
        cnt("N")
        Q0 = red(rm(S, z), q - 1)
        okN = (z in A and Q0 == rm(A, z)) or H(Q0, Aid, -1 if MUT == "N" else 0)
        if not okN: fail("N", S, z, Q0, A)
        # Theorem I, exactly as stated: any S, any z
        cnt("I")
        A1 = red(rm(S, z), q)
        if fmin(A1) > fmin(A) + 1: fail("ThmI", S, z, A, A1)
        # Shape lemma
        if m <= 1:
            cnt("SHAPE")
            if A1 and not H(A1, Aid, -1 if MUT == "SHAPE" else 0): fail("SHAPE", S, z, A, A1)
        else:
            cnt("SHAPE_m>=2(not claimed)")
            if A1 and not H(A1, Aid): counts["SHAPE fails for m>=2 (not claimed)"] = counts.get("SHAPE fails for m>=2 (not claimed)", 0) + 1
print("q", q, "N", N, "mut", MUT, "universe", len(UNIV), "comps", len(comps), flush=True)
# ---------------- Lemma Step: statement, and each step of its written proof ----------------
sadj = -1 if MUT == "STEP" else 0
anchors = [()] + [(a,) for a in range(2 ** (q - 1) + 1, N + 1)]
for A in anchors:
    Aid = lab(A)
    memo = {}
    def good(Q):  # Q empty or H_A(Q)
        if Q not in memo: memo[Q] = (not Q) or H(Q, Aid, sadj)
        return memo[Q]
    smemo = {}
    def stmt(Q):  # exists nonempty reduction sequence to a good set
        if Q in smemo: return smemo[Q]
        res = False
        for y in reducibles(Q, q):
            Q1 = rm(Q, y)
            if good(Q1) or stmt(Q1): res = True; break
        smemo[Q] = res; return res
    for V in list(comps[Aid]):
        k = len(V)
        if k > q - 1: continue
        phi = q - k; d = 2 ** (phi - 1) + sadj
        if reducibles(V, q):
            cnt("STEP stmt")
            if not stmt(V): fail("STEP stmt Q=V", A, V)
        for j, u in enumerate(V):
            p = V[j - 1] if j > 0 else 0
            s = V[j + 1] if j + 1 < k else None
            hi = s if s is not None else 10 ** 9
            for u2 in range(max(p + 1, u - d), min(hi - 1, u + d) + 1):
                if u2 == u: continue
                Q = add(rm(V, u), u2)
                RQ = reducibles(Q, q)
                if not RQ: continue
                cnt("STEP stmt")
                if not stmt(Q): fail("STEP stmt", A, V, u, u2)
                RV = reducibles(V, q)
                for y in RQ:
                    cnt("STEP proof-case")
                    if y != u2 and y != s:
                        if y not in RV or not good(rm(Q, y)): fail("STEP case1", A, V, u, u2, y)
                    elif y == u2:
                        Q0 = red(rm(V, u), q - 1)
                        if not good(Q0): fail("STEP case2", A, V, u, u2)
                    else:
                        if s - u <= 2 ** phi:
                            if s not in RV or not good(rm(Q, s)): fail("STEP case3a", A, V, u, u2)
                        else:
                            if not u2 > u: fail("STEP 3b u'>u", A, V, u, u2)
                            if not RV or s in RV: fail("STEP 3b e", A, V, u, u2)
                            for e in RV:
                                if e != u:
                                    if e not in RQ or not good(rm(Q, e)): fail("STEP 3b e!=u", A, V, u, u2, e)
                                else:
                                    if not good(rm(Q, s)): fail("STEP 3b e=u", A, V, u, u2)
for k_ in sorted(counts): print("  cases", k_, counts[k_])
if fails:
    for k_ in sorted(fails): print("  FAIL", k_, fails[k_])
else: print("  ALL PASS")
