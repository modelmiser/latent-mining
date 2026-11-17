#!/usr/bin/env python3
"""Referee test of solver B's P6 (h^N(M) >= 7M/4 - 17/4).
Part A (exhaustive): for small M, the least H at which SOME manager survives the P6 adversary,
  where every choice the proof leaves open (which fewest class, which r satisfying the inequality)
  is resolved in the MANAGER's favour.  P6 predicts VA(M) >= ceil(7M/4 - 17/4).
Part B (simulation): the P6 adversary as an adaptive algorithm against many managers, big M.
Part C: negative controls.
"""
import sys, math, random, itertools
from fractions import Fraction

def bound(M):  # ceil(7M/4 - 17/4)
    return math.ceil(Fraction(7 * M - 17, 4))

# ---------------------------------------------------------------- Part A
def max_triples(occ, H):
    n = run = 0
    for i in range(H):
        if occ[i]:
            n += run // 3; run = 0
        else:
            run += 1
    return n + run // 3

def pair_placements(occ, H, y, start=0):
    """all ways to put y disjoint 2-blocks on free cells (as lists of start cells, increasing)"""
    if y == 0:
        yield []
        return
    for p in range(start, H - 1):
        if not occ[p] and not occ[p + 1]:
            occ[p] = occ[p + 1] = 1
            for rest in pair_placements(occ, H, y - 1, p + 2):
                yield [p] + rest
            occ[p] = occ[p + 1] = 0

def manager_survives(M, H):
    for ones in itertools.combinations(range(H), M):
        c = [0, 0, 0]
        for x in ones: c[x % 3] += 1
        mn = min(c)
        for drop in range(3):
            if c[drop] != mn: continue           # any fewest class (proof: "a class with the fewest")
            kept = [x for x in ones if x % 3 != drop]
            X = len(kept); y = (M - X) // 2
            occ = [0] * H
            for x in kept: occ[x] = 1
            for twos in pair_placements(occ, H, y):
                for r in range(3):
                    if r == drop: continue
                    xr = c[r]
                    yr = sum(1 for p in twos if p % 3 == r or (p + 1) % 3 == r)
                    if 2 * (2 * xr + yr) < 2 * X + y: continue   # proof: any r with 2x_r+y_r >= X+y/2
                    o2 = [0] * H
                    for x in kept:
                        if x % 3 == r: o2[x] = 1
                    for p in twos:
                        if p % 3 == r or (p + 1) % 3 == r: o2[p] = o2[p + 1] = 1
                    live = xr + 2 * yr
                    assert live == sum(o2) and live <= M
                    t = (M - live) // 3
                    if max_triples(o2, H) >= t:
                        return (ones, drop, twos, r, t)
    return None

def partA(Ms):
    print("Part A: least H at which some manager survives P6 (all open choices favour manager)")
    print(" M  VA(M)  ceil(7M/4-17/4)  ok   witness at VA")
    for M in Ms:
        H = M
        while True:
            w = manager_survives(M, H)
            if w: break
            H += 1
        print(f"{M:2d}  {H:3d}   {bound(M):3d}             {'OK' if H >= bound(M) else 'VIOLATION'}   {w}", flush=True)

# ---------------------------------------------------------------- Part B: game engine (game N)
class Heap:
    def __init__(self, M, cap):
        self.M, self.cap = M, cap
        self.cell = [-1] * cap      # block id or -1
        self.blocks = {}            # id -> (start, size)
        self.live = 0; self.nid = 0; self.extent = 0
    def fits(self, p, s):
        return 0 <= p and p + s <= self.cap and all(self.cell[p + j] < 0 for j in range(s))
    def alloc(self, s, manager):
        assert self.live + s <= self.M, "illegal request"
        p = manager(self, s)
        assert self.fits(p, s), "manager placed on non-free cells"
        for j in range(s): self.cell[p + j] = self.nid
        self.blocks[self.nid] = (p, s); self.nid += 1; self.live += s
        self.extent = max(self.extent, p + s)
    def free(self, b):
        p, s = self.blocks.pop(b)
        for j in range(s): self.cell[p + j] = -1
        self.live -= s

def p6_adversary(M, manager, cap, weak=None):
    """returns (max extent ever, proof's guaranteed 3(x_r+y_r+t)-2).  weak: None | 'worse_r' | 'no_twos'"""
    h = Heap(M, cap)
    for _ in range(M): h.alloc(1, manager)
    c = [0, 0, 0]
    for b, (p, s) in h.blocks.items(): c[p % 3] += 1
    drop = c.index(min(c))
    for b in [b for b, (p, s) in h.blocks.items() if p % 3 == drop]: h.free(b)
    X = h.live
    y = 0 if weak == 'no_twos' else (M - X) // 2
    for _ in range(y): h.alloc(2, manager)
    def covers(b, r):
        p, s = h.blocks[b]
        return any((p + j) % 3 == r for j in range(s))
    cand = [r for r in range(3) if r != drop]
    yr = {r: sum(1 for b, (p, s) in h.blocks.items() if s == 2 and covers(b, r)) for r in cand}
    score = {r: 2 * c[r] + yr[r] for r in cand}
    r = (min if weak == 'worse_r' else max)(cand, key=lambda q: score[q])
    if weak is None: assert 2 * score[r] >= 2 * X + y
    for b in [b for b in list(h.blocks) if not covers(b, r)]: h.free(b)
    assert h.live == c[r] + 2 * yr[r]
    t = (M - h.live) // 3
    for _ in range(t): h.alloc(3, manager)
    # the proof's counting claim, checked directly: distinct occupied cells of class r
    occ_r = sum(1 for i in range(cap) if i % 3 == r and h.cell[i] >= 0)
    assert occ_r == c[r] + yr[r] + t, "class-r count claim failed"
    return h.extent, 3 * occ_r - 2

