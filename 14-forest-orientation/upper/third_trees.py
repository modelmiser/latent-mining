#!/usr/bin/env python3
"""Theorem 5 on small trees, by enumeration.  All quantities are in THIRDS (integers).

For every directed tree with out-degrees at most 2 and at most NT vertices (whether or not play can reach it):
  Lemma 6:   Phi(T) <= n - max over v of gamma(v).
For every pair of such trees A, B with at most NP vertices together, every vertex x of A and y of B with
2 h(x) + eps(x) <= 2 h(y) + eps(y)  (rule L allows flipping from x), and EVERY shortest directed path from x
to an unsaturated vertex:
  the round raises Phi by at least 3 * (length of the path).
The weight is computed from the table of the note, not from the formula with the maximum.
Exit code 1 on any violation.     usage: third_trees.py NT NP     (NT >= 1, NP >= 2)"""
import sys
if len(sys.argv) != 3: sys.exit(__doc__)
NT = int(sys.argv[1]); NP = int(sys.argv[2])
if NT < 1 or NP < 2: sys.exit('need NT >= 1 and NP >= 2')
def enc(ad, v, par): return tuple(sorted((d, enc(ad, w, v)) for (w, d) in ad[v] if w != par))
def canon(out):                                   # canonical form of a directed tree
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
def flags(out, h):                                # eps(v): 0 if the two slots of v are equal, else 1
    r = []
    for x in range(len(out)):
        s = [h[c] for c in out[x]] + [-1] * (2 - len(out[x])); r.append(int(s[0] != s[1]))
    return r
def weight(ch):                                   # ch: list of (height, eps) of the out-neighbours
    if not ch: return 0
    if len(ch) == 1: return ch[0][1] if ch[0][0] == 0 else 1
    (p, ep), (q, eq) = sorted(ch)
    if q == p: return 3 * p - 1 + ep + eq
    if q == p + 1: return 3 * p + 1 + ep + eq
    return 3 * p + 2 + ep
def phi(out, h, e): return sum(weight([(h[c], e[c]) for c in out[x]]) for x in range(len(out)))
def spaths(out, h, x):                            # all shortest directed paths from x to an unsaturated vertex
    if h[x] == 0: return [[x]]
    return [[x] + p for c in out[x] if h[c] == h[x] - 1 for p in spaths(out, h, c)]
trees = {1: {canon(((),)): ((),)}}
bad6 = 0
for n in range(2, NT + 1):
    cur = {}
    for out in trees[n - 1].values():
        for v in range(n - 1):
            t = tuple(tuple(x) for x in ([list(t) for t in out] + [[v]])); cur.setdefault(canon(t), t)
            if len(out[v]) < 2:
                o = [list(t) for t in out] + [[]]; o[v].append(n - 1); t = tuple(tuple(x) for x in o); cur.setdefault(canon(t), t)
    trees[n] = cur
for n in range(1, NT + 1):
    b = 0
    for t in trees[n].values():
        h = hs(t); e = flags(t, h); b += phi(t, h, e) > n - max(3 * h[x] + 1 + e[x] for x in range(n))
    bad6 += b
    print('n', n, 'trees', len(trees[n]), 'violations of Lemma 6:', b, flush=True)
worst = None; cnt = 0; nbad = 0; longest = 0
for na in range(1, NT + 1):
    for nb in range(1, NT + 1):
        if na + nb > NP: continue
        for A in trees[na].values():
            hA = hs(A); eA = flags(A, hA); fA = phi(A, hA, eA)
            for B in trees[nb].values():
                hB = hs(B); eB = flags(B, hB); fB = phi(B, hB, eB)
                for p in range(na):
                    for q in range(nb):
                        if 2 * hA[p] + eA[p] > 2 * hB[q] + eB[q]: continue
                        for s in spaths(A, hA, p):
                            o = [list(t) for t in A] + [[w + na for w in t] for t in B]
                            for a, b in zip(s, s[1:]): o[a].remove(b); o[b].append(a)
                            o[p].append(q + na); k = len(s) - 1; cnt += 1; longest = max(longest, k)
                            ho = hs(o); sl = phi(o, ho, flags(o, ho)) - fA - fB - 3 * k
                            if sl < 0:
                                nbad += 1
                                if nbad <= 8: print('VIOLATION slack %d/3 cost %d A=%s x=%d B=%s y=%d path=%s' % (sl, k, [list(a) for a in A], p, [list(b) for b in B], q, s), flush=True)
                            worst = sl if worst is None else min(worst, sl)
print('rounds checked', cnt, 'violations', nbad, 'least gain minus 3*cost (thirds)', worst, 'longest flipped path', longest)
print('ALL OK' if not (nbad or bad6) else 'FAILED')
sys.exit(1 if (nbad or bad6) else 0)
