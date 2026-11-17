#!/usr/bin/env python3
# referee: mechanical tests of the solvers' lemmas. Each test counts cases and failures.
import itertools, random, sys
random.seed(12345)
def run(w, v):
    v = list(v)
    for i in w:
        if i and v[i-1] > v[i]: v[i-1], v[i] = v[i], v[i-1]
    return v
def issorted(v): return all(v[k] <= v[k+1] for k in range(len(v)-1))
def Z01(n): return list(itertools.product((0, 1), repeat=n))
def sorts(w, n): return all(issorted(run(w, v)) for v in Z01(n))
def sortsperm(w, n): return all(issorted(run(w, v)) for v in itertools.permutations(range(1, n+1)))
def upslips(w, n): return [w[:p]+[i+1]+w[p+1:] for p, i in enumerate(w) if i+1 <= n-1]
def slips(w, n): return [w[:p]+[j]+w[p+1:] for p, i in enumerate(w) for j in (i-1, i+1) if 1 <= j <= n-1]
def dels(w): return [w[:p]+w[p+1:] for p in range(len(w))]
def inU(w, n): return sorts(w, n) and all(sorts(x, n) for x in upslips(w, n))
def slipproof(w, n): return sorts(w, n) and all(sorts(x, n) for x in slips(w, n))
def delproof(w, n): return sorts(w, n) and all(sorts(x, n) for x in dels(w))
def W(n):
    w = []
    for k in list(range(1, n)) + [n-1]: w += list(range(k, 0, -1))
    return w
def OET(n, rounds, first):
    w = []
    for r in range(rounds): w += list(range(1 + (r+first) % 2, n, 2))
    return w
def words(n, L): return (list(t) for t in itertools.product(range(1, n), repeat=L))

# ---- T1 pointwise: active letter on 0-1 input => every slip acts as the deletion
def t1_pointwise(n, ws):
    cases = bad = 0
    for w in ws:
        for x in Z01(n):
            y = list(x)
            for p, i in enumerate(w):
                if y[i-1] > y[i]:
                    d = run(w[p+1:], y)
                    for j in (i-1, i+1):
                        if 1 <= j <= n-1:
                            cases += 1
                            if run([j]+w[p+1:], y) != d: bad += 1
                y = run([i], y)
    return cases, bad
print("T1 pointwise n=4 all words len<=7:", t1_pointwise(4, (w for L in range(8) for w in words(4, L))))
for n in (5, 6, 7, 8):
    print(f"T1 pointwise n={n} 300 random words len 2n(n-1)/2:", t1_pointwise(n, ([random.randint(1, n-1) for _ in range(n*(n-1))] for _ in range(300))), flush=True)
# ---- T1 as stated, word by word (sets equal), from all permutations for n=3, 0-1 for n=4
for n, maxL in ((3, 9), (4, 10)):
    tot = sp = dp = diff = 0
    for L in range(maxL+1):
        for w in words(n, L):
            tot += 1; a = slipproof(w, n); b = delproof(w, n); sp += a; dp += b; diff += (a != b)
    print(f"T1 sets n={n} all words len<={maxL}: words={tot} slipproof={sp} delproof={dp} differ={diff}", flush=True)
# n=2 (excluded by T1): c_1 is slip-proof vacuously but not deletion-proof
print("n=2 word [1]: slipproof", slipproof([1], 2), "delproof", delproof([1], 2))

# ---- T2: on the reversed input; w sorting. For each v: (w minus p_v sorts reversed) <=> exists later letter whose wires hold {v,v+1}
def t2(n, w):
    v = list(range(n, 0, -1)); pv = {}; later = set(); idle = 0; act = 0
    for p, i in enumerate(w):
        a, b = v[i-1], v[i]
        if a > b:
            act += 1
            if a == b+1: pv[b] = p
            v[i-1], v[i] = b, a
        else:
            idle += 1
            if b == a+1: later.add(a)      # idle letter comparing a, a+1 (necessarily after p_a)
    bad = 0
    for x in range(1, n):
        ok = issorted(run(w[:pv[x]]+w[pv[x]+1:], range(n, 0, -1)))
        if ok != (x in later): bad += 1
    return bad, idle, act, len(later)
