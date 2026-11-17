#!/usr/bin/env python3
"""Independent (naive) re-implementation. Physical state = (tuple of actual values
x_{t-d}..x_t, rest flag). No sign compression, no bitsets.
  verify.py relay d B          -> which of the 27 maps hold B (BFS)
  verify.py clockwit d B LOG   -> check the clock witness printed in LOG holds B
  verify.py clocklb d B        -> naive exhaustive search: no clock holds B? (memo DFS)
  verify.py histwit d B AUT    -> check automaton file holds B (product reachability)
  verify.py histlb d B DEPTH   -> naive depth-bounded belief game: controller dies by DEPTH?
  verify.py sim d B kind arg   -> random + greedy adversary simulation of a witness
"""
import sys, itertools, random, re
sys.setrecursionlimit(1000000)
sg = lambda v: (v > 0) - (v < 0)

def step(st, u, B):
    """all adversary successors of st under control u; None if any leaves band"""
    win, rest = st
    out = []
    for w in ((0,) if rest else (-1, 0, 1)):
        nx = win[-1] + u + w
        if abs(nx) > B: return None
        out.append((win[1:] + (nx,), w != 0))
    return out

def init(d): return ((0,) * (d + 1), False)
def obs(st): return sg(st[0][0])

def relay(d, B):
    good = []
    for f in itertools.product((-1, 0, 1), repeat=3):  # f[o+1]
        seen = {init(d)}; todo = [init(d)]; ok = True
        while todo and ok:
            s = todo.pop()
            nxt = step(s, f[obs(s) + 1], B)
            if nxt is None: ok = False; break
            for n in nxt:
                if n not in seen: seen.add(n); todo.append(n)
        if ok: good.append(''.join('-0+'[v + 1] for v in f))
    return good

def parse_clock(log, d, B):
    txt = open(log).read()
    m = re.search(r"d=%d B=%d (?:prune=0 )?CLOCK FEASIBLE.*?\n.*?\n\s*prefix:(.*?)\n\s*cycle:(.*?)\n" % (d, B), txt, re.S)
    conv = lambda s: [tuple('-0+'.index(c) - 1 for c in tok) for tok in s.split()]
    return conv(m.group(1)), conv(m.group(2))

def clockwit(d, B, log):
    pre, cyc = parse_clock(log, d, B)
    def f_at(i): return pre[i] if i < len(pre) else cyc[(i - len(pre)) % len(cyc)]
    def idx(i): return i if i < len(pre) else len(pre) + (i - len(pre)) % len(cyc)
    seen = {(init(d), 0)}; todo = [(init(d), 0)]
    while todo:
        s, i = todo.pop()
        nxt = step(s, f_at(i)[obs(s) + 1], B)
        if nxt is None: return False, len(seen)
        for n in nxt:
            k = (n, idx(i + 1))
            if k not in seen: seen.add(k); todo.append(k)
    return True, len(seen)

def clocklb(d, B):
    """True iff NO clock holds B. Iterative DFS on frozensets, exact revisit = cycle."""
    start = frozenset([init(d)])
    status = {start: 1}
    maps = list(itertools.product((-1, 0, 1), repeat=3))
    def succs(S):
        res = []; seenT = set()
        for f in maps:
            T = set(); ok = True
            for s in S:
                nxt = step(s, f[obs(s) + 1], B)
                if nxt is None: ok = False; break
                T.update(nxt)
            if ok:
                T = frozenset(T)
                if T not in seenT: seenT.add(T); res.append(T)
        return res
    stack = [(start, iter(succs(start)))]
    while stack:
        S, it = stack[-1]
        adv = False
        for T in it:
            stt = status.get(T)
            if stt == 2: continue
            if stt == 1: return False, len(status)
            status[T] = 1; stack.append((T, iter(succs(T)))); adv = True; break
        if not adv: status[S] = 2; stack.pop()
    return True, len(status)

