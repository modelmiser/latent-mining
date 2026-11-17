#!/usr/bin/env python3
"""Exhaustive play of the fixed adversary rule ADV(P) against EVERY online scheduler.

usage: play.py P DELTA [variant] [--brute] [--exact]
  variant: ok (default) | nothreat | fewrounds | notight
  --brute : at every release also run an independent brute-force same-cost feasibility
            search on the raw job list (tiny cases only)
  --exact : at every terminal node also run an exact search for the scheduler's best
            achievable max lateness (tiny cases only)

Machine semantics are the brief's, literally: actions run/idle/switch, A = job most recently
run, a switch tick licenses the next tick only.
Machine state = (jobs, A, flag): jobs = sorted tuple of (d, rem) of unfinished jobs,
A = (d, rem) of the locked job or None, flag = previous tick was a switch tick.
Jobs with equal (d, rem) are interchangeable for everything checked here.
"""
import sys
sys.setrecursionlimit(1000000)

P = int(sys.argv[1]); DELTA = int(sys.argv[2])
VAR = 'ok'
for a in sys.argv[3:]:
    if not a.startswith('--'): VAR = a
BRUTE = '--brute' in sys.argv
EXACT = '--exact' in sys.argv
ROUNDS = P - 1 if VAR != 'fewrounds' else P - 2
L = P - 1                      # lateness the adversary claims to force
TMAX = DELTA + ROUNDS * P      # largest deadline ever used = horizon T

def dK(rho):  return DELTA + (rho + 1) * P          # deadline of decoy K_rho
def dU(rho):
    return DELTA + rho * P                          # deadline of stream units of round rho

# ---------------------------------------------------------------- machine
def add_job(st, d, p):
    jobs, A, fl = st
    return (tuple(sorted(jobs + ((d, p),))), A, fl)

def run(st, c, t):
    """run one unit of a job of class c=(d,rem) at tick t; returns (newstate, lateness of completion or 0)."""
    jobs, A, fl = st
    assert c in jobs
    assert A is None or c == A or fl, "illegal: switch without switch tick"
    d, rem = c
    lst = list(jobs); lst.remove(c)
    if rem == 1:
        return (tuple(lst), None, False), max(0, t + 1 - d)
    lst.append((d, rem - 1)); lst.sort()
    return (tuple(lst), (d, rem - 1), False), 0

def idle(st):   return (st[0], st[1], False)
def switch(st): return (st[0], st[1], st[1] is not None)

def witness_ok(st, t):
    """declared completion of a hindsight plan: finish the locked job, then non-preemptive EDF.
    Returns True iff every job meets its deadline (no switch tick is ever needed)."""
    jobs, A, fl = st
    lst = list(jobs)
    if A is not None:
        lst.remove(A); t += A[1]
        if t > A[0]: return False
    for d, rem in sorted(lst):
        t += rem
        if t > d: return False
    return True

def cert(st, t, late):
    """sound certificate that the scheduler cannot keep max lateness <= L-1."""
    if late >= L: return True
    cum = 0                       # the last unit among jobs with deadline <= d completes at >= t+cum
    jobs = st[0]
    i = 0; n = len(jobs)
    while i < n:
        d = jobs[i][0]
        while i < n and jobs[i][0] == d:
            cum += jobs[i][1]; i += 1
        if t + cum - d >= L: return True
    return False

# exact: can the scheduler finish everything with max lateness <= lim ? (no more releases)
def can_finish(st, t, lim, memo):
    jobs, A, fl = st
    if not jobs: return True
    key = (st, t)
    if key in memo: return memo[key]
    cum = 0; ok = True
    for d, rem in jobs:
        cum += rem
        if t + cum > d + lim: ok = False; break
    res = False
    if ok:
        for c in sorted(set(jobs)):
            if A is None or c == A or fl:
                ns, _ = run(st, c, t)
                if (c[1] > 1 or t + 1 <= c[0] + lim) and can_finish(ns, t + 1, lim, memo):
                    res = True; break
        if not res and A is not None and not fl:
            res = can_finish(switch(st), t + 1, lim, memo)
    memo[key] = res
    return res

# independent brute force same-cost feasibility of a raw job list [(r,p,d)], from tick 0
def brute_feasible(joblist):
    n = len(joblist)
    memo = {}
    horizon = max(d for r, p, d in joblist)
    def rec(t, rems, A, fl):
        if all(x == 0 for x in rems): return True
        if t >= horizon: return False
        for i in range(n):
            if rems[i] and t + rems[i] > joblist[i][2]: return False
        key = (t, rems, A, fl)
        if key in memo: return memo[key]
        res = False
        seen = set()
        for i in range(n):
            if rems[i] and joblist[i][0] <= t and (A is None or A == i or fl):
                sig = (joblist[i], rems[i], A == i)
                if sig in seen: continue
                seen.add(sig)
                nr = rems[:i] + (rems[i] - 1,) + rems[i + 1:]
                if rec(t + 1, nr, (i if nr[i] else None), False): res = True; break
        if not res: res = rec(t + 1, rems, A, False) or (A is not None and rec(t + 1, rems, A, True))
        memo[key] = res
        return res
    return rec(0, tuple(p for r, p, d in joblist), None, False)

