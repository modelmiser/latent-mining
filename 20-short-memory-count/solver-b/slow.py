#!/usr/bin/env python3
"""Slow, literal implementation of the definition.
usage: slow.py W            -> enumerate ALL raw machines with W+1 states, print min g and histogram
       slow.py check FILE   -> each line 'w n t00 t01 t10 t11 ... g' ; recompute g literally and compare
"""
import sys, itertools
from collections import deque

def spreads(w, n, d):
    # d[q][b]; start state 0. Literal product BFS over (state, string-so-far truncated to last w bits, with length<w kept exact)
    C = [set() for _ in range(n)]
    seen = set(); dq = deque([(0, "")]); seen.add((0, ""))
    while dq:
        q, x = dq.popleft()
        if len(x) >= w:
            C[q].add(x.count("1"))
        for b in (0, 1):
            y = (x + str(b))[-w:]
            nd = (d[q][b], y)
            if nd not in seen:
                seen.add(nd); dq.append(nd)
    return C

def simple_cycles(n, d, reach):
    # enumerate all simple cycles (as state tuples with smallest state first), literal DFS
    out = []
    def dfs(start, path, onpath):
        q = path[-1]
        for b in (0, 1):
            t = d[q][b]
            if t == start:
                out.append(tuple(path))
            elif t > start and t not in onpath and t in reach:
                onpath.add(t); path.append(t)
                dfs(start, path, onpath)
                path.pop(); onpath.discard(t)
    for s in sorted(reach):
        dfs(s, [s], {s})
    return out

def g_of(w, n, d):
    C = spreads(w, n, d)
    reach = {q for q in range(n) if C[q]}
    # (every reachable state has nonempty C: extend by reading w more bits backwards? no - reachable in <w steps only
    #  states could have empty C; use true reachability)
    reach = set(); st = [0]; reach.add(0)
    while st:
        q = st.pop()
        for b in (0, 1):
            if d[q][b] not in reach:
                reach.add(d[q][b]); st.append(d[q][b])
    best = -1
    for cyc in simple_cycles(n, d, reach):
        for q in cyc:
            assert C[q], "cycle state with empty C"
        m = min(max(C[q]) - min(C[q]) for q in cyc)
        best = max(best, m)
    return best

def main():
    if sys.argv[1] == "check":
        cnt = 0
        for line in open(sys.argv[2]):
            v = list(map(int, line.split()))
            if not v: continue
            w, n = v[0], v[1]
            d = [(v[2 + 2 * q], v[3 + 2 * q]) for q in range(n)]
            g = v[2 + 2 * n]
            gg = g_of(w, n, d)
            if gg != g:
                print("MISMATCH", line.strip(), "slow says", gg); sys.exit(1)
            cnt += 1
        print("checked", cnt, "machines, all agree")
        return
    w = int(sys.argv[1]); n = w + 1
    hist = {}
    for flat in itertools.product(range(n), repeat=2 * n):
        d = [(flat[2 * q], flat[2 * q + 1]) for q in range(n)]
        g = g_of(w, n, d)
        hist[g] = hist.get(g, 0) + 1
    print("w", w, "raw machines", n ** (2 * n), "min g =", min(hist), "hist", sorted(hist.items()))
main()
