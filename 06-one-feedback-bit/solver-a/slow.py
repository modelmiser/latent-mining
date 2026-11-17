#!/usr/bin/env python3
"""Direct, slow implementation of the definition. F(n,t) = max number of strategies
(u,a,b) with pairwise disjoint outcome sets O(u,a,b), outcomes kept as strings of both
lengths n and n-1.  Exact max independent set by plain branch and bound (no reductions
other than: branch on first remaining vertex, bound = current + remaining count)."""
import sys, itertools

def D(s):
    return {s[:i] + s[i+1:] for i in range(len(s))}

def O(u, a, b):
    return frozenset({u + a} | {u + x for x in D(a)} | {x + b for x in D(u)})

def strategies(n, t):
    W = lambda k: [''.join(p) for p in itertools.product('01', repeat=k)]
    return [(u, a, b) for u in W(t) for a in W(n - t) for b in W(n - t)]

def F_slow(n, t):
    S = strategies(n, t)
    sets = [O(*s) for s in S]
    N = len(S)
    adj = [0] * N
    for i in range(N):
        for j in range(i + 1, N):
            if sets[i] & sets[j]:
                adj[i] |= 1 << j
                adj[j] |= 1 << i
    best = [0, []]
    def rec(cand, chosen):
        if len(chosen) > best[0]:
            best[0] = len(chosen); best[1] = list(chosen)
        if not cand:
            return
        if len(chosen) + bin(cand).count('1') <= best[0]:
            return
        # pick vertex of max degree within cand for branching
        v = None; md = -1
        c = cand
        while c:
            low = c & -c; i = low.bit_length() - 1; c ^= low
            d = bin(adj[i] & cand).count('1')
            if d > md: md = d; v = i
        if md == 0:
            k = bin(cand).count('1')
            if len(chosen) + k > best[0]:
                best[0] = len(chosen) + k
                best[1] = list(chosen) + [i for i in range(N) if cand >> i & 1]
            return
        chosen.append(v)
        rec(cand & ~adj[v] & ~(1 << v), chosen)
        chosen.pop()
        rec(cand & ~(1 << v), chosen)
    rec((1 << N) - 1, [])
    return best[0], [S[i] for i in best[1]]

if __name__ == '__main__':
    nmax = int(sys.argv[1])
    for n in range(2, nmax + 1):
        row = []
        for t in range(0, n + 1):
            v, w = F_slow(n, t)
            row.append(v)
            print(f"n={n} t={t} F={v}", flush=True)
        print("ROW", n, row, flush=True)
