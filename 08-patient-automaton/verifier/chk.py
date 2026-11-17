#!/usr/bin/env python3
# Referee's own checker (shares no code with either solver). Letters: 'a','b'. States 1..n.
import sys, itertools, random
from functools import lru_cache

def aut(sa, sb):
    return {'a': [int(c) for c in sa], 'b': [int(c) for c in sb]}
def img(A, S, l):
    return frozenset(A[l][s - 1] for s in S)
def Qof(A): return frozenset(range(1, len(A['a']) + 1))
def fs(s): return frozenset(int(c) for c in str(s))
def show(S): return ''.join(map(str, sorted(S)))

# ---------- literal definition, bounded T ----------
def literal_minT(A, d, Tmax):
    Q = Qof(A)
    def final_ok(xs, u, T):
        # every completion of the revealed prefix u to length T-1
        for rest in itertools.product('ab', repeat=T - 1 - len(u)):
            ys = u + rest
            S = Q
            for t in range(T):
                S = img(A, S, xs[t])
                if t < T - 1: S = img(A, S, ys[t])
            if len(S) != 1: return False
        return True
    def rec(t, u, xs, T):
        for x in 'ab':
            xs2 = xs + (x,)
            if t == T:
                if final_ok(xs2, u, T): return True
                continue
            k = max(0, t - d)          # letters revealed before x_{t+1}: y_1..y_{t-d}
            if k > len(u):
                if all(rec(t + 1, u + (y,), xs2, T) for y in 'ab'): return True
            else:
                if rec(t + 1, u, xs2, T): return True
        return False
    for T in range(1, Tmax + 1):
        if rec(1, (), (), T): return T
    return None

# ---------- blind game by families of images ----------
def blind_minT(A, cap=10**6):
    Q = Qof(A)
    start = frozenset([Q]); seen = {start}; layer = [start]; T = 0
    while layer:
        T += 1; nxt = []
        for F in layer:
            for x in 'ab':
                mid = [img(A, S, x) for S in F]
                if all(len(S) == 1 for S in mid): return T
                G = frozenset(img(A, S, y) for S in mid for y in 'ab')
                if G not in seen:
                    seen.add(G); nxt.append(G)
        layer = nxt
    return None          # closed: not blind-winnable

# ---------- delay game, rank = minimal uniform T ----------
def delay_minT(A, d, kmax=400):
    Q = Qof(A)
    words = [''.join(w) for w in itertools.product('ab', repeat=d)]
    subsets = set(); stack = [Q]
    while stack:
        S = stack.pop()
        if S in subsets: continue
        subsets.add(S)
        for l in 'ab': stack.append(img(A, S, l))
    def imm(S, w, x):   # all Spoiler completions of the d pending rounds, then x
        fam = {S}
        for c in w:
            fam = {img(A, img(A, T, c), y) for T in fam for y in 'ab'}
        return all(len(img(A, T, x)) == 1 for T in fam)
    V = {}
    for S in subsets:
        for w in words:
            if any(imm(S, w, x) for x in 'ab'): V[(S, w)] = 1
    k = 1
    while True:
        k += 1; new = {}
        for S in subsets:
            for w in words:
                if (S, w) in V: continue
                for x in 'ab':
                    v = w + x; c, u = v[0], v[1:]
                    Sc = img(A, S, c)
                    if all((img(A, Sc, y), u) in V for y in 'ab'): new[(S, w)] = k; break
        if not new: break
        V.update(new)
    best = min((V[(Q, w)] for w in words if (Q, w) in V), default=None)
    bt = blind_minT(A)
    cands = []
    if best is not None: cands.append(d + best)
    if bt is not None: cands.append(bt)
    return min(cands) if cands else None

# ---------- strategies; they get ONLY the legally visible Spoiler prefix ----------
class Illegal(Exception): pass
def strat_A(first, wait, strike, res_a, res_b):
    def f(t, d, vis):
        if t == 1: return first
        lo = max(d + 2, 3)
        if t < lo: return wait
        if not vis: raise Illegal('needs y_1 at t=%d d=%d' % (t, d))
        r = res_a if vis[0] == 'a' else res_b
        tau = lo
        while tau % 3 != r: tau += 1
        return strike if t == tau else wait
    return f
