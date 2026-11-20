#!/usr/bin/env python3
"""closed.py -- K(m) = largest max of a 'closed' set S of m positive integers (definition as in family.py):
 (split) every u>=2 in S is p+q with p,q in S;
 (net)   for u>v in S: u-v in S, or u-v=p+q with p,q in S, or exists 0<c<v with u-c, v-c in S.
Both conditions for the largest element only mention smaller elements, so S is closed iff every prefix (in increasing
order) is closed and the new maximum passes against the prefix: exhaustive DFS by adding elements in increasing order.
usage: closed.py mmax"""
import sys
def okmax(S, Ss, sums, t):
    # S sorted ascending without t; test t as new max
    if t not in sums: return False
    for v in S:
        D = t - v
        if D in Ss or D in sums: continue
        if any((t - c) in Ss and (v - c) in Ss for c in range(1, v)): continue
        return False
    return True
best = {}
def dfs(S, mmax):
    m = len(S); K = S[-1]
    if K > best.get(m, (0,))[0]: best[m] = (K, list(S))
    if m == mmax: return
    Ss = set(S); sums = {a + b for a in S for b in S}
    for t in range(K + 1, 2 * K + 1):
        if okmax(S, Ss, sums, t): dfs(S + [t], mmax)
mmax = int(sys.argv[1]); dfs([1], mmax)
for m in sorted(best): print(m, best[m][0], best[m][1])
