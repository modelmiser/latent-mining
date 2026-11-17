"""Managers for game F.  All are stateless functions of the layout (so they can be cloned/searched)."""
from core import *

def cand_starts(lay, s, extra=3):
    """candidate starts for a block of size s: both ends (and small offsets) of every free run, plus tail"""
    out = set()
    for a, l in runs(lay):
        if l < s:
            continue
        for k in range(0, min(3, l - s + 1)):
            out.add(a + k)
            out.add(a + l - s - k)
    t = top(lay)
    for k in range(extra + 1):
        out.add(t + k)
    return sorted(out)

class FF:
    name = 'FF(no moves)'
    def place(self, lay, s, M):
        pos = 0
        for a, z in lay:
            if a - pos >= s:
                return pos
            pos = a + z
        return pos
    def respond(self, lay, s, M):
        return []

class FFM(FF):
    """first fit; after a freed 2 move the rightmost 1 into the leftmost free cell (solver-b S1)"""
    name = 'FF+move-rightmost-1-to-leftmost-hole'
    def respond(self, lay, s, M):
        mv = []
        budget = s - 1
        l = lay
        while budget > 0:
            ones = [a for a, z in l if z == 1]
            if not ones:
                break
            src = ones[-1]
            rs = runs(l)
            if not rs or rs[0][0] > src:
                break
            dst = rs[0][0]
            mv.append((src, dst))
            l = place(remove(l, src), dst, 1)
            budget -= 1
        return mv

class PG:
    """Potential-greedy manager.  score(lay) -> tuple, lower is better.
       1s can be confined to a 'home' pattern (runs of ones separated by reserved 3-gaps)."""
    def __init__(self, score='phi', home=None, name=None, two_score=None):
        self.scorename = score
        self.home = home            # None or list of run lengths, cycled, each followed by a 3-gap
        self.two_score = two_score
        self.name = name or f'PG[{score}{"" if not home else ",home=" + str(home)}{"" if not two_score else ",2s=" + two_score}]'
        self._homecache = {}

    def _score(self, lay, which=None):
        w = which or self.scorename
        if w == 'phi':
            return (phi(lay), top(lay))
        if w == 'views':
            v = [view_waste(lay, r) for r in range(3)]
            return (sum(v), max(v), phi(lay), top(lay))
        if w == 'viewsmax':
            v = [view_waste(lay, r) for r in range(3)]
            return (max(v), sum(v), phi(lay), top(lay))
        if w == 'phi+views':
            v = [view_waste(lay, r) for r in range(3)]
            return (phi(lay), sum(v), top(lay))
        raise ValueError(w)

    def _is_home(self, c):
        if not self.home:
            return True
        period = sum(self.home) + 3 * len(self.home)
        x = c % period
        for r in self.home:
            if x < r:
                return True
            x -= r
            if x < 3:
                return False
            x -= 3
        return True

    def place(self, lay, s, M):
        if s == 1 and self.home:
            o = occ_set(lay)
            # hide in a hole of length 1 or 2 first (no new waste), else first free home cell
            c = 0
            while True:
                if c not in o and self._is_home(c):
                    return c
                c += 1
        which = self.two_score if (s == 2 and self.two_score) else None
        best = None
        for st in cand_starts(lay, s):
            if not fits(lay, st, s):
                continue
            sc = self._score(place(lay, st, s), which) + (st,)
            if best is None or sc < best[0]:
                best = (sc, st)
        return best[1]

    def _best_move(self, lay, size):
        """best single move of a block of the given size (or None), with its score"""
        base = (self._score(lay) + (0,), None)
        best = base
        for a, z in lay:
            if z != size:
                continue
            l = remove(lay, a)
            dests = set(x for x, ln in runs(l) if ln >= z)
            dests |= set(x + ln - z for x, ln in runs(l) if ln >= z)
            dests.add(top(l))
            for d in dests:
                if d == a or not fits(l, d, z):
                    continue
                sc = self._score(place(l, d, z)) + (1,)
                if sc < best[0]:
                    best = (sc, (a, d))
        return best

    def respond(self, lay, s, M):
        if s == 2:
            sc, mv = self._best_move(lay, 1)
            return [mv] if mv else []
        # s == 3: one move of a 2, or up to two moves of 1s
        sc2, mv2 = self._best_move(lay, 2)
        sc1, mv1 = self._best_move(lay, 1)
        seq1 = []
        scb = sc1
        if mv1:
            seq1 = [mv1]
            l = place(remove(lay, mv1[0]), mv1[1], 1)
            scb2, mvb = self._best_move(l, 1)
            if mvb and scb2 < scb:
                seq1.append(mvb)
                scb = scb2
        if mv2 and sc2 <= scb:
            return [mv2]
        return seq1
