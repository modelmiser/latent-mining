#!/usr/bin/env python3
"""Tests of PROOFS.md part B (lower bound G(h) >= 5h-32) on the lag rule.
All lags are kept as integers in fifths (lag5 = 5*lag), heights as integers.
Usage: test_rule.py [--D d] [--c c] [--th th] [--hb a] [--quick]
  --D  d : readiness claim tested: 'max lag5 >= d  => some max-lag vertex is ready' (proof: 6)
  --c  c : constant of the top-k bound in fifths (proof: 5), W = {z: 5T_z >= c+1}
  --th th: the rule fires only at lag5 >= th (the rule: 5)
  --hb a : height claim after a round: 5*eta <= t + a (proof: 24); after an add: t + a + 4
Exit status 0 iff no claim was violated."""
import sys, random
from collections import defaultdict

def arg(name, default):
    return int(sys.argv[sys.argv.index(name) + 1]) if name in sys.argv else default
D = arg('--D', 6); C = arg('--c', 5); TH = arg('--th', 5); HB = arg('--hb', 24)
QUICK = '--quick' in sys.argv

HARM = [0.0]
def H(n):
    while len(HARM) <= n: HARM.append(HARM[-1] + 1.0 / len(HARM))
    return HARM[n]

class Viol(Exception): pass

class Game:
    def __init__(self, tie, rng=None):
        self.k = defaultdict(int); self.F = defaultdict(int); self.eta = defaultdict(int)
        self.T5 = defaultdict(int)   # 5*T
        self.t = 0; self.tie = tie; self.rng = rng
        self.maxlag = 0; self.maxexc_round = -10**9; self.maxexc_add = -10**9
        self.max_unready_top = 0; self.sk_checks = 0; self.maxn = 0; self.minslack = 1e9
    def lag5(self, x): return self.T5[x] - 5 * self.F[x]
    def add(self, x):
        self.t += 1; self.k[x] += 1; self.eta[x] += 1
        self.T5[x] += 3; self.T5[x - 1] += 1; self.T5[x + 1] += 1
    def check_add(self):
        t = self.t
        verts = [x for x in self.T5 if self.T5[x] > 0]
        M = max(self.lag5(x) for x in verts)
        tops = [x for x in verts if self.lag5(x) == M]
        if all(self.eta[x] < 2 for x in tops):
            self.max_unready_top = max(self.max_unready_top, M)
            if M >= D: raise Viol(f"(D) max lag5 {M} >= {D} but no vertex of that lag is ready, t={t}")
        for x in list(self.eta):
            e = 5 * self.eta[x] - t
            if e > self.maxexc_add: self.maxexc_add = e
            if e > HB + 4: raise Viol(f"height after add: 5eta - t = {e} > {HB+4} at t={t}")
        return M
    def fire_step(self):
        ready = [x for x in self.eta if self.eta[x] >= 2]
        if not ready: return None
        m = max(self.lag5(x) for x in ready)
        if m < TH: return None
        c = [x for x in ready if self.lag5(x) == m]
        if self.tie == 'L': x = min(c)
        elif self.tie == 'R': x = max(c)
        else: x = self.rng.choice(sorted(c))
        self.F[x] += 1; self.eta[x] -= 2; self.eta[x - 1] += 1; self.eta[x + 1] += 1
        return x
    def check_round(self, do_sk=True):
        t = self.t
        for x in list(self.eta):
            if self.eta[x] < 0: raise Viol("negative height")
            # identity (b): 5 eta = (k 5-sum) + 2 lag5_x - lag5_{x-1} - lag5_{x+1}
            s = sum(self.k[x + d] for d in (-2, -1, 0, 1, 2))
            if 5 * self.eta[x] != s + 2 * self.lag5(x) - self.lag5(x - 1) - self.lag5(x + 1):
                raise Viol("identity (b) fails")
            e = 5 * self.eta[x] - t
            if e > self.maxexc_round: self.maxexc_round = e
            if e > HB: raise Viol(f"height after round: 5eta - t = {e} > {HB} at t={t}")
        lags = []
        for x in self.T5:
            l = self.lag5(x)
            if l < 0: raise Viol("negative lag")
            if l > self.maxlag: self.maxlag = l
            if self.T5[x] >= C + 1: lags.append(l)
            elif l > C: raise Viol("lag above c outside W")
        if do_sk and lags:
            lags.sort(reverse=True); n = len(lags); self.maxn = max(self.maxn, n)
            s = 0; Hn = H(n)
            for k in range(1, n + 1):
                s += lags[k - 1]
                slack = k * (C / 5 + Hn - H(k)) - s / 5
                if slack < self.minslack: self.minslack = slack
                if slack < -1e-9: raise Viol(f"top-k bound fails: k={k} n={n} S_k={s/5} t={t}")
            self.sk_checks += 1

