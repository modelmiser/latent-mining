#!/usr/bin/env python3
"""Adversary rules for the free-promise game, and the exhaustive runs."""
import sys, time
import harness
from harness import Game, value, PromiseViolation


# ------------------------------------------------------------------ STAIRCASE
class Staircase:
    """Jobs J_1..J_{P-1} with |J_j| = j+1, and one unit job u.  T = P(P+1)/2.
       Len[j] = sum_{i=j+1}^{P-1} (i+1) + 1   (window length of W_j), Len[0] = T.
       s_0 = 0: release J_1 = (0, 2, T).
       s_j = min(a_j + 1, s_{j-1} + j + 1), a_j = tick of the scheduler's first unit of J_j.
       At s_j (1 <= j <= P-2) release J_{j+1} = (s_j, j+2, s_j + Len[j]).
       At s_{P-1} release u = (s_{P-1}, 1, s_{P-1} + 1).
       Variants (negative controls):
         nostall : s_j = a_j + 1 only (wait for the scheduler for ever)
         short   : lengths |J_j| = j (one shorter), windows recomputed
         nounit  : never release u
         lastlen : final job J_{P-1} has length P-1 instead of P
    """

    def __init__(self, P, variant=None, lens=None):
        self.P = P
        self.variant = variant
        if lens is None:
            lens = [j + 1 for j in range(1, P)]          # J_1..J_{P-1}
            if variant == 'short':
                lens = [max(1, j) for j in range(1, P)]
            if variant == 'lastlen':
                lens[-1] = P - 1
        self.lens = [None] + lens                         # 1-based
        self.n = len(lens)
        self.Len = [sum(self.lens[j + 1:]) + 1 for j in range(0, self.n + 1)]
        self.T = self.Len[0]

    def start(self):
        return (0, 0)          # (number of jobs J released so far = current level, s of last release)

    def done(self, t, ast):
        return ast[0] > self.n

    def release(self, t, ast, view):
        lvl, s_prev = ast
        if lvl == 0:
            return (1, 0), [(self.lens[1], self.T)]
        if lvl > self.n:
            return ast, []
        # current fresh-or-started job is J_lvl = jid lvl-1; has the scheduler started it?
        jid = lvl - 1
        r, p, d, rem = view['jobs'][jid][1:]
        started_now = view['ev'][0] == 'run' and view['ev'][1] == jid and view['ev'][2]
        trigger = started_now
        if self.variant != 'nostall' and rem == p and t == s_prev + lvl + 1:
            trigger = True
        if rem < p and not started_now:
            raise RuntimeError("missed trigger")
        if not trigger:
            return ast, []
        if lvl < self.n:
            return (lvl + 1, t), [(self.lens[lvl + 1], t + self.Len[lvl])]
        if self.variant == 'nounit':
            return (lvl + 1, t), []
        return (lvl + 1, t), [(1, t + 1)]


# ------------------------------------------------------------------ SIMPLE (T = P^2 - 1)
class Simple:
    """N = P-1 jobs (0, P, T) at tick 0, T = N (P+1).  Whenever the scheduler runs the first unit
       of one of them at tick a, release the unit job (a+1, 1, a+2).  The first time the number of
       empty ticks reaches P-1 while not all traps are sprung, release T - (work released) unit
       jobs (t, 1, T) and stop.   Variant: N = P-2 (negative control)."""

    def __init__(self, P, N=None):
        self.P = P
        self.N = P - 1 if N is None else N
        self.T = max(1, self.N * (P + 1))

    def start(self):
        return (0, 0, False)     # traps sprung, wastes (capped), stopped

    def done(self, t, ast):
        return ast[2] or ast[0] == self.N

    def release(self, t, ast, view):
        k, w, stop = ast
        if t == 0:
            return ast, [(self.P, self.T)] * self.N
        if stop:
            return ast, []
        ev = view['ev']
        rel = []
        if ev[0] == 'run':
            if ev[2] and view['jobs'][ev[1]][2] == self.P and view['jobs'][ev[1]][1] == 0:
                k += 1
                rel.append((1, t + 1))
        else:
            w = min(w + 1, self.P - 1)
        if w >= self.P - 1 and k < self.N:
            released = sum(j[2] for j in view['jobs']) + len(rel)
            rel += [(1, self.T)] * (self.T - released)
            stop = True
        return (k, w, stop), rel


class Null:
    """control: one job, nothing else.  value must be 0."""
    def __init__(self, P):
        self.P = P; self.T = P + 1
    def start(self): return 0
    def done(self, t, ast): return ast == 1
    def release(self, t, ast, view):
        return (1, [(self.P, self.T)]) if t == 0 else (1, [])


class Liar:
    """control: breaks the promise (two tight unit jobs at tick 0). harness must raise."""
    def __init__(self, P):
        self.P = P; self.T = 3
    def start(self): return 0
    def done(self, t, ast): return ast == 1
    def release(self, t, ast, view):
        return (1, [(1, 1), (1, 1)]) if t == 0 else (1, [])


def run(name, mk, Lmax, **kw):
    t0 = time.time()
    try:
        v, st = value(mk, Lmax, **kw)
        a = mk()
        print("%-34s P=%d T=%-3d value=%d (target P-1=%d) %s  max_states_per_L=%d releases_checked=%d last_release_tick=%d  %.1fs"
              % (name, a.P, a.T, v, a.P - 1, "WIN" if v >= a.P - 1 else "FAILS", st[0], st[1], st[2], time.time() - t0))
    except PromiseViolation as e:
        print("%-34s PROMISE VIOLATION: %s" % (name, str(e)[:230]))
    sys.stdout.flush()


if __name__ == "__main__":
    what = sys.argv[1]
    if what == "selftest":
        y, n = harness.selftest(20000, 7)
        print("feasibility selftest: EDF == brute force on %d feasible / %d infeasible instances" % (y, n))
    elif what == "stair":
        for P in range(int(sys.argv[2]), int(sys.argv[3]) + 1):
            run("staircase", lambda: Staircase(P), P - 1)
    elif what == "simple":
        for P in range(int(sys.argv[2]), int(sys.argv[3]) + 1):
            run("simple", lambda: Simple(P), P - 1, symmetry=(len(sys.argv) > 4))
    elif what == "controls":
        for P in range(2, 7):
            run("control null (expect 0)", lambda: Null(P), P - 1)
        run("control liar (expect violation)", lambda: Liar(3), 2)
        for P in range(3, 7):
            for v in ("nostall", "short", "nounit", "lastlen"):
                run("control staircase/" + v, lambda: Staircase(P, v), P - 1)
        for P in range(3, 6):
            run("control simple N=P-2", lambda: Simple(P, P - 2), P - 1, symmetry=True)
