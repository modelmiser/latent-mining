#!/usr/bin/env python3
"""Reduced battery for large M: only the managers that did best in battery2 (the ones designed against P6R)."""
import sys, math, time
from core import *
from adversaries import *
from wb import WB, adv_P6RS
ADV = [('P6R', adv_P6R), ('P6R-blk', lambda g: adv_P6R(g, blockers=True)), ('P6RS', adv_P6RS)]
HOMES = [[4, 4, 7], [4, 4, 4, 5, 5, 5], [4, 5], [5]]
for M in [int(x) for x in sys.argv[1].split(',')]:
    base = math.ceil(5 * M / 3)
    print(f"\nM={M} ceil(5M/3)={base}: extent-ceil(5M/3) [moves] {{S}} time", flush=True)
    for h in HOMES:
        row = []; mx = None
        for an, af in ADV:
            g = Game(M, WB(h)); g.choice = ''
            t = time.time(); af(g)
            row.append(f"{an}:{g.extent - base:+d}[{g.nmoves}]{{{g.choice}}} {time.time() - t:.0f}s")
            mx = g.extent - base if mx is None else max(mx, g.extent - base)
        print(f"  WB home={str(h):20s} " + '  '.join(row) + f"   max={mx:+d}", flush=True)
