#!/usr/bin/env python3
"""Checks the PROOF'S invariant (not just the band) on every reachable state of the naive value-window model:
     z_t = x_{t-d} + u_{t-d} + ... + u_t   satisfies |z_t| <= r = (d-1)/2,
     y_t = x_{t-d} + u_{t-d} + ... + u_{t-1} satisfies |y_t| <= r+1,
     x_{t+1} - z_t = w_{t-d}+...+w_t has absolute value <= r+1   (needs the w window, carried in the state).
Also long non-exhaustive runs for large odd d against random and greedy adversaries (sanity only, NOT a proof).
usage: check_invariant.py dmax_exhaustive"""
import sys, random
sg = lambda v: (v > 0) - (v < 0)
cl = lambda v: max(-1, min(1, v))
def exhaustive(d):
    r = (d - 1) // 2; K = r + 1
    s0 = ((0,) * (d + 1), (0,) * (d + 1), (0,) * d); seen = {s0}; todo = [s0]; mz = my = mo = mx = 0
    while todo:
        win, ws, us = todo.pop()          # win = x_{t-d..t}, ws = w_{t-d-1..t-1}, us = u_{t-d..t-1}
        y = win[0] + sum(us); u = -cl(sg(win[0]) * K + sum(us)); z = y + u
        assert abs(y) <= r + 1 and abs(z) <= r, (d, win, us)
        my = max(my, abs(y)); mz = max(mz, abs(z))
        for w in ((0,) if ws[-1] else (-1, 0, 1)):
            nws = ws[1:] + (w,); om = sum(nws); nx = win[-1] + u + w
            assert nx == z + om and abs(om) <= r + 1 and abs(nx) <= d
            mo = max(mo, abs(om)); mx = max(mx, abs(nx))
            k = (win[1:] + (nx,), nws, us[1:] + (u,))
            if k not in seen: seen.add(k); todo.append(k)
    print(f"d={d}: invariant holds on all {len(seen)} reachable states; max|y|={my} (<= {r+1}) max|z|={mz} (<= {r}) max|Omega|={mo} max|x|={mx}", flush=True)
def longrun(d, n, mode, seed):
    rnd = random.Random(seed); r = (d - 1) // 2; K = r + 1
    xs = [0] * (d + 1); us = [0] * d; pw = 0; m = 0
    for t in range(n):
        u = -cl(sg(xs[0]) * K + sum(us))
        if pw: w = 0
        elif mode == 'rand': w = rnd.choice((-1, 0, 1))
        elif mode == 'push': w = 1
        elif mode == 'away': w = sg(xs[-1] + u) or rnd.choice((-1, 1))
        else:  # 'runs': random-length pushes in one direction
            if t % rnd.randint(1, 3 * d) == 0: longrun.dir = rnd.choice((-1, 0, 1))
            w = longrun.dir
        nx = xs[-1] + u + w; m = max(m, abs(nx)); assert abs(nx) <= d, (d, mode, t)
        xs = xs[1:] + [nx]; us = us[1:] + [u]; pw = w
    return m
longrun.dir = 1
if __name__ == '__main__':
    for d in range(1, int(sys.argv[1]) + 1, 2): exhaustive(d)
    for d in (11, 13, 21, 51, 101):
        print(f"d={d} long runs (200000 ticks, non-exhaustive): " + " ".join(f"{m}:max|x|={longrun(d, 200000, m, 1)}" for m in ('rand', 'push', 'away', 'runs')), flush=True)
