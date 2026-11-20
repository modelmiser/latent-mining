#!/usr/bin/env python3
"""Plays the explicit FIFO Adversary strategy for load 4 on 12 bins (see proofs.txt, P6)
against EVERY sequence of Rule answers and asserts that each branch ends with a load-4 bin.
Labelled bins 0..11, direct simulation of the rules; the only imported data is the
7-bin load-3 strategy table adv_n7_L3.txt (itself checked by check.py).
"""
import sys
N, L = 12, 4
sys.setrecursionlimit(10000)
stats = {'leaves': 0, 'maxballs': 0}


def norm_map(st):
    m = {}
    for w, o in st:
        for x in (w, o):
            if x not in m:
                m[x] = len(m)
    return m


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
    """Adversary offers {u,v}; cont(st2, receiver) is called for both Rule answers unless load L appears."""
    assert u != v and 0 <= u < N and 0 <= v < N
    c = comp(st)
    assert c[u] != c[v], 'illegal offer (same component)'
    for w, o in ((u, v), (v, u)):
        st2 = st + ((w, o),)
        stats['maxballs'] = max(stats['maxballs'], len(st2))
        if max(loads(st2)) >= L:
            stats['leaves'] += 1
        else:
            cont(st2, w)


def expire(st, cnt):
    assert len(st) >= cnt
    return st[cnt:]


# ---- stage Ia: certified 7-bin strategy forcing load 3, played on bins 0..6
table = {}
for line in open('adv_n7_L3.txt'):
    if line.startswith('#'):
        continue
    r, mv, s = line.rstrip('\n').split('|')
    key = tuple(tuple(int(x) for x in b.split(':')) for b in s.split(' ')) if s else ()
    table[key] = mv


def stage_Ia(st):
    l = loads(st)
    if max(l) >= 3:
        return stage_Ib(st, l.index(max(l)), len(st))
    m = norm_map(st)
    key = tuple((m[w], m[o]) for w, o in st)
    mv = table[key]
    if mv == 'E':
        return stage_Ia(expire(st, 1))
    _, a, b = mv.split()
    inv = {v: k for k, v in m.items()}
    fresh = [v for v in range(7) if v not in m]      # only bins 0..6 are used in this stage
    def real(lab):
        lab = int(lab)
        return inv[lab] if lab in inv else fresh[lab - len(m)]
    offer(st, real(a), real(b), lambda st2, w: stage_Ia(st2))


# ---- stage Ib: load-2 bin Y on bins 7..10 without expiry; then offer {x, Y}
def stage_Ib(st, x, nold):
    def c1(st, r1):
        def c2(st, r2):
            def c3(st, Y):
                def c4(st, w):
                    assert w == Y            # (x receiving would have been load 4 = leaf)
                    st = expire(st, nold)    # all stage-Ia balls are older: expire them
                    assert len(st) == 4 and loads(st)[Y] == 3
                    if st[0][0] != Y:        # oldest ball is the junk ball: expire -> clean 3-star
                        st = expire(st, 1)
                        return stage_II(st, Y)
                    return stage_Iprime(st, Y)
                offer(st, x, Y, c4)
            offer(st, r1, r2, c3)
        offer(st, 9, 10, c2)
    offer(st, 7, 8, c1)


# ---- stage I': Y has load 3 in a 5-bin component, 7 bins untouched
def stage_Iprime(st, Y):
    f = untouched(st)
    assert len(f) == 7
    def c1(st, r1):
        def c2(st, r2):
            def c3(st, a):          # a has load 2
                def c4(st, g):      # g has load 1, fresh
                    def c5(st, w):
                        if w == a:  # a has load 3, different component from Y
                            before = stats['leaves']
                            offer(st, a, Y, lambda *_: (_ for _ in ()).throw(AssertionError('no win')))
                            assert stats['leaves'] == before + 2
                            return
                        def c6(st, w2):
                            assert w2 == g
                            st = expire(st, len(st) - 3)
                            assert loads(st)[g] == 3 and len(st) == 3
                            return stage_II(st, g)
                        offer(st, g, Y, c6)
                    offer(st, a, g, c5)
                offer(st, f[4], f[5], c4)
            offer(st, r1, r2, c3)
        offer(st, f[2], f[3], c2)
    offer(st, f[0], f[1], c1)


# ---- stage II: clean 3-star at z0 (4 bins), 8 untouched bins: binary merging, no expiry
def stage_II(st, z0):
    assert len(st) == 3 and loads(st)[z0] == 3
    f = untouched(st)
    assert len(f) == 8
    def merge(st, heads, done):
        # heads: bins of equal load in pairwise different components still to be paired
        if len(heads) >= 2:
            return offer(st, heads[0], heads[1], lambda st2, w: merge(st2, heads[2:], done + [w]))
        if len(done) > 1:
            return merge(st, done, [])
        z = done[0]
        assert loads(st)[z] == 3
        before = stats['leaves']
        offer(st, z, z0, lambda *_: (_ for _ in ()).throw(AssertionError('no win')))
        assert stats['leaves'] == before + 2
    merge(st, f, [])


stage_Ia(())
print(f"OK: every Rule line loses on {N} bins; leaves={stats['leaves']} max balls present={stats['maxballs']}")
