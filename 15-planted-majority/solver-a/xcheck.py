#!/usr/bin/env python3
"""xcheck.py -- compare the fast checker (./check) with slow.py on random and mutated protocols.
usage: xcheck.py seed count Nmax ; writes xcheck_protocols.txt, prints agreement statistics."""
import sys, random, subprocess
import slow
seed, count, Nmax = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
rnd = random.Random(seed)
base = [l.strip() for f in ('brief3.txt', 'fam_1_2_4.txt', 'budget1.txt', 'found.txt') for l in open(f) if l.startswith('s=') or 'FOUND' in l]
base = [l[l.index('s='):] for l in base]
def fmt(P):
    return 's=%d qA=%d qB=%d w=%s d: ' % (P['s'], P['qA'], P['qB'], ''.join(P['w'])) + ' '.join('%d,%d->%d,%d' % (k + v) for k, v in sorted(P['d'].items()))
lines = []
for i in range(count):
    if i % 3 == 0:   # fully random small protocol
        s = rnd.choice([2, 3, 3, 4])
        P = {'s': s, 'qA': 0, 'qB': 1, 'w': [rnd.choice('AB') for _ in range(s)], 'd': {}}
        if rnd.random() < .7: P['w'][0] = 'A'; P['w'][1] = 'B'
        for p in range(s):
            for q in range(s):
                if rnd.random() < .6: P['d'][(p, q)] = (rnd.randrange(s), rnd.randrange(s))
    else:            # mutate a known protocol in 1-2 entries
        P = slow.parse(rnd.choice(base)); s = P['s']
        for _ in range(rnd.choice([0, 1, 1, 2])):
            P['d'][(rnd.randrange(s), rnd.randrange(s))] = (rnd.randrange(s), rnd.randrange(s))
        if rnd.random() < .1: q = rnd.randrange(s); P['w'][q] = 'A' if P['w'][q] == 'B' else 'B'
    lines.append(fmt(P))
open('xcheck_protocols.txt', 'w').write('\n'.join(lines) + '\n')
out = subprocess.run(['./check', str(Nmax)], stdin=open('xcheck_protocols.txt'), capture_output=True, text=True).stdout.strip().split('\n')
assert len(out) == len(lines)
agree = 0; hist = {}
for l, o in zip(lines, out):
    fast = int(o.split('N=')[1].split()[0])
    P = slow.parse(l); sl = 1
    for n in range(2, Nmax + 1):
        if slow.wins_upto(P, n)[0]: sl = n
        else: break
    hist[sl] = hist.get(sl, 0) + 1
    if fast == sl: agree += 1
    else: print('DISAGREE', fast, sl, l)
print('protocols %d agree %d ; histogram of max N (slow): %s' % (len(lines), agree, sorted(hist.items())))
