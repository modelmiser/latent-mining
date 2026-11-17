# Verifier's own naive machine, written from the game definition only.
# Jobs are (r,p,d) tuples in a list; job identity = list index.
import sys
from functools import lru_cache
sys.setrecursionlimit(100000)

def actions(t, jobs, rem, A, prevsw, cost):
    """Legal actions in tick t. Returns list of ('idle',)|('switch',)|('run',j)."""
    acts = [('idle',)]
    if cost: acts.append(('switch',))
    for j, (r, p, d) in enumerate(jobs):
        if r > t or rem[j] == 0: continue
        if cost and A is not None and rem[A] > 0 and j != A and not prevsw:
            continue
        acts.append(('run', j))
    return acts

def step(rem, A, prevsw, act):
    if act[0] == 'idle': return rem, A, False
    if act[0] == 'switch': return rem, A, True
    j = act[1]
    rem2 = rem[:j] + (rem[j]-1,) + rem[j+1:]
    return rem2, j, False

def feasible(jobs, cost, want_schedule=False):
    """Exists hindsight schedule from tick 0 (no current job) completing every job by its deadline?
    Plain DFS with memo on the literal state. All jobs known; release times respected."""
    jobs = tuple(jobs)
    n = len(jobs)
    if n == 0: return ([] if want_schedule else True)
    H = max(d for _, _, d in jobs)
    memo = {}
    def go(t, rem, A, prevsw):
        if all(x == 0 for x in rem): return []
        if t >= H: return None
        for j in range(n):
            if rem[j] and max(t, jobs[j][0]) + rem[j] > jobs[j][2]: return None
        key = (t, rem, A, prevsw)
        if key in memo: return memo[key]
        res = None
        for act in actions(t, jobs, rem, A, prevsw, cost):
            rem2, A2, ps2 = step(rem, A, prevsw, act)
            sub = go(t+1, rem2, A2, ps2)
            if sub is not None:
                res = [act] + sub; break
        memo[key] = res
        return res
    s = go(0, tuple(p for _, p, _ in jobs), None, False)
    if want_schedule: return s
    return s is not None

def feasible_brute(jobs, cost):
    """No memo, no pruning except the horizon: enumerate every action sequence of length H."""
    jobs = tuple(jobs); n = len(jobs)
    if n == 0: return True
    H = max(d for _, _, d in jobs)
    def go(t, rem, A, prevsw, done_ok):
        if t == H:
            return all(x == 0 for x in rem)
        for act in actions(t, jobs, rem, A, prevsw, cost):
            rem2, A2, ps2 = step(rem, A, prevsw, act)
            if act[0] == 'run' and rem2[act[1]] == 0 and t+1 > jobs[act[1]][2]:
                continue  # finished late
            if go(t+1, rem2, A2, ps2, True): return True
        return False
    return go(0, tuple(p for _, p, _ in jobs), None, False, True)

def fmt_sched(jobs, sched):
    out = []
    for t, a in enumerate(sched):
        if a[0] == 'run': out.append("%d:run%s" % (t, jobs[a[1]],))
        else: out.append("%d:%s" % (t, a[0]))
    return ' '.join(out)
