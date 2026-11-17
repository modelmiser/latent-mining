#!/usr/bin/env python3
# open-growth: literal definitions + helpers.  Loops are tuples of 0/1, cyclic.
import random, glob, os
BASE = os.path.dirname(os.path.abspath(__file__)) + '/..'
def words(v, k):
    m = len(v); return [tuple(v[(i + j) % m] for j in range(k)) for i in range(m)]
def counts(v, k):
    c = {}
    for x in words(v, k): c[x] = c.get(x, 0) + 1
    return c
def universal(v, n):
    m = len(v)
    if m < (1 << n): return False
    seen = set(); x = 0; mask = (1 << n) - 1
    for j in range(n): x = (x << 1) | v[j % m]
    for i in range(m):
        seen.add(x); x = ((x << 1) & mask) | v[(i + n) % m]
    return len(seen) == (1 << n)
def loss(v, k): return v[:k] + v[k + 1:]
def dbl(v, k): return v[:k + 1] + v[k:]
def edits(v):
    out = [(('pass', -1), v)]
    for k in range(len(v)):
        if len(v) >= 2: out.append((('loss', k), loss(v, k)))
        out.append((('dbl', k), dbl(v, k)))
    return out
def replies(w1, n): return [nm for nm, w2 in edits(w1) if universal(w2, n)]
def repairable(w, n): return all(replies(w1, n) for _, w1 in edits(w))
def runs(v):
    """list of (start, length, letter) of cyclic runs; [] if constant"""
    m = len(v); st = [i for i in range(m) if v[i] != v[i - 1]]
    return [(st[j], ((st[(j + 1) % len(st)] - st[j] - 1) % m) + 1, v[st[j]]) for j in range(len(st))]
def singletons(v):
    m = len(v); return [i for i in range(m) if v[i] != v[i - 1] and v[i] != v[(i + 1) % m]]
def S(v): return len(singletons(v))
def T(s): return tuple(int(c) for c in s)
def load_repairable():
    """dict n -> sorted list of distinct repairable loops (strings) from the solvers' files"""
    out = {}
    def add(n, s):
        out.setdefault(n, set()).add(s)
    for n, Ls in ((3, (10, 11, 12)), (4, (18, 19, 20, 21, 22)), (5, (34, 35, 36))):
        for L in Ls:
            p = f'{BASE}/solver-a/val3_{n}_{L}.txt'
            for line in open(p):
                t = line.split()
                if len(t) >= 2 and t[0] == 'R' and set(t[1]) <= set('01') and len(t[1]) == L: add(n, t[1])
    for line in open(f'{BASE}/solver-b/n5L34.txt'):
        t = line.strip()
        if t and set(t) <= set('01'): add(5, t)
    for line in open(f'{BASE}/solver-b/n4L18.txt'):
        t = line.strip()
        if t and set(t) <= set('01'): add(4, t)
    six = ['1110100100000011110010101000110011010101100010111011011111101000100111000010',
           '000001001011011111100110001010100011110100111000011010101110110010010111010010',
           '11001010100100010111011111100110000110100010011100011110101011011010000001011110']
    for s in six: add(6, s)
    return {n: sorted(v) for n, v in out.items()}
def random_debruijn(n, rng):
    """random Eulerian circuit of the de Bruijn graph on (n-1)-bit nodes (Hierholzer, random edge order)"""
    N = 1 << (n - 1); mask = N - 1
    nxt = {u: rng.sample((0, 1), 2) for u in range(N)}
    stack = [0]; circ = []
    while stack:
        u = stack[-1]
        if nxt[u]:
            b = nxt[u].pop(); stack.append(((u << 1) & mask) | b)
        else: circ.append(stack.pop())
    circ.reverse()            # node sequence, length 2^n + 1, closed
    seq = tuple(u & 1 for u in circ[1:])
    assert universal(seq, n) and len(seq) == 1 << n
    return seq
