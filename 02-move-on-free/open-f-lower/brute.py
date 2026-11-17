#!/usr/bin/env python3
"""Test (2a): exhaustive best response of the manager against the FIXED adversary P6R0
(= adv_P6R without the cosmetic refill of 1s):
   M ones; drop the residue class with fewest 1s (ties: lowest class); fill with 2s;
   while some 2 has phi-gain >= 2: free the best one (ties: lowest cell); manager replies;
   thin by the waste DP; fill with 3s.
VA(M) = least H such that SOME manager (all placements, all replies, all within cells 0..H-1)
survives.  The adversary can never force more than h^F(M), so VA(M) <= h^F(M) must hold
(table M=3..13: 4,6,7,9,11,13,14,16,18,20,21) -- a check that can fail.
Batches of equal-size allocations with no free in between are enumerated as increasing sets
(the adversary does not look at the layout inside a batch).
"""
import sys, itertools, time
from core import *
from wb import phi_fast

HF = {3: 4, 4: 6, 5: 7, 6: 9, 7: 11, 8: 13, 9: 14, 10: 16, 11: 18, 12: 20, 13: 21}

def triples(lay, H):
    return sum(l // 3 for _, l in runs(lay, H))

def pair_sets(lay, H, y, start=0):
    if y == 0:
        yield lay
        return
    o = occ_set(lay)
    for p in range(start, H - 1):
        if p not in o and p + 1 not in o:
            yield from pair_sets(place(lay, p, 2), H, y - 1, p + 2)

def end_ok(lay, M, H):
    W, keep = thin_best(lay)
    l = tuple(b for b in lay if b[1] != 1 or b[0] in keep)
    t = (M - live(l)) // 3
    return triples(l, H) >= t

def free_phase(lay, M, H, memo):
    """True iff the manager has replies that survive from here (adversary deterministic)"""
    if lay in memo:
        return memo[lay]
    base = phi_fast(lay)
    best = None
    for a, z in lay:
        if z == 2:
            g = phi_fast(remove(lay, a)) - base
            if best is None or g > best[0]:
                best = (g, a)
    if best is None or best[0] < 2:
        r = end_ok(lay, M, H)
        memo[lay] = r
        return r
    l = remove(lay, best[1])
    opts = [l]
    o = occ_set(l)
    for a, z in l:
        if z == 1:
            l1 = remove(l, a)
            for d in range(H):
                if d != a and d not in o:
                    opts.append(place(l1, d, 1))
    r = any(free_phase(x, M, H, memo) for x in opts)
    memo[lay] = r
    return r

def survives(M, H, stats):
    for ones in itertools.combinations(range(H), M):
        stats['P'] += 1
        c = [0, 0, 0]
        for x in ones:
            c[x % 3] += 1
        drop = min(range(3), key=lambda k: c[k])
        S = tuple((x, 1) for x in ones if x % 3 != drop)
        y = (M - len(S)) // 2
        memo = {}
        for lay in pair_sets(S, H, y):
            stats['pl'] += 1
            if free_phase(lay, M, H, memo):
                return (ones, lay)
    return None

if __name__ == '__main__':
    lo, hi = int(sys.argv[1]), int(sys.argv[2])
    print(" M  VA(M)  h^F(M)  ceil(5M/3)  VA<=h^F?   #P tried at VA-1 / #placements   witness (ones; layout after 2s)")
    for M in range(lo, hi + 1):
        t0 = time.time()
        H = M
        last = None
        while True:
            st = {'P': 0, 'pl': 0}
            w = survives(M, H, st)
            if w:
                break
            last = st
            H += 1
        hf = HF.get(M)
        ok = 'n/a' if hf is None else ('OK' if H <= hf else 'VIOLATION')
        print(f"{M:2d}  {H:3d}    {hf}     {-(-5 * M // 3)}        {ok}    {last}   {show(w[1], H)}   [{time.time() - t0:.0f}s]", flush=True)
