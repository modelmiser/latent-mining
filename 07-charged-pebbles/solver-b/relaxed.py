#!/usr/bin/env python3
"""RELAXED game: pebbles carry no individual charge; instead at most f F-moves in total.
(Every play of the real game with p pebbles of charge k is a play of the relaxed game with
f = p*k, so relaxed values are UPPER bounds for the real ones.)
0-1 BFS: V[S] = min number of F-moves needed to reach board set S (bitmask) with q pebbles.
Usage: relaxed.py q fmax N [dump]
Prints, for each f<=fmax: max clean node, max node pebbled, among states with V<=f.
Board truncated at N; valid iff max node < N."""
import sys
from collections import deque


def solve(q, fmax, N):
    V = {0: 0}
    dq = deque([0])
    while dq:
        S = dq.popleft()
        v = V[S]
        cnt = bin(S).count("1")
        for i in range(1, N + 1):
            bit = 1 << i
            pred = (i == 1) or (S >> (i - 1)) & 1
            if S & bit:
                T = S ^ bit
                if pred:
                    w = v
                else:
                    w = v + 1
                    if w > fmax:
                        continue
            else:
                if not pred or cnt >= q:
                    continue
                T = S | bit
                w = v
            if T not in V or V[T] > w:
                V[T] = w
                if w == v:
                    dq.appendleft(T)
                else:
                    dq.append(T)
    return V


if __name__ == "__main__":
    q, fmax, N = map(int, sys.argv[1:4])
    V = solve(q, fmax, N)
    clean = [0] * (fmax + 1)
    top = [0] * (fmax + 1)
    for S, v in V.items():
        if S == 0:
            continue
        t = S.bit_length() - 1
        top[v] = max(top[v], t)
        if S & (S - 1) == 0:
            clean[v] = max(clean[v], t)
    for f in range(1, fmax + 1):
        clean[f] = max(clean[f], clean[f - 1])
        top[f] = max(top[f], top[f - 1])
    for f in range(fmax + 1):
        print("q=%d f=%d clean=%d top=%d formula=%d %s" % (q, f, clean[f], top[f], (f + 2) * 2 ** (q - 2) if q >= 2 else 1,
              "TRUNCATED" if top[f] >= N else ""))
    if len(sys.argv) > 4:
        for S, v in sorted(V.items(), key=lambda kv: (kv[1], kv[0])):
            print(v, [i for i in range(1, N + 1) if S >> i & 1])
