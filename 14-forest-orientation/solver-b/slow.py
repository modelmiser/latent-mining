#!/usr/bin/env python3
"""slow.py : direct implementation of the game definition (no charges, no normal form).
State = forest with explicit edge directions. Algorithm may choose ANY valid orientation of the WHOLE
new forest (every edge of every component may be flipped); cost = number of old edges changed.
Only reduction: states are memoised up to isomorphism (canonical form = min over roots of a nested tuple).
usage: slow.py nmax"""
import sys, itertools
from functools import lru_cache
sys.setrecursionlimit(10000)

def enc(adj, x, p):
    # adj[x] = list of (y, d) ; d=1 if edge is y->x (points to x), 0 if x->y
    return tuple(sorted((d, enc(adj, y, x)) for (y, d) in adj[x] if y != p))

def canon(n, edges):
    # edges: list of (tail, head)
    adj = [[] for _ in range(n)]
    for t, h in edges:
        adj[t].append((h, 0)); adj[h].append((t, 1))
    seen = [False]*n; comps = []
    for s in range(n):
        if seen[s]: continue
        st = [s]; seen[s] = True; vs = []
        while st:
            x = st.pop(); vs.append(x)
            for y, _ in adj[x]:
                if not seen[y]: seen[y] = True; st.append(y)
        comps.append(min(enc(adj, r, -1) for r in vs))
    return tuple(sorted(comps))

def build(c):
    # labelled representative of canonical forest c : returns (n, edges, comp-id list)
    edges = []; comp = []; cnt = [0]
    def rec(t, ci):
        me = cnt[0]; cnt[0] += 1; comp.append(ci)
        for d, sub in t:
            ch = rec(sub, ci)
            edges.append((ch, me) if d == 1 else (me, ch))
        return me
    for ci, t in enumerate(c): rec(t, ci)
    return cnt[0], edges, comp

@lru_cache(maxsize=None)
def value(c):
    if len(c) == 1: return 0
    n, edges, comp = build(c)
    m = len(edges); best = 0
    done = set()
    for u in range(n):
        for v in range(u+1, n):
            if comp[u] == comp[v]: continue
            alle = edges + [(u, v)]
            mn = None; seen = {}
            for mask in range(1 << (m+1)):
                out = [0]*n; ok = True; cost = 0; ne = []
                for i, (t, h) in enumerate(alle):
                    if (mask >> i) & 1:
                        t, h = h, t
                        if i < m: cost += 1
                    out[t] += 1
                    if out[t] > 2: ok = False; break
                    ne.append((t, h))
                if not ok: continue
                if mn is not None and cost >= mn: continue
                cc = canon(n, ne)
                val = cost + value(cc)
                if mn is None or val < mn: mn = val
                if mn == 0: break
            if mn > best: best = mn
    return best

if __name__ == "__main__":
    nmax = int(sys.argv[1])
    for n in range(1, nmax+1):
        value.cache_clear()
        t = value(tuple([()]*n))
        print("n=%d T=%d states=%d" % (n, t, value.cache_info().currsize), flush=True)
