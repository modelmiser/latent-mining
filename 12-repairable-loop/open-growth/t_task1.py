#!/usr/bin/env python3
# Mechanical tests for Theorem 1 (no repairable loop of length 2^n+2, n >= 7).  See proofs.txt, steps 1-7.
import itertools, random, sys
from og import *
rng = random.Random(777)
W3 = list(itertools.product((0, 1), repeat=3))
ALT = {(0, 1, 0): 1, (1, 0, 1): 1}
def mech_options(x1):
    """all mechanic moves, by local context, whose effect on the 3-window counts leaves every excess >= 0.
       x1: dict 3-word -> excess after the adversary's move."""
    out = []
    if all(v >= 0 for v in x1.values()): out.append(('pass', None, dict(x1)))
    for o, p, c, q, r in itertools.product((0, 1), repeat=5):       # loss of c in context o p c q r
        x = dict(x1)
        for f in ((o, p, c), (p, c, q), (c, q, r)): x[f] -= 1
        for f in ((o, p, q), (p, q, r)): x[f] += 1
        if all(v >= 0 for v in x.values()): out.append(('loss', (o, p, c, q, r), x))
    for p, c, q in itertools.product((0, 1), repeat=3):             # doubling of c in context p c q
        x = dict(x1); x[(p, c, q)] -= 1; x[(p, c, c)] += 1; x[(c, c, q)] += 1
        if all(v >= 0 for v in x.values()): out.append(('dbl', (p, c, q), x))
    return out
def xtz(t, z):
    return {(0,0,0): z, (0,0,1): t, (0,1,0): 1 + t, (0,1,1): -t, (1,0,0): t, (1,0,1): 1 - t, (1,1,0): -t, (1,1,1): -z}
def after_iso1(x):   # adversary deletes the 1 of 00100
    y = dict(x)
    for f in ((0,0,1), (0,1,0), (1,0,0)): y[f] -= 1
    y[(0,0,0)] += 2; return y
def after_iso0(x):   # adversary deletes the 0 of 11011
    y = dict(x)
    for f in ((1,1,0), (1,0,1), (0,1,1)): y[f] -= 1
    y[(1,1,1)] += 2; return y
def after_s3(x):     # adversary deletes the middle 1 of 1 0 1 0 1
    y = dict(x)
    for f in ((1,0,1), (0,1,0), (1,0,1)): y[f] -= 1
    for f in ((1,0,0), (0,0,1)): y[f] += 1
    return y
print("== Step 4: which (t,z) in [-4,4]^2 admit a mechanic reply (3-window counts) to BOTH isolated moves?")
ok = [(t, z) for t in range(-4, 5) for z in range(-4, 5) if mech_options(after_iso1(xtz(t, z))) and mech_options(after_iso0(xtz(t, z)))]
print("   survivors:", ok, "(proof claims exactly [(0, 0)])")
ok1 = [(t, z) for t in range(-4, 5) for z in range(-4, 5) if mech_options(after_iso1(xtz(t, z)))]
print("   FALSE variant (only the 1-move used): survivors:", ok1, "(more than one expected => the test can fail)")
x0 = xtz(0, 0)
print("== Step 5: replies to the isolated-1 move when x = [alternating]:")
for kind, ctx, x in mech_options(after_iso1(x0)): print("   ", kind, ctx, {''.join(map(str, f)): v for f, v in x.items() if v})
print("== Step 6: replies to the S3 move (middle 1 of 10101) when x = [alternating]:")
for kind, ctx, x in mech_options(after_s3(x0)): print("   ", kind, ctx, {''.join(map(str, f)): v for f, v in x.items() if v})
# ---------------------------------------------------------------------------------------------
print("== structure of the known repairable loops of length 2^n+2 (n=4,5)")
R = load_repairable()
def x3(w, n):
    c = counts(w, 3); b = 1 << (n - 3); return {f: c.get(f, 0) - b for f in W3}
