#!/usr/bin/env python3
"""Generator of an explicit Adversary strategy table for 'force load K on N bins'
(the recursive strategy of PROOFS.md, Theorem U).  UNTRUSTED: its output is checked by
check_adv.py, which shares no code with this file.

usage: gen.py K N out.txt
Output lines:  <canonical state>|<move>
  state: balls newest first, each 'r.o' (receiver.other), bins renamed in order of first
         appearance (receiver before other); empty state = empty string.
  move : 'E' (expire)  or  'O a b' (offer; a, b canonical names; untouched bins are named
         m, m+1 where m = number of touched bins).
"""
import sys, threading

sys.setrecursionlimit(1000000)
threading.stack_size(1 << 29)

K = int(sys.argv[1]); N = int(sys.argv[2]); OUT = sys.argv[3]
table = {}
inprog = dict()
stats = {'rewinds': 0, 'maxbins': 0}


def canon_map(state):
    mp = {}
    for (r, o) in reversed(state):
        if r not in mp: mp[r] = len(mp)
        if o not in mp: mp[o] = len(mp)
    return mp


def canon(state, mp):
    return ' '.join('%d.%d' % (mp[r], mp[o]) for (r, o) in reversed(state))


def find(p, x):
    while p[x] != x:
        p[x] = p[p[x]]; x = p[x]
    return x


def comps(state):
    p = list(range(N))
    for (r, o) in state:
        a, b = find(p, r), find(p, o)
        assert a != b
        p[a] = b
    return p


def load(state, x):
    return sum(1 for (r, o) in state if r == x)


def fresh(state, excl):
    used = set()
    for (r, o) in state:
        used.add(r); used.add(o)
    for b in range(N):
        if b not in used and b not in excl:
            return b
    raise AssertionError('out of bins: N too small')


def fail(*a):
    raise AssertionError('strategy ran off its end without the expected load')


class Cycle(Exception):
    def __init__(self, key, thunk):
        self.key = key; self.thunk = thunk


def rewind(key, idx):
    """forget every table entry made since (and including) the first visit of key"""
    while len(table) > idx:
        table.popitem()
    while inprog:
        k, _ = inprog.popitem()
        if k == key: break
    stats['rewinds'] += 1


def offer(state, u, v, ctx, cont):
    assert u != v
    p = comps(state)
    assert find(p, u) != find(p, v), 'same component'
    mp = canon_map(state)
    key = canon(state, mp)
    if key in table:
        if key in inprog:
            # the play has returned to a position it was in before: drop the detour
            raise Cycle(key, lambda: offer(state, u, v, ctx, cont))
        return
    m = len(mp)
    stats['maxbins'] = max(stats['maxbins'], m + (u not in mp) + (v not in mp))
    names = []
    nxt = m
    for x in (u, v):
        if x in mp: names.append(mp[x])
        else: names.append(nxt); nxt += 1
    idx = len(table)
    table[key] = 'O %d %d' % tuple(names)
    inprog[key] = 1
    try:
        for (r, o) in ((u, v), (v, u)):
            s2 = state + ((r, o),)
            l = load(s2, r)
            hit = None
            for (t, c) in ctx:
                if l == t: hit = (t, c)
                else: assert l < t, 'load overshoot'
            if hit is not None:
                if hit[1] is not None: hit[1](s2, r)
            else:
                cont(s2, r)
    except Cycle as e:
        if e.key != key: raise
        rewind(key, idx)
        return e.thunk()
    inprog.pop(key)


def expire(state, cnt, cont):
    if cnt == 0:
        return cont(state)
    assert state
    mp = canon_map(state)
    key = canon(state, mp)
    if key in table:
        if key in inprog:
            raise Cycle(key, lambda: expire(state, cnt, cont))
        return
    idx = len(table)
    table[key] = 'E'
    inprog[key] = 1
    try:
        expire(state[1:], cnt - 1, cont)
    except Cycle as e:
        if e.key != key: raise
        rewind(key, idx)
        return e.thunk()
    inprog.pop(key)


def tree(s, j, excl, ctx, cont):
    """leader of load exactly j on 2^j untouched bins, no expiry"""
    if j == 0:
        return cont(s, fresh(s, excl))
    tree(s, j - 1, excl, ctx,
         lambda s1, u: tree(s1, j - 1, excl | {u}, ctx,
                            lambda s2, v: offer(s2, u, v, ctx, cont)))


def cascade(s, cur, rest, ctx):
    assert rest, 'cascade ran out'
    offer(s, cur, rest[0], ctx, lambda s2, r: cascade(s2, r, rest[1:], ctx))


def feed(s, f, feeders, ctx, cont):
    if not feeders:
        return cont(s)
    offer(s, f, feeders[0], ctx,
          lambda s2, r: feed(s2, f, feeders[1:], ctx, cont) if r == f
          else cascade(s2, r, feeders[1:], ctx))


def force(state, k, ctx):
    """ctx[-1][0] == k.  Forces a bin of load k; then the continuation in ctx fires."""
    if k == 1:
        u = fresh(state, set()); v = fresh(state, {u})
        return offer(state, u, v, ctx, fail)
    if k == 2:
        return tree(state, 2, set(), ctx, fail)

    def final(s, F):
        tree(s, k - 1, set(), ctx, lambda s1, z: offer(s1, z, F, ctx, fail))

    def round3(s, Y):
        def collect(s1, i, cs):
            if i < 2:
                base = len(s1)
                return tree(s1, 1, set(), ctx,
                            lambda s2, f: feed(s2, f, cs[::-1] + [Y], ctx,
                                               lambda s3: expire(s3, base, lambda s4: final(s4, f))))
            tree(s1, i, set(), ctx, lambda s2, c: collect(s2, i - 1, cs + [c]))
        collect(s, k - 2, [])

    def round2(s, Y):
        if k == 3:
            return final(s, Y)

        def got_a(s1, a):
            base = len(s1)
            nxt = final if k == 4 else round3
            tree(s1, k - 3, set(), ctx,
                 lambda s2, w: feed(s2, w, [a, Y], ctx,
                                    lambda s3: expire(s3, base, lambda s4: nxt(s4, w))))
        tree(s, k - 2, set(), ctx, got_a)

    def afterA(s, x):
        base = len(s)
        tree(s, k - 2, set(), ctx,
             lambda s2, y: offer(s2, x, y, ctx,
                                 lambda s3, r: expire(s3, base, lambda s4: round2(s4, y))))

    force(state, k - 1, ctx + ((k - 1, afterA),))


def main():
    force((), K, ((K, None),))
    with open(OUT, 'w') as f:
        for key, mv in table.items():
            f.write(key + '|' + mv + '\n')
    print('K=%d N=%d table states=%d rewinds=%d max bins used=%d'
          % (K, N, len(table), stats['rewinds'], stats['maxbins']))


t = threading.Thread(target=main)
t.start(); t.join()
