#!/usr/bin/env python3
"""Pair-type abstract game (single witness per tree).  A tree is a pair (p,q), -1<=p<=q: it has a vertex x whose
two out-slots have values >=p and >=q (slot value = h(out-neighbour), -1 for a hole).  Start: n trees (-1,-1).
Adversary picks two trees t=(p,q), t'=(p',q').  Algorithm options (each a guaranteed lower bound on cost and types):
  at x via slot 1: cost p+1, x -> sort(q, p'+1);   at x via slot 2: cost q+1, x -> sort(p, p'+1)
  at y via slot 1: cost p'+1, y -> sort(q', p+1);  at y via slot 2: cost q'+1, y -> sort(p', p+1)
After a reply at x the Adversary keeps either x's new pair or y's old pair as the tree's witness (and vice versa).
V_pair(n) <= T(n)."""
import sys
from functools import lru_cache
sys.setrecursionlimit(1000000)
CAP = int(sys.argv[3]) if len(sys.argv) > 3 else 9
def srt(a, b):
    a = min(a, CAP); b = min(b, CAP)
    return (a, b) if a <= b else (b, a)
@lru_cache(maxsize=None)
def V(state):
    if len(state) <= 1: return 0
    best = 0
    L = len(state); seen = set()
    for a in range(L):
        for b in range(a + 1, L):
            t = state[a]; u = state[b]
            if (t, u) in seen: continue
            seen.add((t, u))
            rest = state[:a] + state[a + 1:b] + state[b + 1:]
            (p, q), (pp, qq) = t, u
            opts = [(p + 1, srt(q, pp + 1), u), (q + 1, srt(p, pp + 1), u),
                    (pp + 1, srt(qq, p + 1), t), (qq + 1, srt(pp, p + 1), t)]
            worst = None
            for (c, nt, keep) in set(opts):
                v = c + max(V(tuple(sorted(rest + (nt,)))), V(tuple(sorted(rest + (keep,)))))
                if worst is None or v < worst:
                    worst = v
                    if worst <= best: break
            if worst > best: best = worst
    return best
lo = int(sys.argv[1]); hi = int(sys.argv[2])
for n in range(lo, hi + 1):
    v = V(tuple([(-1, -1)] * n)); B = max(0, n // 4 - 1)
    print(n, v, 'B=%d' % B, '>B' if v > B else '', 'states', V.cache_info().currsize, flush=True)
