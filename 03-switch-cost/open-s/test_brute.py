#!/usr/bin/env python3
"""compare brute_feasible (memoised DFS) with a naive enumerator of ALL action sequences on tiny job lists,
and compare witness-style answers on hand cases."""
import sys, random, itertools
src = open('play.py').read(); src = src[:src.rindex('main()')]
sys.argv = ['play.py', '3', '16']; exec(src)
def naive(joblist):
    n = len(joblist); H = max(d for r, p, d in joblist)
    acts = list(range(n)) + ['idle', 'sw']
    def rec(t, rems, A, fl):
        if all(x == 0 for x in rems): return True
        if t >= H: return False
        for a in acts:
            if a == 'idle':
                if rec(t + 1, rems, A, False): return True
            elif a == 'sw':
                if rec(t + 1, rems, A, True): return True
            else:
                r, p, d = joblist[a]
                if rems[a] == 0 or r > t: continue
                if not (A is None or A == a or fl): continue
                if rems[a] == 1 and t + 1 > d: continue
                nr = rems[:a] + (rems[a] - 1,) + rems[a + 1:]
                if rec(t + 1, nr, a if nr[a] else None, False): return True
        return False
    return rec(0, tuple(p for r, p, d in joblist), None, False)
hand = [([(0,2,2),(1,1,2)], False), ([(0,2,4),(1,1,2)], True), ([(0,2,3),(1,1,2)], False), ([(0,2,3)], True),
        ([(0,3,5),(1,1,2),(3,1,4)], False), ([(0,3,7),(1,1,2),(3,1,4)], True)]
for jl, exp in hand:
    assert brute_feasible(jl) == exp == naive(jl), jl
rng = random.Random(7); n = 0; feas = 0
for _ in range(4000):
    k = rng.randint(1, 4); jl = []
    for _ in range(k):
        p = rng.randint(1, 3); r = rng.randint(0, 4); d = r + p + rng.randint(0, 3)
        if d > 8: d = 8
        if r + p > d: continue
        jl.append((r, p, d))
    if not jl: continue
    a = brute_feasible(jl); b = naive(jl); n += 1; feas += a
    assert a == b, jl
print('brute_feasible == naive enumerator on %d random tiny job lists (%d feasible, %d infeasible) and %d hand cases' % (n, feas, n - feas, len(hand)))
