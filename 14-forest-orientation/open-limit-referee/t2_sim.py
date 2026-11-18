# (ii) Theorem 1 strategy on explicit forests. Referee's own code.
# usage: t2_sim.py VARIANT   (true | fF | fnoshadow)
import sys, random, math
from fractions import Fraction as Fr
from lib import F as F_true, has_index, max_index
sys.setrecursionlimit(1000000)
variant = sys.argv[1] if len(sys.argv) > 1 else 'true'
def F(i):
    if variant == 'fF' and i % 2 == 1: return Fr(i // 2) + 1      # false potential F(R_k)=k+1
    return F_true(i)

class Forest:
    def __init__(s, n):
        s.n = n; s.out = [[] for _ in range(n)]; s.comp = list(range(n)); s.mem = {v: [v] for v in range(n)}
    def copy(s):
        g = Forest.__new__(Forest); g.n = s.n; g.out = [l[:] for l in s.out]; g.comp = s.comp[:]
        g.mem = {k: v[:] for k, v in s.mem.items()}; return g
    def h(s, v, memo):
        if v not in memo:
            memo[v] = 0 if len(s.out[v]) < 2 else 1 + min(s.h(w, memo) for w in s.out[v])
        return memo[v]
    def slots(s, v):
        memo = {}
        t = sorted([s.h(w, memo) for w in s.out[v]] + [-1] * (2 - len(s.out[v])))
        return (t[0], t[1])
    def paths(s, x):
        if len(s.out[x]) < 2: return [[x]]
        res = []
        def go(p):
            for w in s.out[p[-1]]:
                if len(s.out[w]) < 2: res.append(p + [w])
                else: go(p + [w])
        go([x]); return res
    def flip_path(s, path):
        for a, b in zip(path, path[1:]):
            s.out[a].remove(b); s.out[b].append(a)
    def add(s, e, other):
        s.out[e].append(other); assert len(s.out[e]) <= 2
        a, b = s.comp[e], s.comp[other]
        for v in s.mem[b]: s.comp[v] = a
        s.mem[a] += s.mem.pop(b)
        return a, b

class Adversary:
    """Theorem 1: main phase (equal assigned indices), then cleanup. Tracks paid, index, witness per tree."""
    def __init__(s, n, policy, rng):
        s.T = {v: dict(a=0, w=v, paid=Fr(0), size=1) for v in range(n)}
        s.policy = policy; s.rng = rng; s.cleanup = False; s.Y = None; s.viol = []; s.n = n
        s.im = None; s.m = None
    def P(s, t): return t['paid'] + F(t['a']) - Fr(t['size'], 3)
    def name(s, fo):
        ids = list(s.T)
        if len(ids) == 1: return None
        if not s.cleanup:
            by = {}
            for c in ids: by.setdefault(s.T[c]['a'], []).append(c)
            dup = sorted(i for i in by if len(by[i]) >= 2)
            if dup:
                i = dup[0] if s.policy == 'low' else dup[-1] if s.policy == 'high' else s.rng.choice(dup)
                cs = by[i] if s.policy != 'rand' else s.rng.sample(by[i], len(by[i]))
                s.cur = (cs[0], cs[1], i); 
            else:
                s.cleanup = True
                order = sorted(ids, key=lambda c: s.T[c]['a'])
                s.im = s.T[order[-1]]['a']; s.m = len(order); s.Y = order[-1]
        if s.cleanup:
            rest = sorted((c for c in ids if c != s.Y), key=lambda c: s.T[c]['a'])
            X = rest[-1]; s.cur = (s.Y, X, s.T[X]['a'])
        c1, c2, i = s.cur
        for c in (c1, c2):   # witnesses must have index i in the forest the Adversary reads
            if not has_index(fo.slots(s.T[c]['w']), i): s.viol.append(('nonwitness', c, i))
        return s.T[c1]['w'], s.T[c2]['w']
    def update(s, fo, e, cost, cid_new, cid_gone):
        c1, c2, i = s.cur; k = i // 2
        t1, t2 = s.T[c1], s.T[c2]
        sl = fo.slots(e)
        if i % 2 == 0: j = i + 1; need = k
        elif cost == k: j = i + 1; need = k
        else: j = i; need = k + 1
        if cost < need or not has_index(sl, j): s.viol.append(('lemma', i, cost, sl))
        new = dict(a=j, w=e, paid=t1['paid'] + t2['paid'] + cost, size=t1['size'] + t2['size'])
        if not s.cleanup:
            if s.P(new) < s.P(t1) + s.P(t2) or s.P(new) < 0: s.viol.append(('P', i, j, cost))
        else:
            if s.P(new) < s.P(t1) + s.P(t2) - (F(t1['a']) - F(i)): s.viol.append(('Pclean', i, j, cost))
        del s.T[c1]; del s.T[c2]; s.T[cid_new] = new
        if s.cleanup: s.Y = cid_new
    def final_check(s, total):
        n = s.n
        loose = Fr(n, 3) - Fr(7, 3) * Fr(math.log2(n + 1)) + Fr(5, 6)
        if s.im is None: s.im, s.m = list(s.T.values())[0]['a'], 1
        sharp = Fr(n, 3) - F(s.im) - Fr(2, 3) * (s.m - 1)
        if total < sharp: s.viol.append(('sharp', total, sharp))
        if total < loose: s.viol.append(('loose', total, float(loose)))
        if s.im > 2 * math.log2(n + 1) - 1 + 1e-9: s.viol.append(('im', s.im))
        return float(sharp), float(loose)

def alg_short(fo, x, y, rng):
    c = [(len(p), rng.random(), e, p) for e in (x, y) for p in fo.paths(e)]; return min(c)[2:]
def alg_long(fo, x, y, rng):
    c = [(len(p), rng.random(), e, p) for e in (x, y) for p in fo.paths(e)]; return max(c)[2:]
def alg_rand(fo, x, y, rng):
    c = [(e, p) for e in (x, y) for p in fo.paths(e)]; return rng.choice(c)
def alg_pot(fo, x, y, rng):   # minimise cost + F(best index of replying endpoint afterwards)
    best = None
    for e, o in ((x, y), (y, x)):
        for p in fo.paths(e):
            g = fo.copy(); g.flip_path(p); g.out[e].append(o)
            val = (len(p) - 1 + F_true(max_index(g.slots(e))), rng.random())
            if best is None or val < best[0]: best = (val, e, p)
    return best[1:]

def play(n, alg, policy, seed):
    rng = random.Random(seed); fo = Forest(n); adv = Adversary(n, policy, rng); total = 0
    for _ in range(n - 1):
        x, y = adv.name(fo)
        e, p = alg(fo, x, y, rng); o = y if e == x else x
        fo.flip_path(p); a, b = fo.add(e, o); total += len(p) - 1
        adv.update(fo, e, len(p) - 1, a, b)
    sh, lo = adv.final_check(total)
    return total, sh, lo, adv

def exhaustive(n, policy):
    """min over ALL normal-reply sequences against the (deterministic) strategy; invariants checked at every node"""
    best = [10 ** 9]; nodes = [0]; viol = []
    def rec(fo, adv, total, r):
        nodes[0] += 1
        if r == n - 1:
            adv2 = adv; sh, lo = adv2.final_check(total); viol.extend(adv2.viol)
            best[0] = min(best[0], total); return
        x, y = adv.name(fo); cur = adv.cur; st = (adv.cleanup, adv.Y, adv.im, adv.m)
        seen = set()
        for e, o in ((x, y), (y, x)):
            for p in fo.paths(e):
                g = fo.copy(); g.flip_path(p); a, b = g.add(e, o)
                ad = Adversary.__new__(Adversary); ad.__dict__ = dict(adv.__dict__)
                ad.T = dict(adv.T); ad.viol = []; ad.cur = cur
                ad.update(g, e, len(p) - 1, a, b); viol.extend(ad.viol); ad.viol = []
                rec(g, ad, total + len(p) - 1, r + 1)
    rec(Forest(n), Adversary(n, policy, random.Random(0)), 0, 0)
    return best[0], nodes[0], viol

def play_shadow(n, policy, seed, extra, use_shadow=True):
    """Algorithm replies with ARBITRARY valid re-orientations (anywhere in the forest). Adversary runs the
    strategy on the Lemma-1 shadow position (use_shadow) or, falsely, on the real position."""
    rng = random.Random(seed); real = Forest(n); sh = Forest(n); adv = Adversary(n, policy, rng)
    costA = 0; costL = 0; bad = 0
    for _ in range(n - 1):
        x, y = adv.name(sh if use_shadow else real)
        before = set((a, b) for a in range(n) for b in real.out[a])
        e = rng.choice((x, y)); o = y if e == x else x
        p = rng.choice(real.paths(e)); real.flip_path(p); real.add(e, o)
        for _ in range(extra):                      # extra flips anywhere, keeping validity
            a = rng.randrange(n)
            if real.out[a]:
                b = rng.choice(real.out[a])
                if (a, b) != (e, o) and len(real.out[b]) < 2:
                    real.out[a].remove(b); real.out[b].append(a)
        after = set((a, b) for a in range(n) for b in real.out[a])
        cA = len(before - after); costA += cA
        if use_shadow:
            if len(sh.out[e]) < 2: lp = [e]
            else:
                lp = [e]
                while True:
                    v = lp[-1]; nx = [w for w in sh.out[v] if v in real.out[w]]
                    assert nx, 'Lemma 1 walk stuck'
                    lp.append(nx[0])
                    if len(sh.out[nx[0]]) < 2: break
            sh.flip_path(lp); a, b = sh.add(e, o); costL += len(lp) - 1
            d = sum(1 for a2 in range(n) for b2 in sh.out[a2] if b2 not in real.out[a2])
            if costL + d > costA: bad += 1
            adv.update(sh, e, len(lp) - 1, a, b)
        else:
            adv.update(real, e, cA, real.comp[e], None) if False else None
            c1, c2, i = adv.cur
            t1, t2 = adv.T[c1], adv.T[c2]
            adv.update(real, e, cA, c1, c2)
    s1, lo = adv.final_check(costL if use_shadow else costA)
    return costA, costL, bad, s1, lo, adv

if __name__ == '__main__':
    tot_viol = 0
    if variant in ('true', 'fF'):
        for n in (16, 37, 64, 256, 333, 1000):
            for name, alg in (('short', alg_short), ('long', alg_long), ('rand', alg_rand), ('pot', alg_pot)):
                for policy in ('low', 'high', 'rand'):
                    for seed in (1, 2):
                        if name == 'pot' and n > 333: continue
                        t, shp, lo, adv = play(n, alg, policy, seed); tot_viol += len(adv.viol)
                        if seed == 1: print(f'n={n} alg={name} pol={policy} paid={t} sharp={shp:.2f} loose={lo:.2f} m={adv.m} im={adv.im} viol={len(adv.viol)}', adv.viol[:1])
        for n in range(2, 15):
            for policy in ('low', 'high'):
                b, nodes, viol = exhaustive(n, policy); tot_viol += len(viol)
                print(f'exhaustive n={n} pol={policy} min_paid={b} nodes={nodes} viol={len(viol)}', viol[:1])
    if variant in ('true', 'fnoshadow'):
        for n in (16, 64, 256, 600):
            for extra in (1, 3):
                for seed in (1, 2, 3):
                    cA, cL, bad, shp, lo, adv = play_shadow(n, 'low', seed, extra, use_shadow=(variant == 'true'))
                    tot_viol += len(adv.viol) + bad
                    print(f'nonnormal n={n} extra={extra} seed={seed} costA={cA} costL={cL} lemma1_bad={bad} sharp={shp:.2f} viol={len(adv.viol)}', adv.viol[:1])
    print('VARIANT', variant, 'TOTAL VIOLATIONS', tot_viol)
