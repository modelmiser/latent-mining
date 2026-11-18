#!/usr/bin/env python3
"""Play the Adversary strategy of Theorem 1 on explicit oriented forests against several Algorithms
(normal-form replies), checking after every round:
  (L) the Type Lemma outcome for the two named witnesses,
  (I) the invariant 3*paid(C) + F3(assigned index of C) >= |C| in the main phase,
and at the end  paid >= n/3 - (7/3)*log2(n+1) + 5/6   and the sharper  n/3 - F(i_m) - (2/3)(m-1).
usage: sim.py N ALG SEED [BREAK]   ALG in greedy|random|anti|loop|long
BREAK=1: deliberately wrong Adversary (names a NON-witness vertex of each tree) -> checks must fail."""
import sys, random, math
from trees import hvals, slots, idx_of, F3
N = int(sys.argv[1]); ALG = sys.argv[2]; SEED = int(sys.argv[3]); BREAK = int(sys.argv[4]) if len(sys.argv) > 4 else 0
rnd = random.Random(SEED)
out = [[] for _ in range(N)]
trees = {v: {'vs': [v], 'paid': 0, 'idx': 0} for v in range(N)}   # keyed by representative
total = 0; viol = 0

def local_h(vs):
    h = {}
    def rec(v):
        st = [v]
        while st:
            u = st[-1]
            if u in h: st.pop(); continue
            if len(out[u]) < 2: h[u] = 0; st.pop(); continue
            miss = [w for w in out[u] if w not in h]
            if miss: st.extend(miss)
            else: h[u] = 1 + min(h[w] for w in out[u]); st.pop()
    for v in vs: rec(v)
    return h
def vidx(v, h):
    s = sorted([h[w] for w in out[v]] + [-1] * (2 - len(out[v])))
    return idx_of(s[0], s[1])
def witness(t, i):
    h = local_h(t['vs'])
    c = [v for v in t['vs'] if vidx(v, h) >= i]
    if BREAK:
        c = [v for v in t['vs'] if vidx(v, h) < i] or c
    assert c, 'no witness: assigned index invalid'
    return rnd.choice(c)
def paths(x, limit=64):
    """directed paths from saturated x through saturated vertices to the first unsaturated one"""
    res = []; st = [[x]]
    while st and len(res) < limit:
        p = st.pop(); v = p[-1]
        if len(out[v]) < 2: res.append(p); continue
        for w in out[v]: st.append(p + [w])
    return res
def shortest(x):
    fr = [[x]]
    while True:
        nf = []
        for p in fr:
            if len(out[p[-1]]) < 2: return p
            for w in out[p[-1]]: nf.append(p + [w])
        fr = nf
def apply(path, x, y):
    for a, b in zip(path, path[1:]):
        out[a].remove(b); out[b].append(a)
    out[x].append(y)
def undo(path, x, y):
    out[x].remove(y)
    for a, b in zip(path, path[1:]):
        out[b].remove(a); out[a].append(b)

def merge(ra, rb, phase):
    global total, viol
    A = trees[ra]; B = trees[rb]
    i = min(A['idx'], B['idx']); k = i // 2
    x = witness(A, i); y = witness(B, i)
    # Algorithm's options
    opts = []
    for (u, v) in ((x, y), (y, x)):
        if len(out[u]) < 2: opts.append((0, [u], u, v))
        else:
            ps = paths(u) if ALG != 'greedy' else [shortest(u)]
            for p in ps: opts.append((len(p) - 1, p, u, v))
    allvs = A['vs'] + B['vs']
    def score(o):
        c, p, u, v = o
        apply(p, u, v); h = local_h(allvs); ni = max(vidx(w, h) for w in allvs); undo(p, u, v)
        return 3 * c + F3(ni)
    if ALG == 'greedy': o = min(opts, key=lambda o: (o[0], rnd.random()))
    elif ALG == 'random': o = rnd.choice(opts)
    elif ALG == 'anti': o = min(opts, key=lambda o: (score(o), rnd.random()))
    elif ALG == 'loop':   # prefer the self-loop reply (cost k+1 through the high slot) when it exists
        o = min(opts, key=lambda o: (abs(o[0] - (k + 1)), rnd.random()))
    elif ALG == 'long': o = max(opts, key=lambda o: (o[0], rnd.random()))
    c, p, u, v = o
    apply(p, u, v)
    h = local_h(allvs)
    ni = vidx(u, h)
    if i % 2 == 0:
        ok = c >= k and ni >= i + 1; g = i + 1
    else:
        if c >= k and ni >= i + 1: ok = True; g = i + 1
        elif c >= k + 1 and ni >= i: ok = True; g = i
        else: ok = False; g = i
    if not ok:
        viol += 1
        if viol <= 3: print('LEMMA VIOLATION i=%d cost=%d new=%d' % (i, c, ni))
    total += c
    T = {'vs': allvs, 'paid': A['paid'] + B['paid'] + c, 'idx': g}
    del trees[ra]; del trees[rb]; trees[allvs[0]] = T
    if phase == 'main' and 3 * T['paid'] + F3(g) < len(allvs):
        viol += 1
        if viol <= 3: print('INVARIANT VIOLATION', T['paid'], g, len(allvs))
    return allvs[0]

# main phase: merge two trees of equal assigned index (lowest first)
while True:
    by = {}
    pair = None
    for r, t in trees.items():
        by.setdefault(t['idx'], []).append(r)
    for i in sorted(by):
        if len(by[i]) >= 2: pair = (by[i][0], by[i][1]); break
    if pair is None: break
    merge(pair[0], pair[1], 'main')
left = sorted((t['idx'], r) for r, t in trees.items())
m = len(left); im = left[-1][0]
sharp = N / 3 - F3(im) / 3 - (2 / 3) * (m - 1)
pot = sum(len(t['vs']) / 3 - F3(t['idx']) / 3 for t in trees.values())
# cleanup: top-down
cur = left[-1][1]
for (i, r) in reversed(left[:-1]):
    cur = merge(cur, r, 'cleanup')
bound = N / 3 - (7 / 3) * math.log2(N + 1) + 5 / 6
okfin = total >= sharp - 1e-9 and total >= bound - 1e-9
print('N=%d alg=%s seed=%d paid=%d  n/3=%.2f  sharp=%.2f  loose=%.2f  leftover m=%d i_m=%d  violations=%d  %s' %
      (N, ALG, SEED, total, N / 3, sharp, bound, m, im, viol, 'OK' if (okfin and viol == 0) else 'FAILED'))
