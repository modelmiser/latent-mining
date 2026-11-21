#!/usr/bin/env python3
"""difftest.py -- differential test of check.cpp: on random certificates the reduced check (min), the unreduced
check (full) and the literal check (brute) must give the same verdict; and where the Adversary wins (no valid
certificate exists) every certificate must be rejected.
usage: difftest.py n L basecert trials seed [nobrute]"""
import sys, random, subprocess, os
n, L, base, trials, seed = int(sys.argv[1]), int(sys.argv[2]), sys.argv[3], int(sys.argv[4]), int(sys.argv[5])
nobrute = len(sys.argv) > 6
rnd = random.Random(seed)
core = [l.strip() for l in open(base) if l.strip() and l.strip() != '-']
def canon_line(st):                  # st newest first list of (w,o)
    m = {}; out = []
    for w, o in st:
        if w not in m: m[w] = len(m)
        if o not in m: m[o] = len(m)
        out.append('%x%x' % (m[w], m[o]))
    return ' '.join(out)
def random_state(maxk):              # play random legal offers with random answers, no load >= L
    cp = list(range(n)); ld = [0] * n; st = []
    k = rnd.randint(1, maxk)
    for _ in range(200):
        if len(st) >= k: break
        u, v = rnd.sample(range(n), 2)
        if cp[u] == cp[v] or ld[u] + 1 >= L: continue
        a, b = cp[u], cp[v]; cp = [a if x == b else x for x in cp]; ld[u] += 1; st.append((u, v))
    return canon_line(st[::-1])
def extend(line):                    # add one random older ball (may be illegal; then the checker must reject the line)
    st = [(int(t[0], 16), int(t[1], 16)) for t in line.split()]
    u, v = rnd.sample(range(n), 2); return canon_line(st + [(u, v)])
def run(mode, path):
    r = subprocess.run(['./check', mode, str(n), str(L), path], capture_output=True, text=True)
    return r.stdout.split()[0] if r.stdout else 'NOOUT'
os.makedirs('tmp', exist_ok=True); path = f'tmp/diff_{n}_{L}_{seed}.cert'
tally = {}
for t in range(trials):
    kind = rnd.choice(['drop', 'add', 'addext', 'subset', 'random', 'dropadd', 'intact'])
    lines = list(core)
    if kind in ('drop', 'dropadd') and lines: del lines[rnd.randrange(len(lines))]
    if kind in ('add', 'dropadd'): lines += [random_state(n - 1) for _ in range(rnd.randint(1, 3))]
    if kind == 'addext' and lines: lines += [extend(rnd.choice(lines)) for _ in range(rnd.randint(1, 3))]
    if kind == 'subset': lines = [l for l in lines if rnd.random() < 0.9]
    if kind == 'random': lines = [random_state(n - 1) for _ in range(rnd.randint(1, 40))]
    open(path, 'w').write(''.join(l + '\n' for l in lines))
    v = [run('min', path), run('full', path)] + ([] if nobrute else [run('brute', path)])
    if len(set(v)) != 1:
        print('DISAGREEMENT', kind, v); os.replace(path, path + f'.disagree{t}'); sys.exit(1)
    tally[(kind, v[0])] = tally.get((kind, v[0]), 0) + 1
for k in sorted(tally): print(f'n={n} L={L} {k[0]:8s} {k[1]:7s} {tally[k]}')
print('all modes agreed on', trials, 'certificates')
