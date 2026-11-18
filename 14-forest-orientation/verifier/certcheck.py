#!/usr/bin/env python3
"""Referee's certificate checker (own code).
  certcheck.py B FILE n k        solver-b adversary certificate, normal-form replies (Lemma 1)
  certcheck.py A FILE J n lb     solver-a W_J lower-bound certificate, ALL replies inside the merged tree
A tree is (n, arcs) with arcs = list of (tail, head)."""
import sys
from itertools import combinations
sys.setrecursionlimit(1000000)

def adjacency(n, arcs):
    adj = [[] for _ in range(n)]
    for t, h in arcs:
        adj[t].append((h, 'd')); adj[h].append((t, 'u'))
    return adj

def enc(adj, v, p, c):
    return '(' + c + ''.join(sorted(enc(adj, w, v, d) for w, d in adj[v] if w != p)) + ')'

def centres(n, adj):
    deg = [len(a) for a in adj]; alive = n; layer = [v for v in range(n) if deg[v] <= 1]; dead = [False] * n
    while alive > 2:
        nxt = []
        for v in layer:
            dead[v] = True; alive -= 1
            for w, _ in adj[v]:
                if not dead[w]:
                    deg[w] -= 1
                    if deg[w] == 1: nxt.append(w)
        layer = nxt
    return layer

def canon(n, arcs):
    adj = adjacency(n, arcs)
    return min(enc(adj, r, -1, 'r') for r in centres(n, adj))

def outdeg(n, arcs):
    od = [0] * n
    for t, _ in arcs: od[t] += 1
    return od

def valid(n, arcs):
    return max(outdeg(n, arcs) + [0]) <= 2

# ---------- solver-b format: '(' digit children ')', digit = 2 - outdegree, root = first vertex
def parse_b(s):
    par = []; need = []; st = []; i = 0
    while i < len(s):
        if s[i] == '(':
            par.append(st[-1] if st else -1); need.append(2 - int(s[i + 1])); st.append(len(par) - 1); i += 2
        elif s[i] == ')':
            st.pop(); i += 1
        else:
            raise ValueError('bad char')
    if st: raise ValueError('unbalanced')
    n = len(par); want = list(need); arcs = []
    for v in range(n - 1, 0, -1):          # children come after parents in preorder
        if need[v] == 1: arcs.append((v, par[v]))
        elif need[v] == 0: arcs.append((par[v], v)); need[par[v]] -= 1
        else: raise ValueError('outdegrees not realisable')
    if need[0] != 0: raise ValueError('outdegrees not realisable at root')
    od = outdeg(n, arcs)
    if od != want or max(od) > 2 or min(want) < 0: raise ValueError('bad orientation')
    return n, arcs

def normal_replies(tA, tB):
    """Adversary names vertex 0 of tA and vertex 0 of tB. Yields (cost, canonical merged tree)."""
    nA, aA = tA; nB, aB = tB; n = nA + nB
    old = list(aA) + [(t + nA, h + nA) for t, h in aB]
    u, v = 0, nA
    out = [[] for _ in range(n)]
    for t, h in old: out[t].append(h)
    res = []
    for x, y in ((u, v), (v, u)):
        if len(out[x]) <= 1:
            res.append((0, canon(n, old + [(x, y)])))
        else:
            stack = [[x]]
            while stack:
                p = stack.pop()
                for w in out[p[-1]]:
                    q = p + [w]
                    if len(out[w]) <= 1:
                        fl = set(zip(q, q[1:]))
                        new = [(h, t) if (t, h) in fl else (t, h) for t, h in old] + [(x, y)]
                        assert valid(n, new)
                        res.append((len(q) - 1, canon(n, new)))
                    else:
                        stack.append(q)
    return res

def main_b(fn, n, k):
    ccache = {}
    def cb(s):
        if s not in ccache:
            t = parse_b(s); ccache[s] = (canon(*t), t)
        return ccache[s]
    cert = {}
    for line in open(fn):
        if line.startswith('#') or not line.strip(): continue
        f = [x.strip() for x in line.split('|')]
        kk = int(f[0]); trees = f[1].split(); sA, ru = f[2].split(); sB, rv = f[3].split()
        key = tuple(sorted(cb(t)[0] for t in trees))
        assert sum(cb(t)[1][0] for t in trees) == n, 'wrong vertex count'
        assert cb(sA)[0] == cb(ru)[0] and cb(sB)[0] == cb(rv)[0], 'rooted tree is not the named tree'
        if key in cert: raise AssertionError('duplicate position')
        cert[key] = (kk, cb(sA)[0], cb(ru)[1], cb(sB)[0], cb(rv)[1])
    memo = {}; rc = {}; why = []
    def ok(state, k):
        if k <= 0: return True
        if len(state) <= 1: return False
        if (state, k) in memo: return memo[(state, k)]
        e = cert.get(state); r = True
        if e is None or e[0] < k:
            r = False; why.append('no entry with k>=%d for a reached position' % k)
        else:
            _, cA, tA, cB, tB = e; rest = list(state)
            try:
                rest.remove(cA); rest.remove(cB)
            except ValueError:
                r = False; why.append('named tree not in position')
            if r:
                kk = (enc(adjacency(*tA), 0, -1, 'r'), enc(adjacency(*tB), 0, -1, 'r'))
                if kk not in rc: rc[kk] = normal_replies(tA, tB)
                for cost, t in rc[kk]:
                    if cost < k and not ok(tuple(sorted(rest + [t])), k - cost): r = False; break
        memo[(state, k)] = r
        return r
    single = canon(1, [])
    good = ok(tuple([single] * n), k)
    print('B-cert %s n=%d k=%d: %s (entries=%d, (position,k) pairs visited=%d)%s' %
          (fn.split('/')[-1], n, k, 'ACCEPTED' if good else 'REJECTED', len(cert), len(memo), '' if good or not why else ' reason: ' + why[0]))
    return good

