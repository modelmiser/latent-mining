#!/usr/bin/env python3
"""Round 2: look-ahead on both sides.

endgame(lay)  : value W of the adversary's end phase from layout `lay` (1s and 2s live):
                greedy frees of 2s (each answered by the manager's phi-best DELETION of a 1 --
                a moved 1 can be freed at once at no cost, so a move is at best a deletion),
                then the DP thinning.  Final extent >= M - 2 + W.
WB manager    : knows the P6R script.  1s go on a 'home' pattern (runs separated by reserved
                3-gaps); when the batch of 2s starts it plans all y placements by greedy + local
                search against endgame(); replies to a freed 2 by the phi-best move.
adv_P6RS      : P6R whose kept set S is chosen from a menu by its own look-ahead (it predicts the
                planned placement and takes the S with the largest predicted W).
"""
import sys, math, time
from core import *
from managers import *
from adversaries import *

def phi_fast(lay):
    b0, b1, b2 = 0, -1, -1
    for a, z in lay:
        c0 = b0 + (a % 3) if b0 >= 0 else -1
        c1 = b1 + ((a - 1) % 3) if b1 >= 0 else -1
        c2 = b2 + ((a - 2) % 3) if b2 >= 0 else -1
        w = max(c0, c1, c2)
        e = (a + z) % 3
        if z == 1:
            n = [b0, b1, b2]
            if w > n[e]:
                n[e] = w
            b0, b1, b2 = n
        else:
            n = [-1, -1, -1]
            n[e] = w
            b0, b1, b2 = n
    return max(b0, b1, b2)

def best_deletion(lay):
    base = phi_fast(lay)
    best = (base, None)
    for a, z in lay:
        if z == 1:
            v = phi_fast(remove(lay, a))
            if v < best[0]:
                best = (v, a)
    return best

def endgame(lay, thresh=2, trace=None):
    while True:
        base = phi_fast(lay)
        best = None
        for a, z in lay:
            if z == 2:
                g = phi_fast(remove(lay, a)) - base
                if best is None or g > best[0]:
                    best = (g, a)
        if best is None or best[0] < thresh:
            return base
        lay = remove(lay, best[1])
        v, d = best_deletion(lay)
        if d is not None:
            lay = remove(lay, d)
        if trace is not None:
            trace.append((best[1], d))

def plan_twos(lay, y, passes=2, thresh=2):
    """manager's plan for y 2s: greedy by endgame value, then local search"""
    cur = lay
    spots = []
    for i in range(y):
        best = None
        for st in cand_starts(cur, 2, extra=4):
            if not fits(cur, st, 2):
                continue
            l2 = place(cur, st, 2)
            v = (endgame(l2, thresh), phi_fast(l2), top(l2), st)
            if best is None or v < best[0]:
                best = (v, st)
        spots.append(best[1])
        cur = place(cur, best[1], 2)
    val = endgame(cur, thresh)
    for _ in range(passes):
        improved = False
        for i in range(len(spots)):
            l0 = remove(cur, spots[i])
            for st in cand_starts(l0, 2, extra=4):
                if st == spots[i] or not fits(l0, st, 2):
                    continue
                l2 = place(l0, st, 2)
                v = endgame(l2, thresh)
                if v < val:
                    val, cur, improved = v, l2, True
                    spots[i] = st
                    l0 = remove(cur, st)
        if not improved:
            break
    return spots, val

class WB(PG):
    def __init__(self, home, thresh=2):
        PG.__init__(self, 'phi+views', home=home, name=f'WB[home={home}]')
        self.plan = []
        self.thresh = thresh
    def place(self, lay, s, M):
        if s == 2:
            self.plan = [p for p in self.plan if fits(lay, p, 2)]
            if not self.plan:
                y = (M - live(lay)) // 2
                self.plan, _ = plan_twos(lay, y, thresh=self.thresh)
            return self.plan.pop(0)
        return PG.place(self, lay, s, M)

