#!/usr/bin/env python3
"""Battery: adversaries x managers at larger M.  Prints final extent, extent - ceil(5M/3), ratio."""
import sys, math, time
from core import *
from managers import *
from adversaries import *

def run(adv, mgr, M, mode='F', log=None):
    g = Game(M, mgr, mode, log=log)
    adv(g)
    return g

ADV = {
    'P1': adv_P1,
    'O-dp': adv_O,
    'P6N': adv_P6N,
    'alt2': adv_alt2,
    'P6R': adv_P6R,
    'P6R-block': lambda g: adv_P6R(g, blockers=True),
    'P6R-t1': lambda g: adv_P6R(g, thresh=1),
}
MGR = [
    FF(), FFM(),
    PG('phi'), PG('phi+views'),
    PG('phi', two_score='views'),
    PG('phi', home=[6], two_score='views'),
    PG('phi', home=[7, 7, 4], two_score='views'),
    PG('phi', home=[7, 7, 4], two_score='viewsmax'),
]

if __name__ == '__main__':
    Ms = [int(x) for x in sys.argv[1].split(',')] if len(sys.argv) > 1 else [24, 36, 48, 60]
    advs = sys.argv[2].split(',') if len(sys.argv) > 2 else list(ADV)
    for M in Ms:
        base = math.ceil(5 * M / 3)
        print(f"\nM={M}  ceil(5M/3)={base}   entries: extent (extent-ceil(5M/3)) [moves]")
        print(f"{'manager':52s} " + ' '.join(f"{a:>14s}" for a in advs))
        for m in MGR:
            row = []
            for a in advs:
                t = time.time()
                g = run(ADV[a], m, M)
                row.append(f"{g.extent:4d}({g.extent - base:+3d})[{g.nmoves:2d}]")
            print(f"{m.name:52s} " + ' '.join(f"{x:>14s}" for x in row), flush=True)
