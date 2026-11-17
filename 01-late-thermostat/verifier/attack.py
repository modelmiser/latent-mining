#!/usr/bin/env python3
"""Referee test of solver A's Theorem 3 adversary, implemented as an algorithm.
Controller = automaton {node: (u, {obs: next_node})}, node 0 = state after consuming o_0 = 0.
attack() builds an explicit legal w-sequence following the proof's steps and the result is
judged ONLY by an honest simulation of that sequence (sim), never by the proof's formulas."""
import sys, random
sys.path.insert(0, "verifier")
from game3 import sg, solve_H

class Undefined(Exception): pass

def legal(w):
    return all(v in (-1, 0, 1) for v in w) and all(not (w[i] and w[i + 1]) for i in range(len(w) - 1))

def sim(aut, d, w):
    assert legal(w), "illegal adversary sequence"
    x = [0]; node = 0
    for t in range(len(w)):
        if t > 0:
            o = sg(x[t - d]) if t - d >= 0 else 0
            nxt = aut[node][1].get(o)
            if nxt is None: raise Undefined((t, o, max(abs(v) for v in x)))
            node = nxt
        x.append(x[t] + aut[node][0] + w[t])
    return x

def mx(x): return max(abs(v) for v in x)

def P_w(n): return [1 if t % 2 == 0 else 0 for t in range(n)]

def find_cycle(aut, d, target, cap=2_000_000):
    """run P until joint state repeats or |x| >= target. returns ('hit', x) or ('cycle', mu, per)"""
    x = [0]; node = 0; seen = {}
    t = 0
    while True:
        if t > 0:
            o = sg(x[t - d]) if t - d >= 0 else 0
            node = aut[node][1].get(o)
            if node is None: raise Undefined((t, o))
        key = (node, tuple(x[max(0, t - d):t + 1]), t % 2) if t >= d else None
        if key is not None:
            if key in seen: return ('cycle', seen[key], t - seen[key])
            seen[key] = t
        x.append(x[t] + aut[node][0] + (1 if t % 2 == 0 else 0))
        t += 1
        if abs(x[t]) >= target: return ('hit', t, None)
        if t > cap: raise RuntimeError("no cycle")

def attack(aut, d, target):
    """returns (stage, max|x| achieved by the final explicit play, info)"""
    r = find_cycle(aut, d, target)
    if r[0] == 'hit':
        return ('P', mx(sim(aut, d, P_w(r[1]))), {})
    _, mu, per = r
    assert per % 2 == 0
    N0 = mu + 3 * per + 6 * d + 20
    P = sim(aut, d, P_w(N0))
    # ---- Step 1
    for t in range(d + 1, mu + 2 * per + 2):
        viol = (P[t] <= 1) if d % 2 == 1 else (t % 2 == 1 and P[t] <= 1)
        if viol:
            sigma = t - d - 1 if (t - d - 1) % 2 == 0 else t - d
            w = P_w(t);
            for r_ in range(sigma, t):
                if r_ % 2 == 0: w[r_] = -1
            return ('step1', mx(sim(aut, d, w)), {'t': t})
    # ---- Step 3
    lam = min(P[mu:mu + per])
    T0 = max(mu, 2 * d + 2); T0 += T0 % 2
    omitted = [T0 + 2 * i for i in range(lam - 1)]
    T1 = T0 + 2 * (lam - 1)
    N = T1 + 3 * per + 8 * d + 40
    Pl = sim(aut, d, P_w(N))
    wq = P_w(N)
    for r_ in omitted: wq[r_] = 0
    Q = sim(aut, d, wq)
    pred_ok = all(Q[t] == Pl[t] - sum(1 for r_ in omitted if r_ < t) for t in range(N + 1))
    info = {'lam': lam, 'T0': T0, 'per': per, 'Q_pred_ok': pred_ok}
    cand = [t for t in range(T1 + d + 1, T1 + d + 2 + per) if Q[t] == 1]
    if not cand: return ('FAIL-noQ1', mx(Q), info)
    if d % 2 == 1:
        t = cand[0]
        sigma = t - d - 1 if (t - d - 1) % 2 == 0 else t - d
        w = wq[:t]
        for r_ in range(sigma, t):
            if r_ % 2 == 0: w[r_] = -1
        return ('step4', mx(sim(aut, d, w)), info)
    odd = [t for t in cand if t % 2 == 1]
    if odd:
        t = odd[0]; sigma = t - d - 1
        w = wq[:t]
        for r_ in range(sigma, t):
            if r_ % 2 == 0: w[r_] = -1
        return ('step4', mx(sim(aut, d, w)), info)
    # ---- Step 5: phase shift
    a = T1 + d + 2; a += a % 2
    wh = wq[:]
    for r_ in range(a, N): wh[r_] = 1 if r_ % 2 == 1 else 0
    Qh = sim(aut, d, wh)
    info['Qhat_pred_ok'] = all(Qh[t] == Q[t] - (1 if (t % 2 == 1 and t >= a) else 0) for t in range(N + 1))
    cs = [t for t in range(a + d + 2, a + d + 4 + per) if t % 2 == 0 and Qh[t] == 1]
    if not cs: return ('FAIL-noQhat1', mx(Qh), info)
    s_star = cs[0]; sigma = s_star - d - 1
    w = wh[:s_star]
    for r_ in range(sigma, s_star):
        if r_ % 2 == 1: w[r_] = -1
    return ('step6', mx(sim(aut, d, w)), info)

