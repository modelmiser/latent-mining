#!/usr/bin/env python3
"""Referee: ANY-removal reading. States = oriented forests up to isomorphism (own canonical form: min over roots
of a rooted string). usage: anyrem.py solve n L | anyrem.py check n L file"""
import sys
from itertools import combinations
mode, n, L = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
def canon(balls):
    adj = {}
    for w, o in balls:
        adj.setdefault(w, []).append((o, 'r'))   # edge seen from w: w receives
        adj.setdefault(o, []).append((w, 'g'))   # seen from o: o gave
    def rooted(v, par):
        return '(' + ''.join(sorted(f + rooted(u, v) for u, f in adj[v] if u != par)) + ')'
    seen = set(); trees = []
    for v in adj:
        if v in seen: continue
        compv = []; st = [v]; seen.add(v)
        while st:
            x = st.pop(); compv.append(x)
            for u, f in adj[x]:
                if u not in seen: seen.add(u); st.append(u)
        trees.append(min(rooted(r, None) for r in compv))
    return '|'.join(sorted(trees))
def normal(balls):      # relabel to 0..m-1
    m = {}
    out = []
    for w, o in balls:
        if w not in m: m[w] = len(m)
        if o not in m: m[o] = len(m)
        out.append((m[w], m[o]))
    return tuple(out)
def moves(balls):
    """returns (removals [states], offers [(sA,sB)]) with states as ball tuples or 'T'"""
    cp = list(range(n)); ld = [0] * n
    def f(x):
        while cp[x] != x: x = cp[x]
        return x
    for w, o in balls:
        if w >= n or o >= n: raise SystemExit('REJECT: more than n bins')
        a, b = f(w), f(o)
        if a == b: raise SystemExit('REJECT: not a forest')
        cp[a] = b; ld[w] += 1
    rem = [balls[:i] + balls[i + 1:] for i in range(len(balls))]
    used = 1 + max([max(b) for b in balls], default=-1)
    off = []
    for u, v in combinations(range(min(n, used + 2)), 2):
        if f(u) == f(v): continue
        if u >= used and (u, v) != (used, used + 1): continue
        if u < used <= v and v != used: continue
        off.append(tuple('T' if ld[w] + 1 >= L else balls + ((w, o),) for w, o in ((u, v), (v, u))))
    return rem, off, max(ld)
if mode == 'solve':
    rep = {'': ()}; order = ['']; succ = {}
    i = 0
    while i < len(order):
        c = order[i]; i += 1
        rem, off, _ = moves(rep[c])
        def cid(t):
            if t == 'T': return 'T'
            k = canon(t)
            if k not in rep: rep[k] = normal(t); order.append(k)
            return k
        succ[c] = ([cid(t) for t in rem], [(cid(a), cid(b)) for a, b in off])
    win = set(); ch = True
    while ch:
        ch = False
        for c, (rem, off) in succ.items():
            if c in win: continue
            if any(r in win for r in rem) or any(all(x == 'T' or x in win for x in p) for p in off):
                win.add(c); ch = True
    print(f"any-removal n={n} L={L} classes={len(order)} winning={len(win)} adversary_{'CAN' if '' in win else 'CANNOT'}")
else:
    S = {}
    for l in open(sys.argv[4]):
        if l.startswith('#'): continue
        b = normal(tuple(tuple(int(x) for x in t.split(':')) for t in l.split()))
        S[canon(b)] = b
    if '' not in S: raise SystemExit('REJECT: empty missing')
    for c, b in S.items():
        rem, off, ml = moves(b)
        if ml >= L: raise SystemExit('REJECT: load >= L')
        for r in rem:
            if canon(r) not in S: raise SystemExit(f'REJECT: not closed under removal at {b}')
        for p in off:
            if not any(x != 'T' and canon(x) in S for x in p): raise SystemExit(f'REJECT: unanswerable offer at {b}')
    print(f'ACCEPT any-removal rule set n={n} L={L}: {len(S)} classes')
