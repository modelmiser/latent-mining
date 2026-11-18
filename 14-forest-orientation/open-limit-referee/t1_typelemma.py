# (i) Type Lemma + Lemma B, exactly as stated, on explicitly merged trees with h recomputed from scratch.
# usage: t1_typelemma.py MODE VARIANT   MODE in {pairs6, big, allorient}; VARIANT in {true, f1, f2, f3}
import sys, random, itertools
from lib import *
mode, variant = sys.argv[1], sys.argv[2]
L = gen_trees(9)
print('tree counts by size', {m: len(L[m]) for m in L}, 'total', sum(len(L[m]) for m in L))

def info(T):
    m, e = T; o = outs(m, e); h = hvals(m, o)
    return (m, e, o, h, [slots(v, o, h) for v in range(m)])

def lemma_ok(i, cost, sl_e):
    k = i // 2
    if variant == 'f1':   # false: A_k costs >= k+1
        if i % 2 == 0: return cost >= k + 1 and has_index(sl_e, i + 1)
    if variant == 'f2':   # false: R_k always yields A_(k+1)
        if i % 2 == 1: return cost >= k and has_index(sl_e, i + 1)
    if i % 2 == 0: return cost >= k and has_index(sl_e, i + 1)
    return (cost >= k and has_index(sl_e, i + 1)) or (cost >= k + 1 and has_index(sl_e, i))

stats = dict(cases=0, fail=0, replies=0, lemmaB_fail=0)
first = []
def test_pair(X, Y):
    mx, ex, ox, hx, sx = X; my, ey, oy, hy, sy = Y
    m = mx + my
    eY = tuple((a + mx, b + mx) for (a, b) in ey)
    base = ex + eY
    for x in range(mx):
        for y0 in range(my):
            y = y0 + mx
            imax = min(max_index(sx[x]), max_index(sy[y0]))
            o = outs(m, base)
            for (e, other, se, so) in ((x, y, sx[x], sy[y0]), (y, x, sy[y0], sx[x])):
                if len(o[e]) < 2: reps = [[e]]
                else: reps = normal_paths(e, o)
                for path in reps:
                    fl = set(zip(path, path[1:]))
                    ne = tuple((b, a) if (a, b) in fl else (a, b) for (a, b) in base) + ((e, other),)
                    o2 = outs(m, ne)
                    assert all(len(z) <= 2 for z in o2)
                    h2 = hvals(m, o2)
                    cost = len(path) - 1
                    s_e = slots(e, o2, h2); s_o = slots(other, o2, h2)
                    wit = s_o if variant == 'f3' else s_e   # f3 false: other endpoint is the witness
                    stats['replies'] += 1
                    # Lemma B with true slot values
                    (p, q), (pp, qq) = se, so
                    X1 = cost >= p + 1 and s_e >= tuple(sorted((q, pp + 1))) and s_e[0] >= min(q, pp + 1)
                    def ge(s, t): t = tuple(sorted(t)); return s[0] >= t[0] and s[1] >= t[1]
                    X1 = cost >= p + 1 and ge(s_e, (q, pp + 1))
                    X2 = cost >= q + 1 and ge(s_e, (p, pp + 1))
                    if not ((X1 or X2) and s_o == so): stats['lemmaB_fail'] += 1
                    for i in range(imax + 1):
                        stats['cases'] += 1
                        if not lemma_ok(i, cost, wit):
                            stats['fail'] += 1
                            if len(first) < 2: first.append((base, x, y, e, path, i, cost, s_e))

if mode == 'pairs6':
    S = [info(T) for m in range(1, 7) for T in L[m]]
    for X in S:
        for Y in S: test_pair(X, Y)
elif mode == 'big':
    random.seed(5)
    small = [info(T) for m in range(1, 5) for T in L[m]]
    deep = [info(T) for m in range(7, 10) for T in L[m]]
    deep = [T for T in deep if max(T[3]) >= 2] + random.sample(deep, 25)   # all Y with a vertex of h>=2, plus random
    print('Y set', len(small), '+', len(deep))
    Xs = [info(T) for m in range(1, 10) for T in L[m]]
    for X in Xs:
        for Y in small: test_pair(X, Y)
    Xd = [T for T in Xs if max(T[3]) >= 1]
    random.shuffle(Xd)
    for X in Xd[:1500]:
        for Y in deep: test_pair(X, Y)
print(mode, variant, stats)
for f in first: print(' first failure', f)
