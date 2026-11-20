#!/usr/bin/env python3
"""redu.py -- empirical test of Lemma R: for random ORDERED protocols P (swaps and order-dependent rules allowed),
build the unordered swap-free P' of the lemma and check  maxN(P') >= maxN(P)  with the fast checker.
usage: redu.py seed count Nmax"""
import sys, random, subprocess
import slow
seed, count, Nmax = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
rnd = random.Random(seed)
base = [l[l.index('s='):].strip() for f in ('brief3.txt', 'fam_1_2_4.txt', 'budget1.txt', 'found.txt', 'fam_1_2.txt') for l in open(f) if 's=' in l and not l.startswith('#')]
def fmt(P):
    return 's=%d qA=%d qB=%d w=%s d: ' % (P['s'], P['qA'], P['qB'], ''.join(P['w'])) + ' '.join('%d,%d->%d,%d' % (k + v) for k, v in sorted(P['d'].items()))
def reduce(P):
    s = P['s']; d = P['d']; d2 = {}
    for p in range(s):
        for q in range(p, s):
            if p == q:
                if (p, p) in d: d2[(p, p)] = d[(p, p)]
                continue
            a = d.get((p, q), (p, q)); b = d.get((q, p), (q, p))
            if a not in ((p, q), (q, p)): o = a
            elif b not in ((q, p), (p, q)): o = (b[1], b[0])
            else: continue
            d2[(p, q)] = o; d2[(q, p)] = (o[1], o[0])
    return dict(P, d=d2)
A, B = [], []
for i in range(count):
    P = slow.parse(rnd.choice(base)); s = P['s']
    for _ in range(rnd.choice([1, 2, 3])):
        p, q = rnd.randrange(s), rnd.randrange(s)
        r = rnd.random()
        if r < .35: P['d'][(p, q)] = (q, p)                       # a swap
        elif r < .6: P['d'].pop((p, q), None)                     # one order becomes the identity
        elif r < .8 and (q, p) in P['d']: P['d'][(p, q)] = P['d'][(q, p)]
        else: P['d'][(p, q)] = (rnd.randrange(s), rnd.randrange(s))
    A.append(fmt(P)); B.append(fmt(reduce(P)))
def run(lines):
    out = subprocess.run(['./check', str(Nmax)], input='\n'.join(lines) + '\n', capture_output=True, text=True).stdout.strip().split('\n')
    return [int(o.split('N=')[1].split()[0]) for o in out]
ra, rb = run(A), run(B)
bad = [(x, y, l) for x, y, l in zip(ra, rb, A) if y < x]
print('protocols %d ; reduced never worse: %s ; original wins>=2: %d ; strictly improved by reduction: %d' % (count, not bad, sum(x >= 2 for x in ra), sum(y > x for x, y in zip(ra, rb))))
for b in bad[:5]: print('VIOLATION', b)
