#!/usr/bin/env python3
"""Mechanisation of the hand proofs "b = 2 / b = 3 admit no solution for three processes".

Each proof is constructive: for ANY three automata it names a schedule that violates safety or
progress.  refute(auts, b, flavour) follows the written proof step by step (flavour 'A' =
solver-a Theorem 1/2, 'B' = solver-b T2/T3) and returns the schedule the proof names.  The
schedule is then judged by validate(), which knows nothing about the proof.  If a step of the
proof did not follow, some automaton would make the builder hit an assertion (GAP) or return a
schedule that validate() rejects.

Automata here are total: state = ('R',[succ]*b) or ('A',c,succ); 0 = rem, 1 = crit.
"""
import os
import random
import sys
from collections import Counter


class Gap(Exception):
    pass


def need(cond, msg):
    if not cond:
        raise Gap(msg)


def act(a, q, r, b):
    st = a[q]
    if st[0] == 'A':
        return st[2], (r + st[1]) % b, st[1]
    return st[1][r], r, 0


class Sim:
    def __init__(self, auts, b, sched=()):
        self.auts, self.b = auts, b
        self.st = [0] * len(auts)
        self.r = 0
        self.sched = []
        for p in sched:
            self.step(p)

    def step(self, p, k=1):
        for _ in range(k):
            self.st[p], self.r, _c = act(self.auts[p], self.st[p], self.r, self.b)
            self.sched.append(p)

    def cfg(self):
        return (tuple(self.st), self.r)

    def to_crit(self, p):
        """p alone until it is in crit.  False if it never gets there."""
        seen = set()
        while self.st[p] != 1:
            key = (self.st[p], self.r)
            if key in seen:
                return False
            seen.add(key)
            self.step(p)
        return True

    def lasso(self, procs):
        """round-robin over procs for ever; returns (prefix, cycle)."""
        seen = {self.cfg(): len(self.sched)}
        while True:
            for p in procs:
                self.step(p)
            c = self.cfg()
            if c in seen:
                return self.sched[:seen[c]], self.sched[seen[c]:]
            seen[c] = len(self.sched)


# ---------------------------------------------------------------- independent judge
def validate(auts, b, verdict):
    n = len(auts)
    st, r = [0] * n, 0

    def stp(p):
        nonlocal r
        s = auts[p][st[p]]
        if s[0] == 'A':
            st[p], r = s[2], (r + s[1]) % b
        else:
            st[p] = s[1][r]

    if verdict[0] == 'SAFETY':
        for p in verdict[1]:
            stp(p)
        return sum(1 for q in st if q == 1) >= 2
    _, prefix, cycle = verdict
    for p in prefix:
        stp(p)
    if not cycle or all(q == 0 for q in st) or 1 in st:
        return False
    start = (tuple(st), r)
    active = {p for p in range(n) if st[p] != 0}
    for p in cycle:
        stp(p)
        if 1 in st:
            return False
        active |= {x for x in range(n) if st[x] != 0}
    return (tuple(st), r) == start and active <= set(cycle)


# ---------------------------------------------------------------- solo analysis
def solo(a, q, r, b):
    """p alone from state q with counter r.  Returns (steps, crit?) where steps is the list of
    (state, counter before, constant added); stops on reaching crit or on a repeat."""
    seen, steps = set(), []
    while q != 1:
        if (q, r) in seen:
            return steps, False
        seen.add((q, r))
        q2, r2, c = act(a, q, r, b)
        steps.append((q, r, c))
        q, r = q2, r2
    return steps, True


def negate(auts, b):
    out = []
    for a in auts:
        na = []
        for s in a:
            if s[0] == 'A':
                na.append(('A', (-s[1]) % b, s[2]))
            else:
                na.append(('R', [s[1][(-v) % b] for v in range(b)]))
        out.append(na)
    return out