# ---------- controllers ----------
def load_b(d, B):
    fn = f"solver-b/aut_d{d}_B{B}.txt"
    with open(fn) as f:
        dd, BB, n = map(int, f.readline().split())
        aut = {}
        for i in range(n):
            u, a, b, c = map(int, f.readline().split())
            aut[i] = (u, {o: j for o, j in zip((-1, 0, 1), (a, b, c)) if j >= 0})
    return aut

def rand_moore(rng, k):
    return {i: (rng.choice((-1, 0, 1)), {o: rng.randrange(k) for o in (-1, 0, 1)}) for i in range(k)}

def rand_mealy(rng, k, p):
    out = [[(-o if rng.random() < p else rng.choice((-1, 0, 1))) for o in (-1, 0, 1)] for _ in range(k)]
    nxt = [[rng.randrange(k) for _ in range(3)] for _ in range(k)]
    idx = lambda s, o: s * 3 + (o + 1)
    aut = {}
    for s in range(k):
        for o in (-1, 0, 1):
            aut[idx(s, o)] = (out[s][o + 1], {o2: idx(nxt[s][o + 1], o2) for o2 in (-1, 0, 1)})
    # start node must be 0 = state after consuming o_0 = 0 from state 0: relabel
    st = idx(0, 0)
    perm = {st: 0, 0: st}
    f = lambda n: perm.get(n, n)
    return {f(n): (u, {o: f(j) for o, j in m.items()}) for n, (u, m) in aut.items()}

def mutate(rng, aut, nflip):
    keys = list(aut.keys()); a = {n: (u, dict(m)) for n, (u, m) in aut.items()}
    for n in a:
        for o in (-1, 0, 1):
            if o not in a[n][1]: a[n][1][o] = rng.choice(keys)
    for n in rng.sample(keys, min(nflip, len(keys))):
        a[n] = (rng.choice([v for v in (-1, 0, 1) if v != a[n][0]]), a[n][1])
    return a

def clock_aut():   # half-duty relay: nodes (parity, obs)
    idx = lambda par, o: par * 3 + (o + 1)
    aut = {}
    for par in (0, 1):
        for o in (-1, 0, 1):
            aut[idx(par, o)] = ((-o if par == 0 else 0), {o2: idx(1 - par, o2) for o2 in (-1, 0, 1)})
    st = idx(0, 0); perm = {st: 0, 0: st}; f = lambda n: perm.get(n, n)
    return {f(n): (u, {o: f(j) for o, j in m.items()}) for n, (u, m) in aut.items()}

def relay_aut():
    return {0: (0, {-1: 1, 0: 0, 1: 2}), 1: (1, {-1: 1, 0: 0, 1: 2}), 2: (-1, {-1: 1, 0: 0, 1: 2})}

if __name__ == "__main__":
    rng = random.Random(20261116)
    dmax_b = int(sys.argv[1]) if len(sys.argv) > 1 else 9
    Bof = {1: 1, 2: 3, 3: 3, 4: 5, 5: 5, 6: 7, 7: 7, 8: 9, 9: 9, 10: 11, 11: 11, 12: 13}
    total = fails = 0
    print("== (a) optimal controllers ==")
    for d in range(1, dmax_b + 1):
        target = 2 * (d // 2) + 1
        ctrls = [("solver-b automaton", load_b(d, Bof[d]))]
        if d <= 6:
            win, n, mine = solve_H(d, target); assert win
            ctrls.append(("referee strategy", mine))
        ctrls += [("half-duty clock", clock_aut()), ("relay", relay_aut())]
        for name, aut in ctrls:
            try:
                stage, m, info = attack(aut, d, target)
            except Undefined as e:
                stage, m, info = 'UNDEFINED-TRANSITION', -1, {'e': e.args}
            total += 1; ok = m >= target; fails += not ok
            print(f"d={d} target={target} {name:20s} stage={stage:6s} max|x|={m} {'OK' if ok else '**FAIL**'} {info}")
    print("== (b) random / mutated controllers ==")
    from collections import Counter
    for d in range(1, 9):
        target = 2 * (d // 2) + 1
        base = load_b(d, Bof[d])
        cnt = Counter(); bad = 0; predbad = 0
        for i in range(400):
            kind = i % 4
            if kind == 0: aut = rand_moore(rng, rng.randint(2, 12))
            elif kind == 1: aut = rand_mealy(rng, rng.randint(1, 8), rng.choice((0.5, 0.8, 0.95)))
            else: aut = mutate(rng, base, rng.choice((0, 1, 2, 5)) if kind == 2 else rng.randint(1, max(1, len(base) // 4)))
            stage, m, info = attack(aut, d, target)
            total += 1; cnt[stage] += 1
            if m < target: bad += 1; fails += 1; print("  **FAIL**", d, kind, stage, m, info)
            if info.get('Q_pred_ok') is False or info.get('Qhat_pred_ok') is False: predbad += 1
        print(f"d={d} target={target} n=400 stages={dict(cnt)} fails={bad} formula-mismatch={predbad}")
    print(f"TOTAL controllers={total} FAILS={fails}")
