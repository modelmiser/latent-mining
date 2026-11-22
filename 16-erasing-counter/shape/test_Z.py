#!/usr/bin/env python3
"""Random test of Lemma A and Theorem Z1 (PROOFS.md), from the definition of the model.

Trees are built with a spine z_1..z_k (the all-zero path) and subtrees S_i under the 1-edges of the spine.
Class C0 (hypothesis H): the data sets D_i = idx(S_i) minus spine are pairwise disjoint; S_i may read z_j, j > i.
For every cycle of every generated tree:
  LemmaA : a cycle with no home state (home = all spine bits 0) has length <= k
  Z1a    : (home cycles) for each mode i, all jumps out of mode i happen at one value of x_i
  Z1b    : (home cycles) N <= 2*C_1 + C_2 + ... + C_l, the chain m_1=j0, m_{t+1}=j(m_t) and c_i measured on the cycle
  WD     : N <= W_depth(T)
usage: test_Z.py NTREES SEED [mut]
  mut=1 : drop hypothesis H (the S_i draw data from one common pool)  -> Z1a/Z1b are expected to FAIL sometimes
  mut=2 : LemmaA tested with bound k-1                                  -> expected to FAIL
  mut=3 : Z1b tested with 2*C_1 replaced by C_1                         -> expected to FAIL
"""
import sys
import random

W = [1]
for _ in range(12):
    W.append(W[-1] * (W[-1] + 1))
NT = int(sys.argv[1]); SEED = int(sys.argv[2]); MUT = int(sys.argv[3]) if len(sys.argv) > 3 else 0
rnd = random.Random(SEED)


def rand_sub(avail, path, depth):
    """random subtree; avail = indices that may be read here, path = indices already on the path"""
    cand = [i for i in avail if i not in path]
    if depth == 0 or not cand or rnd.random() < 0.18:
        r = rnd.random()
        if r < 0.3:
            return ('L', -1)
        return ('L', rnd.choice(path))
    i = rnd.choice(cand)
    return ('N', i, rand_sub(avail, path + [i], depth - 1), rand_sub(avail, path + [i], depth - 1))


def gen():
    k = rnd.randint(1, 4)
    spine = list(range(k))
    nxt = k
    D = []
    pool = list(range(k, k + rnd.randint(1, 6)))
    for i in range(k):
        if MUT == 1:
            D.append(pool)
        else:
            n = rnd.randint(0, 3) if k > 1 else rnd.randint(0, 5)
            D.append(list(range(nxt, nxt + n))); nxt += n
    if MUT == 1:
        nxt = pool[-1] + 1
    if nxt > 11:
        return None
    S = []
    for i in range(k):
        zs = [z for z in spine[i + 1:] if rnd.random() < 0.5]
        S.append(rand_sub(D[i] + zs, spine[:i + 1], rnd.randint(0, 4)))
    lam = ('L', rnd.choice(spine)) if rnd.random() < 0.9 else ('L', -1)
    t = lam
    for i in reversed(range(k)):
        t = ('N', i, t, S[i])
    return t, k, D, nxt


def depth(t):
    return 0 if t[0] == 'L' else 1 + max(depth(t[2]), depth(t[3]))


def show(t):
    if t[0] == 'L':
        return '[]' if t[1] < 0 else '[%d]' % (t[1] + 1)
    return '(%d %s %s)' % (t[1] + 1, show(t[2]), show(t[3]))


def step(t, s):
    P = 0
    while t[0] == 'N':
        P |= 1 << t[1]
        t = t[3] if (s >> t[1]) & 1 else t[2]
    o = s & ~P
    if t[1] >= 0:
        o |= 1 << t[1]
    return o


stats = dict(trees=0, cycles=0, nohome=0, home=0, viol_A=0, viol_Z1a=0, viol_Z1b=0, viol_WD=0, tight_Z1b=0, maxlen=0, loops=0)
shown = 0


def witness(what, t, s, a, b):
    global shown
    if shown < 6:
        shown += 1
        print('WITNESS', what, show(t), 'state', s, ':', a, 'vs', b)


