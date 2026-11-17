#!/usr/bin/env python3
"""Mid-level: labelled state = tuple over x of remaining obligation (or -1 dead); all subsets S of U; memo.
Unclipped obligations, so a fixed point W_q == W_{q+1} proves infinity."""
import sys
from functools import lru_cache

def solve(n, k, qmax):
    full = (1 << n) - 1
    @lru_cache(maxsize=None)
    def win(st, q):
        alive = [i for i in range(n) if st[i] >= 0]
        if len(alive) == 1: return True
        if q == 0: return False
        for S in range(full + 1):
            a = sum(1 for i in alive if S >> i & 1); b = len(alive) - a
            ell = min(a, b)
            good = True
            for ans in (1, 0):
                new = []
                for i in range(n):
                    r = st[i]
                    if r < 0: new.append(-1); continue
                    truthful = ((S >> i) & 1) == ans
                    if truthful: new.append(max(r-1, 0))
                    elif r == 0: new.append(k*ell)
                    else: new.append(-1)
                new = tuple(new)
                if all(v < 0 for v in new): continue
                if not win(new, q-1): good = False; break
            if good: return True
        return False
    st0 = tuple([0]*n)
    for q in range(qmax+1):
        if win(st0, q): return q
    return None

def u1(n, qmax):
    # brute force one-lie Ulam game on (a,b)
    @lru_cache(maxsize=None)
    def win(a, b, q):
        if a + b <= 1: return True
        if q == 0: return False
        if a*(q+1) + b > 2**q: return False  # Berlekamp volume bound (necessary)
        for i in range(a+1):
            for j in range(b+1):
                # yes: (i, j + a-i) ; no: (a-i, b-j + i)
                o1 = (i, j + a - i); o2 = (a - i, b - j + i)
                if (o1[0]+o1[1] == 0 or win(o1[0], o1[1], q-1)) and (o2[0]+o2[1] == 0 or win(o2[0], o2[1], q-1)):
                    return True
        return False
    for q in range(qmax+1):
        if win(n, 0, q): return q
    return None

def pelc(n):
    q = 0
    while True:
        if n % 2 == 0 and n*(q+1) <= 2**q: return q
        if n % 2 == 1 and n*(q+1) + (q-1) <= 2**q: return q
        q += 1

if __name__ == "__main__":
    k = int(sys.argv[1]); nmax = int(sys.argv[2]); qmax = int(sys.argv[3])
    for n in range(2, nmax+1):
        print(f"n={n} k={k} Q={solve(n,k,qmax)} (None: > {qmax})  U1_bruteforce={u1(n,40)} pelc={pelc(n)}", flush=True)
