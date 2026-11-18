# referee's own library: oriented trees, h, slots, indices, normal replies
import sys
from fractions import Fraction as Fr
sys.setrecursionlimit(100000)

def canon(m, edges):
    adj = [[] for _ in range(m)]
    for (a, b) in edges:
        adj[a].append((b, '>')); adj[b].append((a, '<'))
    def enc(v, par):
        return '(' + ''.join(sorted(d + enc(c, v) for (c, d) in adj[v] if c != par)) + ')'
    return min(enc(r, -1) for r in range(m))

def gen_trees(maxm):
    """all oriented trees (outdeg<=2) up to isomorphism, by size; each = (m, tuple(edges tail,head))"""
    levels = {1: [(1, ())]}
    for m in range(1, maxm):
        seen = {}
        for (_, edges) in levels[m]:
            od = [0] * m
            for (a, b) in edges: od[a] += 1
            for v in range(m):
                cands = [edges + ((m, v),)]
                if od[v] < 2: cands.append(edges + ((v, m),))
                for e in cands:
                    c = canon(m + 1, e)
                    if c not in seen: seen[c] = (m + 1, e)
        levels[m + 1] = list(seen.values())
    return levels

def outs(m, edges):
    o = [[] for _ in range(m)]
    for (a, b) in edges: o[a].append(b)
    return o

def hvals(m, o):
    h = [None] * m
    def f(v):
        if h[v] is None:
            h[v] = 0 if len(o[v]) < 2 else 1 + min(f(w) for w in o[v])
        return h[v]
    for v in range(m): f(v)
    return h

def slots(v, o, h):
    s = sorted([h[w] for w in o[v]] + [-1] * (2 - len(o[v])))
    return (s[0], s[1])

def has_index(sl, i):
    k = i // 2
    if i % 2 == 0: return sl[0] >= k - 1
    return sl[0] >= k - 1 and sl[1] >= k

def max_index(sl):
    return 2 * (sl[0] + 1) + (1 if sl[1] > sl[0] else 0)

def F(i):
    return Fr(i // 2) + (Fr(1, 3) if i % 2 == 0 else Fr(2, 3))

def normal_paths(x, o):
    """all directed paths x=w0->..->wd, w0..w(d-1) saturated, wd unsaturated (x saturated)"""
    res = []
    def go(path):
        v = path[-1]
        for w in o[v]:
            if len(o[w]) < 2: res.append(path + [w])
            else: go(path + [w])
    go([x])
    return res
