"""Adaptive adversaries for game F.  Each is a function adv(game) that issues requests through the
referee (core.Game) and looks only at the layout.  Final extent is game.extent."""
from core import *

def fill(game, s):
    while live(game.lay) + s <= game.M:
        game.alloc(s)

def finalize(game):
    """free 1s (free of charge in F) to the waste-maximising subset, then allocate 3s"""
    W, keep = thin_best(game.lay)
    for a, z in list(game.lay):
        if z == 1 and a not in keep:
            game.free(a)
    fill(game, 3)

def adv_P1(game):
    """solver-b P1 (control): M ones, keep the fullest residue class, 3s."""
    fill(game, 1)
    c = [0, 0, 0]
    for a, z in game.lay:
        c[a % 3] += 1
    r = max(range(3), key=lambda k: c[k])
    for a, z in list(game.lay):
        if a % 3 != r:
            game.free(a)
    fill(game, 3)

def adv_O(game):
    """control: only 1s are ever freed, finalisation by the waste DP (legal in game O)"""
    fill(game, 1)
    finalize(game)

def two_class_keep(game, blockers=False):
    c = [0, 0, 0]
    for a, z in game.lay:
        if z == 1:
            c[a % 3] += 1
    drop = min(range(3), key=lambda k: c[k])
    cand = [a for a, z in game.lay if z == 1 and a % 3 == drop]
    for a in cand:
        if blockers:
            # keep the 1 if freeing it would open a free run of length >= 2 (a slot for a 2)
            l = remove(game.lay, a)
            ln = [x for x in runs(l, top(l) + 1) if x[0] <= a < x[0] + x[1]][0][1]
            if ln >= 2:
                continue
        game.free(a)
    return drop

def free_twos_greedy(game, thresh=2):
    """free a 2 whenever doing so raises the finalisation value phi by >= thresh before the manager's
    reply.  A reply (one moved 1) lowers phi by at most 2, so with thresh=2 this never loses."""
    n = 0
    while True:
        base = phi(game.lay)
        best = None
        for a, z in game.lay:
            if z != 2:
                continue
            g = phi(remove(game.lay, a)) - base
            if best is None or g > best[0]:
                best = (g, a)
        if best is None or best[0] < thresh:
            return n
        game.free(best[1])
        n += 1
        if n > 10 * game.M:
            return n

def adv_P6R(game, blockers=False, thresh=2, rounds=1):
    """P6R: M ones; keep TWO residue classes (redundant pins); 2s; free 2s greedily while it pays
    even against the manager's best reply; thin by DP; 3s."""
    fill(game, 1)
    for _ in range(rounds):
        two_class_keep(game, blockers)
        fill(game, 2)
        if blockers:
            # release the blockers now that the 2s are placed (free of charge)
            pass
        free_twos_greedy(game, thresh)
        fill(game, 1)
    finalize(game)

def adv_P6N(game):
    """solver-b P6 as written for N, played in F (commits to one class r, frees junk 2s)."""
    fill(game, 1)
    drop = two_class_keep(game)
    fill(game, 2)
    best = None
    for r in range(3):
        if r == drop:
            continue
        x = sum(1 for a, z in game.lay if z == 1 and a % 3 == r)
        y = sum(1 for a, z in game.lay if z == 2 and (a % 3 == r or (a + 1) % 3 == r))
        if best is None or 2 * x + y > best[0]:
            best = (2 * x + y, r)
    r = best[1]
    for a, z in list(game.lay):
        if z == 1 and a % 3 != r:
            game.free(a)
    while True:
        junk = [a for a, z in game.lay if z == 2 and a % 3 != r and (a + 1) % 3 != r]
        if not junk:
            break
        game.free(junk[0])
    for a, z in list(game.lay):
        if z == 1 and a % 3 != r:
            game.free(a)
    fill(game, 3)

def adv_alt2(game):
    """solver-b S1 attack: thin 1s to two classes, 2s, free every second 2 (by position), thin, 3s"""
    fill(game, 1)
    two_class_keep(game)
    fill(game, 2)
    twos = [a for a, z in game.lay if z == 2]
    for a in twos[1::2]:
        if any(b == a and z == 2 for b, z in game.lay):
            game.free(a)
    finalize(game)
