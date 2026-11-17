#!/usr/bin/env python3
"""Variant of greedy_hist.py: push (u = -sgn(lo+hi)) only if |lo+hi| >= thr. usage: greedy_hist2.py dmin dmax thr"""
import sys
import greedy_hist as g
sg = g.sg
def run(d, thr, cap=200):
    s0 = (0, (0,) * d, False); b0 = frozenset([s0]); seen = {b0}; todo = [b0]; m = 0
    while todo:
        bel = todo.pop(); xs = [s[0] for s in bel]; c = min(xs) + max(xs)
        u = -sg(c) if abs(c) >= thr else 0
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
for d in range(int(sys.argv[1]), int(sys.argv[2]) + 1):
    print(f"d={d} thr={sys.argv[3]} sup|x|={run(d, int(sys.argv[3]))} (2*floor(d/2)+1={2*(d//2)+1})", flush=True)