# ---------- solver-a format: tree := '(' { ('a'|'b') tree } ')', a = parent->child, b = child->parent
def parse_a(s):
    arcs = []; st = []; n = 0; i = 0; pend = None
    while i < len(s):
        ch = s[i]
        if ch in 'ab': pend = ch; i += 1
        elif ch == '(':
            v = n; n += 1
            if st:
                arcs.append((st[-1], v) if pend == 'a' else (v, st[-1]))
            st.append(v); i += 1
        elif ch == ')': st.pop(); i += 1
        else: raise ValueError('bad char')
    assert not st and valid(n, arcs)
    return n, arcs

def hdist(n, arcs, u):
    out = [[] for _ in range(n)]
    for t, h in arcs: out[t].append(h)
    if len(out[u]) < 2: return 0
    dist = {u: 0}; q = [u]
    for x in q:
        for y in out[x]:
            if y not in dist:
                dist[y] = dist[x] + 1
                if len(out[y]) < 2: return dist[y]
                q.append(y)
    raise AssertionError

def maxd(n, arcs): return max(hdist(n, arcs, u) for u in range(n))

def psi(n, arcs, J):
    best = min(J, maxd(n, arcs))
    for c in range(1, J):
        for F in combinations(range(len(arcs)), c):
            a2 = [(h, t) if i in F else (t, h) for i, (t, h) in enumerate(arcs)]
            if valid(n, a2): best = min(best, c + min(J, maxd(n, a2)))
    return best

def main_a(fn, J, N, LB):
    pc = {}
    def ca(s):
        if s not in pc:
            t = parse_a(s); pc[s] = (canon(*t), t)
        return pc[s]
    table = {}; trees = {}
    for line in open(fn):
        if line.startswith('#') or not line.strip(): continue
        v, rest = line.split('|'); comps = rest.split()
        key = tuple(sorted(ca(c)[0] for c in comps))
        for c in comps: trees[ca(c)[0]] = ca(c)[1]
        assert sum(ca(c)[1][0] for c in comps) == N, 'wrong vertex count'
        table[key] = max(table.get(key, 0), int(v))
    psic = {}
    def lb(state, tr):
        if len(state) == 1:
            if state[0] not in psic: psic[state[0]] = psi(tr[0], tr[1], J) if J else 0
            return psic[state[0]]
        return table.get(state, 0)
    bad = 0
    for S, v in table.items():
        if len(S) < 2: bad += 1; continue
        okc = False; seen = set()
        for i in range(len(S)):
            for j in range(i + 1, len(S)):
                if okc or (S[i], S[j]) in seen: continue
                seen.add((S[i], S[j]))
                rest = [S[x] for x in range(len(S)) if x != i and x != j]
                (nA, aA), (nB, aB) = trees[S[i]], trees[S[j]]; n = nA + nB
                old = list(aA) + [(t + nA, h + nA) for t, h in aB]
                adjA = adjacency(nA, aA); adjB = adjacency(nB, aB); su = set(); us = []; sv = set(); vs = []
                for u in range(nA):
                    e = enc(adjA, u, -1, 'r')
                    if e not in su: su.add(e); us.append(u)
                for w in range(nB):
                    e = enc(adjB, w, -1, 'r')
                    if e not in sv: sv.add(e); vs.append(w + nA)
                for u in us:
                    for w in vs:
                        if okc: break
                        good = True
                        for c in range(v):
                            for F in combinations(range(len(old)), c):
                                a2 = [(h, t) if x in F else (t, h) for x, (t, h) in enumerate(old)]
                                od = outdeg(n, a2)
                                if max(od) > 2: continue
                                for new in ((u, w), (w, u)):
                                    if od[new[0]] >= 2: continue
                                    a3 = a2 + [new]
                                    if c + lb(tuple(sorted(rest + [canon(n, a3)])), (n, a3)) < v: good = False; break
                                if not good: break
                            if not good: break
                        if good: okc = True
        if not okc: bad += 1
    single = canon(1, [])
    got = table.get(tuple([single] * N), 0)
    good = bad == 0 and got >= LB
    print('A-cert %s J=%d n=%d: claims=%d unjustified=%d start bound=%d (wanted >= %d): %s' %
          (fn.split('/')[-1], J, N, len(table), bad, got, LB, 'ACCEPTED' if good else 'REJECTED'))
    return good

if __name__ == '__main__':
    try:
        if sys.argv[1] == 'B': g = main_b(sys.argv[2], int(sys.argv[3]), int(sys.argv[4]))
        else: g = main_a(sys.argv[2], int(sys.argv[3]), int(sys.argv[4]), int(sys.argv[5]))
    except (AssertionError, ValueError) as e:
        print('REJECTED (malformed):', sys.argv[2].split('/')[-1], repr(e)); g = False
    sys.exit(0 if g else 1)
