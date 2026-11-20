#!/usr/bin/env python3
"""sets.py -- recursive sets S_e(M):  S_1 = {1..M};  S_{e+1} = S_e  U  {M^e - x : x in S_e, x < M^e}  U  {c*M^e : 1<=c<=M}.
Tests closedness (family.closed_info) and prints |S|, K=max S, number of protocol states 2|S|+2.
usage: sets.py dmax Mmax"""
import sys
from family import closed_info
def S(e, M):
    if e == 1: return set(range(1, M + 1))
    T = S(e - 1, M); P = M ** (e - 1)
    return T | {P - x for x in T if x < P} | {c * P for c in range(1, M + 1)}
if __name__ == '__main__':
    dmax, Mmax = int(sys.argv[1]), int(sys.argv[2])
    for e in range(1, dmax + 1):
        for M in range(2, Mmax + 1):
            s = sorted(S(e, M))
            if len(s) > 700: continue
            ok, info, _ = closed_info(s)
            print('e=%d M=%d |S|=%d K=%d states=%d closed=%s %s' % (e, M, len(s), s[-1], 2 * len(s) + 2, ok, '' if ok else info))