# ----------------------------------------------------------------------------- adversary side
def gap_info(lay):
    """free runs of length >= 3 inside the layout: list of (start, len)"""
    return [(a, l) for a, l in runs(lay) if l >= 3]

def keep_sets(lay):
    """menu of candidate kept sets S (as dict name -> set of starts of 1s to KEEP)"""
    ones = [a for a, z in lay if z == 1]
    menu = {}
    def with_blockers(keep):
        keep = set(keep)
        changed = True
        while changed:
            changed = False
            l = tuple((a, 1) for a in sorted(keep))
            for a in ones:
                if a in keep:
                    continue
                # a is free: would it sit in a free run of length >= 2 that is NOT part of an original gap >= 3?
                for x, ln in runs(l, top(lay)):
                    if x <= a < x + ln and ln == 2:
                        keep.add(a); changed = True
                        break
                    if x <= a < x + ln and ln >= 4:
                        # original gap plus dropped neighbours: block the dropped neighbour
                        keep.add(a); changed = True
                        break
                if changed:
                    break
        return keep
    for d in range(3):
        k = set(a for a in ones if a % 3 != d)
        menu[f'G{d}'] = k
        menu[f'G{d}b'] = with_blockers(k)
    gaps = gap_info(lay)
    if gaps:
        mids = [((a + 1) % 3, a, a + l) for a, l in gaps]
        for o in range(3):
            k = set()
            for a in ones:
                left = [m for m in mids if m[2] <= a]
                right = [m for m in mids if m[1] > a]
                mL = left[-1][0] if left else None
                mR = right[0][0] if right else None
                if mL is None: mL = (mR - 1) % 3
                if mR is None: mR = (mL + 1) % 3
                if o == 0: cls = {mL, (mL + 1) % 3}
                elif o == 1: cls = {(mR - 1) % 3, mR}
                else: cls = {mL, mR} if mL != mR else {mL, (mL + 1) % 3}
                if a % 3 in cls:
                    k.add(a)
            menu[f'R{o}'] = k
            menu[f'R{o}b'] = with_blockers(k)
    return menu

def adv_P6RS(game, menu_filter=None, thresh=2, verbose=False):
    fill(game, 1)
    lay = game.lay
    best = None
    for name, keep in keep_sets(lay).items():
        if menu_filter and not any(name.startswith(f) for f in menu_filter):
            continue
        l = tuple(b for b in lay if b[1] != 1 or b[0] in keep)
        y = (game.M - live(l)) // 2
        if y == 0:
            v = phi_fast(l)
        else:
            _, v = plan_twos(l, y, passes=1, thresh=thresh)
        if verbose:
            print('   menu', name, 'kept', len(keep), 'y', y, 'predicted W', v)
        if best is None or v > best[0]:
            best = (v, name, keep)
    v, name, keep = best
    game.choice = name
    for a, z in list(game.lay):
        if z == 1 and a not in keep:
            game.free(a)
    fill(game, 2)
    free_twos_greedy(game, thresh)
    finalize(game)

if __name__ == '__main__':
    Ms = [int(x) for x in sys.argv[1].split(',')]
    homes = [None, [6], [5], [5, 8], [7, 7, 4], [4, 7], [8], [9]]
    advs = {'P6R': adv_P6R, 'P6R-block': lambda g: adv_P6R(g, blockers=True), 'P6RS': adv_P6RS}
    which = sys.argv[2].split(',') if len(sys.argv) > 2 else list(advs)
    for M in Ms:
        base = math.ceil(5 * M / 3)
        print(f"\nM={M} ceil(5M/3)={base}: extent(extent-ceil(5M/3))[moves]{{S chosen}}", flush=True)
        for h in homes:
            row = []
            for an in which:
                m = WB(h) if h else WB(None)
                g = Game(M, m)
                g.choice = ''
                t = time.time()
                advs[an](g)
                row.append(f"{an}:{g.extent}({g.extent - base:+d})[{g.nmoves}]{{{g.choice}}} {time.time() - t:.0f}s")
            print(f"  WB home={str(h):12s} " + '   '.join(row), flush=True)
