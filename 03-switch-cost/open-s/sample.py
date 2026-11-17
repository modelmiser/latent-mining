#!/usr/bin/env python3
"""SAMPLED (not exhaustive) play of ADV(P): usage sample.py P DELTA NPLAYS [variant] [seed]
Random and structured schedulers; every release is promise-checked through the witness plans,
every terminal is checked with the load-bound certificate (exact search only if it fails)."""
import sys, random
P_, D_, N = sys.argv[1], sys.argv[2], int(sys.argv[3])
var = sys.argv[4] if len(sys.argv) > 4 else 'ok'
seed = int(sys.argv[5]) if len(sys.argv) > 5 else 1
src = open('play.py').read()
src = src[:src.rindex('main()')]
sys.argv = ['play.py', P_, D_, var]
exec(src)
rng = random.Random(seed)

def one_play(pol):
    t = 0; adv = ('NEXT', -1, 0, 0, None, ((), None, False)); online = ((), None, False); late = 0
    pe, pedf = pol
    while True:
        mode = adv[0]
        if mode == 'FWIN':
            terminal_check(online, t, late, 'fork'); return t
        if mode == 'CASH0': adv, rel = cash(adv[4], dU(adv[1]), t, 0)
        else: adv, rel = adv_release(adv, t)
        mode = adv[0]
        for d, p in rel:
            if not (1 <= p <= P and t + p <= d <= TMAX and t < TMAX): raise Fail('ILLEGAL JOB')
            online = add_job(online, d, p)
        if rel:
            for pl in (adv[4], adv[5]):
                if pl is not None and not witness_ok(pl, t): raise Fail('PROMISE at t=%d mode=%s' % (t, mode))
        if mode == 'DONE':
            terminal_check(online, t, late, 'cash'); return t
        if cert(online, t, late): return t
        jobs, A, fl = online
        kd = dK(adv[1])
        acts = [c for c in sorted(set(jobs)) if A is None or c == A or fl]
        r = rng.random()
        if not acts or r < pe:
            online = switch(online) if (A is not None and rng.random() < 0.8) else idle(online)
            adv = adv_observe(adv, t, 'e', False)
        else:
            c = acts[0] if rng.random() < pedf else rng.choice(acts)
            ns, la = run(online, c, t)
            kind = 'k' if c[0] == kd and mode != 'W' else 'i'
            if mode == 'W': adv = adv_observe(adv, t, 'i', False)
            else: adv = adv_observe(adv, t, kind, kind == 'k' and c[1] == 1)
            online = ns; late = max(late, la)
        t += 1

pols = [(0.0, 1.0), (0.02, 0.98), (0.05, 0.9), (0.1, 0.8), (0.2, 0.5), (0.3, 0.0), (0.1, 0.0), (0.03, 1.0), (0.08, 1.0), (0.15, 0.95)]
try:
    mx = 0
    for i in range(N):
        mx = max(mx, one_play(pols[i % len(pols)]))
    print('P=%s DELTA=%s T=%d variant=%s: %d sampled plays, adversary won all (lateness >= %d); longest play ended at t=%d; maxcash=%d certless=%d'
          % (P_, D_, TMAX, var, N, L, mx, stats['maxcash'], stats.get('certless_wins', 0)))
except Fail as e:
    print('P=%s DELTA=%s variant=%s: FAIL after %d plays: %s' % (P_, D_, var, i, e))
