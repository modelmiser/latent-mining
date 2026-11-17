#!/usr/bin/env python3
"""Exhaustive play of a FIXED adversary rule against EVERY online scheduler.

Game: see BRIEF.txt (free promise).  The adversary is a deterministic rule; only the
scheduler branches.  For a lateness bound L we search for ANY scheduler line that keeps
every job's lateness <= L.  value(rule) = smallest L for which such a line exists.

Machine model is the unsimplified one of the brief: run / idle / switch, with
A = most recently run job, and "prev tick was a switch tick" flag.

Checks made on every release (any failure raises):
  - job legal: 1 <= p <= P, r + p <= d <= T, r < T, r == current tick
  - promise: the multiset of all jobs released so far is preemptively feasible
    (EDF simulation from tick 0; compared with brute force in selftest()).
"""
import sys, itertools, random
sys.setrecursionlimit(1000000)


# ---------------------------------------------------------------- feasibility
def edf_feasible(jobs):
    """jobs: list of (r,p,d). Exact test for preemptive (switch cost 0) feasibility."""
    if not jobs:
        return True
    rem = [p for (r, p, d) in jobs]
    left = sum(rem)
    t = min(r for (r, p, d) in jobs)
    while left:
        best = -1
        for i, (r, p, d) in enumerate(jobs):
            if rem[i] and r <= t and (best < 0 or d < jobs[best][2]):
                best = i
        if best >= 0:
            rem[best] -= 1
            left -= 1
            if rem[best] == 0 and t + 1 > jobs[best][2]:
                return False
            if rem[best] and t + 1 >= jobs[best][2]:
                return False
        t += 1
    return True


def brute_feasible(jobs):
    """Independent brute force: try every per-tick choice."""
    if not jobs:
        return True
    horizon = max(d for (r, p, d) in jobs)

    def rec(t, rem):
        if all(x == 0 for x in rem):
            return True
        if t >= horizon:
            return False
        for i, (r, p, d) in enumerate(jobs):
            if rem[i] and d - t < rem[i]:
                return False
        tried = False
        for i, (r, p, d) in enumerate(jobs):
            if rem[i] and r <= t:
                tried = True
                rem[i] -= 1
                ok = rec(t + 1, rem)
                rem[i] += 1
                if ok:
                    return True
        return rec(t + 1, rem)  # idle (also needed when nothing is released yet)
    return rec(0, [p for (r, p, d) in jobs])


def selftest(n=4000, seed=1):
    rng = random.Random(seed)
    yes = no = 0
    for _ in range(n):
        k = rng.randint(1, 4)
        jobs = []
        for _ in range(k):
            p = rng.randint(1, 3)
            r = rng.randint(0, 4)
            d = r + p + rng.randint(0, 3)
            jobs.append((r, p, d))
        a, b = edf_feasible(jobs), brute_feasible(jobs)
        if a != b:
            raise SystemExit("FEASIBILITY MISMATCH %r edf=%s brute=%s" % (jobs, a, b))
        yes += a
        no += (not a)
    return yes, no


# ---------------------------------------------------------------- game engine
class PromiseViolation(Exception):
    pass


