#!/usr/bin/env python3
"""Slow, obvious reference checker, written independently of mc.h.
usage: slowcheck.py file   -> prints OK / ILLEGAL / SAFETY / PROGRESS
Progress is tested straight from the definition: a violation is an infinite
schedule suffix with no crit, in which the set M of processes that move is
exactly the set of processes that are ever active.  Since the system is finite
this is a closed walk c -> ... -> c through crit-free configs in which every
process not in rem at some point moves; we search (config, set-of-movers)
pairs by brute force from every reachable crit-free config."""
import sys
from collections import deque

def load(fn):
    tok = open(fn).read().split()
    it = iter(tok)
    n = int(next(it)); b = int(next(it))
    procs = []
    for _ in range(n):
        ns = int(next(it)); st = []
        for _ in range(ns):
            t = next(it)
            if t == 'R':
                st.append(('R', [int(next(it)) for _ in range(b)]))
            else:
                c = int(next(it)) % b; q = int(next(it))
                st.append(('A', c, q))
        procs.append(st)
    return n, b, procs

def legal(n, b, procs):
    for st in procs:
        for s, a in enumerate(st):
            succ = a[1] if a[0] == 'R' else [a[2]]
            for q in succ:
                if not (0 <= q < len(st)): return False
                if s == 0 and q == 0: return False
                if s == 1 and q != 0: return False
                if s != 1 and q == 0: return False
    return True

def step(procs, b, cfg, p):
    states, r = cfg
    a = procs[p][states[p]]
    if a[0] == 'R':
        q = a[1][r]; r2 = r
    else:
        q = a[2]; r2 = (r + a[1]) % b
    s2 = list(states); s2[p] = q
    return (tuple(s2), r2)

def check(n, b, procs):
    if not legal(n, b, procs): return 'ILLEGAL'
    init = (tuple([0]*n), 0)
    seen = {init}; dq = deque([init])
    while dq:
        c = dq.popleft()
        if sum(1 for s in c[0] if s == 1) >= 2: return 'SAFETY'
        for p in range(n):
            d = step(procs, b, c, p)
            if d not in seen: seen.add(d); dq.append(d)
    nocrit = [c for c in seen if 1 not in c[0]]
    for c0 in nocrit:
        # walk from c0 back to c0; track movers and processes seen active
        start = (c0, frozenset(), frozenset(i for i in range(n) if c0[0][i] != 0))
        vis = {start}; dq = deque([start])
        while dq:
            c, mv, act = dq.popleft()
            for p in range(n):
                d = step(procs, b, c, p)
                if 1 in d[0]: continue
                mv2 = mv | {p}
                act2 = act | frozenset(i for i in range(n) if d[0][i] != 0)
                if d == c0 and act2 and act2 <= mv2:
                    return 'PROGRESS'
                nx = (d, mv2, act2)
                if nx not in vis: vis.add(nx); dq.append(nx)
    return 'OK'

if __name__ == '__main__':
    n, b, procs = load(sys.argv[1])
    print(check(n, b, procs))
