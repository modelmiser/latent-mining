#!/usr/bin/env python3
"""Lemma 4 of NOTE.md, checked on ALL directed trees with out-degree <= 2 (reachable or not):
   (1) every round of rule R (flip a shortest path from an endpoint p with h(p) <= h(q)) of cost k raises Phi
       by at least k;   (2) max Phi/n by size.
   The lemma is the case cap = 2; caps 3 and 99 are other potentials, printed for comparison only.
   Exit code 1 if cap 2 is violated.
   Phi_cap = sum over vertices x with two out-neighbours of (m + min(M, m+cap))/2, where m <= M are the h of the two,
   plus 1/2 for each vertex with one out-neighbour whose h is at least 1.
   usage: lemma4_check.py NTREE NPAIR     (NOTE.md quotes 10 15: 114,526,410 rounds, about 25 minutes)"""
import sys, itertools
NT = int(sys.argv[1]); NP = int(sys.argv[2]); CAPS = (2, 3, 99)
def enc(ad, v, par): return tuple(sorted((d, enc(ad, w, v)) for (w, d) in ad[v] if w != par))
def canon(out):
    n = len(out); ad = [[] for _ in range(n)]
    for v in range(n):
        for w in out[v]: ad[v].append((w, 1)); ad[w].append((v, 0))
    return min(enc(ad, v, -1) for v in range(n))
def hs(out):
    h = [None] * len(out)
    def f(x):
        if h[x] is None: h[x] = 0 if len(out[x]) < 2 else 1 + min(f(out[x][0]), f(out[x][1]))
        return h[x]
    for x in range(len(out)): f(x)
    return h
def phi(out, h, cap):
    s = 0
    for x in range(len(out)):
        if len(out[x]) == 2:
            a, b = sorted((h[out[x][0]], h[out[x][1]])); s += a + min(b, a + cap)
        elif len(out[x]) == 1: s += min(h[out[x][0]], 1)
    return s / 2
trees = {1: {canon(((),)): ((),)}}
for n in range(2, NT + 1):
    cur = {}
    for out in trees[n - 1].values():
        for v in range(n - 1):
            o = [list(t) for t in out] + [[v]]; t = tuple(tuple(x) for x in o); cur.setdefault(canon(t), t)   # new -> v
            if len(out[v]) < 2:
                o = [list(t) for t in out] + [[]]; o[v].append(n - 1); t = tuple(tuple(x) for x in o); cur.setdefault(canon(t), t)
    trees[n] = cur
    best = {c: max(phi(t, hs(t), c) for t in cur.values()) for c in CAPS}
    print('n', n, 'trees', len(cur), ' max Phi/n:', {c: round(best[c] / n, 4) for c in CAPS}, flush=True)
def spaths(out, h, x):
    if h[x] == 0: return [[x]]
    return [[x] + p for c in out[x] if h[c] == h[x] - 1 for p in spaths(out, h, c)]
worst = {c: 99 for c in CAPS}; cnt = 0
for na in range(1, NT + 1):
    for nb in range(1, NT + 1):
        if na + nb > NP: continue
        for A in trees[na].values():
            hA = hs(A); pA = {c: phi(A, hA, c) for c in CAPS}
            for B in trees[nb].values():
                hB = hs(B); pB = {c: phi(B, hB, c) for c in CAPS}
                for p in range(na):
                    for q in range(nb):
                        if hA[p] > hB[q]: continue
                        for s in spaths(A, hA, p):
                            o = [list(t) for t in A] + [[w + na for w in t] for t in B]
                            for a, b in zip(s, s[1:]): o[a].remove(b); o[b].append(a)
                            o[p].append(q + na); k = len(s) - 1; h = hs(o); cnt += 1
                            for c in CAPS:
                                sl = phi(o, h, c) - pA[c] - pB[c] - k
                                if sl < worst[c]:
                                    worst[c] = sl
                                    if sl < 0: print('VIOLATION cap', c, 'slack', sl, 'cost', k, 'A', A, 'p', p, 'B', B, 'q', q, 'path', s, flush=True)
print('merges checked', cnt, 'min slack by cap', worst)
sys.exit(1 if worst[2] < 0 else 0)
