#!/usr/bin/env python3
"""Referee's slow reference, written from the problem statement only.
 ref.py wins h a1 a2 ...          -> WIN / LOSE (+ a surviving line)
 ref.py line h a1 .. am -- l1 .. lm   (li = P or vertex) -> checks the Firer line survives
 ref.py C hmax                    -> C(h) table
 ref.py G h W mmax                -> least m with a winning list in window [0,W-1] (brute force over ALL lists)
 ref.py witfile FILE              -> check lines 'WIN h list' / 'LOSE h list | line'
"""
import sys, itertools

def step_states(S, a, h):
    """S: dict state(tuple of sorted (pos,chips)) -> line (tuple). returns new dict after one round."""
    out = {}
    for st, ln in S.items():
        d = dict(st)
        d[a] = d.get(a, 0) + 1
        if d[a] >= h:
            continue                      # height h after step (1): this line is dead
        key = tuple(sorted(d.items()))
        if key not in out:
            out[key] = ln + ('P',)        # pass
        for x, c in d.items():
            if c >= 2:
                e = dict(d)
                e[x] -= 2
                if e[x] == 0:
                    del e[x]
                e[x-1] = e.get(x-1, 0) + 1
                e[x+1] = e.get(x+1, 0) + 1
                if e[x-1] >= h or e[x+1] >= h:
                    continue              # height h after step (2)
                k2 = tuple(sorted(e.items()))
                if k2 not in out:
                    out[k2] = ln + (x,)
    return out

def survivor(h, lst):
    S = {(): ()}
    for a in lst:
        S = step_states(S, a, h)
        if not S:
            return None
    return next(iter(S.values()))

def check_line(h, lst, line):
    """independent of step_states: plain simulation"""
    if len(line) != len(lst):
        return "length mismatch"
    d = {}
    for t, (a, mv) in enumerate(zip(lst, line), 1):
        d[a] = d.get(a, 0) + 1
        if max(d.values()) >= h:
            return "height %d reached after add of round %d" % (h, t)
        if mv != 'P':
            x = int(mv)
            if d.get(x, 0) < 2:
                return "illegal firing of %d in round %d" % (x, t)
            d[x] -= 2
            d[x-1] = d.get(x-1, 0) + 1
            d[x+1] = d.get(x+1, 0) + 1
            if max(d.values()) >= h:
                return "height %d reached after firing in round %d" % (h, t)
    return "OK"

def C(h, cap=200):
    S = {(): ()}
    for m in range(1, cap):
        S = step_states(S, 0, h)
        if not S:
            return m

def G(h, W, mmax):
    for m in range(1, mmax + 1):
        for lst in itertools.product(range(W), repeat=m - 1):
            l = (0,) + lst if False else None
        # a_1 ranges over the window too (no symmetry assumed)
        for lst in itertools.product(range(W), repeat=m):
            if survivor(h, lst) is None:
                return m, lst
    return None

if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "wins":
        h = int(sys.argv[2]); lst = [int(x) for x in sys.argv[3:]]
        s = survivor(h, lst)
        print("WIN" if s is None else "LOSE " + " ".join(map(str, s)))
    elif cmd == "line":
        i = sys.argv.index("--")
        h = int(sys.argv[2]); lst = [int(x) for x in sys.argv[3:i]]
        print(check_line(h, lst, sys.argv[i+1:]))
    elif cmd == "C":
        for h in range(1, int(sys.argv[2]) + 1):
            print("C(%d) = %d" % (h, C(h)))
    elif cmd == "G":
        print(G(int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])))
    elif cmd == "witfile":
        for raw in open(sys.argv[2]):
            raw = raw.strip()
            if not raw or raw.startswith("#"):
                continue
            w = raw.split()
            if w[0] == "WIN":
                h = int(w[1]); lst = [int(x) for x in w[2:]]
                print("ACCEPT" if survivor(h, lst) is None else "REJECT", raw)
            elif w[0] == "LOSE":
                i = w.index("|")
                h = int(w[1]); lst = [int(x) for x in w[2:i]]
                r = check_line(h, lst, w[i+1:])
                print("ACCEPT" if r == "OK" else "REJECT (" + r + ")", raw)
