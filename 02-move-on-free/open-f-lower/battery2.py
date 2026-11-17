#!/usr/bin/env python3
"""Test (2b): final battery.  Adversaries x managers at larger M, game F, referee = core.Game.
Columns: final extent minus ceil(5M/3).  Last column of each row: max over the F-adversaries.
Negative controls:
  NC1  adversaries P1 and O-dp free only 1s, so against first fit they are bound by solver-b's P5:
       extent <= floor((5M+2)/3).  Checked (assert) for every M.
  NC2  P6N (solver-b's P6 played in F, commits to one class): the reply of FF+move / PG managers
       should cancel it (extent near the P1 level), while the same P6N beats FF without moves.
  NC3  every run goes through the referee, which rejects any illegal move/placement (Illegal).
"""
import sys, math, time
from core import *
from managers import *
from adversaries import *
from wb import WB, adv_P6RS

ADV = [
    ('P1', adv_P1), ('O-dp', adv_O), ('P6N', adv_P6N),
    ('P6R', adv_P6R),
    ('P6R-blk', lambda g: adv_P6R(g, blockers=True)),
    ('P6RS', adv_P6RS),
]
def managers():
    return [FF(), FFM(), PG('phi'), PG('phi+views'),
            WB(None), WB([6]), WB([9]), WB([5]), WB([4, 5]), WB([5, 7]), WB([4, 4, 7]), WB([7, 7, 4]), WB([4, 4, 4, 5, 5, 5])]

if __name__ == '__main__':
    Ms = [int(x) for x in sys.argv[1].split(',')]
    table = {}
    for M in Ms:
        base = math.ceil(5 * M / 3)
        print(f"\nM={M}  ceil(5M/3)={base}  floor((5M+2)/3)={(5 * M + 2) // 3}", flush=True)
        print(f"{'manager':38s} " + ' '.join(f"{a:>9s}" for a, _ in ADV) + '   maxF  S-choice')
        for m in managers():
            row = []; ext = {}
            choice = ''
            for an, af in ADV:
                mm = type(m)(m.home) if isinstance(m, WB) else m
                g = Game(M, mm); g.choice = ''
                af(g)
                ext[an] = g.extent
                if g.choice: choice = g.choice
                row.append(f"{g.extent - base:+4d}[{g.nmoves:2d}]")
            if isinstance(m, FF) and type(m) is FF:
                assert ext['P1'] <= (5 * M + 2) // 3 and ext['O-dp'] <= (5 * M + 2) // 3, 'NC1 violated'
            mx = max(ext[a] for a in ('P6N', 'P6R', 'P6R-blk', 'P6RS')) - base
            table[(m.name, M)] = mx
            print(f"{m.name:38s} " + ' '.join(f"{x:>9s}" for x in row) + f"   {mx:+4d}  {choice}", flush=True)
    print("\nSUMMARY  max over F-adversaries of (extent - ceil(5M/3)), by manager and M; last column = slope between first and last M")
    names = []
    for (n, M) in table:
        if n not in names: names.append(n)
    print(f"{'manager':38s} " + ' '.join(f"M={M:<4d}" for M in Ms) + '  d/dM')
    best = {M: None for M in Ms}
    for n in names:
        vals = [table[(n, M)] for M in Ms]
        sl = (vals[-1] - vals[0]) / (Ms[-1] - Ms[0]) if len(Ms) > 1 else 0
        print(f"{n:38s} " + ' '.join(f"{v:+6d}" for v in vals) + f"  {sl:+.4f}")
        for M, v in zip(Ms, vals):
            if best[M] is None or v < best[M][0]: best[M] = (v, n)
    print("best manager per M (min over managers of max over adversaries):")
    for M in Ms:
        print(f"   M={M}: {best[M][0]:+d}  by {best[M][1]}")