# ---------------------------------------------------------------- adversary rule
# adversary state: (mode, rho, s, aux, plan0, plan1)
#   plan0 / plan1 are hindsight machine states (None when dropped)
#   modes: 'A' phase 1 clean; 'R' scheduler is inside its first run of the decoy;
#          'W' wait until time aux then next round; 'P2' phase 2; 'F' fork; 'DONE'
def units_min(plan, kcls_d):
    """smallest-deadline job of the plan other than the decoy (deadline kcls_d); None if none."""
    for c in plan[0]:
        if c[0] != kcls_d: return c
    return None

class Fail(Exception): pass

stats = dict(nodes=0, maxcash=0, maxrel=0, wins_fork=0, wins_cash0=0, wins_cash1=0, early=0, maxlate_seen=0)

def start_round_or_cash(rho, t, H):
    """called at the start of tick t with reference plan H (unlocked, unit jobs only).
    returns (adv_state, releases)"""
    assert H[1] is None
    if rho == ROUNDS:
        return cash(H, DELTA + ROUNDS * P, t, 1)
    k = (dK(rho), P)
    p1 = add_job(H, *k)
    return ('A', rho, t, 0, p1, p1), [k]

def cash(plan, Dstar, t, which):
    pend = sum(rem for d, rem in plan[0] if d <= Dstar)
    f = Dstar - t - pend
    if f < 0: raise Fail('PROMISE: no room for cash-in at t=%d' % t)
    rel = [(Dstar, P)] * (f // P) + ([(Dstar, f % P)] if f % P else [])
    for j in rel: plan = add_job(plan, *j)
    stats['maxcash'] = max(stats['maxcash'], t + pend)
    return ('DONE', which, t, 0, plan if which == 0 else None, plan if which == 1 else None), rel

def adv_release(adv, t):
    """releases for tick t (decided before the scheduler moves) -> (adv', releases, planmoves)"""
    mode, rho, s, aux, p0, p1 = adv
    rel = []
    if mode == 'W' and t >= aux:
        adv, rel = start_round_or_cash(rho + 1, t, p1)
        mode, rho, s, aux, p0, p1 = adv
        if mode == 'DONE': return adv, rel
    elif mode == 'NEXT':
        adv, rel = start_round_or_cash(rho + 1, t, p1)
        mode, rho, s, aux, p0, p1 = adv
        if mode == 'DONE': return adv, rel
    kd = dK(rho)
    if mode in ('A', 'R'):
        if units_min(p0, kd) is None:
            u = (dU(rho), 1); rel.append(u)
            p0 = add_job(p0, *u); p1 = add_job(p1, *u)
    elif mode == 'P2':
        if not p1[0]:          # reference plan is clear, scheduler still owes the whole decoy: lag >= P
            return cash(p1, dK(rho), t, 1)
    elif mode == 'F':
        if (aux == 0 and VAR != 'nothreat') or not p1[0]:
            u = ((t + 1) if VAR != 'notight' else dU(rho), 1); rel.append(u); p1 = add_job(p1, *u)
    return (mode, rho, s, aux, p0, p1), rel

def adv_observe(adv, t, kind, kdone):
    """advance plans through tick t and update the mode from the scheduler's action kind:
       'i' inner unit, 'k' decoy unit, 'e' empty."""
    mode, rho, s, aux, p0, p1 = adv
    if mode == 'DONE': return adv
    kd = dK(rho)
    # ---- hindsight plan moves for tick t (legality asserted inside run())
    if p0 is not None:                       # plan0: one inner unit per tick, never the decoy
        c = units_min(p0, kd)
        if c is None: raise Fail('plan0 has no inner unit at t=%d' % t)
        p0, la = run(p0, c, t)
        if la: raise Fail('PROMISE: plan0 late at t=%d' % t)
    kc = None
    for c in p1[0]:
        if c[0] == kd and (c[1] == P or p1[1] == c): kc = c
    if kc is not None and mode in ('A', 'R', 'W'):
        p1, la = run(p1, kc, t)              # plan1: decoy first, contiguously
    else:
        c = units_min(p1, None)
        if c is None:
            if mode == 'W': p1 = idle(p1)
            else: raise Fail('plan1 idle at t=%d mode %s' % (t, mode))
            la = 0
        else:
            p1, la = run(p1, c, t)
    if la: raise Fail('PROMISE: plan1 late at t=%d' % t)
    # ---- mode update
    if mode == 'A':
        if kind == 'i':
            if t + 1 == s + P: return ('P2', rho, s, 0, None, p1)
            return ('A', rho, s, 0, p0, p1)
        if kind == 'e': return ('W', rho, s, s + P, None, p1)
        return ('R', rho, s, 0, p0, p1) if not kdone else None
    if mode == 'R':
        if kind == 'k':
            if kdone: return ('CASH0', rho, s, 0, p0, None)
            return ('R', rho, s, 0, p0, p1)
        if kind == 'e': return ('W', rho, s, max(t + 1, s + P), None, p1)
        raise Fail('scheduler ran inner while locked on decoy?')
    if mode == 'W':
        return ('W', rho, s, aux, None, p1)
    if mode == 'P2':
        if kind == 'i': return ('P2', rho, s, 0, None, p1)
        if kind == 'e': return ('NEXT', rho, s, 0, None, p1)
        return ('F', rho, s, 0, None, p1) if not kdone else ('FWIN', rho, s, 0, None, p1)
    if mode == 'F':
        if kind == 'k':
            return ('FWIN', rho, s, 0, None, p1) if kdone else ('F', rho, s, 1, None, p1)
        if kind == 'e': return ('NEXT', rho, s, 0, None, p1)
        raise Fail('scheduler ran inner during fork while locked?')
    raise Fail('bad mode ' + mode)

# ---------------------------------------------------------------- exhaustive play
visited = set()
joblog = []     # raw job list (r,p,d) along the current path, for --brute
path = []

def terminal_check(online, t, late, what):
    if not cert(online, t, late):
        # no load-bound certificate: decide exactly whether the scheduler can still keep lateness <= L-1
        if late <= L - 1 and can_finish(online, t, L - 1, {}):
            raise Fail('SCHEDULER SURVIVES: at terminal %s t=%d exact search finds a completion with max lateness <= %d' % (what, t, L - 1))
        stats['certless_wins'] = stats.get('certless_wins', 0) + 1
    elif EXACT:
        if late < L and can_finish(online, t, L - 1, {}):
            raise Fail('certificate true but exact search says scheduler survives (BUG)')

def explore(t, adv, online, late):
    if not BRUTE:                 # with --brute the raw job list matters, so no merging of histories
        key = (t, adv, online, min(late, L))
        if key in visited: return
        visited.add(key)
    stats['nodes'] += 1
    mode = adv[0]
    if mode == 'FWIN':
        stats['wins_fork'] += 1
        terminal_check(online, t, late, 'fork'); return
    if mode == 'CASH0':
        adv, rel = cash(adv[4], dU(adv[1]), t, 0)
    else:
        adv, rel = adv_release(adv, t)
    mode = adv[0]
    npush = 0
    if rel:
        stats['maxrel'] = max(stats['maxrel'], t)
        for d, p in rel:
            if not (1 <= p <= P and t + p <= d <= TMAX and t < TMAX):
                raise Fail('ILLEGAL JOB (r=%d,p=%d,d=%d) T=%d' % (t, p, d, TMAX))
            online = add_job(online, d, p)
            joblog.append((t, p, d)); npush += 1
        # promise: every live plan must have a valid completion for all jobs released so far
        for pl in (adv[4], adv[5]):
            if pl is not None and not witness_ok(pl, t):
                raise Fail('PROMISE: plan has no valid completion at t=%d mode=%s' % (t, mode))
        if BRUTE and not brute_feasible(joblog):
            raise Fail('PROMISE(brute): job list infeasible at t=%d' % t)
    try:
        if mode == 'DONE':
            stats['wins_cash%d' % adv[1]] += 1
            terminal_check(online, t, late, 'cash'); return
        if cert(online, t, late):
            stats['early'] += 1
            return                               # lateness >= P-1 already unavoidable
        jobs, A, fl = online
        kd = dK(adv[1])
        acts = []
        for c in sorted(set(jobs)):
            if A is None or c == A or fl: acts.append(c)
        for c in acts:
            ns, la = run(online, c, t)
            kind = 'k' if c[0] == kd and mode != 'W' else 'i'
            kdone = kind == 'k' and c[1] == 1
            if mode == 'W': na = adv_observe(adv, t, 'i', False)
            else: na = adv_observe(adv, t, kind, kdone)
            path.append((t, 'run', c))
            explore(t + 1, na, ns, max(late, la))
            path.pop()
        for nm, ns in (('idle', idle(online)), ('switch', switch(online))):
            if nm == 'switch' and A is None: continue
            na = adv_observe(adv, t, 'e', False)
            path.append((t, nm))
            explore(t + 1, na, ns, late)
            path.pop()
    finally:
        for _ in range(npush): joblog.pop()

def main():
    try:
        explore(0, ('NEXT', -1, 0, 0, None, ((), None, False)), ((), None, False), 0)
        print('P=%d DELTA=%d T=%d variant=%s : ADVERSARY WINS against every scheduler (lateness >= %d)' % (P, DELTA, TMAX, VAR, L))
    except Fail as e:
        print('P=%d DELTA=%d T=%d variant=%s : FAIL: %s' % (P, DELTA, TMAX, VAR, e))
        print('  path:', path[-40:])
        print('  jobs (r,p,d):', joblog)
    print('  ', stats)

main()
