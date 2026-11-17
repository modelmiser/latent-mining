#!/usr/bin/env python3
"""Scan stage-1 layouts (run-length lists, each run followed by a 3-gap) with the periodic game solver,
blocker family (FILTER=2).  Output: value (slope dW/dM; 2/3 = held at 5M/3), span/M."""
import itertools, subprocess, sys, os
seen = set(); cands = []
for k in range(1, 5):
    for runs in itertools.product(range(2, 10), repeat=k):
        # canonical rotation, skip periodic repeats
        rots = [runs[i:] + runs[:i] for i in range(k)]
        if runs != min(rots): continue
        if any(k % q == 0 and runs == runs[:q] * (k // q) for q in range(1, k)): continue
        n = sum(runs); t = 1
        while (t * n) % 3: t += 1
        n1 = t * n
        if n1 > 30: continue
        span = (n + 3 * k) / n
        if span > 1.70: continue          # must fit in ~5M/3
        cands.append((runs, n1, span))
print(len(cands), 'candidates', flush=True)
env = dict(os.environ, FILTER='2', PRUNE='1')
res = []
for runs, n1, span in cands:
    h = ','.join(map(str, runs))
    try:
        out = subprocess.run(['timeout', '240', 'nice', '-n', '15', './pergame2', h, '4', '8', '3', '', '1'],
                             capture_output=True, text=True, env=env).stdout
        val = [l for l in out.splitlines() if '==>' in l]
        v = float(val[0].split('slope=')[1].split()[0]) if val else None
    except Exception as e:
        v = None
    res.append((v, h, n1, span))
    print(f"home={h:12s} n1={n1:2d} span/M={span:.3f} value={v}", flush=True)
ok = sorted(r for r in res if r[0] is not None)
print('\nLOWEST VALUES (best layouts for the manager against the blocker family):')
for v, h, n1, span in ok[:15]:
    print(f"  value={v:.4f}  (H/M -> {1 + v:.4f})  home={h}  n1={n1} span/M={span:.3f}")
print('timeouts:', [r[1] for r in res if r[0] is None])
