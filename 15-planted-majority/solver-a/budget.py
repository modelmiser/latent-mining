#!/usr/bin/env python3
"""budget.py K -- the problem statement's "budget" protocol with weights -K..-1, 1..K and wa, wb (s = 2K+2)."""
import sys
K = int(sys.argv[1])
names = [K, -K] + [sg * i for i in range(K - 1, 0, -1) for sg in (1, -1)] + ['wa', 'wb']
idx = {n: i for i, n in enumerate(names)}
def st(v): return v if v not in (0,) else None
d = {}
def put(x, y, x2, y2):
    d[(idx[x], idx[y])] = (idx[x2], idx[y2]); d[(idx[y], idx[x])] = (idx[y2], idx[x2])
for i in range(1, K + 1):
    for j in range(1, K + 1):
        if i == j: put(i, -j, 'wa', 'wb')
        elif i > j: put(i, -j, i - j, 'wa')
        else: put(i, -j, 'wa', -(j - i))
    put(i, 'wb', (i - 1) if i > 1 else 'wa', 'wa')
    put(-i, 'wa', -(i - 1) if i > 1 else 'wb', 'wb')
w = ''.join('A' if (n == 'wa' or (n != 'wb' and n > 0)) else 'B' for n in names)
print('s=%d qA=0 qB=1 w=%s d: ' % (len(names), w) + ' '.join('%d,%d->%d,%d' % (k + v) for k, v in sorted(d.items())))
