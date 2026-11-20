#!/usr/bin/env python3
"""Slow, obvious implementation of the definition. Usage:
  slow.py brute W          -> min g over ALL (W+1)^(2(W+1)) labelled machines, start state 0
  slow.py check FILE       -> check a witness file (lines: w, s, n, then n lines 'd0 d1', then lit states)
"""
import sys, itertools

def Csets(w, n, d):
    # explicit: pairs (state, last-w-bits tuple) reachable by strings of length >= w
    cur = {(0, ())}
    for _ in range(w):
        cur = {(d[q][b], x + (b,)) for (q, x) in cur for b in (0, 1)}
    seen = set(cur); stack = list(cur)
    while stack:
        q, x = stack.pop()
        for b in (0, 1):
            p = (d[q][b], (x + (b,))[1:])
            if p not in seen:
                seen.add(p); stack.append(p)
    C = [set() for _ in range(n)]
    for q, x in seen:
        C[q].add(sum(x))
    return C

def reachable(n, d):
    r = {0}; st = [0]
    while st:
        q = st.pop()
        for b in (0, 1):
            if d[q][b] not in r:
                r.add(d[q][b]); st.append(d[q][b])
    return r

def simple_cycles(n, d, R):
    # all simple cycles as state tuples, smallest state first (bits irrelevant for g)
    out = []
    def ext(path):
        q = path[-1]
        for b in (0, 1):
            t = d[q][b]
            if t == path[0]:
                out.append(tuple(path))
            elif t > path[0] and t not in path:
                ext(path + [t])
    for s in sorted(R):
        ext([s])
    return out

def g(w, n, d):
    C = Csets(w, n, d); R = reachable(n, d)
    best = -1
    for Z in simple_cycles(n, d, R):
        for q in Z:
            assert C[q], "cycle state with empty C"
        best = max(best, min(max(C[q]) - min(C[q]) for q in Z))
    return best, C

def brute(w):
    n = w + 1
    best = None; cnt = 0; total = 0
    for flat in itertools.product(range(n), repeat=2 * n):
        d = [(flat[2 * i], flat[2 * i + 1]) for i in range(n)]
        v, _ = g(w, n, d); total += 1
        if best is None or v < best:
            best, cnt = v, 1
        elif v == best:
            cnt += 1
    print(f"w={w} machines={total} f={best} labelled_minimisers={cnt}")

def check(fn):
    toks = [l.split('#')[0].split() for l in open(fn)]
    toks = [t for t in toks if t]
    w = int(toks[0][0]); s = int(toks[1][0]); n = int(toks[2][0])
    assert n <= w + 1
    d = [(int(toks[3 + i][0]), int(toks[3 + i][1])) for i in range(n)]
    lit = set(map(int, toks[3 + n])) if len(toks) > 3 + n else None
    v, C = g(w, n, d)
    R = reachable(n, d)
    spread = {q: (max(C[q]) - min(C[q]) if C[q] else None) for q in range(n)}
    print("C sets:", {q: sorted(C[q]) for q in range(n)})
    ok = (v <= s)
    if lit is not None:
        # (2) lit spreads <= s ; (1) no cycle among reachable non-lit states
        ok2 = all(spread[q] is None or spread[q] <= s for q in lit)
        rest = R - lit
        ch = True
        while ch:
            ch = False
            for q in list(rest):
                if d[q][0] not in rest and d[q][1] not in rest:
                    rest.discard(q); ch = True
        ok2 = ok2 and not rest
        print("lit-set legal:", ok2); ok = ok and ok2
    print(f"w={w} claimed s={s} g(M)={v} ->", "ACCEPT" if ok else "REJECT")
    return ok

if sys.argv[1] == 'brute':
    brute(int(sys.argv[2]))
else:
    sys.exit(0 if check(sys.argv[2]) else 1)
