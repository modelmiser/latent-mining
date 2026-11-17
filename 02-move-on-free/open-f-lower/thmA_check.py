#!/usr/bin/env python3
"""Test of Theorem A (proofs.txt): the PROOF VERSION of P6R, played through the referee against
managers that put the first M 1s on consecutive cells.  Checks, for every M in the range,
   hypothesis (no two adjacent free cells between the lowest and highest remaining 1)  and
   extent >= ceil(7M/4 - 7).
A violation would falsify the proof (or the code).  Also prints extent - ceil(5M/3)."""
import sys, math
from core import *
from managers import *
from adversaries import fill
from wb import WB

def adv_A(game):
    M = game.M
    fill(game, 1)
    c = [0, 0, 0]
    for a, z in game.lay:
        c[a % 3] += 1
    d = min(range(3), key=lambda k: c[k])
    for a, z in list(game.lay):
        if a % 3 == d:
            game.free(a)
    ones = [a for a, z in game.lay]
    lo, hi = ones[0], ones[-1]
    o = occ_set(game.lay)
    hyp = not any(x not in o and x + 1 not in o for x in range(lo, hi))
    fill(game, 2)
    hyp = hyp and not any(z == 2 and lo < a < hi for a, z in game.lay)
    lower = [a for a, z in game.lay if z == 2 and a < lo]
    upper = [a for a, z in game.lay if z == 2 and a > hi]
    # counted runs and clean interior 2s
    def clean_list(chain, first_left):
        out = []
        for i, a in enumerate(chain):
            left = first_left if i == 0 else a - (chain[i - 1] + 2)
            if left is None or i == len(chain) - 1:
                out.append(False); continue
            right = chain[i + 1] - (a + 2)
            out.append(left % 3 == 0 and right % 3 == 0)
        return out
    F = []
    for chain, fl in ((lower, lower[0] if lower else None), (upper, None)):
        cl = clean_list(chain, fl)
        if chain is lower and chain:
            pass
        i = 0
        while i < len(chain):
            if cl[i]:
                j = i
                while j < len(chain) and cl[j]:
                    if (j - i) % 2 == 0:
                        F.append(chain[j])
                    j += 1
                i = j
            else:
                i += 1
    for a in F:
        mv = game.free(a)
        for old, new in mv:          # L2: free the moved 1 at once
            game.free(new)
    x = {}
    for a, z in game.lay:
        if z == 1:
            x[a % 3] = x.get(a % 3, 0) + 1
    r = max(x, key=lambda k: x[k]) if x else 0
    for a, z in list(game.lay):
        if z == 1 and a % 3 != r:
            game.free(a)
    fill(game, 3)
    return hyp, len(F)

if __name__ == '__main__':
    lo, hi, step = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
    mgrs = [FF(), FFM(), PG('phi'), PG('phi+views'), WB(None)]
    bad = 0
    print("M   bound=ceil(7M/4-7)  ceil(5M/3) | per manager: extent (extent-bound) [freed 2s] hyp")
    for M in range(lo, hi + 1, step):
        bound = math.ceil(7 * M / 4 - 7)
        row = []
        for m in mgrs:
            g = Game(M, m)
            hyp, nf = adv_A(g)
            ok = (not hyp) or g.extent >= bound
            if not ok: bad += 1
            row.append(f"{g.extent}({g.extent - bound:+d})[{nf}]{'h' if hyp else '-'}{'' if ok else ' VIOLATION'}")
        print(f"{M:3d}  {bound:4d}  {math.ceil(5 * M / 3):4d} | " + '  '.join(row), flush=True)
    print("managers:", [m.name for m in mgrs])
    print("violations:", bad)
