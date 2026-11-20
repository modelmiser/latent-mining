#!/usr/bin/env python3
# spreads of the saturating counter with states 0..K, for window w (literal product BFS)
import sys
from collections import deque
def Csets(w, n, d):
    C = [set() for _ in range(n)]
    seen = {(0, 0, 0)}; dq = deque([(0, 0, 0)])  # (state, window as int, length capped at w)
    mask = (1 << w) - 1
    while dq:
        q, u, l = dq.popleft()
        if l >= w: C[q].add(bin(u).count("1"))
        for b in (0, 1):
            nd = (d[q][b], ((u << 1) | b) & mask, min(l + 1, w))
            if nd not in seen: seen.add(nd); dq.append(nd)
    return C
if __name__ == "__main__":
    for w in range(1, 13):
        row = []
        for K in range(1, w + 1):
            d = [(max(k - 1, 0), min(k + 1, K)) for k in range(K + 1)]
            C = Csets(w, K + 1, d)
            row.append(max(max(c) - min(c) for c in C))
        print("w=%2d max spread for K=1..w:" % w, row)
