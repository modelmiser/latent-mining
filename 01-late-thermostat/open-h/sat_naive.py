#!/usr/bin/env python3
"""Naive-model exhaustive check (original coordinates, no change of variable) of the SATURATED-SUM controller
     u_t = -clamp( o_t*K + (u_{t-d}+...+u_{t-1}), -1, +1 ),   K = (d+1)/2  (K overridable for mutants)
State = (x_{t-d..t} value window, adversary-must-rest flag, last d controls).  All adversaries.
usage: sat_naive.py dmin dmax [cap_offset=0] [K_offset=0]   -> prints sup|x| or FAIL(>cap) with cap = d+cap_offset"""
import sys
sg = lambda v: (v > 0) - (v < 0)
cl = lambda v: max(-1, min(1, v))
def supx(d, cap, K):
    s0 = ((0,) * (d + 1), False, (0,) * d); seen = {s0}; todo = [s0]; m = 0
    while todo:
        win, rest, us = todo.pop()
        u = -cl(sg(win[0]) * K + sum(us))
        nus = us[1:] + (u,)
        for w in ((0,) if rest else (-1, 0, 1)):
            nx = win[-1] + u + w
            if abs(nx) > cap: return None, len(seen)
            m = max(m, abs(nx))
            k = (win[1:] + (nx,), w != 0, nus)
            if k not in seen: seen.add(k); todo.append(k)
    return m, len(seen)
if __name__ == '__main__':
    co = int(sys.argv[3]) if len(sys.argv) > 3 else 0
    ko = int(sys.argv[4]) if len(sys.argv) > 4 else 0
    for d in range(int(sys.argv[1]), int(sys.argv[2]) + 1):
        K = (d + 1) // 2 + ko
        m, n = supx(d, d + co, K)
        print(f"d={d} K={K} cap={d+co} sup|x|={'FAIL(exceeds cap)' if m is None else m} states={n}", flush=True)
