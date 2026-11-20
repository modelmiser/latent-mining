#!/usr/bin/env python3
"""Direct, slow implementation of the definition.
survives(lst,h): does Firer have a line keeping every vertex <= h-1 after every step?
G_window(h,W): least m such that some list in {0..W-1}^m wins (brute force over all lists).
Usage: slow.py check h a1,a2,...   |  slow.py G h W maxm  |  slow.py C h
"""
import sys, itertools
from functools import lru_cache

ALT=False  # ALT=True: heights are only tested at the END of each round (after step 2)
def survives(lst, h, want_line=False):
    lst = tuple(lst)
    sys.setrecursionlimit(10000)
    memo = {}
    def rec(t, cfg):  # cfg: sorted tuple of (vertex,chips) with chips>0
        if t == len(lst):
            return []
        key = (t, cfg)
        if key in memo:
            return memo[key]
        d = dict(cfg)
        a = lst[t]
        d[a] = d.get(a, 0) + 1
        res = None
        if d[a] <= h - 1 or ALT:
            # options: pass, or fire one ready vertex
            opts = [None] + [v for v in sorted(d) if d[v] >= 2]
            for v in opts:
                e = dict(d)
                if v is not None:
                    e[v] -= 2
                    e[v-1] = e.get(v-1, 0) + 1
                    e[v+1] = e.get(v+1, 0) + 1
                    if e[v] == 0:
                        del e[v]
                if max(e.values()) >= h:
                    continue
                r = rec(t + 1, tuple(sorted(e.items())))
                if r is not None:
                    res = [v] + r
                    break
        memo[key] = res
        return res
    r = rec(0, ())
    return r if want_line else (r is not None)

def C(h, maxm=200):
    for m in range(1, maxm):
        if not survives([0]*m, h):
            return m

def Gw(h, W, maxm):
    for m in range(1, maxm + 1):
        for lst in itertools.product(range(W), repeat=m):
            if lst[0] > (W-1)//2 and W > 1 and False:
                continue
            if not survives(lst, h):
                return m, lst
    return None

if __name__ == "__main__":
    if sys.argv[1] == "ALT":
        ALT = True; del sys.argv[1]
    cmd = sys.argv[1]
    if cmd == "check":
        h = int(sys.argv[2]); lst = [int(x) for x in sys.argv[3].split(",")]
        line = survives(lst, h, True)
        if line is None:
            print("LIST WINS (no surviving Firer line), length", len(lst))
        else:
            print("LIST LOSES; surviving Firer line (None=pass):", line)
    elif cmd == "C":
        print(C(int(sys.argv[2])))
    elif cmd == "G":
        print(Gw(int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])))
