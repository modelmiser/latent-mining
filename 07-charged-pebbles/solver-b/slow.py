#!/usr/bin/env python3
"""Slow, obvious BFS straight from the definition.
State: (board tuple, index 1..N: -1 empty / charge), hand = sorted tuple of charges.
Usage: slow.py p k N   -> prints p k N  maxclean  maxnode_ever_pebbled  nstates
The board is truncated at N; the truncation is harmless iff maxnode < N
(a pebble on N+1 needs a pebble on N first)."""
import sys
from collections import deque


def run(p, k, N):
    start = (tuple([-1] * (N + 1)), tuple([k] * p))
    seen = {start}
    dq = deque([start])
    best = 0
    maxnode = 0
    while dq:
        board, hand = dq.popleft()
        occ = [i for i in range(1, N + 1) if board[i] >= 0]
        if occ:
            maxnode = max(maxnode, occ[-1])
            if len(occ) == 1:
                best = max(best, occ[0])

        def pebbled(i):
            return i == 0 or board[i] >= 0
        nxt = []
        for c in set(hand):  # P
            for i in range(1, N + 1):
                if board[i] < 0 and pebbled(i - 1):
                    b = list(board)
                    b[i] = c
                    h = list(hand)
                    h.remove(c)
                    nxt.append((tuple(b), tuple(h)))
        for i in occ:
            c = board[i]
            if pebbled(i - 1):  # R
                b = list(board)
                b[i] = -1
                nxt.append((tuple(b), tuple(sorted(hand + (c,)))))
            elif i >= 2 and c >= 1:  # F
                b = list(board)
                b[i] = -1
                nxt.append((tuple(b), tuple(sorted(hand + (c - 1,)))))
        for s in nxt:
            if s not in seen:
                seen.add(s)
                dq.append(s)
    return best, maxnode, len(seen)


if __name__ == "__main__":
    p, k, N = map(int, sys.argv[1:4])
    print(p, k, N, *run(p, k, N))
