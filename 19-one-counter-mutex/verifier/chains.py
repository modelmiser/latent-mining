#!/usr/bin/env python3
"""Hand-built flag-like automata with long chains (m yield levels then hold, or a cycle of m yield
levels), to exercise Steps 5-7 / Steps 3-5 of the b=3 proofs at depth."""
import random, sys
from collections import Counter
from refuter import refute, validate, Gap, negate
random.seed(3)
def chain_aut(m, hold, mirror_wait):
    # 0 rem, 1 crit, 2 wait-before-entry, then per level t: P,W,L,D
    a = [None, ('A', 2, 0), None]
    base = lambda t: 3 + 4 * t
    nlev = m + (1 if hold else 0)
    a[0] = ('R', [base(0), 2, 2]); a[2] = ('R', [base(0), 2, 2])
    for t in range(nlev):
        P, W, L, D = base(t), base(t) + 1, base(t) + 2, base(t) + 3
        last_hold = hold and t == nlev - 1
        nxt = base((t + 1) % nlev) if not last_hold else P
        junk = random.choice([W, D]) if mirror_wait else W
        a += [('A', 1, W), ('R', [junk, 1, W if last_hold else L]), ('A', 2, D), ('R', [nxt, D, D])]
    return a
c = Counter(); bad = 0
for _ in range(4000):
    tr = [chain_aut(random.randint(0 if h else 1, 6), h, random.random() < .5) for h in [random.random() < .5 for _ in range(3)]]
    if random.random() < .2: tr = negate(tr, 3)
    for flav in 'AB':
        tags = []
        try:
            v = refute(tr, 3, flav, tags); ok = validate(tr, 3, v)
        except Gap as g:
            ok = False; tags.append('GAP %s' % g)
        bad += not ok
        t = tags[-1]
        if t.startswith('6'):
            t = '6 hold/hold'
        c[(flav, t, ok, len(v[2]) > 40 if ok and v[0] == 'PROGRESS' else None)] += 1
print('failures', bad)
for k in sorted(c, key=str): print(k, c[k])
