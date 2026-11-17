#!/usr/bin/env python3
"""Third, independent implementation (referee). Written from the game definition only.
H(d): belief-set safety game.  R(d): reachability for each of the 27 relay maps.
Also exports a winning history strategy as an automaton (list of (u, {obs: next}))."""
import sys
from collections import deque

def sg(x): return (x > 0) - (x < 0)

# ---------- R(d) ----------
def relay_value(d, f, cap):
    # state at tick t: (signs of x_{t-d..t-1}, x_t, must_rest)
    start = ((0,) * d, 0, False)
    seen = {start}; dq = deque([start]); best = 0
    while dq:
        signs, x, rest = dq.popleft()
        u = f[signs[0]]
        for w in ((0,) if rest else (-1, 0, 1)):
            nx = x + u + w
            if abs(nx) > best: best = abs(nx)
            if abs(nx) >= cap: return None
            st = (signs[1:] + (sg(x),), nx, w != 0)
            if st not in seen: seen.add(st); dq.append(st)
    return best

def R(d):
    cap = 3 * d + 8
    res = {}
    for a in (-1, 0, 1):
        for b in (-1, 0, 1):
            for c in (-1, 0, 1):
                f = {-1: a, 0: b, 1: c}
                res[(a, b, c)] = relay_value(d, f, cap)
    bounded = {k: v for k, v in res.items() if v is not None}
    return min(bounded.values()), bounded

# ---------- H(d) ----------
def step(bel, u, d, B):
    """bel: frozenset of (signs of x_{t-d+1..t-1}, x_t, rest), observation o_t already consumed.
    Returns dict next_obs -> frozenset, or None if some play leaves [-B,B]."""
    parts = {}
    for signs, x, rest in bel:
        L = signs + (sg(x),)          # signs of x_{t-d+1..t}
        o = L[0]                      # = sgn(x_{t+1-d}), shown at tick t+1
        for w in ((0,) if rest else (-1, 0, 1)):
            nx = x + u + w
            if abs(nx) > B: return None
            parts.setdefault(o, set()).add((L[1:], nx, w != 0))
    return {o: frozenset(s) for o, s in parts.items()}

def solve_H(d, B, limit=3_000_000):
    init = frozenset([((0,) * (d - 1), 0, False)])
    ids = {init: 0}; bels = [init]; succ = []   # succ[n][u] = None or dict obs->id
    i = 0
    while i < len(bels):
        b = bels[i]; row = {}
        for u in (-1, 0, 1):
            p = step(b, u, d, B)
            if p is None: row[u] = None; continue
            m = {}
            for o, nb in p.items():
                j = ids.get(nb)
                if j is None:
                    j = len(bels); ids[nb] = j; bels.append(nb)
                    if j > limit: raise MemoryError("belief limit")
                m[o] = j
            row[u] = m
        succ.append(row); i += 1
    n = len(bels)
    dead = [False] * n
    preds = [[] for _ in range(n)]
    for a in range(n):
        for u in (-1, 0, 1):
            if succ[a][u]:
                for j in succ[a][u].values(): preds[j].append(a)
    def isdead(a):
        for u in (-1, 0, 1):
            m = succ[a][u]
            if m is not None and not any(dead[j] for j in m.values()): return False
        return True
    work = [a for a in range(n) if isdead(a)]
    for a in work: dead[a] = True
    while work:
        a = work.pop()
        for p in preds[a]:
            if not dead[p] and isdead(p): dead[p] = True; work.append(p)
    win = not dead[0]
    aut = None
    if win:
        # strategy automaton: node -> (u, {obs: node}); prefer u opposing nothing in particular: first safe u
        aut = {}
        stack = [0]
        while stack:
            a = stack.pop()
            if a in aut: continue
            for u in (0, -1, 1):
                m = succ[a][u]
                if m is not None and not any(dead[j] for j in m.values()):
                    aut[a] = (u, dict(m)); stack.extend(m.values()); break
    return win, n, aut

def H(d, Bmax=None):
    B = 0
    while True:
        win, n, aut = solve_H(d, B)
        print(f"  d={d} B={B} {'FEASIBLE' if win else 'INFEASIBLE'} beliefs={n}", flush=True)
        if win: return B, aut
        B += 1

if __name__ == "__main__":
    mode = sys.argv[1]
    if mode == "R":
        for d in range(1, int(sys.argv[2]) + 1):
            v, bounded = R(d)
            print(f"R({d}) = {v}   formula floor(3d/2)+1 = {3*d//2+1}   bounded maps (f-,f0,f+)->value: {bounded}")
    else:
        for d in range(1, int(sys.argv[2]) + 1):
            v, aut = H(d)
            print(f"H({d}) = {v}   2*floor(d/2)+1 = {2*(d//2)+1}   strategy nodes={len(aut)}")