def t2_batch(n, ws):
    cases = bad = badcount = dpw = 0
    for w in ws:
        if not issorted(run(w, range(n, 0, -1))): continue
        cases += 1; b, idle, act, nl = t2(n, w); bad += b
        if act != n*(n-1)//2: badcount += 1
        if len(w) <= 12 and n <= 4 and delproof(w, n):
            dpw += 1
            if nl < n-1 or idle < n-1: badcount += 1
    return cases, bad, badcount, dpw
print("T2 n=3 all words len<=9 (sorting words, equivalence failures, count failures, delproof words):", t2_batch(3, (w for L in range(10) for w in words(3, L))))
print("T2 n=4 all words len<=10:", t2_batch(4, (w for L in range(11) for w in words(4, L))), flush=True)
for n in (5, 6, 7):
    print(f"T2 n={n} 3000 random words len n(n-1):", t2_batch(n, ([random.randint(1, n-1) for _ in range(n*(n-1))] for _ in range(3000))), flush=True)

# ---- L4 / L5 decomposition
def decompose(w, n):
    k = 1; star = []; wasted = 0; traj = 0
    for i in w:
        if i == k and k <= n-1: k += 1; traj += 1
        elif i == k-1: wasted += 1
        elif i <= k-2: star.append(i)
        else: star.append(i-1)
    return star, wasted, traj, k
def l4_check(n, w, mode):
    star, wasted, traj, k = decompose(w, n); bad = []
    if k != n or traj != n-1: bad.append('path')
    if len(w) != len(star) + (n-1) + wasted: bad.append('size')
    for _ in range(5):   # others are transformed by w*
        o = random.sample(range(1, n), n-1); r = run(w, [n]+o)
        if r[:-1] != run(star, o) or r[-1] != n: bad.append('action')
    if mode == 'up' and not inU(star, n-1): bad.append('star-not-in-U')
    if mode == 'del' and n-1 >= 2 and not delproof(star, n-1): bad.append('star-not-delproof')
    if wasted < 1: bad.append('no-wasted')
    return bad
for n, Ls in ((3, range(4, 10)), (4, range(9, 12))):
    for L in Ls:
        nu = nd = bu = bd = 0
        for w in words(n, L):
            if inU(w, n):
                nu += 1; bu += bool(l4_check(n, w, 'up'))
            if delproof(w, n):
                nd += 1; bd += bool(l4_check(n, w, 'del'))
        print(f"L4/L5 n={n} L={L}: U-words={nu} L4-failures={bu} delproof-words={nd} L5-failures={bd}", flush=True)
ups = {5: [1,2,1,3,2,1,3,4,3,2,1,3,2,1], 6: [1,2,1,3,2,1,4,3,2,1,4,5,4,3,2,1,4,3,2,1], 7: [1,2,1,3,2,1,4,3,2,1,5,4,3,2,1,5,6,5,4,3,2,1,5,4,3,2,1]}
for n in (5, 6, 7):
    assert inU(ups[n], n), "solver-b up witness"
    nu = bu = nd = bd = 0
    for base, mode in ((ups[n], 'up'), (W(n), 'up'), (W(n), 'del'), (OET(n, n+2, 0), 'del'), (OET(n, n+2, 1), 'up')):
        for t in range(40):
            w = list(base)
            for _ in range(random.randint(0, 4)): w.insert(random.randint(0, len(w)), random.randint(1, n-1))
            if mode == 'up':
                assert inU(w, n); nu += 1; bu += bool(l4_check(n, w, 'up'))
            else:
                assert delproof(w, n); nd += 1; bd += bool(l4_check(n, w, 'del'))
    print(f"L4/L5 n={n} witnesses with random insertions: U-words={nu} L4-failures={bu} delproof-words={nd} L5-failures={bd}", flush=True)
