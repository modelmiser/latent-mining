#!/usr/bin/env python3
"""Damage test for check_adv.py: usage damage.py L n table sample_or_0
For each (sampled) line: (a) delete it, (b) swap its move E <-> an offer / replace offer by E,
(c) change the second bin of an offer by +1.  Runs the checker on each damaged table."""
import sys, subprocess, random, os
L, n, path, k = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4])
lines = open(path).read().split('\n')[:-1]
random.seed(1)
idx = list(range(len(lines))) if k == 0 else random.sample(range(len(lines)), k)
res = {}
tmp = 'damaged_tmp_%s_%s.txt' % (L, n)
for i in idx:
    st, mv = lines[i].split('|')
    variants = {'delete': None}
    if mv == 'E': variants['E->O 0 1'] = st + '|O 0 1'
    else:
        a, b = mv.split()[1:]
        variants['O->E'] = st + '|E'
        variants['bin+1'] = st + '|O %s %d' % (a, int(b) + 1)
    for name, new in variants.items():
        out = lines[:i] + ([new] if new else []) + lines[i+1:]
        open(tmp, 'w').write('\n'.join(out) + '\n')
        r = subprocess.run([sys.executable, 'check_adv.py', L, n, tmp], capture_output=True, text=True)
        verdict = 'ACCEPT' if r.stdout.startswith('ACCEPT') else 'REJECT'
        res.setdefault((name, verdict), []).append(i)
os.remove(tmp)
for key in sorted(res):
    print(key, len(res[key]), 'e.g. line', res[key][0] + 1)
