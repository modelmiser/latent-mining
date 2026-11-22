#!/usr/bin/env python3
"""k=5 table, the 'bin+1' damage on a random sample of 400 offer lines: classify each accepted
case as 'same move' (an unnamed bin renamed) or 'genuinely different move'."""
import subprocess, sys, random
L, n, path = '5', '21', 'adv_k5_n21.txt'
lines = open(path).read().split('\n')[:-1]
random.seed(7)
offers = [i for i, l in enumerate(lines) if not l.endswith('|E')]
same = other = rej = 0
for i in random.sample(offers, 400):
    st, mv = lines[i].split('|')
    a, b = map(int, mv.split()[1:])
    m = 1 + max([int(x) for t in st.split() for x in t.split('.')], default=-1)
    out = lines[:i] + [st + '|O %d %d' % (a, b + 1)] + lines[i+1:]
    open('dt5.txt', 'w').write('\n'.join(out) + '\n')
    r = subprocess.run([sys.executable, 'check_adv.py', L, n, 'dt5.txt'], capture_output=True, text=True)
    if r.stdout.startswith('ACCEPT'):
        if b >= m and a != b + 1: same += 1
        else: other += 1; print('accepted, genuinely different move:', lines[i], '->', b + 1)
    else: rej += 1
print('k=5 sample of 400 offers: rejected %d; accepted as the same move (unnamed bin renamed) %d; accepted with a different move %d' % (rej, same, other))