def play(lst, tie, rng=None, sk_every=1):
    g = Game(tie, rng)
    for i, x in enumerate(lst):
        g.add(x); g.check_add(); g.fire_step(); g.check_round(i % sk_every == 0)
    return g

def greedy_list(n, w, tie, rng, mode):
    """adaptive adversary with one-step lookahead on a window of w vertices"""
    g = Game(tie, rng); lst = []
    import copy
    for _ in range(n):
        best = None
        for x in range(w):
            g2 = copy.deepcopy(g); g2.add(x); g2.fire_step()
            if mode == 0: v = max(g2.lag5(z) for z in g2.T5)
            elif mode == 1: v = max(5 * g2.eta[z] for z in list(g2.eta))
            else: v = sum(sorted((g2.lag5(z) for z in g2.T5), reverse=True)[:2])
            v = (v, rng.random())
            if best is None or v > best[0]: best = (v, x)
        x = best[1]; lst.append(x)
        g.add(x); g.check_add(); g.fire_step(); g.check_round()
    return g

def main():
    rng = random.Random(12345)
    tot = dict(maxlag=0, exc_r=-10**9, exc_a=-10**9, unready=0, sk=0, n=0, slack=1e9, lists=0)
    def acc(g):
        tot['maxlag'] = max(tot['maxlag'], g.maxlag); tot['exc_r'] = max(tot['exc_r'], g.maxexc_round)
        tot['exc_a'] = max(tot['exc_a'], g.maxexc_add); tot['unready'] = max(tot['unready'], g.max_unready_top)
        tot['sk'] += g.sk_checks; tot['n'] = max(tot['n'], g.maxn); tot['slack'] = min(tot['slack'], g.minslack)
        tot['lists'] += 1
    try:
        # 1. all lists of length <= L on a window of w vertices (exhaustive), leftmost and rightmost ties
        import itertools
        for w, L in ((1, 40), (2, 12), (3, 8), (5, 6), (7, 5)) if not QUICK else ((2, 8), (3, 6)):
            for lst in itertools.product(range(w), repeat=L):
                for tie in 'LR': acc(play(lst, tie))
        print("exhaustive windows done", tot, flush=True)
        # 2. random lists
        for it in range(300 if QUICK else 3000):
            w = rng.choice((1, 2, 3, 4, 5, 6, 7, 9, 12, 20, 40)); n = rng.choice((20, 50, 100, 200, 400))
            lst = [rng.randrange(w) for _ in range(n)]
            if it % 3 == 0:  # clustered: favour the middle
                lst = [min(w - 1, max(0, int(rng.gauss(w / 2, 1.2)))) for _ in range(n)]
            acc(play(lst, rng.choice('LRX'), rng))
        print("random done", tot, flush=True)
        # 3. adaptive greedy adversaries
        for it in range(6 if QUICK else 60):
            acc(greedy_list(rng.choice((60, 150, 300)), rng.choice((3, 5, 7, 9, 13)), rng.choice('LRX'), rng, it % 3))
        print("greedy done", tot, flush=True)
        # 4. the long lists of the repository built to make the lag large (read-only)
        if not QUICK:
            base = '25-throttled-chips/'
            for f, every in (('lag/list_34_L.txt', 1), ('lag/list_34_R.txt', 1), ('constant/list_n512.txt', 5)):
                lst = [int(s) for s in open(base + f).read().split()]
                g = play(lst, 'L', None, every); acc(g)
                print(f, "len", len(lst), "maxlag5", g.maxlag, "5eta-t round/add", g.maxexc_round, g.maxexc_add,
                      "n", g.maxn, "min slack", round(g.minslack, 4), flush=True)
    except Viol as v:
        print("VIOLATION:", v); print(tot); sys.exit(1)
    print(f"PASS  D={D} c={C} th={TH} hb={HB}: lists={tot['lists']} max lag5={tot['maxlag']} "
          f"max 5eta-t after round={tot['exc_r']} after add={tot['exc_a']} "
          f"largest max-lag5 with no ready top vertex={tot['unready']} top-k checks={tot['sk']} max n={tot['n']} "
          f"min slack of top-k bound={tot['slack']:.4f}")

main()
