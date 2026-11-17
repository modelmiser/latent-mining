#!/usr/bin/env python3
"""Periodic density game for the P6R family (prototype; the C++ version pergame.cpp is the fast one).

Manager's stage-1 layout P: runs of 1s (lengths `home`, cycled) separated by 3-gaps (the only
layouts that survive the pure game-O finalisation; see pat.py).
Adversary keeps a periodic subset S of P.  Manager places the y = (M-|S|)/2 2s: any periodic
placement on free pairs inside the pattern, the rest contiguously at the tail.
Adversary frees a periodic subset F of the in-pattern 2s and optionally every other tail 2.
Manager deletes any <= |F| 1s (a moved 1 can be freed at no cost, so a move is at best a deletion)
WITH FORESIGHT of F (this favours the manager, so the value is a lower bound for the adversary
against this layout), computed exactly by an interdiction DP.
Adversary thins by the waste DP.  value = W;  final extent >= M - 2 + W.
We report the slope dW/dM between R1 and R2 repetitions; 2/3 means 'held at 5M/3'.
"""
import sys, itertools
from fractions import Fraction
from core import *

NEG = -10**9

def interdict(lay, K):
    """min over deletions of <= K ones of max over thinnings of W (exact)."""
    # state: (k, shape) -> vmax ; shape = tuple(v[e]-vmax clamped to >= -2)
    cur = {(0, (0, -2, -2)): 0}     # start: end class 0 at value 0; others unreachable (clamped is sound: see proofs.txt D2)
    # NOTE clamping -inf to vmax-2 is sound only if a block could really end there; at the very start
    # nothing precedes, so we instead keep true -inf with a flag
    cur = {(0, (0, NEG, NEG)): 0}
    for a, z in lay:
        nxt = {}
        for (k, sh), vm in cur.items():
            v = [vm + d if d > NEG // 2 else NEG for d in sh]
            w = max(v[e] + (a - e) % 3 for e in range(3) if v[e] > NEG // 2)
            e2 = (a + z) % 3
            opts = []
            if z == 1:
                nv = list(v)
                if w > nv[e2]:
                    nv[e2] = w
                opts.append((k, nv))
                if k < K:
                    opts.append((k + 1, list(v)))
            else:
                nv = [NEG, NEG, NEG]
                nv[e2] = w
                opts.append((k, nv))
            for kk, nv in opts:
                m = max(nv)
                shp = tuple((max(x - m, -2) if x > NEG // 2 else NEG) for x in nv)
                key = (kk, shp)
                if key not in nxt or nxt[key] > m:
                    nxt[key] = m
        # dominance pruning across k: keep as is (small)
        cur = nxt
    return min(cur.values())

def build(home, R):
    """P for R super-periods; returns (cells of ones, Lsp, n')"""
    n = sum(home); k = len(home)
    t = 1
    while (t * n) % 3:
        t += 1
    Lp = n + 3 * k
    ones = []
    pos = 0
    for _ in range(t):
        for r in home:
            ones += list(range(pos, pos + r))
            pos += r + 3
    return ones, t * Lp, t * n

def layouts(home, Smask, placement, R, tail_n):
    ones, Lsp, n1 = build(home, R)
    lay = []
    for rep in range(R):
        off = rep * Lsp
        for i, c in enumerate(ones):
            if Smask >> i & 1:
                lay.append((off + c, 1))
        for p in placement:
            lay.append((off + p, 2))
    lay.sort()
    t = (lay[-1][0] + lay[-1][1]) if lay else 0
    t += (-t) % 3 + 0
    tail = []
    for i in range(tail_n * R):
        tail.append((t + 2 * i, 2))
    return tuple(lay), tuple(tail)

def placements(free, Lsp, jmax):
    """all sets of <= jmax disjoint pairs (p,p+1) with both cells free, inside one super-period (no wrap)"""
    pairs = [p for p in range(Lsp - 1) if p in free and p + 1 in free]
    out = [()]
    def rec(start, cur):
        if len(cur) == jmax:
            return
        for p in pairs:
            if p >= start:
                out.append(cur + (p,))
                rec(p + 2, cur + (p,))
    rec(0, ())
    return out

def value(home, Smask, pl, yp, R, cache={}):
    """adversary's best W against this (S, placement) at R repetitions: max over periodic F"""
    j = len(pl)
    tail_n = yp - j
    lay, tail = layouts(home, Smask, pl, R, tail_n)
    best = None
    for fm in range(1 << j):
        for tf in ((0, 1) if tail_n else (0,)):
            rem = set()
            Lsp = build(home, 1)[1]
            for rep in range(R):
                for i, p in enumerate(pl):
                    if fm >> i & 1:
                        rem.add(rep * Lsp + p)
            tl = tail
            K = len(rem)
            if tf:
                tl = tuple(b for i, b in enumerate(tail) if i % 2 == 0)
                K += len(tail) - len(tl)
            l = tuple(b for b in lay if not (b[1] == 2 and b[0] in rem)) + tl
            v = interdict(l, K)
            if best is None or v > best[0]:
                best = (v, fm, tf)
    return best

def solve(home, R1, R2, sizes=None, verbose=True):
    ones, Lsp, n1 = build(home, 1)
    dM = (R2 - R1) * n1
    results = []
    for Smask in range(1 << n1):
        kept = bin(Smask).count('1')
        if (n1 - kept) % 2:
            continue
        yp = (n1 - kept) // 2
        if sizes and kept not in sizes:
            continue
        occ = set(c for i, c in enumerate(ones) if Smask >> i & 1)
        free = set(range(Lsp)) - occ
        best_pl = None
        for pl in placements(free, Lsp, yp):
            v1 = value(home, Smask, pl, yp, R1)
            v2 = value(home, Smask, pl, yp, R2)
            slope = Fraction(v2[0] - v1[0], dM)
            if best_pl is None or slope < best_pl[0]:
                best_pl = (slope, pl, v1, v2)
        results.append((best_pl[0], Smask, kept, yp, best_pl[1], best_pl[2], best_pl[3]))
    results.sort(reverse=True)
    return results, ones, Lsp, n1

if __name__ == '__main__':
    home = [int(x) for x in sys.argv[1].split(',')]
    R1, R2 = int(sys.argv[2]), int(sys.argv[3])
    sizes = [int(x) for x in sys.argv[4].split(',')] if len(sys.argv) > 4 else None
    res, ones, Lsp, n1 = solve(home, R1, R2, sizes)
    print(f"home={home} super-period: {n1} ones on {Lsp} cells; span/M={Lsp / n1:.3f}")
    for slope, Smask, kept, yp, pl, v1, v2 in res[:12]:
        s = ['.'] * Lsp
        for i, c in enumerate(ones):
            if Smask >> i & 1: s[c] = '1'
        for p in pl:
            s[p] = s[p + 1] = '2'
        print(f"  slope dW/dM={float(slope):.4f} ({slope})  kept={kept} y={yp}  best manager placement: {''.join(s)} tail={yp - len(pl)}  adv F={v2[1]:b} tailfree={v2[2]}  W(R1)={v1[0]} W(R2)={v2[0]}")
    print(f"  ==> periodic game value (max over S of min over placements): slope={float(res[0][0]):.4f}  vs 2/3={2 / 3:.4f}")
