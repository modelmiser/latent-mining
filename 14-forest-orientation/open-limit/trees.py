"""Oriented forests with out-degree <= 2: explicit orientations, h, types, normal replies.
A tree is (m, out) where out[v] = tuple of out-neighbours (len <= 2); vertices 0..m-1."""
import itertools, sys
sys.setrecursionlimit(10000)

def hvals(m, out):
    """h[v] = shortest directed distance from v to an unsaturated vertex (0 if v unsaturated)."""
    h = [None] * m
    def rec(v):
        if h[v] is not None: return h[v]
        if len(out[v]) < 2: h[v] = 0
        else: h[v] = 1 + min(rec(w) for w in out[v])
        return h[v]
    for v in range(m): rec(v)
    return h

def slots(v, out, h):
    s = sorted([h[w] for w in out[v]] + [-1] * (2 - len(out[v])))
    return s[0], s[1]

def idx_of(p, q):
    """best type index of a vertex with slot values p<=q: 2k = A_k (h>=k), 2k+1 = R_k (slots >=k-1, >=k)."""
    return 2 * p + 3 if q >= p + 1 else 2 * p + 2

def F3(i):
    """3*F(i): F(A_k)=k+1/3, F(R_k)=k+2/3."""
    return 3 * (i // 2) + 1 + (i % 2)

def replies(m, out, x):
    """All normal replies at endpoint x of its own tree: list of (cost, new_out) where new_out is the
    orientation after flipping (x keeps a free slot for the new edge, which the caller adds)."""
    if len(out[x]) < 2:
        return [(0, [tuple(o) for o in out])]
    res = []
    def walk(path):
        v = path[-1]
        if len(out[v]) < 2:
            o = [list(t) for t in out]
            for a, b in zip(path, path[1:]):
                o[a].remove(b); o[b].append(a)
            res.append((len(path) - 1, [tuple(t) for t in o]))
            return
        for w in out[v]:
            walk(path + [w])
    walk([x])
    return res

def canon(m, out):
    """canonical form of an oriented tree (min over roots of a rooted code)."""
    adj = [[] for _ in range(m)]
    for v in range(m):
        for w in out[v]:
            adj[v].append((w, 'o')); adj[w].append((v, 'i'))
    def code(v, par):
        return '(' + ''.join(sorted(d + code(w, v) for w, d in adj[v] if w != par)) + ')'
    return min(code(r, -1) for r in range(m))

def all_trees(maxm):
    """all oriented trees with outdeg<=2 up to isomorphism, sizes 1..maxm: dict m -> list of out-lists."""
    res = {1: [[()]]}
    for m in range(2, maxm + 1):
        seen = {}
        for out in res[m - 1]:
            for v in range(m - 1):
                # new leaf m-1 with edge v->leaf or leaf->v
                if len(out[v]) < 2:
                    o = [tuple(t) for t in out] + [()]
                    o[v] = o[v] + (m - 1,)
                    c = canon(m, o)
                    if c not in seen: seen[c] = o
                o = [tuple(t) for t in out] + [(v,)]
                c = canon(m, o)
                if c not in seen: seen[c] = o
        res[m] = list(seen.values())
    return res
