#!/usr/bin/env python3
"""Direct, slow implementation of the definition.
wins(lst,h): True iff for every legal Firer line some vertex reaches >= h.
Computed as: set of all positions reachable by Firer in which every vertex has
stayed <= h-1 after every step so far; list wins iff that set becomes empty."""
import sys, itertools

def step(S, a, h):
    """S: set of frozenset-of-(pos,height) safe positions. One round with add at a."""
    out = set()
    for c in S:
        d = dict(c)
        d[a] = d.get(a, 0) + 1
        if d[a] >= h:
            continue                      # height h reached after step (1)
        out.add(frozenset(d.items()))     # pass
        for x, v in list(d.items()):
            if v >= 2:
                e = dict(d)
                e[x] = v - 2
                if e[x] == 0: del e[x]
                e[x-1] = e.get(x-1, 0) + 1
                e[x+1] = e.get(x+1, 0) + 1
                if e[x-1] >= h or e[x+1] >= h:
                    continue              # height h reached after step (2)
                out.add(frozenset(e.items()))
    return out

def wins(lst, h):
    S = {frozenset()}
    for a in lst:
        S = step(S, a, h)
        if not S:
            return True
    return False

def survivor(lst, h):
    """Return a Firer line (list of 'P' or vertex fired) that survives, or None."""
    S = {frozenset(): []}
    for a in lst:
        T = {}
        for c, line in S.items():
            d = dict(c); d[a] = d.get(a, 0) + 1
            if d[a] >= h: continue
            T.setdefault(frozenset(d.items()), line + ['P'])
            for x, v in list(d.items()):
                if v >= 2:
                    e = dict(d); e[x] = v - 2
                    if e[x] == 0: del e[x]
                    e[x-1] = e.get(x-1, 0) + 1; e[x+1] = e.get(x+1, 0) + 1
                    if e[x-1] >= h or e[x+1] >= h: continue
                    T.setdefault(frozenset(e.items()), line + [x])
        S = T
        if not S: return None
    return next(iter(S.values()))

def C(h, cap=400):
    S = {frozenset()}
    for m in range(1, cap):
        S = step(S, 0, h)
        if not S: return m
    return None

def G_brute(h, m, R):
    """Is there a winning list of length m with a_1=0 and all a_i in [-R,R]?"""
    for rest in itertools.product(range(-R, R+1), repeat=m-1):
        lst = (0,) + rest
        if wins(lst, h): return lst
    return None

if __name__ == '__main__':
    cmd = sys.argv[1]
    if cmd == 'C':
        for h in range(1, int(sys.argv[2])+1):
            print('C(%d) =' % h, C(h), flush=True)
    elif cmd == 'G':
        h, m, R = map(int, sys.argv[2:5])
        print('h=%d m=%d R=%d ->' % (h, m, R), G_brute(h, m, R))
    elif cmd == 'wins':
        h = int(sys.argv[2]); lst = [int(x) for x in sys.argv[3:]]
        print(wins(lst, h), survivor(lst, h))
