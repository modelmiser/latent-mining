#!/usr/bin/env python3
"""Single-entry mutants of a witness, judged by vcheck.  Also writes three named broken files."""
import copy
import sys
from collections import Counter

from vcheck import parse, check


def dump(n, b, auts, fn):
    with open(fn, 'w') as f:
        f.write('%d %d\n' % (n, b))
        for a in auts:
            f.write('%d\n' % len(a))
            for st in a:
                if st[0] == 'A':
                    f.write('A %d %d\n' % (st[1], st[2]))
                else:
                    f.write('R ' + ' '.join(map(str, st[1])) + '\n')


for fn in sys.argv[1:]:
    n, b, s, auts = parse(fn)
    assert check(n, b, s, auts)[0] == 'OK'
    c = Counter()
    for i in range(n):
        k = len(auts[i])
        for q in range(k):
            st = auts[i][q]
            if st is None:
                continue
            alts = []
            if st[0] == 'A':
                alts += [('A', x, st[2]) for x in range(b) if x != st[1]]
                alts += [('A', st[1], y) for y in range(k) if y != st[2]]
            else:
                for v in range(b):
                    for y in range(k):
                        if y != st[1][v]:
                            t = list(st[1]); t[v] = y
                            alts.append(('R', t))
            for alt in alts:
                m = copy.deepcopy(auts)
                m[i][q] = alt
                c[check(n, b, s, m)[0].split(':')[0]] += 1
    print(fn, dict(c))

# three deliberately broken versions of solver-b's headline witness
n, b, s, auts = parse('../solver-b/w_n3_b4_s7.txt')
m = copy.deepcopy(auts); m[2][4] = ('R', [5, 1, 1, 1]); dump(n, b, m, 'broken1_safety.txt')      # p3 ignores "higher value 3"
m = copy.deepcopy(auts); m[1][5] = ('A', 3, 5); dump(n, b, m, 'broken2_progress.txt')            # p2 never stops lowering
m = copy.deepcopy(auts); m[0][6] = ('R', [6, 1, 0, 6]); dump(n, b, m, 'broken3_illegal.txt')     # non-crit state jumps to rem
m = copy.deepcopy(auts); m[0][1] = ('A', 2, 0); dump(n, b, m, 'broken4_wrong_exit.txt')          # exit adds 2 instead of 3
for fn in ('broken1_safety.txt', 'broken2_progress.txt', 'broken3_illegal.txt', 'broken4_wrong_exit.txt'):
    print(fn, check(*parse(fn))[0])