class Game:
    """adversary object must provide:
         P, T
         start() -> advstate
         release(t, advstate, view) -> (advstate, [(p, d), ...])   jobs released at tick t
             view = dict(ev=last online event, jobs=[(jid, r, p, d, rem)...] all released jobs)
             ev is None at t = 0, else ('run', jid, was_first_unit) | ('idle',) | ('switch',)
         done(t, advstate) -> True if no further release can ever happen
       advstate must be hashable and, together with (t, remaining work, lock, flag),
       determine all future adversary behaviour.
    """

    def __init__(self, adv, check_promise=True, symmetry=False, verbose=False):
        self.adv = adv
        self.check_promise = check_promise
        self.symmetry = symmetry
        self.nodes = 0
        self.releases_checked = 0
        self.max_release_tick = -1

    def can_survive(self, L):
        """True iff some scheduler keeps every lateness <= L against the rule."""
        self.L = L
        self.memo = {}
        self.witness = None
        st = self.adv.start()
        return self._after_tick(0, st, (), (), None, False, None, ())

    # jobs: tuple of (r,p,d) for all released jobs (index = jid); rem: tuple of remaining
    def _after_tick(self, t, ast, jobs, rem, lock, flag, ev, trace):
        # adversary releases at tick t
        if t < self.adv.T:
            view = dict(ev=ev, jobs=[(i,) + jobs[i] + (rem[i],) for i in range(len(jobs))])
            ast, rel = self.adv.release(t, ast, view)
            if rel:
                jl = list(jobs)
                rl = list(rem)
                for (p, d) in rel:
                    if not (1 <= p <= self.adv.P and t + p <= d <= self.adv.T and t < self.adv.T):
                        raise PromiseViolation("illegal job (r=%d,p=%d,d=%d) P=%d T=%d" % (t, p, d, self.adv.P, self.adv.T))
                    jl.append((t, p, d))
                    rl.append(p)
                    if self.check_promise:
                        self.releases_checked += 1
                        if not edf_feasible(jl):
                            raise PromiseViolation("promise broken at t=%d by (r=%d,p=%d,d=%d); all jobs %r; trace %r" % (t, t, p, d, jl, trace))
                jobs = tuple(jl)
                rem = tuple(rl)
                self.max_release_tick = max(self.max_release_tick, t)
        return self._choose(t, ast, jobs, rem, lock, flag, trace)

    def _choose(self, t, ast, jobs, rem, lock, flag, trace):
        L = self.L
        # prune: some unfinished job can no longer meet d + L
        for i, x in enumerate(rem):
            if x and t + x - jobs[i][2] > L:
                return False
        if not any(rem) and (t >= self.adv.T or self.adv.done(t, ast)):
            self.witness = trace
            return True
        if t > self.adv.T + self.adv.P * (len(jobs) + 2) + 5:
            raise RuntimeError("runaway")
        key = (t, ast, jobs, rem, lock, flag)
        if key in self.memo:
            return self.memo[key]
        self.nodes += 1
        res = False
        locked = lock is not None and rem[lock] > 0
        seen = set()
        moves = []
        for i, x in enumerate(rem):
            if not x:
                continue
            if locked and i != lock and not flag:
                continue
            if self.symmetry and i != lock:
                sig = (jobs[i], x)
                if sig in seen:
                    continue
                seen.add(sig)
            moves.append(i)
        for i in moves:
            r2 = list(rem)
            r2[i] -= 1
            first = (rem[i] == jobs[i][1])
            if r2[i] == 0 and t + 1 - jobs[i][2] > L:
                continue
            if self._after_tick(t + 1, ast, jobs, tuple(r2), i, False, ('run', i, first), trace + (('r', i),) if TRACE else ()):
                res = True
                break
        if not res:
            if self._after_tick(t + 1, ast, jobs, rem, lock, True, ('switch',), trace + (('s',),) if TRACE else ()):
                res = True
        if not res:
            if self._after_tick(t + 1, ast, jobs, rem, lock, False, ('idle',), trace + (('i',),) if TRACE else ()):
                res = True
        self.memo[key] = res
        return res


TRACE = False


def value(make_adv, Lmax, **kw):
    """smallest L such that some scheduler keeps lateness <= L (searches L = 0..Lmax)."""
    stats = None
    hardest = 0
    for L in range(0, Lmax + 1):
        g = Game(make_adv(), **kw)
        ok = g.can_survive(L)
        hardest = max(hardest, g.nodes)   # states explored in the largest refutation/search
        stats = (hardest, g.releases_checked, g.max_release_tick)
        if ok:
            return L, stats
    return Lmax + 1, stats


if __name__ == "__main__":
    y, n = selftest()
    print("feasibility selftest: EDF == brute force on %d feasible / %d infeasible random instances" % (y, n))