for n in (4, 5):
    L = 2 ** n + 2; ws = [T(s) for s in R[n] if len(s) == L]
    a = b_ = c_ = d_ = 0
    for w in ws:
        if x3(w, n) == {f: ALT.get(f, 0) for f in W3}: a += 1
        if S(w) == 2 ** (n - 2) + 2 and len(runs(w)) == 2 ** (n - 1) + 2: b_ += 1
        # w = de Bruijn cycle with an alternating stretch extended by two cells?
        m = len(w); okp = False
        for i in range(m):
            if w[i] != w[(i + 1) % m] and w[i - 1] == w[(i + 1) % m] and w[i] == w[(i + 2) % m]:
                v = tuple(w[j] for j in range(m) if j != i and j != (i + 1) % m)
                if universal(v, n): okp = True
        if okp: c_ += 1
    print(f"   n={n}: loops={len(ws)}; x=[alt] on 3-words: {a}; S=sigma+2 and rho=T+2: {b_}; 'de Bruijn cycle + alternating pair': {c_}")
# ---------------------------------------------------------------------------------------------
print("== Lemma Z: universal, length 2^n+2, excess(000)=2 on 3-words  =>  exactly one 0-run of length >= n-1, of length n+2")
for n in (3, 4):
    L = 2 ** n + 2; t = v = 0; vf = 0
    for w in itertools.product((0, 1), repeat=L):
        if not universal(w, n): continue
        if x3(w, n)[(0, 0, 0)] != 2: continue
        t += 1
        zr = [l for (_, l, a) in runs(w) if a == 0 and l >= n - 1]
        if zr != [n + 2]: v += 1
        if [l for (_, l, a) in runs(w) if a == 1 and l >= n - 1] != [n + 2]: vf += 1
    print(f"   n={n}: loops with excess(000)=2: {t}, violations={v}; FALSE variant (1-runs instead of 0-runs) violations={vf}")
# ---------------------------------------------------------------------------------------------
print("== Step 7 prediction on loops with x=[alt]: an isolated 1 (0-runs p,q>=2 around it) can be answered only if {p,q}={n,2}")
def family1(D, rng):
    m = len(D); pos = [i for i in range(m) if D[i] != D[(i + 1) % m] and D[i] == D[(i + 2) % m]]
    i = rng.choice(pos); r = D[i:] + D[:i]          # r[0..2] = a b a
    return r[:2] + (r[0], r[1]) + r[2:]             # a b a b a ...
def family2(D, rng):
    # D -> double a singleton 0 flanked by 1s (101 -> 1001), then insert a 1 into some 00 flanked by 1s (1001 -> 10101)
    m = len(D); pos = [i for i in range(m) if D[i] == 0 and D[i - 1] == 1 and D[(i + 1) % m] == 1]
    i = rng.choice(pos); w = dbl(D, i); m = len(w)
    pos = [i for i in range(m) if w[i] == 0 and w[(i + 1) % m] == 0 and w[i - 1] == 1 and w[(i + 2) % m] == 1]
    i = rng.choice(pos); return w[:i + 1] + (1,) + w[i + 1:]
def iso_singletons(w, letter):
    m = len(w); rr = runs(w); out = []
    for j, (st, l, a) in enumerate(rr):
        if l == 1 and a == letter:
            p = rr[j - 1][1]; q = rr[(j + 1) % len(rr)][1]
            if p >= 2 and q >= 2: out.append((st, p, q))
    return out
for n in (4, 5, 6, 7, 8, 9):
    trials = {4: 60, 5: 60, 6: 40, 7: 25, 8: 10, 9: 4}[n]
    tot = ans = viol = violF = 0; rep = 0; nall = 0; deep = 0; deepans = 0
    for tr in range(trials):
        D = random_debruijn(n, rng)
        if rng.random() < .5: D = tuple(1 - b for b in D)
        w = family1(D, rng) if tr % 2 == 0 else family2(D, rng)
        assert x3(w, n) == {f: ALT.get(f, 0) for f in W3}
        for letter in (0, 1):
            for (st, p, q) in iso_singletons(w, letter):
                tot += 1
                a = bool(replies(loss(w, st), n))
                if p >= 3 and q >= 3:
                    deep += 1; deepans += a
                if a:
                    ans += 1
                    if sorted((p, q)) != [2, n]: viol += 1
                    if (p, q) != (n, 2): violF += 1
        if n <= 7:
            nall += 1; rep += repairable(w, n)
    print(f"   n={n}: loops={trials} isolated singletons tested={tot} answerable={ans} violations={viol} | with p,q>=3: {deep}, answerable {deepans}"
          f" | FALSE variant '(p,q)=(n,2) only' violations={violF} | fully repairable: {rep} of {nall}")
