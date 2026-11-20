#!/usr/bin/env python3
"""show.py FILE : for each machine line 'w n t.. g' print C(q) per state (literal BFS)"""
import sys
from collections import deque
def spreads(w, n, d):
    C = [set() for _ in range(n)]
    seen = {(0, "")}; dq = deque([(0, "")])
    while dq:
        q, x = dq.popleft()
        if len(x) >= w: C[q].add(x.count("1"))
        for b in (0, 1):
            nd = (d[q][b], (x + str(b))[-w:])
            if nd not in seen: seen.add(nd); dq.append(nd)
    return C
for line in open(sys.argv[1]):
    v = list(map(int, line.split()))
    if not v: continue
    w, n = v[0], v[1]; d = [(v[2+2*q], v[3+2*q]) for q in range(n)]
    C = spreads(w, n, d)
    print(line.strip())
    for q in range(n): print("   q%d -> 0:%d 1:%d  C=[%s] spread %s" % (q, d[q][0], d[q][1], ",".join(map(str,sorted(C[q]))), (max(C[q])-min(C[q])) if C[q] else None))
