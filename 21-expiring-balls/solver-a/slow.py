#!/usr/bin/env python3
"""Direct, slow implementation of the definition. Labelled bins, no symmetry reduction.
State = tuple of balls in arrival order, each ball = (u, v, w) with u<v, w in {u,v}.
MODE 'fifo': Expire removes the oldest ball.  MODE 'any': Adversary may remove any ball.
Usage: slow.py n L [fifo|any] [dumpfile]   -> prints whether Adversary forces load L.
"""
import sys
from itertools import combinations


def comps(n, st):
    p = list(range(n))
    def f(x):
        while p[x] != x:
            x = p[x]
        return x
    for (u, v, w) in st:
        p[f(u)] = f(v)
    return [f(x) for x in range(n)]


def maxload(n, st):
    l = [0] * n
    for (u, v, w) in st:
        l[w] += 1
    return max(l) if l else 0


def solve(n, L, mode='fifo'):
    start = ()
    idx = {start: 0}
    order = [start]
    succ = []   # per state: (terminal?, list of adversary options); option = tuple of successor states (1 = expire, 2 = offer)
    i = 0
    while i < len(order):
        st = order[i]; i += 1
        if maxload(n, st) >= L:
            succ.append(None)
            continue
        opts = []
        if st:
            if mode == 'fifo':
                opts.append((st[1:],))
            else:
                for j in range(len(st)):
                    opts.append((st[:j] + st[j+1:],))
        c = comps(n, st)
        for u, v in combinations(range(n), 2):
            if c[u] != c[v]:
                opts.append((st + ((u, v, u),), st + ((u, v, v),)))
        for o in opts:
            for t in o:
                if t not in idx:
                    idx[t] = len(order)
                    order.append(t)
        succ.append(opts)
    win = [s is None for s in succ]
    changed = True
    while changed:
        changed = False
        for k in range(len(order)):
            if win[k]:
                continue
            for o in succ[k]:
                if all(win[idx[t]] for t in o):
                    win[k] = True
                    changed = True
                    break
    return win[0], order, win


if __name__ == '__main__':
    n = int(sys.argv[1]); L = int(sys.argv[2])
    mode = sys.argv[3] if len(sys.argv) > 3 else 'fifo'
    w, order, win = solve(n, L, mode)
    print(f"n={n} L={L} mode={mode} states={len(order)} adversary_wins={w} winstates={sum(win)}")
    if len(sys.argv) > 4:
        with open(sys.argv[4], 'w') as f:
            for st, x in zip(order, win):
                f.write(' '.join(f"{w_}:{u+v-w_}" for (u, v, w_) in st) + f"|{int(x)}\n")
