#!/usr/bin/env python3
"""Lemma 5 of NOTE.md.  Check on ALL directed trees with out-degree <= 2 up to N vertices:
   S(T) = sum over edges x->c of min(h(c), h(x)+1)  <=  n - 2*H(T) - 1,   and report how many trees attain equality.
   usage: lemma5_check.py N     (NOTE.md quotes N = 10).  Exit code 1 on a violation."""
import sys
N = int(sys.argv[1])
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
prev = {canon(((),)): ((),)}
print('n 1 trees 1 violations 0 tight 1 max S-(n-2H-1) 0   (single vertex: S = 0 = 1 - 0 - 1)')
for n in range(2, N + 1):
    cur = {}
    for out in prev.values():
        for v in range(n - 1):
            t = tuple(tuple(x) for x in ([list(t) for t in out] + [[v]])); cur.setdefault(canon(t), t)
            if len(out[v]) < 2:
                o = [list(t) for t in out] + [[]]; o[v].append(n - 1); t = tuple(tuple(x) for x in o); cur.setdefault(canon(t), t)
    bad = 0; tight = 0; worst = -99
    for t in cur.values():
        h = hs(t); S = sum(min(h[c], h[x] + 1) for x in range(n) for c in t[x]); g = S - (n - 2 * max(h) - 1)
        worst = max(worst, g); bad += g > 0; tight += g == 0
    print('n', n, 'trees', len(cur), 'violations', bad, 'tight', tight, 'max S-(n-2H-1)', worst, flush=True)
    if bad: sys.exit(1)
    prev = cur
