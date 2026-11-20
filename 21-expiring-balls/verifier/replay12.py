#!/usr/bin/env python3
"""Referee replay of solver-a's P6 strategy (load 4 on 12 bins), written from the prose of proofs.txt,
not from verify12.py and not using adv_n7_L3.txt.  The engine enforces the rules of the problem; the strategy
only sees the engine.  Every sequence of Rule answers is explored (one fresh play per leaf).
usage: replay12.py [N] [variant]   variant: ok | noIprime | nojunk | nofifo"""
import sys
N = int(sys.argv[1]) if len(sys.argv) > 1 else 12
VAR = sys.argv[2] if len(sys.argv) > 2 else 'ok'
L = 4
class Win(Exception): pass
class Need(Exception): pass
class Game:
    def __init__(self, answers):
        self.balls = []          # (u, v, w), oldest first
        self.ans = answers; self.i = 0; self.maxballs = 0; self.moves = 0
    def load(self, x): return sum(1 for b in self.balls if b[2] == x)
    def comp(self):
        c = list(range(N))
        def f(x):
            while c[x] != x: x = c[x]
            return x
        for u, v, w in self.balls: c[f(u)] = f(v)
        return [f(x) for x in range(N)]
    def untouched(self):
        t = set()
        for u, v, w in self.balls: t.add(u); t.add(v)
        return [x for x in range(N) if x not in t]
    def offer(self, u, v):
        if not (0 <= u < N and 0 <= v < N and u != v): raise AssertionError('offer outside bins')
        c = self.comp()
        if c[u] == c[v]: raise AssertionError('ILLEGAL offer: same component')
        if self.i >= len(self.ans): raise Need()
        w = (u, v)[self.ans[self.i]]; self.i += 1; self.moves += 1
        self.balls.append((u, v, w)); self.maxballs = max(self.maxballs, len(self.balls))
        if self.load(w) >= L: raise Win()
        return w
    def expire(self, cnt=1):
        for _ in range(cnt):
            if not self.balls: raise AssertionError('ILLEGAL expire: no ball')
            if VAR == 'nofifo': self.balls.pop()      # broken engine on purpose: removes the NEWEST ball
            else: self.balls.pop(0)
            self.moves += 1

def build2(g, b):            # load-2 bin on 4 bins, no expiry
    r1 = g.offer(b[0], b[1]); r2 = g.offer(b[2], b[3]); return g.offer(r1, r2), r1, r2
def merge_all(g, bins):      # binary merging without expiry; returns the final receiver
    heads = list(bins)
    while len(heads) > 1:
        nxt = [g.offer(heads[i], heads[i + 1]) for i in range(0, len(heads) - 1, 2)]
        if len(heads) % 2: nxt.append(heads[-1])
        heads = nxt
    return heads[0]
def strategy(g):
    # Ia: load 3 on bins 0..6 (P5 in words)
    x, r1, r2 = build2(g, [0, 1, 2, 3])
    if x == r2:
        g.expire(1); c = x
    else:
        f = g.offer(4, 5)
        w = g.offer(x, f)
        if w == x: c = None
        else: g.expire(3); c = f
    if c is not None:
        free = [b for b in g.untouched() if b < 7]
        z, _, _ = build2(g, free[:4])
        x = g.offer(c, z)
    nold = len(g.balls)
    # Ib
    fr = g.untouched()
    Y, r1, r2 = build2(g, fr[:4])
    g.offer(x, Y)                       # x receiving is load 4 (engine raises Win)
    g.expire(nold)
    if Y == r2 and VAR != 'nojunk':
        g.expire(1); centre = Y
    elif Y == r2 or VAR == 'noIprime':
        centre = Y
    else:
        fr = g.untouched()
        a, _, _ = build2(g, fr[:4])
        f = g.offer(fr[4], fr[5])
        w = g.offer(a, f)
        if w == a:
            g.offer(a, Y); return       # must have won
        g.offer(f, Y)
        g.expire(len(g.balls) - 3); centre = f
    # II
    z = merge_all(g, g.untouched())
    g.offer(z, centre)

def run_all():
    leaves = 0; maxb = 0; maxm = 0
    ans = []
    while True:
        # extend with zeros until the play ends
        while True:
            g = Game(ans)
            try:
                strategy(g)
                print('FAIL: strategy ended without load', L, 'answers', ans, 'state', g.balls); return False
            except Need:
                ans = ans + [0]; continue
            except Win:
                if g.i != len(ans): raise AssertionError('unused answers')
                break
            except AssertionError as e:
                print('FAIL:', e, 'answers', ans, 'state', g.balls); return False
        leaves += 1; maxb = max(maxb, g.maxballs); maxm = max(maxm, g.moves)
        # next answer string
        while ans and ans[-1] == 1: ans.pop()
        if not ans: break
        ans[-1] = 1
    print(f'OK N={N}: all {leaves} Rule lines end with load {L}; max balls present {maxb}; max moves {maxm}')
    return True
sys.exit(0 if run_all() else 1)
