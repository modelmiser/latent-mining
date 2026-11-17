#!/usr/bin/env python3
"""Direct implementation of the definition. No state abstraction, no memo:
recursion over full histories (list of (S, answer)).  Tiny n only.
win(n,k,q): can questioner force |C_t|=1 for some t<=q."""
import sys
from itertools import combinations

def alive_set(n, k, hist):
    """hist: list of (S frozenset, ans bool). Returns list C_0..C_t following the definition."""
    Cs = [set(range(1, n+1))]
    ells = []
    for t in range(1, len(hist)+1):
        S, _ = hist[t-1]
        Cprev = Cs[-1]
        ells.append(min(len(S & Cprev), len(Cprev - S)))
        C = set()
        for x in range(1, n+1):
            # L is forced by (i)
            L = [s for s in range(1, t+1) if (x in hist[s-1][0]) != hist[s-1][1]]
            ok = True
            for s in L:
                lo, hi = s+1, s + k*ells[s-1]
                if any(lo <= u <= hi for u in L):
                    ok = False; break
            if ok: C.add(x)
        Cs.append(C)
    return Cs

def win(n, k, q, hist=()):
    Cs = alive_set(n, k, list(hist))
    C = Cs[-1]
    if len(C) == 1: return True
    if q == 0: return False
    U = list(range(1, n+1))
    for m in range(0, n+1):
        for S in combinations(U, m):
            S = frozenset(S)
            good = True
            for ans in (True, False):
                h2 = hist + ((S, ans),)
                C2 = alive_set(n, k, list(h2))[-1]
                if len(C2) == 0: continue  # responder may not do this
                if not win(n, k, q-1, h2):
                    good = False; break
            if good: return True
    return False

if __name__ == "__main__":
    n = int(sys.argv[1]); k = int(sys.argv[2]); qmax = int(sys.argv[3])
    for q in range(0, qmax+1):
        w = win(n, k, q)
        print(f"n={n} k={k} q={q} win={w}", flush=True)
        if w: break
