#!/usr/bin/env python3
# Referee: mechanical small-case tests of the solvers' lemmas (uses ref.py only).
import itertools, random, sys
from ref import covers_all, edits, repairable
def words(v, n):
    m = len(v); return [tuple(v[(i + j) % m] for j in range(n)) for i in range(m)]
def runs(v):  # cyclic run lengths; [] if constant
    m = len(v); st = [i for i in range(m) if v[i] != v[i - 1]]
    if not st: return []
    return [((st[(j + 1) % len(st)] - st[j] - 1) % m) + 1 for j in range(len(st))]
def S(v): return sum(1 for r in runs(v) if r == 1)
def debruijn(n):
    # prefer-one greedy from 0^n
    seq = [0] * n; seen = {tuple(seq)}
    while True:
        for b in (1, 0):
            t = tuple(seq[len(seq) - n + 1:] + [b]) if n > 1 else (b,)
            if t not in seen: seen.add(t); seq.append(b); break
        else: break
    return tuple(seq[:2 ** n])
res = {}
# P2: one edit creates at most n-1 new words (both lengths >= n)
bad = 0; cnt = 0
for n in (2, 3, 4):
    for L in range(n + 1, 12):
        for v in itertools.product((0, 1), repeat=L):
            c0 = set(words(v, n))
            for name, v1 in edits(v):
                if len(v1) < n: continue
                cnt += 1
                if len(set(words(v1, n)) - c0) > n - 1: bad += 1
print(f"P2 (A) / filter used by referee: {cnt} edits tested, violations={bad}")
# P4, Fact A, P3/Lemma 4 on all universal loops, n=2..4, length 2^n .. 2^n+3
for n in (2, 3, 4):
    T = 2 ** (n - 1); sig = 2 ** (n - 2); p4bad = 0; fabad = 0; l4bad = 0; nu = 0; p4n = 0
    for e in range(0, 4):
        L = 2 ** n + e
        for v in itertools.product((0, 1), repeat=L):
            if v[0] != 0 or not covers_all(v, n): continue
            nu += 1; r = runs(v)
            if e <= 1:
                p4n += 1
                if len(r) != T: p4bad += 1
            if n >= 3 and not (sig <= S(v) <= sig + e): fabad += 1
            if L <= 11:
                # every adversary move except loss of a length-1 run has a reply
                m = len(v)
                for name, v1 in edits(v):
                    if name[0] == 'loss' and v[name[1]] != v[name[1] - 1] and v[name[1]] != v[(name[1] + 1) % m]: continue
                    if not any(covers_all(v2, n) for _, v2 in edits(v1)): l4bad += 1
    print(f"n={n}: universal loops tested={nu}; P4 violations={p4bad} of {p4n}; FactA violations={fabad}; P3/Lemma4 violations={l4bad}")
# Fact B on all loops with >= 4 runs, L <= 11
bad = 0; cnt = 0
for L in range(4, 12):
    for v in itertools.product((0, 1), repeat=L):
        r = runs(v)
        if len(r) < 4: continue
        m = len(v); s0 = S(v)
        st = [i for i in range(m) if v[i] != v[i - 1]]
        for name, v1 in edits(v):
            if name[0] == 'pass': continue
            k = name[1]
            # run containing k and neighbours
            j = max(i for i in range(len(st)) if st[i] <= k) if st[0] <= k else len(st) - 1
            rl = r[j]; p = r[j - 1]; q = r[(j + 1) % len(r)]
            if name[0] == 'dbl': exp = -1 if rl == 1 else 0
            else: exp = 0 if rl >= 3 else (1 if rl == 2 else -1 - (p == 1) - (q == 1))
            cnt += 1
            if S(v1) - s0 != exp: bad += 1
print(f"Fact B: {cnt} edits on loops with >=4 runs, violations={bad}")
# Fact C on random order-6 universal loops (random rotations/reversals/complements of one de Bruijn cycle joined variants are not
# independent samples; this is a smoke test only) and its failure for n=5
def factC(u):
    out = True
    m = len(u); cand = []
    for k in range(m):
        for b in (0, 1): cand.append(u[:k] + (b,) + u[k:])
        if u[k] == u[k - 1] or u[k] == u[(k + 1) % m]: cand.append(u[:k] + u[k + 1:])
    cand.append(u)
    for w in cand:
        r = runs(w)
        adj = any(r[i] == 1 and r[(i + 1) % len(r)] == 1 for i in range(len(r)))
        if not (len(r) >= 4 and 2 in r and adj): out = False
    return out
d6 = debruijn(6); print("Fact C on greedy de Bruijn order 6 and its complement/reverse:", factC(d6), factC(tuple(1 - x for x in d6)), factC(d6[::-1]))
# P1 / Theorem 2: de Bruijn twice survives every edit
for n in range(1, 7):
    D = debruijn(n); assert covers_all(D, n) and len(D) == 2 ** n
    w = D + D
    print(f"Thm2/P1 n={n}: every edit of DD still universal: {all(covers_all(w1, n) for _, w1 in edits(w))}")
