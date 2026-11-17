#!/usr/bin/env python3
"""Referee's own flag DP (iterative, by levels u and a within-level topological order) and checks of
the solvers' tables, Theorem 3 finite checks (both versions), closed forms, k=3/k=4 recurrences."""
import itertools, math, sys
OPT = ['', 'U', 'K', 'UU', 'UK', 'KU', 'KK']          # bottom->top

def acts(st, k, upos):
    """all non-noop attempts: name -> list of (succ, du)"""
    out = {}
    if upos and len(st[0]) < 2: out['IN'] = [((st[0] + 'U',) + st[1:], -1)]
    for i in range(k - 1):
        if st[i] and len(st[i+1]) < 2:
            t = list(st); t[i+1] += t[i][-1]; t[i] = t[i][:-1]; out['SH%d' % (i+1)] = [(tuple(t), 0)]
    for i in range(k):
        if st[i] and st[i][-1] == 'U':
            t = list(st); t[i] = t[i][:-1]; t = tuple('U' * len(s) for s in t)
            f = list(st); f[i] = f[i][:-1] + 'K'
            out['OUT%d' % (i+1)] = [(t, 0), (tuple(f), 0)]
    return out

def rank(st):   # decreases along every same-level transition
    return (sum(len(s) for s in st), tuple(len(s) for s in st), sum(s.count('U') for s in st))

def dp(k, U):
    states = sorted(itertools.product(OPT, repeat=k), key=rank)
    V = []; Q = []
    for u in range(U + 1):
        cur = {}; q = {}
        for st in states:
            a = acts(st, k, u > 0)
            for nm, succ in a.items():
                for s, du in succ:
                    if du == 0: assert rank(s) < rank(st)
            q[st] = {nm: sum(cur[s] if du == 0 else V[u-1][s] for s, du in succ) for nm, succ in a.items()}
            cur[st] = 1 if (u == 0 and not any(st)) else max(q[st].values(), default=0)
        V.append(cur); Q.append(q)
    return states, V, Q

def ge_lam2(d1, d0):          # d1 >= (5-sqrt5)/2 * d0, exact
    A = 2*d1 - 5*d0; B = d0
    if A >= 0 and B >= 0: return True
    if A < 0 and B < 0: return False
    return A*A >= 5*B*B if A >= 0 else 5*B*B >= A*A

what = sys.argv[1]
if what == 'table':
    for k in range(1, 7):
        nmax = 12 if k <= 6 else (12 if k <= 8 else 10)
        st, V, Q = dp(k, nmax)
        e = ('',) * k
        row = [V[n][e] for n in range(1, nmax + 1)]
        print(k, row, "N(k,k+1)==(k+1)!:", V[k+1][e] == math.factorial(k+1), "deficit(k+2)=", math.factorial(k+2) - V[k+2][e] if k + 2 <= nmax else None, flush=True)
