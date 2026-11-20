#!/usr/bin/env python3
"""Referee reference: direct labelled implementation of the game, no symmetry.
usage: ref.py n L   -> prints whether Adversary can force load L on n bins (FIFO)."""
import sys
from itertools import combinations
n, L = int(sys.argv[1]), int(sys.argv[2])
def comp(st):
    c = list(range(n))
    def f(x):
        while c[x] != x: x = c[x]
        return x
    for (u, v, w) in st:
        c[f(u)] = f(v)
    return [f(x) for x in range(n)]
def load(st):
    l = [0]*n
    for (u, v, w) in st: l[w] += 1
    return l
# explore all states reachable from empty; terminal = some load >= L
succ = {}   # state -> (expire_succ or None, [ (sA, sB) per offer ])
TERM = 'T'
stack = [()]
seen = {()}
while stack:
    s = stack.pop()
    ex = s[1:] if s else None
    offers = []
    c = comp(s)
    for u, v in combinations(range(n), 2):
        if c[u] == c[v]: continue
        pair = []
        for w in (u, v):
            t = s + ((u, v, w),)
            if max(load(t)) >= L: pair.append(TERM)
            else: pair.append(t)
        offers.append(tuple(pair))
    succ[s] = (ex, offers)
    for t in ([ex] if ex is not None else []) + [x for p in offers for x in p]:
        if t != TERM and t not in seen:
            seen.add(t); stack.append(t)
win = set()
changed = True
while changed:
    changed = False
    for s, (ex, offers) in succ.items():
        if s in win: continue
        ok = (ex is not None and ex in win) or any(all(x == TERM or x in win for x in p) for p in offers)
        if ok: win.add(s); changed = True
print(f"n={n} L={L} states={len(succ)} winning={len(win)} adversary_{'CAN' if () in win else 'CANNOT'}")
