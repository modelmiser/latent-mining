#!/usr/bin/env python3
"""Check a witness in the 'Q <config> <EMPTY|0|1>' / 'E <config> <op> <config>'
format (unlisted E = identity, unlisted Q = EMPTY) with checker.check.
usage: check_qe.py file [t]     (any unparsable line is an error)"""
import sys
from checker import check, count_legal

n = t = None
Qt, Et = {}, {}
OPS = {'PUSH0': 'P0', 'PUSH1': 'P1', 'POP': 'POP'}
for line in open(sys.argv[1]):
    f = line.split('#')[0].split()
    if not f:
        continue
    if f[0] == 'n':
        n = int(f[1])
    elif f[0] == 't':
        t = int(f[1])
    elif f[0] == 'init':
        assert f[1] == '-', 'only the empty initial stack is accepted'
    elif f[0] == 'Q' and len(f) == 3:
        assert len(f[1]) == n and f[1] not in Qt
        Qt[int(f[1][::-1], 2)] = {'EMPTY': 'E', '0': 0, '1': 1}[f[2]]
    elif f[0] == 'E' and len(f) == 4:
        assert len(f[1]) == n and len(f[3]) == n
        k = (int(f[1][::-1], 2), OPS[f[2]])
        assert k not in Et
        Et[k] = int(f[3][::-1], 2)
    else:
        raise ValueError('bad line: ' + line)
if len(sys.argv) > 2:
    t = int(sys.argv[2])
ok, info = check(n, t, lambda c: Qt.get(c, 'E'), lambda c, op: Et.get((c, op), c))
print('%s n=%d t=%d %s %s (legal sequences by formula: %d)' % (
    sys.argv[1], n, t, 'SURVIVES' if ok else 'FAILS', info, count_legal(t)))