# u(3), u(4) base case
for n, maxL in ((3, 5), (4, 9)):
    for L in range(maxL+1):
        ws = [w for w in words(n, L) if inU(w, n)]
        if ws: print(f"u({n}): first nonempty size {L}, {len(ws)} words, perm-confirmed={all(sortsperm(x, n) and all(sortsperm(y, n) for y in upslips(x, n)) for x in ws)}, e.g. {ws[0]}"); break
        else: print(f"u({n}): no word of size {L}")
# characterisation implied by T1's argument: U(n) = sorts after deleting any letter other than c_{n-1}
diff = 0
for L in range(11):
    for w in words(4, L):
        a = inU(w, 4); b = sorts(w, 4) and all(sorts(w[:p]+w[p+1:], 4) for p in range(L) if w[p] != 3); diff += (a != b)
print("U(4) == 'deletion-proof for letters other than c_{n-1}', words len<=10, mismatches:", diff, flush=True)

# ---- upper-bound invariants. A: Z_m sets on 0-1 inputs.  B: x-low on permutations.
def inZ(v):   # 0^a 1^c 0 1^d or sorted
    if issorted(v): return True
    s = ''.join(map(str, v)); import re
    return re.fullmatch(r'0*1*01*', s) is not None
def xlow(v):
    if issorted(v): return True
    for k, x in enumerate(v):
        if issorted(v[:k]+v[k+1:]) and all(x < y for y in v[k+1:]): return True
    return False
def blocks(n): return [list(range(k, 0, -1)) for k in list(range(1, n)) + [n-1]]
for n in range(3, 11):
    bl = blocks(n); cases = bad = 0
    for b in range(len(bl)):
        for j in range(len(bl[b])):
            for x in Z01(n):
                v = list(x)
                for t, blk in enumerate(bl):
                    m = len(blk) + 1 if t < n-1 else n      # wires touched so far: 1..m
                    v = run(blk if t != b else blk[:j]+blk[j+1:], v)
                    cases += 1
                    if t < b and not issorted(v[:m]): bad += 1
                    if t >= b and t < len(bl)-1 and not inZ(v[:m]): bad += 1
                    if t == len(bl)-1 and not issorted(v): bad += 1
    print(f"T3 invariant (Z_m after each block, 0-1 inputs) n={n}: cases={cases} failures={bad}", flush=True)
for n in range(3, 8):
    bl = blocks(n); cases = bad = 0
    for b in range(len(bl)):
        for j in range(len(bl[b])):
            for x in itertools.permutations(range(1, n+1)):
                v = list(x)
                for t, blk in enumerate(bl):
                    m = len(blk) + 1 if t < n-1 else n
                    v = run(blk if t != b else blk[:j]+blk[j+1:], v)
                    cases += 1
                    if t < b and not issorted(v[:m]): bad += 1
                    if t >= b and t < len(bl)-1 and not xlow(v[:m]): bad += 1
                    if t == len(bl)-1 and not issorted(v): bad += 1
    print(f"L2 invariant (x-low after each block, all permutations) n={n}: cases={cases} failures={bad}", flush=True)
# ---- L1 monotonicity, exhaustive on 0-1 vectors
def pre(v):
    s = 0; o = []
    for a in v: s += a; o.append(s)
    return o
for n in range(2, 9):
    bad = 0; Z = Z01(n); P = {z: pre(z) for z in Z}
    for x in Z:
        for y in Z:
            if sum(x) == sum(y) and all(a <= b for a, b in zip(P[x], P[y])):
                for i in range(1, n):
                    cx, cy = run([i], x), run([i], y)
                    if not all(a <= b for a, b in zip(pre(cx), pre(cy))): bad += 1
                    if not all(a <= b for a, b in zip(pre(cx), P[x])): bad += 1
    print(f"L1 monotone n={n}: failures={bad}")
