#!/usr/bin/env python3
"""sets2.py -- sets of numbers with at most t non-zero signed binary digits:
B_t(L) = { positive x <= 2^L : x = sum of at most t terms +-2^i, 0<=i<=L }.  Tests closedness.  usage: sets2.py t Lmax"""
import sys, itertools
from family import closed_info
def B(t, L):
    cur = {0}
    for _ in range(t):
        cur = cur | {x + sg * (1 << i) for x in cur for i in range(L + 1) for sg in (1, -1)}
    return sorted(x for x in cur if 0 < x <= (1 << L))
if __name__ == '__main__':
    t, Lmax = int(sys.argv[1]), int(sys.argv[2])
    for L in range(1, Lmax + 1):
        s = B(t, L); ok, info, _ = closed_info(s)
        print('t=%d L=%d |S|=%d K=%d states=%d closed=%s %s' % (t, L, len(s), s[-1], 2 * len(s) + 2, ok, '' if ok else info))