def refute(auts, b, flav, tags):
    """returns verdict; appends the name of the proof step that fired to tags."""
    n = 3
    S = lambda sched=(): Sim(auts, b, sched)
    sig, K, A_, Rr = [], [], [], []
    # Lemma 0a / L1(a): solo from the start reaches crit
    for i in range(n):
        steps, ok = solo(auts[i], 0, 0, b)
        if not ok:
            tags.append('0a solo never critical')
            s = S()
            return ('PROGRESS',) + s.lasso([i])
        sig.append(steps)
    # Lemma 0b / L1(b): counter nonzero at solo crit
    for i in range(n):
        r_i = sum(c for _, _, c in sig[i]) % b
        if r_i == 0:
            tags.append('0b solo crit with R=0')
            s = S([i] * len(sig[i]))
            j = (i + 1) % n
            need(s.to_crit(j), '0b: other does not follow sigma')
            return ('SAFETY', s.sched)
        k = next(t for t, x in enumerate(sig[i]) if x[2] != 0)
        K.append(k); A_.append(sig[i][k][2]); Rr.append(r_i)
    U = [sig[i][K[i]][0] for i in range(n)]
    L = [len(sig[i]) for i in range(n)]

    def to_u(s, i):
        s.step(i, K[i])
        need(s.st[i] == U[i], 'to_u: wrong state')

    def crit(s, i, why):
        need(s.to_crit(i), why)

    if b == 2:
        if flav == 'A':
            tags.append('T1')
            s = S(); to_u(s, 0); to_u(s, 1); s.step(0); s.step(1); need(s.r == 0, 'T1 R')
            crit(s, 2, 'T1 p3'); need(s.r == 1, 'T1 r3'); crit(s, 0, 'T1 p1')
        else:
            tags.append('T2')
            i, j, k = 0, 1, 2
            s = S(); to_u(s, j); crit(s, i, 'T2 i'); s.step(j); need(s.r == 0, 'T2 R'); crit(s, k, 'T2 k')
        return ('SAFETY', s.sched)

    need(b == 3, 'b')
    E = [crit_const(auts[i]) for i in range(n)]
    if flav == 'A':
        # Step 1
        if len(set(A_)) > 1:
            tags.append('A1 first adds differ')
            x = A_.index(1); y = A_.index(2); z = 3 - x - y
            s = S(); to_u(s, x); to_u(s, y); s.step(x); s.step(y); need(s.r == 0, 'A1 R')
            crit(s, z, 'A1 z'); need(s.r == Rr[z], 'A1 rz')
            crit(s, x if s.r == 1 else y, 'A1 second')
            return ('SAFETY', s.sched)
        if A_[0] == 2:
            tags.append('neg')
            return refute(negate(auts, b), b, flav, tags)
        # Step 2
        for j in range(n):
            if Rr[j] == 2:
                tags.append('A2 r=2')
                a_, b_ = [x for x in range(n) if x != j]
                s = S(); to_u(s, a_); to_u(s, b_); crit(s, j, 'A2 j'); need(s.r == 2, 'A2 R')
                s.step(a_); s.step(b_); need(s.r == 1, 'A2 R1'); crit(s, a_, 'A2 a')
                return ('SAFETY', s.sched)
        # Step 3
        for j in range(n):
            if E[j] == 0:
                k = (j + 1) % n
                s = S([j] * L[j])
                if s.to_crit(k):
                    tags.append('A3 e=0 safety')
                    return ('SAFETY', s.sched)
                tags.append('A3 e=0 progress')
                s = S([j] * (L[j] + 1)); need(s.cfg() == ((0, 0, 0), 1), 'A3 allrem1')
                return ('PROGRESS',) + s.lasso([k])
            if E[j] == 1:
                base = [j] * (L[j] + 1)
                need(S(base).cfg() == ((0, 0, 0), 2), 'A3 C2')
                rho = []
                for k in range(n):
                    s = S(base)
                    if not s.to_crit(k):
                        tags.append('A3 e=1 tau progress')
                        s = S(base)
                        return ('PROGRESS',) + s.lasso([k])
                    rho.append(s.r)
                for k in range(n):
                    if rho[k] != 1:
                        tags.append('A3 e=1 rho=%d' % rho[k])
                        s = S(base); crit(s, k, 'x'); m = (k + 1) % n
                        crit(s, m, 'A3 rho m')
                        return ('SAFETY', s.sched)
                tags.append('A3 e=1 rho=1')
                s = S(); to_u(s, 0); to_u(s, 1); s.step(0); s.step(1); need(s.r == 2, 'A3 R2')
                crit(s, 2, 'A3 tau3'); need(s.r == 1, 'A3 rho3'); crit(s, 0, 'A3 p1')
                return ('SAFETY', s.sched)
    else:
        # B Step 1: a_j = e_i for all i != j, via L1(c)
        for i in range(n):
            for j in range(n):
                if i != j and A_[j] != Rr[i]:
                    tags.append('B1 a_j != e_i')
                    k = 3 - i - j
                    s = S(); to_u(s, j); crit(s, i, 'B1 i'); s.step(j); need(s.r == 0, 'B1 R')
                    crit(s, k, 'B1 k')
                    return ('SAFETY', s.sched)
        need(len(set(A_ + Rr)) == 1, 'B1: not one common value')
        if A_[0] == 2:
            tags.append('neg')
            return refute(negate(auts, b), b, flav, tags)
        # exit must be add 2: otherwise all-rem with R = 1+c != 0 is reachable
        for p in range(n):
            if E[p] != 2:
                v = (1 + E[p]) % 3
                base = [p] * (L[p] + 1)
                need(S(base).cfg() == ((0, 0, 0), v), 'B1 allrem')
                k = (p + 1) % n
                s = S(base)
                if not s.to_crit(k):
                    tags.append('B1 exit: progress from all-rem R=%d' % v)
                    s = S(base)
                    return ('PROGRESS',) + s.lasso([k])
                i, j = [x for x in range(n) if x != k]
                if v == 1:   # e_i not in E_k
                    tags.append('B1 exit: 1 in E_k')
                    s = S(); crit(s, i, 'x'); need(s.r == 1, 'x'); crit(s, k, 'B1 e_i in E_k')
                else:        # e_i + a_j not in E_k
                    tags.append('B1 exit: 2 in E_k')
                    s = S(); to_u(s, j); crit(s, i, 'x'); s.step(j); need(s.r == 2, 'x'); crit(s, k, 'B1 L1c')
                return ('SAFETY', s.sched)

    need(all(x == 1 for x in A_) and all(x == 1 for x in Rr) and all(x == 2 for x in E), 'normal form')

    # ------------------------------------------------ stages / chains
    # chain[i] = list of dicts {u, type, n1 (steps of 1-run up to and incl. the add 2), v, n0 (steps v -> next u)}
    chain, loopto = [None] * n, [None] * n

    def stage(i, t):  # t is 1-based
        c = chain[i]
        if t <= len(c):
            return c[t - 1]
        lt = loopto[i]
        return c[lt + (t - 1 - lt) % (len(c) - lt)]

    def pump(s, i, t, h):
        """from K(stage t of i) to K(stage t+1 of i), partner h in rem before and after."""
        g = stage(i, t)
        need(s.st[i] == g['u'] and s.st[h] == 0 and s.r == 0, 'pump: precondition')
        if flav == 'A':
            to_u(s, h); s.step(h); need(s.r == 1, 'pump R1'); s.step(i, g['n1'])
        else:
            to_u(s, h); s.step(i); need(s.r == 1, 'pump R1'); s.step(h); need(s.r == 2, 'pump R2'); s.step(i, g['n1'] - 1)
        need(s.st[i] == g['v'] and s.r == 1, 'pump: not at v with R=1')
        s.step(h, L[h] - K[h] - 1)
        need(s.st[h] == 1 and s.r == 1, 'pump: partner not critical')
        s.step(h)
        need(s.st[h] == 0 and s.r == 0, 'pump: partner exit')
        return s

    def walk(s, i, t, h, expect=None):
        need(s.st[i] == 0 and s.r == 0, 'walk: precondition')
        to_u(s, i)
        for m in range(1, t):
            pump(s, i, m, h)
            s.step(i, stage(i, m)['n0'])
        need(s.st[i] == (stage(i, t)['u'] if expect is None else expect) and s.r == 0, 'walk: wrong stage state')
        return s

    def reachK(i, t, h, u):
        s = walk(S(), i, t, h, expect=u)
        need(all(s.st[x] == 0 for x in range(n) if x != i), 'K(u): others not in rem')
        return s

    for i in range(n):
        j, k = (i + 1) % n, (i + 2) % n
        chain[i] = []
        u = U[i]
        while True:
            t = len(chain[i]) + 1
            need(auts[i][u][0] == 'A' and auts[i][u][1] == 1, 'stage state is not add 1')
            steps, got = solo(auts[i], u, 1, b)
            tsteps = steps[1:]
            nz = [x for x, y in enumerate(tsteps) if y[2] != 0]
            if got:
                # Claim A / B(i): 1-run enters crit
                tags.append('4A 1-run enters crit')
                s = reachK(i, t, j, u); crit(s, j, 'x'); need(s.r == 1, 'x'); crit(s, i, 'ClaimA')
                return ('SAFETY', s.sched)
            if flav == 'A':
                if nz and tsteps[nz[0]][2] == 1:
                    tags.append('4B second add is +1')
                    s = reachK(i, t, j, u); to_u(s, j); s.step(j); need(s.r == 1, 'x')
                    s.step(i, nz[0] + 2); need(s.r == 0, 'ClaimB R')
                    crit(s, k, 'ClaimB k'); need(s.r == 1, 'ClaimB R1'); crit(s, j, 'ClaimB j')
                    return ('SAFETY', s.sched)
            else:
                # B(i): R must stay in {1,2} during the whole of tau (h critical, R = 1 + c_p)
                cp = 1
                for x, y in enumerate(tsteps):
                    cp = (cp + y[2]) % 3
                    if cp == 2:
                        tags.append('4B(i) R becomes 0' + ('' if nz and x == nz[0] else ' (later add)'))
                        s = reachK(i, t, j, u); crit(s, j, 'x'); s.step(i, x + 2); need(s.r == 0, 'B(i) R')
                        crit(s, k, 'B(i) q')
                        return ('SAFETY', s.sched)
            if not nz:
                chain[i].append(dict(u=u, type='H'))
                break
            need(tsteps[nz[0]][2] == 2, 'type L add')
            n1 = nz[0] + 2
            v = act(auts[i], tsteps[nz[0]][0], 0, b)[0]
            g = dict(u=u, type='L', n1=n1, v=v)
            chain[i].append(g)
            # Claim C / (iii)
            s = pump(reachK(i, t, j, u), i, t, j)
            need(s.cfg()[0][i] == v and s.cfg()[0][j] == 0 and s.cfg()[0][k] == 0 and s.r == 0, 'ClaimC')
            baseN = list(s.sched)
            # Claim D / (iv)
            steps0, got0 = solo(auts[i], v, 0, b)
            nz0 = [x for x, y in enumerate(steps0) if y[2] != 0]
            if not nz0:
                if not got0:
                    tags.append('4D no crit from N(v): progress')
                    s = S(baseN)
                    return ('PROGRESS',) + s.lasso([i])
                tags.append('4D crit with R=0')
                s = S(baseN); crit(s, i, 'x'); need(s.r == 0, 'x'); crit(s, j, 'ClaimD sigma_j')
                return ('SAFETY', s.sched)
            w = nz0[0]
            if steps0[w][2] == 2 and not os.environ.get('SKIP_CLAIM_D'):
                tags.append('4D first add from v is +2')
                s = S(baseN); s.step(i, w)
                if flav == 'A':
                    to_u(s, j); crit(s, k, 'x'); need(s.r == 1, 'x'); s.step(i); need(s.r == 0, 'x')
                    s.step(j); need(s.r == 1, 'x'); crit(s, j, 'ClaimD j')
                else:
                    crit(s, j, 'x'); need(s.r == 1, 'x'); s.step(i); need(s.r == 0, 'x'); crit(s, k, 'B(iv) q')
                return ('SAFETY', s.sched)
            g['n0'] = w
            u = steps0[w][0]
            prev = [x for x, y in enumerate(chain[i]) if y['u'] == u]
            if prev:
                loopto[i] = prev[0]
                break
    fin = [i for i in range(n) if loopto[i] is None]
    inf = [i for i in range(n) if loopto[i] is not None]
    if len(fin) >= 2:
        i, j = fin[0], fin[1]
        if len(chain[i]) > len(chain[j]):
            i, j = j, i
        k = 3 - i - j
        fi, fj = len(chain[i]), len(chain[j])
        tags.append('6 hold/hold deadlock f=%d,%d' % (min(fi, 3), min(fj, 3)))
        if flav == 'A':
            s = S(); walk(s, i, 1, j); walk(s, j, fj - fi + 1, k)
            for d in range(fi - 1):
                duel(s, i, 1 + d, j, fj - fi + 1 + d, stage)
        else:
            s = S(); walk(s, i, fi, k); walk(s, j, fj, k)
        need(s.st[i] == chain[i][-1]['u'] and s.st[j] == chain[j][-1]['u'] and s.st[k] == 0 and s.r == 0, 'J(f_i,f_j)')
        s.step(i); s.step(j)
        return ('PROGRESS',) + s.lasso([i, j])
    i, j = inf[0], inf[1]
    k = 3 - i - j
    tags.append('7 lockstep livelock')
    s = S(); walk(s, i, 1, j if flav == 'A' else k); walk(s, j, 1, k)
    seen = {s.cfg(): len(s.sched)}
    t = 1
    while True:
        duel(s, i, t, j, t, stage)
        t += 1
        need(s.st[i] == stage(i, t)['u'] and s.st[j] == stage(j, t)['u'], 'duel landing')
        c = s.cfg()
        if c in seen:
            return ('PROGRESS', s.sched[:seen[c]], s.sched[seen[c]:])
        seen[c] = len(s.sched)


def duel(s, i, ti, j, tj, stage):
    gi, gj = stage(i, ti), stage(j, tj)
    need(gi['type'] == 'L' and gj['type'] == 'L', 'duel: not both L')
    need(s.st[i] == gi['u'] and s.st[j] == gj['u'] and s.r == 0, 'duel: precondition')
    s.step(i); s.step(j); need(s.r == 2, 'duel R2')
    s.step(i, gi['n1'] - 2); s.step(j, gj['n1'] - 2); need(s.r == 2, 'duel R2b')
    s.step(i); s.step(j); need(s.r == 0 and s.st[i] == gi['v'] and s.st[j] == gj['v'], 'duel R0')
    s.step(i, gi['n0']); s.step(j, gj['n0']); need(s.r == 0, 'duel end')


def crit_const(a):
    return a[1][1] if a[1][0] == 'A' else 0