def run(t, k, D, m):
    stats['trees'] += 1
    n = 1 << m
    f = [step(t, s) for s in range(n)]
    col = [0] * n
    zmask = (1 << k) - 1
    dmask = [sum(1 << u for u in D[i]) for i in range(k)]
    dep = depth(t)
    for s0 in range(n):
        if col[s0]:
            continue
        x = s0
        while not col[x]:
            col[x] = s0 + 1
            x = f[x]
        if col[x] != s0 + 1:
            continue
        cyc = [x]
        y = f[x]
        while y != x:
            cyc.append(y); y = f[y]
        N = len(cyc)
        stats['cycles'] += 1
        stats['maxlen'] = max(stats['maxlen'], N)
        if N > W[dep]:
            stats['viol_WD'] += 1; witness('WD', t, x, N, W[dep])
        if N < 2:
            continue
        if all(c & zmask for c in cyc):
            stats['nohome'] += 1
            if N > k - (1 if MUT == 2 else 0):
                stats['viol_A'] += 1; witness('LemmaA', t, x, N, k)
            continue
        stats['home'] += 1
        # modes
        jumpfrom = {}   # mode -> set of (x_i value, target)
        vals = {}       # mode -> set of x_i values at mode-i states
        j0 = None
        bad = False
        for c in cyc:
            z = c & zmask
            nz = f[c] & zmask
            if z == 0:
                j0 = nz.bit_length() - 1
                continue
            if z & (z - 1):
                bad = True  # more than one 1 on the spine: contradicts Case B
                break
            i = z.bit_length() - 1
            vals.setdefault(i, set()).add(c & dmask[i])
            if nz:
                jumpfrom.setdefault(i, set()).add((c & dmask[i], nz.bit_length() - 1))
        if bad:
            stats['viol_Z1a'] += 1; witness('CaseB', t, x, N, 0); continue
        if any(len(v) > 1 for v in jumpfrom.values()):
            stats['viol_Z1a'] += 1; witness('Z1a', t, x, N, 0)
            continue
        chain = [j0]
        while chain[-1] in jumpfrom:
            nx = next(iter(jumpfrom[chain[-1]]))[1]
            if nx in chain:
                stats['loops'] += 1
                break
            chain.append(nx)
        cs = [len(vals[i]) for i in chain]
        suf = [1] * (len(cs) + 1)
        for q in reversed(range(len(cs))):
            suf[q] = suf[q + 1] * cs[q]
        phi = (1 if MUT == 3 else 2) * suf[0] + sum(suf[1:len(cs)])
        if N > phi:
            stats['viol_Z1b'] += 1; witness('Z1b', t, x, N, phi)
        if N == phi:
            stats['tight_Z1b'] += 1


made = 0
while made < NT:
    g = gen()
    if g is None:
        continue
    made += 1
    run(*g)
# fixed examples: (a), (b) of upper/PROOFS.md section 5 and the P1 counterexample (0-based indices)
if MUT == 0:
    def P(s):
        import re
        toks = re.findall(r'[()\[\]]|\d+', s); p = [0]
        def rec():
            if toks[p[0]] == '[':
                if toks[p[0] + 1] == ']':
                    p[0] += 2; return ('L', -1)
                w = int(toks[p[0] + 1]) - 1; p[0] += 3; return ('L', w)
            i = int(toks[p[0] + 1]) - 1; p[0] += 2
            a = rec(); b = rec(); p[0] += 1
            return ('N', i, a, b)
        return rec()
    run(P('(1 (2 [1] []) (2 [2] [2]))'), 2, [[], []], 2)
    run(P('(1 (2 [1] (3 [3] [])) (2 (4 [4] [2]) []))'), 2, [[3], [2]], 4)
    run(P('(1 (2 (3 [2] (4 [4] (5 [5] []))) [1]) (3 (6 [6] (7 [7] [3])) []))'), 3, [[5, 6], [], [3, 4]], 7)
print('NT', NT, 'SEED', SEED, 'MUT', MUT, stats)