def load_aut(fn):
    L = open(fn).read().split('\n')
    d, B, n = map(int, L[0].split())
    nodes = [tuple(map(int, L[1 + i].split())) for i in range(n)]
    return d, B, nodes

def histwit(d, B, fn):
    d2, B2, nodes = load_aut(fn); assert (d, B) == (d2, B2)
    seen = {(init(d), 0)}; todo = [(init(d), 0)]
    while todo:
        s, q = todo.pop()
        nxt = step(s, nodes[q][0], B)
        if nxt is None: return False, len(seen)
        for n in nxt:
            q2 = nodes[q][1 + obs(n) + 1]
            if q2 < 0: return False, len(seen)
            if (n, q2) not in seen: seen.add((n, q2)); todo.append((n, q2))
    return True, len(seen)

def histlb(d, B, depth):
    """True iff controller cannot survive `depth` ticks with bound B (naive belief game)."""
    memo = {}
    def win(bel, k):  # controller can survive k more ticks from belief
        if k == 0: return True
        key = (bel, k)
        if key in memo: return memo[key]
        r = False
        for u in (-1, 0, 1):
            parts = {}; ok = True
            for s in bel:
                nxt = step(s, u, B)
                if nxt is None: ok = False; break
                for n in nxt: parts.setdefault(obs(n), set()).add(n)
            if ok and all(win(frozenset(p), k - 1) for p in parts.values()): r = True; break
        memo[key] = r
        return r
    return (not win(frozenset([init(d)]), depth)), len(memo)

def sim(d, B, kind, arg, trials=300, T=3000, seed=1):
    rng = random.Random(seed)
    if kind == 'relay':
        f = tuple('-0+'.index(c) - 1 for c in arg)
        ctrl0 = None; act = lambda q, o, t: (f[o + 1], None)
    elif kind == 'clock':
        pre, cyc = parse_clock(arg, d, B)
        act = lambda q, o, t: ((pre[t] if t < len(pre) else cyc[(t - len(pre)) % len(cyc)])[o + 1], None)
    else:
        _, _, nodes = load_aut(arg)
        def act(q, o, t):
            q = 0 if t == 0 else nodes[q][1 + o + 1]
            return nodes[q][0], q
    worst = 0
    for tr in range(trials):
        xs = [0] * (d + 1); rest = False; q = None
        mode = tr % 4; bias = rng.choice((-1, 1))
        for t in range(T):
            o = sg(xs[-1 - d]); u, q = act(q, o, t)
            if rest: w = 0
            elif mode == 0: w = rng.choice((-1, 0, 1))
            elif mode == 1: w = sg(xs[-1] + u) or bias          # greedy: push outward
            elif mode == 2: w = u if u else rng.choice((-1, 0, 1))  # amplify the controller
            else: w = bias if rng.random() < 0.9 else -bias     # drift, then flip
            if mode == 3 and rng.random() < 0.02: bias = -bias
            rest = (w != 0)
            xs.append(xs[-1] + u + w); xs = xs[-(d + 2):]
            worst = max(worst, abs(xs[-1]))
            if abs(xs[-1]) > B: return False, worst
    return True, worst

if __name__ == '__main__':
    cmd = sys.argv[1]; d = int(sys.argv[2]); B = int(sys.argv[3])
    if cmd == 'relay': print('relay', d, B, relay(d, B))
    elif cmd == 'clockwit': print('clockwit', d, B, clockwit(d, B, sys.argv[4]))
    elif cmd == 'clocklb': print('clocklb(no clock holds B?)', d, B, clocklb(d, B))
    elif cmd == 'histwit': print('histwit', d, B, histwit(d, B, sys.argv[4]))
    elif cmd == 'histlb': print('histlb(controller dies within depth?)', d, B, sys.argv[4], histlb(d, B, int(sys.argv[4])))
    elif cmd == 'sim': print('sim', d, B, sys.argv[4], sim(d, B, sys.argv[4], sys.argv[5]))
