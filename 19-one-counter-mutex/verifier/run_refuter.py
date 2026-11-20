#!/usr/bin/env python3
"""Drive refuter.py over many triples of automata; every schedule is judged by refuter.validate
(and a sample also by vcheck's full model checker saying 'not a solution')."""
import itertools
import random
import sys
from collections import Counter

from refuter import Gap, refute, validate, negate

random.seed(int(sys.argv[1]) if len(sys.argv) > 1 else 1)
SCALE = float(sys.argv[2]) if len(sys.argv) > 2 else 1.0


def rand_aut(s, b, padd=0.5):
    a = []
    for q in range(s):
        if q == 1:
            a.append(('A', random.randrange(b), 0))
        elif random.random() < padd:
            a.append(('A', random.randrange(1, b), random.randrange(1, s)))
        else:
            a.append(('R', [random.randrange(1, s) for _ in range(b)]))
    return a


def all_auts(s, b):
    opts = [('R', list(t)) for t in itertools.product(range(1, s), repeat=b)]
    opts += [('A', c, q) for c in range(1, b) for q in range(1, s)]
    for combo in itertools.product(opts, repeat=s - 1):
        for c in range(b):
            a = [combo[0], ('A', c, 0)] + list(combo[1:])
            yield a


tally = {}
gaps = []
nrun = 0


def run(auts, b, label):
    global nrun
    for flav in 'AB':
        tags = []
        nrun += 1
        try:
            v = refute(auts, b, flav, tags)
            ok = validate(auts, b, v)
        except Gap as g:
            ok, tags = False, tags + ['GAP: %s' % g]
        except Exception as ex:  # a crash of the builder is also a gap in the mechanisation
            ok, tags = False, tags + ['CRASH: %r' % ex]
        key = (label, flav, ' > '.join(tags), ok)
        tally[key] = tally.get(key, 0) + 1
        if not ok and len(gaps) < 20:
            gaps.append((label, flav, tags, auts))
    return tags


def deep(a, b):
    tags = []
    try:
        refute([a, a, a], b, 'A', tags)
    except Exception:
        return None
    t = tags[-1]
    if t.startswith('6') or t.startswith('7'):
        return ('neg' in tags, t[0])
    return None


# ---------- b = 2
A2 = list(all_auts(3, 2))
for _ in range(int(20000 * SCALE)):
    run([random.choice(A2) for _ in range(3)], 2, 'b2 s3 uniform')
for _ in range(int(20000 * SCALE)):
    run([rand_aut(random.randint(2, 6), 2) for _ in range(3)], 2, 'b2 s<=6 random')

# ---------- b = 3
A3 = list(all_auts(3, 3))
for _ in range(int(20000 * SCALE)):
    run([random.choice(A3) for _ in range(3)], 3, 'b3 s3 uniform')
for _ in range(int(20000 * SCALE)):
    run([rand_aut(random.randint(2, 7), 3) for _ in range(3)], 3, 'b3 s<=7 random')

# pools of automata that survive every single-process claim (flag-like), s <= 4 exhaustive
pool = {'H': [], 'I': []}
npool = 0
basic = []
for s in (2, 3, 4):
    for a in all_auts(s, 3):
        d = deep(a, 3)
        if d:
            npool += 1
            x = negate([a], 3)[0] if d[0] else a
            pool['H' if d[1] == '6' else 'I'].append(x)
# random larger flag-like automata
big = {'H': [], 'I': []}
tries = 0
while tries < int(400000 * SCALE):
    tries += 1
    a = rand_aut(random.randint(5, 8), 3, padd=0.45)
    d = deep(a, 3)
    if d:
        big['H' if d[1] == '6' else 'I'].append(negate([a], 3)[0] if d[0] else a)
print('flag-like automata: s<=4 exhaustive: hold-type %d, infinite-type %d; random s 5..8: hold %d, infinite %d'
      % (len(pool['H']), len(pool['I']), len(big['H']), len(big['I'])))
allflag = pool['H'] + pool['I'] + big['H'] + big['I']
for _ in range(int(60000 * SCALE)):
    run([random.choice(allflag) for _ in range(3)], 3, 'b3 flag-like triple')
for kind in ('HHH', 'HHI', 'HII', 'III'):
    for _ in range(int(8000 * SCALE)):
        tr = [random.choice(pool[c] + big[c]) if random.random() < 0.5 or not big[c] else random.choice(big[c]) for c in kind]
        random.shuffle(tr)
        run(tr, 3, 'b3 flag-like ' + kind)
# two flag-like + one arbitrary (exercises each forcing claim against good partners)
for _ in range(int(60000 * SCALE)):
    tr = [random.choice(allflag), random.choice(allflag), rand_aut(random.randint(2, 7), 3)]
    random.shuffle(tr)
    run(tr, 3, 'b3 2 flag-like + 1 random')
# two flag-like + one single-entry mutant of a flag-like automaton
for _ in range(int(60000 * SCALE)):
    m = [x if x[0] == 'A' else ('R', list(x[1])) for x in random.choice(allflag)]
    q = random.choice([x for x in range(len(m)) if x != 1])
    if random.random() < 0.15:
        m[1] = ('A', random.randrange(3), 0)
    elif random.random() < 0.5:
        m[q] = ('A', random.randrange(1, 3), random.randrange(1, len(m)))
    else:
        old = m[q][1] if m[q][0] == 'R' else [m[q][2]] * 3
        new = list(old); new[random.randrange(3)] = random.randrange(1, len(m))
        m[q] = ('R', new)
    tr = [random.choice(allflag), random.choice(allflag), m]
    random.shuffle(tr)
    run(tr, 3, 'b3 2 flag-like + 1 mutant')
# mixtures with negated automata
for _ in range(int(20000 * SCALE)):
    tr = [random.choice(allflag) for _ in range(3)]
    x = random.randrange(3)
    tr[x] = negate([tr[x]], 3)[0]
    run(tr, 3, 'b3 flag-like, one negated')

# the judge must be able to say no: damage accepted schedules
killed = tried = 0
for _ in range(int(3000 * SCALE)):
    tr = [random.choice(allflag) for _ in range(3)]
    v = refute(tr, 3, 'A', [])
    assert validate(tr, 3, v)
    if v[0] == 'SAFETY':
        continue
    pre, cyc = list(v[1]), list(v[2])
    x = random.randrange(len(cyc))
    del cyc[x]
    tried += 1
    killed += not validate(tr, 3, ('PROGRESS', pre, cyc))
print('judge self-test: %d damaged lassos, %d rejected' % (tried, killed))
print('refutations attempted:', nrun)
bad = sum(c for k, c in tally.items() if not k[3])
print('schedules rejected by the judge or builder stuck:', bad)
agg = Counter()
for (label, flav, tags, ok), c in tally.items():
    agg[(flav, tags.split(' > ')[-1] if ok else 'FAIL ' + tags, ok)] += c
for k in sorted(agg):
    print('  %s  %-45s %s %d' % (k[0], k[1], 'ok' if k[2] else 'FAIL', agg[k]))
for g in gaps[:8]:
    print('GAP EXAMPLE', g)
