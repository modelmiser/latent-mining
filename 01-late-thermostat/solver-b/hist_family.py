#!/usr/bin/env python3
"""Explicit finite-memory history strategy family ("budgeted relay"):
   u_t = -o_t  if the controller has pushed in direction -o_t fewer than theta times net
               over its last d controls ( -o_t * (u_{t-d}+...+u_{t-1}) < theta ), else 0.
Exact sup|x| by exhaustive reachability (naive model: value window + rest flag + last d controls).
usage: hist_family.py dmin dmax"""
import sys
sg = lambda v: (v > 0) - (v < 0)
def supx(d, theta, cap):
    s0 = ((0,) * (d + 1), False, (0,) * d); seen = {s0}; todo = [s0]; m = 0
    while todo:
        win, rest, us = todo.pop()
        o = sg(win[0]); u = -o if (-o * sum(us) < theta) else 0
        nus = us[1:] + (u,)
        for w in ((0,) if rest else (-1, 0, 1)):
            nx = win[-1] + u + w
            if abs(nx) > cap: return None
            m = max(m, abs(nx))
            k = (win[1:] + (nx,), w != 0, nus)
            if k not in seen: seen.add(k); todo.append(k)
    return m
if __name__ == '__main__':
    for d in range(int(sys.argv[1]), int(sys.argv[2]) + 1):
        res = {th: supx(d, th, 2 * d + 2) for th in range(1, d + 2)}
        best = min(v for v in res.values() if v is not None)
        print(f"d={d} target 2*floor(d/2)+1={2*(d//2)+1} best={best} by theta={[t for t,v in res.items() if v==best]} all={res}", flush=True)
