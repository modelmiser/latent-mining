# (iii) pair-type game of Theorem 2, referee's own DP (no pruning: both Adversary successor choices explored).
# usage: t3_pairdp.py NMAX VARIANT   (true | fcost: slot-1 cost p+2 | fkeep: Adversary may not keep the other pair... still valid lower bd)
import sys
from functools import lru_cache
sys.setrecursionlimit(1000000)
NMAX = int(sys.argv[1]); variant = sys.argv[2]
def srt(a, b): return (a, b) if a <= b else (b, a)
def options(t, u):
    (p, q), (pp, qq) = t, u
    c1 = p + 2 if variant == 'fcost' else p + 1
    c3 = pp + 2 if variant == 'fcost' else pp + 1
    return [(c1, srt(q, pp + 1), u), (q + 1, srt(p, pp + 1), u),
            (c3, srt(qq, p + 1), t), (qq + 1, srt(pp, p + 1), t)]
@lru_cache(maxsize=None)
def V(S):
    if len(S) == 1: return 0
    types = sorted(set(S)); best = 0
    for a in range(len(types)):
        for b in range(a, len(types)):
            t, u = types[a], types[b]
            if t == u and S.count(t) < 2: continue
            rest = list(S); rest.remove(t); rest.remove(u)
            val = None
            for (c, new, kept) in options(t, u):
                w = c + max(V(tuple(sorted(rest + [new]))), V(tuple(sorted(rest + [kept]))))
                if val is None or w < val: val = w
                if val <= best: break
            if val > best: best = val
    return best
Texact = {n: 0 for n in range(1, 8)}; Texact.update({n: 1 for n in range(8, 12)}); Texact.update({n: 2 for n in range(12, 16)})
author = {}
for n in range(1, 30): author[n] = max(0, n // 4 - 1)
for n, v in zip(range(30, 49), [7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,12,12,13]): author[n] = v
for n, v in zip(range(49, 67), [13,13,13,14,14,14,14,15,15,15,15,16,16,17,17,18,18,18]): author[n] = v
bad = 0
for n in range(1, NMAX + 1):
    v = V(tuple([(-1, -1)] * n))
    flag = ''
    if v != author[n]: flag += ' DIFFERS-FROM-AUTHOR(%d)' % author[n]; bad += 1
    if n in Texact and v > Texact[n]: flag += ' EXCEEDS-EXACT-T(%d)' % Texact[n]; bad += 1
    print(n, v, 'B=%d' % max(0, n // 4 - 1), 'states', V.cache_info().currsize, flag, flush=True)
print('VARIANT', variant, 'mismatches', bad)