def strat_B(res_a=2, res_b=0, peek=False):
    def f(t, d, vis):
        if t == 1: return 'b'
        if t <= d + 1 and not peek: return 'a'
        if not vis: raise Illegal('needs y_1 at t=%d d=%d' % (t, d))
        rr = res_a if vis[0] == 'a' else res_b
        r = d + 1
        while r % 3 != rr: r += 1
        return 'b' if t == r + 1 else 'a'
    return f
def check_strategy_all_plays(A, f, d, T):
    """literal: every Spoiler word y_1..y_{T-1}; returns (ok, counterexample)"""
    Q = Qof(A)
    for ys in itertools.product('ab', repeat=T - 1):
        S = Q
        for t in range(1, T + 1):
            vis = ys[:max(0, t - 1 - d)]
            S = img(A, S, f(t, d, vis))
            if t < T: S = img(A, S, ys[t - 1])
        if len(S) != 1: return False, ''.join(ys)
    return True, None
def strategy_T_by_families(A, f, d, Tcap):
    """strategies here depend on y_1 only: track the family of possible images per y_1. returns least uniform T or None"""
    Q = Qof(A); Ts = []
    for y1 in 'ab':
        fam = {Q}; done = None
        for t in range(1, Tcap + 1):
            vis = (y1,) + ('?',) * max(0, t - 2 - d) if t - 1 - d >= 1 else ()
            x = f(t, d, vis)
            mid = {img(A, S, x) for S in fam}
            if all(len(S) == 1 for S in mid): done = t; break
            ys = [y1] if t == 1 else 'ab'
            fam = {img(A, S, y) for S in mid for y in ys}
        if done is None: return None
        Ts.append(done)
    return max(Ts)

def canon(A):
    n = len(A['a']); best = None
    for p in itertools.permutations(range(1, n + 1)):
        for sw in (('a', 'b'), ('b', 'a')):
            rows = []
            for l in sw:
                row = [0] * n
                for i in range(n): row[p[i] - 1] = p[A[l][i] - 1]
                rows.append(''.join(map(str, row)))
            s = rows[0] + ' ' + rows[1]
            if best is None or s < best: best = s
    return best

