"""Game F engine: layouts, rule enforcement, waste potential.  Written from the game definition.

Layout = sorted tuple of (start, size).  Blocks are identified by their start cell.
Game F: after free(b), |b| = s, the manager may do moves whose moved sizes total <= s-1.
"""
from bisect import insort

def live(lay):
    return sum(s for _, s in lay)

def top(lay):
    return lay[-1][0] + lay[-1][1] if lay else 0

def occ_set(lay):
    o = set()
    for st, s in lay:
        for k in range(s):
            o.add(st + k)
    return o

def fits(lay, st, s):
    if st < 0:
        return False
    for a, z in lay:
        if a < st + s and st < a + z:
            return False
    return True

def place(lay, st, s):
    assert fits(lay, st, s), (lay, st, s)
    l = list(lay)
    insort(l, (st, s))
    return tuple(l)

def remove(lay, st):
    l = [b for b in lay if b[0] != st]
    assert len(l) == len(lay) - 1, (lay, st)
    return tuple(l)

def size_at(lay, st):
    for a, z in lay:
        if a == st:
            return z
    raise KeyError(st)

def runs(lay, upto=None):
    """free runs (start, length) below `upto` (default: top of layout, i.e. internal runs only)"""
    out = []
    pos = 0
    for a, z in lay:
        if a > pos:
            out.append((pos, a - pos))
        pos = a + z
    if upto is not None and upto > pos:
        out.append((pos, upto - pos))
    return out

def waste(lay, H=None):
    """sum over free runs of (length mod 3); runs below H if given, else internal runs only"""
    return sum(l % 3 for _, l in runs(lay, H))

def show(lay, width=None):
    w = width or top(lay)
    c = ['.'] * max(w, top(lay))
    for a, z in lay:
        for k in range(z):
            c[a + k] = str(z)
    return ''.join(c)

def thin_best(lay):
    """Adversary's free finalisation: 1s may be freed at no cost (a freed 1 grants no move).
    Returns (W, keep) maximising W = sum of (internal free run mod 3) over subsets of the 1s
    (2s and 3s stay).  DP over blocks, state = end of last kept block mod 3."""
    best = {0: (0, None)}          # end mod 3 -> (W, backpointer)
    trail = []
    for i, (a, z) in enumerate(lay):
        new = {}
        if z == 1:
            for e, (w, _) in best.items():
                new[e] = (w, (e, False))
        for e, (w, _) in best.items():
            w2 = w + (a - e) % 3
            e2 = (a + z) % 3
            if e2 not in new or new[e2][0] < w2:
                new[e2] = (w2, (e, True))
        trail.append(new)
        best = new
    e = max(best, key=lambda k: best[k][0])
    W = best[e][0]
    keep = []
    for i in range(len(lay) - 1, -1, -1):
        w, (pe, kept) = trail[i][e]
        if kept:
            keep.append(lay[i][0])
        e = pe
    return W, set(keep)

def phi(lay):
    return thin_best(lay)[0]

def view_waste(lay, r):
    """waste after freeing every 1 that is not on class r"""
    l = tuple(b for b in lay if b[1] != 1 or b[0] % 3 == r)
    return waste(l)

class Illegal(Exception):
    pass

class Game:
    """Referee for game F (mode 'F'), N ('N').  The manager object supplies
       place(lay, s, M) -> start   and   respond(lay, s, M) -> list of (old_start, new_start)."""
    def __init__(self, M, manager, mode='F', H=None, log=None):
        self.M, self.mgr, self.mode, self.H = M, manager, mode, H
        self.lay = ()
        self.extent = 0
        self.log = log
        self.nmoves = 0

    def alloc(self, s):
        if live(self.lay) + s > self.M:
            raise Illegal('budget')
        st = self.mgr.place(self.lay, s, self.M)
        self.lay = place(self.lay, st, s)
        self.extent = max(self.extent, top(self.lay))
        if self.log is not None:
            self.log.append(f"alloc {s} -> @{st}   {show(self.lay)}")
        return st

    def free(self, st):
        s = size_at(self.lay, st)
        self.lay = remove(self.lay, st)
        mv = []
        if self.mode == 'F' and s > 1:
            mv = self.mgr.respond(self.lay, s, self.M) or []
            budget = s - 1
            for old, new in mv:
                z = size_at(self.lay, old)
                if z > budget:
                    raise Illegal('move allowance')
                budget -= z
                l = remove(self.lay, old)
                if not fits(l, new, z):
                    raise Illegal('move target')
                self.lay = place(l, new, z)
                self.nmoves += 1
            self.extent = max(self.extent, top(self.lay))
        if self.log is not None:
            self.log.append(f"free {s}@{st} moves={mv}   {show(self.lay)}")
        return mv
