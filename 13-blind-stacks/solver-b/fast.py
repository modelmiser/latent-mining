#!/usr/bin/env python3
"""Exact N(k,n) by DP over Sorter's information states.
State: (stacks, r). stacks = tuple of k tuples (bottom..top) of flags;
flag 1 = item was refused by OUT since the last successful output (known != current min).
r = number of unarrived items. V(state) = max number of value-assignments
(consistent with flags) of the remaining values to the remaining items that Sorter beats.
No pruning other than: attempts whose answer is already known "refused" (and change nothing) are skipped.
"""
import sys
from functools import lru_cache
sys.setrecursionlimit(1000000)

def moves(st, r):
    """yield (name, [successor states])"""
    k = len(st)
    if r > 0 and len(st[0]) < 2:
        s = list(st); s[0] = st[0] + (0,)
        yield ('IN', [(tuple(s), r-1)])
    for i in range(k-1):
        if st[i] and len(st[i+1]) < 2:
            s = list(st); s[i+1] = st[i+1] + (st[i][-1],); s[i] = st[i][:-1]
            yield ('SHIFT%d' % (i+1), [(tuple(s), r)])
    for i in range(k):
        if st[i] and st[i][-1] == 0:
            s = list(st); s[i] = st[i][:-1]
            succ = (tuple(tuple(0 for _ in x) for x in s), r)
            f = list(st); f[i] = st[i][:-1] + (1,)
            yield ('OUT%d' % (i+1), [succ, (tuple(f), r)])

@lru_cache(maxsize=None)
def V(st, r):
    if r == 0 and not any(st):
        return 1
    best = 0
    for name, succ in moves(st, r):
        v = sum(V(*s) for s in succ)
        if v > best: best = v
    return best

def best_move(st, r):
    best = -1; bn = None
    for name, succ in moves(st, r):
        v = sum(V(*s) for s in succ)
        if v > best: best = v; bn = name
    return bn

def N(k, n):
    return V(tuple(() for _ in range(k)), n)

if __name__ == '__main__':
    k = int(sys.argv[1]); nmax = int(sys.argv[2])
    for n in range(1, nmax+1):
        print(k, n, N(k, n), flush=True)