# managers ---------------------------------------------------------------
def first_fit(h, s):
    for p in range(h.cap):
        if h.fits(p, s): return p
def mk_random(q, rng, window):
    def m(h, s):
        if rng.random() < q:
            lim = min(h.cap - s, int(window * h.M))
            ps = [p for p in range(lim + 1) if h.fits(p, s)]
            if ps: return rng.choice(ps)
        return first_fit(h, s)
    return m
def mk_spread(gap, off):
    """leave `gap` free cells after every block when possible (deliberately spread out)"""
    def m(h, s):
        for p in range(off, h.cap):
            if h.fits(p, s) and all(h.cell[q] < 0 for q in range(max(0, p - gap), p)) \
               and all(h.cell[q] < 0 for q in range(p + s, min(h.cap, p + s + gap))): return p
        return first_fit(h, s)
    return m
def mk_stride(k, rng=None):
    """1s only on cells = 0 mod k region first, 2s/3s aligned to their own size, far apart zones"""
    def m(h, s):
        zone = {1: 0, 2: 2 * h.M, 3: 4 * h.M}[s]
        for p in range(zone, h.cap, max(k, s)):
            if h.fits(p, s): return p
        return first_fit(h, s)
    return m
def best_fit(h, s):
    best = None; i = 0
    while i < h.cap:
        if h.cell[i] < 0:
            j = i
            while j < h.cap and h.cell[j] < 0: j += 1
            L = j - i
            if L >= s and j < h.cap and (best is None or L < best[0]): best = (L, i)
            if j >= h.cap and best is None: best = (10**9, i)
            i = j
        else: i += 1
    return best[1]
def mk_smart():
    """tries to defeat P6: 1s packed (balanced classes); 2s placed in holes so that they straddle the
    emptied class, alternating which kept class they touch; 3s first fit."""
    st = {'n2': 0}
    def m(h, s):
        if s != 2: return first_fit(h, s)
        # holes of the emptied class: free cell with both neighbours occupied by 1s -> cannot hold a 2.
        # so put 2s beyond the 1s, alternately at residues making them cover {drop,a} / {a+1,drop}
        want = None
        cnt = [0, 0, 0]
        for b, (p, sz) in h.blocks.items():
            if sz == 1: cnt[p % 3] += 1
        drop = cnt.index(min(cnt))
        want = drop if st['n2'] % 2 == 0 else (drop - 1) % 3   # start residue: {drop,drop+1} or {drop-1,drop}
        st['n2'] += 1
        for p in range(h.cap):
            if p % 3 == want and h.fits(p, 2): return p
    return m

def partB(Ms, seed=1):
    rng = random.Random(seed)
    print("\nPart B: P6 adversary vs managers, game N, heap cap 8M.  min over managers of forced extent")
    print("  M  bound  #mgrs  min_extent  min_guarantee  argmin   violations")
    for M in Ms:
        cap = 8 * M
        mgrs = [('first_fit', first_fit), ('best_fit', best_fit)]
        mgrs += [(f'spread{g}_{o}', mk_spread(g, o)) for g in (1, 2, 3, 4) for o in (0, 1, 2)]
        mgrs += [(f'stride{k}', mk_stride(k)) for k in (1, 2, 3, 4, 5, 6)]
        mgrs += [('smart', mk_smart())]
        for i in range(300):
            q = rng.choice([0.05, 0.2, 0.5, 1.0]); w = rng.choice([1.0, 1.5, 2.0, 3.0, 6.0])
            mgrs.append((f'rand{i}_q{q}_w{w}', mk_random(q, random.Random(rng.random()), w)))
        best = None; bestg = None; viol = 0
        for name, m in mgrs:
            e, g = p6_adversary(M, m, cap)
            assert e >= g
            if e < bound(M) or g < bound(M): viol += 1; print("   VIOLATION", name, e, g)
            if best is None or e < best[0]: best = (e, name)
            if bestg is None or g < bestg: bestg = g
        print(f"{M:4d} {bound(M):5d} {len(mgrs):6d} {best[0]:10d} {bestg:13d}   {best[1]:10s} {viol}", flush=True)

def partC(Ms):
    print("\nPart C: negative controls (each line should show the control FALLING SHORT of the P6 bound)")
    print("  M  bound | P6 vs smart | worse_r vs smart | no_twos vs first_fit | no_twos vs smart")
    for M in Ms:
        cap = 8 * M
        a = p6_adversary(M, mk_smart(), cap)[0]
        b = p6_adversary(M, mk_smart(), cap, weak='worse_r')[0]
        c = p6_adversary(M, first_fit, cap, weak='no_twos')[0]
        d = p6_adversary(M, mk_smart(), cap, weak='no_twos')[0]
        print(f"{M:4d} {bound(M):5d} | {a:5d} (+{a-bound(M)}) | {b:5d} {'short' if b < bound(M) else 'NOT short'} | "
              f"{c:5d} {'short' if c < bound(M) else 'NOT short'} | {d:5d} {'short' if d < bound(M) else 'NOT short'}")

if __name__ == '__main__':
    what = sys.argv[1]
    if what == 'A': partA(range(int(sys.argv[2]), int(sys.argv[3]) + 1))
    if what == 'B': partB([int(x) for x in sys.argv[2:]])
    if what == 'C': partC([int(x) for x in sys.argv[2:]])
