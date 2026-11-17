#!/usr/bin/env python3
"""Run gen.py's strategy through check.py's definition-level simulator for a range of (n,k);
report first completion vs floor(n - n/k) and the Theorem-U upper bound. usage: sweep.py nmax kmax"""
import sys, os, tempfile
from gen import strategy
from check import check
from ub import UB
nmax = int(sys.argv[1]); kmax = int(sys.argv[2])
bad = 0; cnt = 0; exact = 0
d = os.path.dirname(os.path.abspath(__file__))
for n in range(3, nmax + 1):
    for k in range(1, kmax + 1):
        T, rounds = strategy(n, k)
        p = os.path.join(d, 'tmp_sweep.txt')
        with open(p, 'w') as f:
            f.write('%d %d\n' % (n, k))
            for t, D in enumerate(rounds, 1):
                f.write('%d: %s\n' % (t, ' '.join('%d>%d' % l for l in D)))
        _, _, status, first = check(p)
        m = min(k, n)
        claim = n - (-(-n // m))
        cnt += 1
        if status != 'LEGAL' or first < claim:
            bad += 1
            print('FAIL', n, k, status, first, claim)
        if max(first, n // 2) == UB(n, k):
            exact += 1
print('checked', cnt, 'pairs; failures', bad, '; pairs where witness meets Theorem-U bound:', exact)
