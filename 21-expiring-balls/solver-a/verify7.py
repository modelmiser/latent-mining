#!/usr/bin/env python3
"""Replays the hand-written 7-bin FIFO strategy for load 3 (proofs.txt P5, 'in words')
against every Rule answer. Direct labelled simulation, no tables."""
N, L = 7, 3
leaves = [0]


def loads(st):
    l = [0] * N
    for w, o in st:
        l[w] += 1
    return l


def comp(st):
    c = list(range(N))
    for w, o in st:
        a, b = c[w], c[o]
        if a != b:
            c = [b if x == a else x for x in c]
    return c


def untouched(st):
    t = set(x for b in st for x in b)
    return [v for v in range(N) if v not in t]


def offer(st, u, v, cont):
    c = comp(st)
    assert u != v and c[u] != c[v], 'illegal offer'
    for w, o in ((u, v), (v, u)):
        st2 = st + ((w, o),)
        if max(loads(st2)) >= L:
            leaves[0] += 1
        else:
            cont(st2, w)


def fail(*_):
    raise AssertionError('branch without load 3')


def build2(st, bins, cont):
    """load-2 bin on 4 untouched bins, no expiry"""
    offer(st, bins[0], bins[1], lambda s1, r1: offer(s1, bins[2], bins[3], lambda s2, r2: offer(s2, r1, r2, cont)))


def step3(st, f):
    assert len(st) == 2 and loads(st)[f] == 2
    u = untouched(st)
    assert len(u) == 4
    build2(st, u, lambda s, z: offer(s, f, z, fail))


def step1(st, x):
    if st[1][0] == x and st[2][0] == x:          # x holds the two youngest balls
        return step3(st[1:], x)
    u = untouched(st)
    def c2(s, f):
        def c3(s, w):
            assert w == f
            return step3(s[len(s) - 2:], f)
        offer(s, x, f, c3)
    offer(st, u[0], u[1], c2)


build2((), [0, 1, 2, 3], step1)
print('OK: every Rule line reaches load 3 on 7 bins; leaves =', leaves[0])
