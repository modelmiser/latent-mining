# Theorem 1 bound vs exact worst case of the strategy in the abstract index game; Theorem 3 potential; Corollary 7.
import sys, math
from functools import lru_cache
from fractions import Fraction as Fr
sys.setrecursionlimit(1000000)
variant = sys.argv[1]
def F(i): return Fr(i // 2) + (Fr(1, 3) if i % 2 == 0 else Fr(2, 3))
def reply_opts(i, cap):          # (cost, new index) alternatives of the Type Lemma; cap = largest allowed index
    k = i // 2
    if i % 2 == 0: o = [(k, i + 1)]
    else: o = [(k, i + 1), (k + 1, i)]
    return [x for x in o if x[1] <= cap]
def make(policy, cap):
    @lru_cache(maxsize=None)
    def clean(b, rest):              # Y has index b, rest = ascending distinct indices
        if not rest: return 0
        i = rest[-1]
        return min(c + clean(j, rest[:-1]) for (c, j) in reply_opts(i, cap))
    @lru_cache(maxsize=None)
    def W(S):
        dup = sorted(set(i for i in S if S.count(i) >= 2))
        if not dup: return clean(S[-1], S[:-1])
        if policy == 'low': dup = dup[:1]
        best = None
        for i in dup:
            r = list(S); r.remove(i); r.remove(i)
            v = min(c + W(tuple(sorted(r + [j]))) for (c, j) in reply_opts(i, cap))
            best = v if best is None else max(best, v)
        return best
    return W
coef = Fr(7, 3) if variant != 'fbound' else Fr(1)
W = make('low', 10 ** 9); fails = 0
for n in range(1, 301):
    w = W(tuple([0] * n)); lo = n / 3 - float(coef) * math.log2(n + 1) + 5 / 6
    if w < lo - 1e-9: fails += 1
    if n in (16, 30, 64, 256, 300): print('strategy worst case (abstract, lowest-duplicate policy) n=%d: %d   Thm1 bound %.2f' % (n, w, lo))
print('bound-vs-worstcase failures n<=300:', fails)
Wb = make('best', 10 ** 9)
print('adversary-optimal order of equal-index merges: n=16,64:', Wb(tuple([0] * 16)), Wb(tuple([0] * 64)))
# Theorem 3
def phi(d):
    if variant == 'fphi': return Fr(4, 3) if d == 0 else Fr(1, 3)
    return Fr(4, 3) if d == 0 else Fr(2, 3) if d == 1 else Fr(1, 3)
def g(t): return t[1] + phi(t[1] - t[0])
def srt(a, b): return (a, b) if a <= b else (b, a)
bad_any = bad_presc = cnt = 0
R = range(-1, 13)
for p in R:
  for q in R:
    if q < p: continue
    for pp in R:
      for qq in R:
        if qq < pp: continue
        t, u = (p, q), (pp, qq); cnt += 1
        opts = {'X1': (p + 1, srt(q, pp + 1), u), 'X2': (q + 1, srt(p, pp + 1), u),
                'Y1': (pp + 1, srt(qq, p + 1), t), 'Y2': (qq + 1, srt(pp, p + 1), t)}
        ok = {k: c + max(g(nw), g(kp)) <= g(t) + g(u) for k, (c, nw, kp) in opts.items()}
        if not any(ok.values()): bad_any += 1
        # author's prescription: wlog p<=p'; exceptional t=(p,p+1),t'=(p,p) -> Y1; else X1
        a, b, names = (t, u, ('X1', 'Y1')) if p <= pp else (u, t, ('Y1', 'X1'))
        if b == (a[0], a[0]) and a == (a[0], a[0] + 1): ch = names[1]
        else: ch = names[0]
        if not ok[ch]: bad_presc += 1
print('Theorem 3: pairs', cnt, 'no option works:', bad_any, ' prescribed option fails:', bad_presc, ' g(-1,-1)=', g((-1, -1)))
# Corollary 7
cbad = 0
for D in range(1, 9):
    mult = 6 if variant != 'fdelta' else 3
    delta = Fr(1, mult * 4 ** D)
    a = lambda k: delta * 4 ** k + k + Fr(1, 3)
    r = lambda k: 2 * a(k) - k
    for k in range(D + 1):
        if k + r(k) < 2 * a(k): cbad += 1
        if k < D and k + a(k + 1) < 2 * r(k): cbad += 1
        if k + 1 + r(k) < 2 * r(k): cbad += 1
    if a(0) < Fr(1, 3) + delta: cbad += 1
    if sum(a(k) + r(k) for k in range(D + 1)) > (D + 1) * (D + Fr(3, 2)): cbad += 1
    if D <= 3:
        Wd = make('low', 2 * D + 1)
        for n in range(1, 301):
            if Wd(tuple([0] * n)) < (Fr(1, 3) + delta) * n - (D + 1) * (D + Fr(3, 2)): cbad += 1
        print('Cor 7 D=%d: capped abstract worst case n=300: %d, claimed bound %.2f' % (D, Wd(tuple([0] * 300)), float((Fr(1, 3) + delta) * 300 - (D + 1) * (D + Fr(3, 2)))))
print('Corollary 7 failures:', cbad, ' VARIANT', variant)