def main():
    W1 = aut('2213', '4311'); W2 = aut('1424', '3221'); C2 = aut('1123', '4134'); C1 = aut('1123', '3422')
    W1p = aut('22132', '43114'); C2p = aut('112311', '413411')
    print('== orbits');
    for nm, A in [('A.W1', W1), ('A.W2', W2), ('B.C2', C2), ('B.C1', C1)]: print(nm, canon(A))

    print('== literal definition vs rank solver (min T), all n=2,3 automata + n=4 sample; d<=2, T<=6')
    random.seed(1); mism = 0; cnt = 0; wins = 0
    cases = []
    for n in (2, 3):
        for ra in itertools.product(range(1, n + 1), repeat=n):
            for rb in itertools.product(range(1, n + 1), repeat=n): cases.append((ra, rb))
    for _ in range(150):
        cases.append((tuple(random.randint(1, 4) for _ in range(4)), tuple(random.randint(1, 4) for _ in range(4))))
    for A in (W1, W2, C1, C2): cases.append((tuple(A['a']), tuple(A['b'])))
    for ra, rb in cases:
        A = {'a': list(ra), 'b': list(rb)}
        for d in (0, 1, 2):
            L = literal_minT(A, d, 6); R = delay_minT(A, d); cnt += 1
            Rc = R if (R is not None and R <= 6) else None
            if L != Rc: mism += 1; print('MISMATCH', ra, rb, d, L, R)
            if L: wins += 1
    print('cases', cnt, 'literal wins', wins, 'mismatches', mism)

    print('== rank solver vs C++ table: count n=3 and n=4 winnable at d=0,1,2 and blind')
    for n in (3, 4):
        c = [0, 0, 0]; cb = 0
        its = list(itertools.product(range(1, n + 1), repeat=n))
        if n == 4 and '--full4' not in sys.argv:
            print(' (n=4 python recount skipped; pass --full4)'); continue
        for ra in its:
            for rb in its:
                A = {'a': list(ra), 'b': list(rb)}
                if blind_minT(A) is not None: cb += 1
                for d in (0, 1, 2):
                    if delay_minT(A, d) is not None: c[d] += 1
        print(' n=%d W_0,1,2 = %s  W_inf = %d' % (n, c, cb))

    print('== minimal T by rank solver, d=0..10')
    for nm, A in [('A.W1', W1), ('A.W2', W2), ('B.C2', C2), ('B.C1', C1)]:
        print(nm, [delay_minT(A, d) for d in range(11)], 'blind', blind_minT(A))

    print('== strategies, literal over all Spoiler plays')
    sA1 = strat_A('b', 'a', 'b', 0, 1); sA2 = strat_A('a', 'b', 'a', 1, 0); sB = strat_B()
    claimA = [4, 4, 6, 7, 7, 9, 10, 10, 12, 13, 13, 15]
    for nm, A, f in [('A.W1', W1, sA1), ('A.W1pad5', W1p, sA1), ('A.W2', W2, sA2), ('B.C2', C2, sB), ('B.C1', C1, sB), ('B.C2pad6', C2p, sB)]:
        res = []
        for d in range(0, 13):
            T = claimA[d] if (nm.startswith('A') and d < 12) else d + 4
            ok, cex = check_strategy_all_plays(A, f, d, T)
            # is the claimed T tight for this strategy?
            okm, _ = check_strategy_all_plays(A, f, d, T - 1)
            res.append('%d:%s%s' % (d, 'OK' if ok else 'FAIL(' + cex + ')', '' if not okm else '(T-1 also wins)'))
        print(nm, ' '.join(res))
    print('== strategies by family tracking, d=0..300 (uniform T reached; None = fails)')
    for nm, A, f in [('A.W1', W1, sA1), ('A.W1pad5', W1p, sA1), ('A.W2', W2, sA2), ('B.C2', C2, sB), ('B.C1', C1, sB), ('B.C2pad6', C2p, sB)]:
        Ts = [strategy_T_by_families(A, f, d, d + 10) for d in range(301)]
        bad = [d for d, T in enumerate(Ts) if T is None]
        print(nm, 'fails at', bad, 'T(0..12)=', Ts[:13], 'max T-d =', max(T - d for d, T in enumerate(Ts) if T))
    # cross-check family tracker against literal on small d
    for nm, A, f in [('A.W1', W1, sA1), ('B.C2', C2, sB)]:
        for d in range(8):
            T = strategy_T_by_families(A, f, d, d + 10)
            assert check_strategy_all_plays(A, f, d, T)[0] and not check_strategy_all_plays(A, f, d, T - 1)[0]
    print('family tracker agrees with literal enumeration for d<=7')

    print('== NEGATIVE CONTROLS (each must be rejected)')
    def expect_reject(label, thunk):
        try:
            ok = thunk()
            print(' ', label, '->', 'ACCEPTED (CHECKER BROKEN)' if ok else 'rejected')
        except Illegal as e:
            print(' ', label, '-> rejected as illegal:', e)
    expect_reject('A.W1 with residues swapped (1,0), d=3, T=7', lambda: check_strategy_all_plays(W1, strat_A('b', 'a', 'b', 1, 0), 3, 7)[0])
    expect_reject('A.W1 residues (0,2), d=5, T=20', lambda: check_strategy_all_plays(W1, strat_A('b', 'a', 'b', 0, 2), 5, 12)[0])
    expect_reject('B sigma on C2 with b(4)=3 instead of 4, d=2, T=6', lambda: check_strategy_all_plays(aut('1123', '4133'), sB, 2, 6)[0])
    expect_reject('B sigma on A.W1 (wrong automaton), d=2, T=6', lambda: check_strategy_all_plays(W1, sB, 2, 6)[0])
    expect_reject('B sigma peeking at y_1 one round early, d=3', lambda: check_strategy_all_plays(C2, strat_B(peek=True), 3, 7)[0])
    expect_reject('B sigma with claimed T=d+3 at d=2 (r=5 branch needs 6)', lambda: check_strategy_all_plays(C2, sB, 2, 5)[0])
    expect_reject('constant strategy a on C2, d=0, T=8', lambda: check_strategy_all_plays(C2, lambda t, d, v: 'a', 0, 8)[0])
    print('  blind: A.W1 with a(4)=1 (2211/4311) claimed non-blind-winnable ->', 'rejected (blind T=%s)' % blind_minT(aut('2211', '4311')) if blind_minT(aut('2211', '4311')) else 'ACCEPTED (BROKEN)')

    print('== solver-a Theorem 2: round tables and the 4-family certificate, recomputed')
    def rounds(A, P, x):
        m = img(A, P, x); out = set()
        for y in 'ab':
            r = img(A, m, y); out.add(r if len(r) == 2 and len(m) == 2 else 'M')
        return out
    claim_tab = {'a': {12: 'M', 13: 'M 34', 23: 'M 34', 14: '12 13', 24: '12 13', 34: '12 14'},
                 'b': {12: '13 M', 13: '23 14', 23: '12 14', 14: '23 14', 24: '12 14', 34: 'M'}}
    bad = 0
    for x in 'ab':
        for P, s in claim_tab[x].items():
            want = set('M' if z == 'M' else fs(z) for z in s.split())
            if rounds(W1, fs(P), x) != want: bad += 1; print(' round table differs', x, P, rounds(W1, fs(P), x))
    print(' round-table entries wrong:', bad)
    def cert_ok(A, fams, starts):
        fams = [frozenset(fs(p) for p in F) for F in fams]
        for F in fams:
            for x in 'ab':
                reach = set()
                for P in F: reach |= {r for r in rounds(A, P, x) if r != 'M'}
                if not any(G <= reach for G in fams): return False
        Q = Qof(A)
        for x in 'ab':
            m = img(A, Q, x)
            if len(m) < 2: return False
            ims = [img(A, m, y) for y in 'ab']
            pairs = {frozenset(p) for S in ims for p in itertools.combinations(sorted(S), 2)}
            if not any(G <= pairs for G in fams): return False
        return True
    F4 = [[13, 14], [14, 23], [12, 13, 34], [12, 14, 34]]
    print(' 4-family certificate on W1:', cert_ok(W1, F4, None))
    print(' NEG: same certificate, F3 removed:', cert_ok(W1, [F4[0], F4[1], F4[3]], None), ' | on mutated automaton 2211/4311:', cert_ok(aut('2211', '4311'), F4, None), ' | F1 replaced by {13}:', cert_ok(W1, [[13]] + F4[1:], None))
    # A's 9-family file: antichains of maximal images
    def maxi(F):
        F = set(F); return frozenset(S for S in F if not any(S < T for T in F))
    lines = [l.split() for l in open('solver-a/blind_cert_2213_4311.txt') if not l.startswith('#') and l.strip()]
    fams = {frozenset(fs(t) for t in l) for l in lines}
    ok = frozenset([Qof(W1)]) in fams
    for F in fams:
        for x in 'ab':
            mid = [img(W1, S, x) for S in F]
            if all(len(S) == 1 for S in mid): ok = False
            G = maxi(img(W1, S, y) for S in mid for y in 'ab')
            G = frozenset(S for S in G if len(S) > 1) if any(len(S) > 1 for S in G) else G
            if G not in fams: ok = False; print('  not closed:', [show(S) for S in F], x, '->', [show(S) for S in G])
    print(' 9-family file closed & no killing letter:', ok, '(families:', len(fams), ')')

    print('== solver-a Theorem 1 invariants I(t), J(t), all plays, d<=9')
    bad = 0
    I = {0: ['34'], 1: ['14', '12'], 2: ['13', '12']}; J = {0: ['13', '12'], 1: ['34'], 2: ['14', '12']}
    for d in range(10):
        for y1 in 'ab':
            lo = max(d + 2, 3); tau = lo
            while tau % 3 != (0 if y1 == 'a' else 1): tau += 1
            inv = I if y1 == 'a' else J
            for rest in itertools.product('ab', repeat=tau - 2):
                ys = (y1,) + rest; S = Qof(W1)
                for t in range(1, tau):
                    S = img(W1, img(W1, S, sA1(t, d, ys[:max(0, t - 1 - d)])), ys[t - 1])
                    turn = t + 1
                    if turn >= 3 and len(S) != 1 and show(S) not in inv[turn % 3]: bad += 1
    print(' invariant violations:', bad)

    print('== solver-b Lemma A claims, all plays, d<=9')
    bad = 0; badM = []
    for d in range(10):
        for y1 in 'ab':
            rr = 2 if y1 == 'a' else 0; r = d + 1
            while r % 3 != rr: r += 1
            M = {1: img(C2, img(C2, Qof(C2), 'b'), y1)}
            for k in range(1, r): M[k + 1] = img(C2, img(C2, M[k], 'a'), 'b')
            if show(M[r]) != '14': bad += 1
            if show(M[r - 1]) != '13': badM.append((d, y1, r, show(M[r - 1])))
            for rest in itertools.product('ab', repeat=r - 1):
                ys = (y1,) + rest; S = Qof(C2)
                for k in range(1, r + 1):
                    S = img(C2, img(C2, S, sB(k, d, ys[:max(0, k - 1 - d)])), ys[k - 1])
                    if S == M[k] or len(S) == 1: continue
                    if show(S) == '12' and k >= 2 and show(M[k - 1]) in ('14', '34'): continue
                    bad += 1
                if not (show(S) == '14' or len(S) == 1): bad += 1
    print(' claim violations:', bad, '| "M_(r-1) = 13" false at (d, y1, r, M_(r-1)):', badM)

    print('== solver-b Lemma B: Spoiler rule against every word of length <= 16 (C2), and same rule on C1')
    def spoiler_reply(word):
        # word finite, extended by b^inf.  returns replies y_1..y_T
        T = len(word); ys = []
        for i, x in enumerate(word):
            if x == 'a': ys.append('b'); continue
            nxt = word[i + 1] if i + 1 < T else 'b'
            if nxt == 'b': ys.append('b'); continue
            j = i + 1; m = 0
            while j < T and word[j] == 'a': m += 1; j += 1
            ys.append('a' if m % 3 == 2 else 'b')      # a-run is followed by b (in the word or in the b-tail)
        return ys
    def rule_holds(A, L, mod=2):
        fails = 0; first = None
        for T in range(1, L + 1):
            for w in itertools.product('ab', repeat=T):
                ys = spoiler_reply(w) if mod == 2 else [('a' if y == 'b' else 'b') if False else y for y in spoiler_reply(w)]
                S = Qof(A); ok = True
                for t in range(T):
                    S = img(A, S, w[t])
                    if len(S) == 1: ok = False; break
                    S = img(A, S, ys[t])
                if not ok:
                    fails += 1
                    if first is None: first = ''.join(w)
        return fails, first
    print(' C2: words beaten-by-synchronizer:', rule_holds(C2, 16))
    print(' C1 (rule not claimed for it):', rule_holds(C1, 12))
    print(' NEG: same rule on A.W1:', rule_holds(W1, 10), ' on mutated C2 1123/4133:', rule_holds(aut('1123', '4133'), 10))
    # wrong-residue rule
    def reply_wrong(word):
        T = len(word); ys = []
        for i, x in enumerate(word):
            if x == 'a': ys.append('b'); continue
            nxt = word[i + 1] if i + 1 < T else 'b'
            if nxt == 'b': ys.append('b'); continue
            j = i + 1; m = 0
            while j < T and word[j] == 'a': m += 1; j += 1
            ys.append('a' if m % 3 == 1 else 'b')
        return ys
    f = 0
    for T in range(1, 11):
        for w in itertools.product('ab', repeat=T):
            ys = reply_wrong(w); S = Qof(C2)
            for t in range(T):
                S = img(C2, S, w[t])
                if len(S) == 1: f += 1; break
                S = img(C2, S, ys[t])
    print(' NEG: rule with residue 1 instead of 2 on C2, words beaten:', f)
    print('== blind brute force: no winning word up to length 13 against all fills (families method), and blind_minT')
    for nm, A in [('A.W1', W1), ('A.W2', W2), ('B.C2', C2), ('B.C1', C1), ('A.W1pad5', W1p), ('B.C2pad6', C2p)]:
        print(' ', nm, 'blind_minT =', blind_minT(A))

main()
