#!/usr/bin/env python3
"""Independent game searches.
 game.py relaxed q F [any] [mut]: 0-1 BFS of the relaxed game up to F F-moves; checks (a),(b) with f = exact min cost.
        'any' = an F-move may delete ANY pebble (the proof's remark).  mut: test (a) with bound-1.
 game.py real p k : plain BFS of the charged game; prints C(p,k) and max node, vs formula."""
import sys
from collections import deque
def rm(S, x): return tuple(y for y in S if y != x)
def add(S, x): return tuple(sorted(S + (x,)))
if sys.argv[1] == "relaxed":
    q = int(sys.argv[2]); F = int(sys.argv[3]); ANY = "any" in sys.argv; MUTA = "mut" in sys.argv
    dist = {(): 0}; dq = deque([()])
    while dq:
        S = dq.popleft(); d = dist[S]; s = set(S)
        free = []; cost = []
        if len(S) < q:
            if 1 not in s: free.append(add(S, 1))
            for x in S:
                if x + 1 not in s: free.append(add(S, x + 1))
        for x in S:
            if x == 1 or x - 1 in s: free.append(rm(S, x))
            if ANY or (x >= 2 and x - 1 not in s): cost.append(rm(S, x))
        for T in free:
            if dist.get(T, 99) > d: dist[T] = d; dq.appendleft(T)
        if d < F:
            for T in cost:
                if dist.get(T, 99) > d + 1: dist[T] = d + 1; dq.append(T)
    U = 2 ** (q - 2); bad = 0; bestclean = [0] * (F + 1); bestnode = [0] * (F + 1)
    for S, f in dist.items():
        if not S: continue
        if len(S) == 1:
            bestclean[f] = max(bestclean[f], S[0])
            if S[0] > (f + 2) * U - (1 if MUTA else 0): bad += 1
        bestnode[f] = max(bestnode[f], S[-1])
        if S[-1] > (f + 4) * U - 1: bad += 1
    print("relaxed q", q, "F", F, "any" if ANY else "", "mut" if MUTA else "", "states", len(dist), "violations", bad)
    print("  max clean n by exact f:", bestclean, "bound", [(f + 2) * U for f in range(F + 1)])
    print("  max node by exact f:  ", bestnode, "bound", [(f + 4) * U - 1 for f in range(F + 1)])
else:
    p = int(sys.argv[2]); k = int(sys.argv[3])
    # state: (board = sorted tuple of (node, charge), hand = sorted tuple of charges)
    start = ((), tuple([k] * p)); seen = {start}; dq = deque([start]); C = 0; mx = 0
    while dq:
        board, hand = dq.popleft(); occ = {n: c for n, c in board}
        if len(board) == 1: C = max(C, board[0][0])
        if board: mx = max(mx, board[-1][0])
        nxt = []
        spots = [n + 1 for n in [0] + list(occ) if n + 1 not in occ]
        for c in set(hand):
            h = list(hand); h.remove(c)
            for i in spots: nxt.append((tuple(sorted(board + ((i, c),))), tuple(h)))
        for n, c in board:
            rest = tuple(b for b in board if b[0] != n)
            if n == 1 or n - 1 in occ: nxt.append((rest, tuple(sorted(hand + (c,)))))
            elif n >= 2 and c >= 1: nxt.append((rest, tuple(sorted(hand + (c - 1,)))))
        for T in nxt:
            if T not in seen: seen.add(T); dq.append(T)
    print("real p", p, "k", k, "states", len(seen), "C =", C, "formula", (p * k + 2) * 2 ** (p - 2), "max node", mx, "bound(b)", (p * k + 4) * 2 ** (p - 2) - 1)
