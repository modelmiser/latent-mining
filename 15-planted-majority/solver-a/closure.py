#!/usr/bin/env python3
"""closure.py -- greedy completion of the recursive set S_e(M) (sets.py) to a closed set, by adding for a failing
pair (u,v) the element (u-v) - p with p the largest element below u-v (and for a failing split of u: u - p).
Heuristic only; the output set is re-verified by family.closed_info.  usage: closure.py e M [limit]"""
import sys
from family import closed_info
from sets import S
e, M = int(sys.argv[1]), int(sys.argv[2]); limit = int(sys.argv[3]) if len(sys.argv) > 3 else 400
s = set(S(e, M)); n0 = len(s)
while True:
    ok, info, _ = closed_info(sorted(s))
    if ok or len(s) > limit: break
    D = info[1] - info[2] if info[0] == 'net' else info[1]
    p = max(x for x in s if x < D); s.add(D - p)
print('e=%d M=%d start=%d final=%d closed=%s K=%d states=%d' % (e, M, n0, len(s), ok, max(s), 2 * len(s) + 2))
if ok: print(' '.join(map(str, sorted(s))))
