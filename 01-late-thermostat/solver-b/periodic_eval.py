#!/usr/bin/env python3
"""Exact sup|x| of a purely periodic clock (from t=0), independent naive model (value windows).
usage: periodic_eval.py dmin dmax maxperiod   -- searches on/off patterns of the relay (+,0,-):
 pattern bit 1 = map (+0-), bit 0 = map (000)."""
import sys, itertools
sg = lambda v: (v > 0) - (v < 0)
def supx(d, pat, cap):
    p = len(pat); s0 = ((0,) * (d + 1), False, 0); seen = {s0}; todo = [s0]; m = 0
    while todo:
        win, rest, ph = todo.pop()
        u = -sg(win[0]) if pat[ph] else 0
        for w in ((0,) if rest else (-1, 0, 1)):
            nx = win[-1] + u + w
            if abs(nx) > cap: return None
            m = max(m, abs(nx))
            k = (win[1:] + (nx,), w != 0, (ph + 1) % p)
            if k not in seen: seen.add(k); todo.append(k)
    return m
if __name__ == '__main__':
    d0, d1, P = map(int, sys.argv[1:4])
    for d in range(d0, d1 + 1):
        best = None; arg = []
        for p in range(1, P + 1):
            for pat in itertools.product((0, 1), repeat=p):
                v = supx(d, pat, best if best is not None else 2 * d + 3)
                if v is None: continue
                if best is None or v < best: best = v; arg = [pat]
                elif v == best and len(arg) < 4: arg.append(pat)
        alt = supx(d, (1, 0), 3 * d + 5); alt2 = supx(d, (0, 1), 3 * d + 5)
        print(f"d={d} best_onoff_period<={P}: sup|x|={best} patterns={arg}  | (1,0)->{alt} (0,1)->{alt2}", flush=True)