if what == 'k2':
    U = 300
    states, V, Q = dp(2, U)
    e = ('', '')
    a = [V[n][e] for n in range(U + 1)]
    print("N(2,1..12) =", a[1:13])
    print("recurrence a(n)=5a(n-1)-5a(n-2) holds for n in 5..%d:" % U, all(a[n] == 5*a[n-1] - 5*a[n-2] for n in range(5, U + 1)), "; at n=4:", a[4] == 5*a[3] - 5*a[2])
    # closed forms
    F = [0, 1]; L = [2, 1]
    for i in range(2, U + 2): F.append(F[-1] + F[-2]); L.append(L[-1] + L[-2])
    okB = all(a[2*j+1] == 3 * 5**(j-1) * F[2*j+1] for j in range(1, (U - 1)//2 + 1)) and all(a[2*j] == 3 * 5**(j-2) * L[2*j] for j in range(2, U//2 + 1))
    Us = [0, 1]
    for i in range(2, U + 1): Us.append(5*Us[-1] - 5*Us[-2])
    okA = all(a[n] == 21*Us[n-3] - 30*Us[n-4] for n in range(4, U + 1))
    # GF x(1-3x+x^2+x^3)/(1-5x+5x^2)
    num = [0, 1, -3, 1, 1] + [0]*U; g = [0]*(U+1)
    for n in range(1, U + 1): g[n] = num[n] + 5*g[n-1] - (5*g[n-2] if n >= 2 else 0)
    print("closed form B (Fibonacci/Lucas) to n=%d:" % U, okB, "; closed form A (21U-30U):", okA, "; GF:", g[1:] == a[1:])
    # (b) smallest u0 such that EVERY state value satisfies the recurrence for all u>=u0
    for name, test in (("f(s,u)", lambda st, u: V[u][st] == 5*V[u-1][st] - 5*V[u-2][st]),):
        badu = max([u for st in states for u in range(2, U + 1) if not test(st, u)], default=None)
        print("largest u at which some state value violates the recurrence:", badu,
              "; states violating at u=5:", [st for st in states if not test(st, 5)])
    allQ = [(st, nm) for st in states for nm in Q[1][st]]
    for u in range(1, U + 1):
        for st in states: assert set(Q[u][st]) == set(Q[1][st])
    badQ = {}
    for st, nm in allQ:
        bu = max([u for u in range(3, U + 1) if Q[u][st][nm] != 5*Q[u-1][st][nm] - 5*Q[u-2][st][nm]], default=None)
        badQ[(st, nm)] = bu
    print("#(state,action) pairs (all attempts incl. optional OUT):", len(allQ), "; largest u where some Q violates the recurrence:", max(v for v in badQ.values() if v is not None))
    # solver A's S(u) with A's action set (forced probe of the first U top), window U0..U1
    def A_actions(st):
        for i in range(2):
            if st[i] and st[i][-1] == 'U': return ['OUT%d' % (i+1)]
        return [nm for nm in Q[1][st]]
    def S_window(U0, U1, actset):
        fails = 0
        for st in states:
            names = actset(st)
            if not names: continue
            for u in range(U0, U1 + 1):
                for nm in names:
                    if Q[u][st][nm] != 5*Q[u-1][st][nm] - 5*Q[u-2][st][nm]: fails += 1
            ok = False
            for astar in names:
                good = True
                for nm in names:
                    for u in range(U0, U1 + 1):
                        d0 = Q[u-1][st][astar] - Q[u-1][st][nm]; d1 = Q[u][st][astar] - Q[u][st][nm]
                        if d0 < 0 or d1 < 0 or not ge_lam2(d1, d0): good = False
                if good: ok = True; break
            if not ok: fails += 1
        return fails
    for w in ((5, 8), (4, 7), (3, 6), (5, 6), (6, 7)):
        print("A-style S(u) on window %s: forced-probe action set failures=%d ; full action set failures=%d" % (w, S_window(w[0], w[1], A_actions), S_window(w[0], w[1], lambda st: list(Q[1][st]))))
    # is A's forced probe really optimal (probe value == state value) for every state with a U top, all u?
    print("forced probe attains the max in every state, every u<=%d:" % U, all(V[u][st] == Q[u][st][A_actions(st)[0]] for st in states if A_actions(st) and A_actions(st)[0].startswith('OUT') for u in range(U + 1) if (u > 0 or True) and A_actions(st)[0] in Q[u][st]))
    # solver B's version: fixed policy = first maximiser at u=130, slacks
    PI = {st: max(Q[130][st], key=lambda nm: Q[130][st][nm]) for st in states if Q[130][st]}
    nsl = 0; minneg = None; firstzero = 0
    for st in PI:
        for nm in Q[1][st]:
            if nm == PI[st]: continue
            nsl += 1
            s = [None] + [Q[u][st][PI[st]] - Q[u][st][nm] for u in range(1, U + 1)]
            if any(x < 0 for x in s[1:]): minneg = (st, nm)
            z = max([r for r in range(1, U - 1) if s[r+2] - 5*s[r+1] + 5*s[r] != 0], default=0)
            firstzero = max(firstzero, z)
            assert ge_lam2(s[9], s[8])
    print("B-style: slack pairs=%d ; any negative slack (u<=%d): %s ; largest r where slack(r+2)-5slack(r+1)+5slack(r)!=0: %d" % (nsl, U, minneg, firstzero))
    print("policy at u=130 is a maximiser at every u in 1..%d:" % U, all(V[u][st] == Q[u][st][PI[st]] for st in PI for u in range(1, U + 1)))
if what == 'rec':
    st, V, Q = dp(3, 70); e = ('',)*3; a = [V[n][e] for n in range(71)]
    bad = [n for n in range(5, 71) if a[n] != 11*a[n-1] - 40*a[n-2] + 58*a[n-3] - 30*a[n-4]]
    print("k=3 order-4 recurrence violated at n =", bad)
    st, V, Q = dp(4, 90); e = ('',)*4; a = [V[n][e] for n in range(91)]
    c = [23, -216, 1089, -3245, 5866, -6173, 3032, 300, -923, 245]
    bad = [n for n in range(11, 91) if a[n] != sum(c[i]*a[n-1-i] for i in range(10))]
    print("k=4 order-10 recurrence violated at n =", bad)
    st, V, Q = dp(1, 40); print("k=1 N=2^(n-1) to n=40:", all(V[n][('',)] == 2**(n-1) for n in range(1, 41)))
