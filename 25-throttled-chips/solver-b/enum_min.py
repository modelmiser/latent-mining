#!/usr/bin/env python3
"""All winning lists of length m with a_1 = 0 and a_i in [-R,R] (slow direct implementation)."""
import sys, itertools
from slow import wins
h, m, R = map(int, sys.argv[1:4])
out = [(0,) + r for r in itertools.product(range(-R, R + 1), repeat=m - 1) if wins((0,) + r, h)]
print('h=%d m=%d R=%d: %d winning lists (a_1=0)' % (h, m, R, len(out)))
for l in out: print(' '.join(map(str, l)))
