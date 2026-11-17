#!/usr/bin/env python3
"""Explicit history strategy "centre the belief": keep the exact belief (set of physical states
consistent with o_0..o_t); let lo,hi = min,max of x_t over it; play u = -sgn(lo+hi).
Exact sup|x| over all adversaries = max |x| over all beliefs reachable under this strategy.
Sign-compressed physical state (x, signs of x_{t-d..t-1}, rest). usage: greedy_hist.py dmin dmax [rule]"""
import sys
sg = lambda v: (v > 0) - (v < 0)
def run(d, rule=0, cap=200):
    s0 = (0, (0,) * d, False)
    b0 = frozenset([s0]); seen = {b0}; todo = [b0]; m = 0
    while todo:
        bel = todo.pop()
        xs = [s[0] for s in bel]; lo, hi = min(xs), max(xs)
        u = -sg(lo + hi)
        if rule == 1 and u == 0: u = -next(iter(bel))[1][0] if False else 0
        parts = {}
        for (x, sig, rest) in bel:
            nsig = sig[1:] + (sg(x),)
            for w in ((0,) if rest else (-1, 0, 1)):
                nx = x + u + w; m = max(m, abs(nx))
                if m > cap: return None, len(seen)
                parts.setdefault(nsig[0], set()).add((nx, nsig, w != 0))
        for p in parts.values():
            p = frozenset(p)
            if p not in seen: seen.add(p); todo.append(p)
    return m, len(seen)
if __name__ == '__main__':
    for d in range(int(sys.argv[1]), int(sys.argv[2]) + 1):
        print(f"d={d} centre-the-belief sup|x|={run(d)}  (2*floor(d/2)+1={2*(d//2)+1})", flush=True)
