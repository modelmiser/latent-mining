#!/usr/bin/env python3
# Referee's witness checker (own code).  usage: wcheck.py FILE...
# Cop STRATEGIES are checked in the game AS WRITTEN: an explicit robber vertex r is carried along, every robber
# start and every robber move is followed, glimpse answers are the true ones; the product graph
# (strategy position, cop, robber, glimpses) must be acyclic and every path must end in capture.
# (For solver A's tables the strategy position is the cop's own bookkeeping key (j,c,S); it is used only for lookup.)
# LOSING certificates are checked as closed sets in the belief game.
import sys, re
sys.setrecursionlimit(1000000)
class T:
    def __init__(s, h, base):
        s.h = h; s.n = 2 ** (h + 1) - 1; s.base = base
        s.verts = list(range(base, s.n + base))
        par = (lambda v: v // 2) if base == 1 else (lambda v: (v - 1) // 2)
        s.nb = {v: [] for v in s.verts}
        for v in s.verts:
            if v != base: s.nb[v].append(par(v)); s.nb[par(v)].append(v)
        s.ALL = sum(1 << v for v in s.verts)
        s.nbm = {v: (1 << v) | sum(1 << u for u in s.nb[v]) for v in s.verts}
        s.br = {}
        for c in s.verts:
            for u in s.nb[c]:
                seen = {u}; st = [u]
                while st:
                    x = st.pop()
                    for y in s.nb[x]:
                        if y != c and y not in seen: seen.add(y); st.append(y)
                s.br[(c, u)] = sum(1 << v for v in seen)
    def expand(s, S1, c2):
        R = 0; x = S1
        while x:
            low = x & -x; R |= s.nbm[low.bit_length() - 1]; x ^= low
        return R & ~(1 << c2)
    def toward(s, c, r):
        for u in s.nb[c]:
            if s.br[(c, u)] >> r & 1: return u
    def answers(s, G, c, S):
        if G == 'E': return {v: 1 << v for v in s.verts if S >> v & 1}
        return {u: S & s.br[(c, u)] for u in s.nb[c] if S & s.br[(c, u)]}
class Reject(Exception): pass
def need(cond, msg):
    if not cond: raise Reject(msg)

def product_check(start_positions, stepfn):
    # generic acyclicity + longest path over product states; stepfn(state) -> list of successor states ([] = captured)
    done = {}; onstack = set()
    def run(st):
        if st in done: return done[st]
        need(st not in onstack, "cycle: robber can evade for ever at %r" % (st,))
        onstack.add(st)
        succ = stepfn(st)
        r = 1 + max([run(x) for x in succ], default=0)
        onstack.discard(st); done[st] = r; return r
    worst = max(run(st) for st in start_positions)
    return worst, len(done)

def check_A(fn):
    txt = open(fn).read().split('\n'); head = txt[0]
    m = re.search(r'game ([ED]) h=(\d+) k=(\d+)', head); G, h, K = m.group(1), int(m.group(2)), int(m.group(3))
    t = T(h, 1); rows = [l.split() for l in txt[1:] if l.strip() and not l.startswith('#')]
    if 'start=' in head:
        start = int(re.search(r'start=(\d+)', head).group(1)); tab = {}
        for r in rows:
            key = (int(r[0]), int(r[1]), int(r[2], 16))
            tab[key] = ('M', int(r[4])) if r[3] == 'M' else ('G', {int(a.split(':')[0]): int(a.split(':')[1]) for a in r[4:]})
        def stepfn(st):
            j, c, S, r = st
            need((j, c, S) in tab, "no table entry for %r" % ((j, c, S),))
            need(S >> r & 1 and r != c, "cop's bookkeeping lost the real robber at %r" % (st,))
            act = tab[(j, c, S)]
            if act[0] == 'M': c2 = act[1]; A = S; j2 = j
            else:
                need(j >= 1, "glimpse without budget at %r" % ((j, c, S),))
                a = r if G == 'E' else t.toward(c, r)
                need(a in act[1], "true answer %d not handled at %r" % (a, (j, c, S)))
                c2 = act[1][a]; A = (1 << r) if G == 'E' else S & t.br[(c, a)]; j2 = j - 1
            need(c2 == c or c2 in t.nb[c], "illegal cop move")
            if c2 == r: return []
            S2 = t.expand(A & ~(1 << c2), c2)
            return [(j2, c2, S2, r2) for r2 in t.nb[r] + [r] if r2 != c2]
        N, cnt = product_check([(K, start, t.ALL & ~(1 << start), r) for r in t.verts if r != start], stepfn)
        return "ACCEPT strategy %s(h=%d,k=%d): every robber caught within %d rounds (real-game product states %d)" % (G, h, K, N, cnt)
    else:
        L = {(int(r[0]), int(r[1]), int(r[2], 16)): int(r[3]) for r in rows}
        for c in t.verts: need((K, c, t.ALL & ~(1 << c)) in L, "start %d not covered" % c)
        for (j, c, S), bad in L.items():
            need(S and not S >> c & 1 and 0 <= j <= K, "bad state")
            for c2 in t.nb[c] + [c]:
                S1 = S & ~(1 << c2); need(S1, "capture possible at %r" % ((j, c, S),))
                need((j, c2, t.expand(S1, c2)) in L, "blind move leaves the set at %r" % ((j, c, S),))
            if j > 0:
                ans = t.answers(G, c, S); need(bad in ans, "listed answer illegal at %r" % ((j, c, S),)); A = ans[bad]
                for c2 in t.nb[c] + [c]:
                    A1 = A & ~(1 << c2); need(A1, "capture after glimpse at %r" % ((j, c, S),))
                    need((j - 1, c2, t.expand(A1, c2)) in L, "glimpse move leaves the set at %r" % ((j, c, S),))
        return "ACCEPT losing %s(h=%d,k=%d): closed set of %d states" % (G, h, K, len(L))

def check_B(fn):
    lines = [l.split() for l in open(fn) if l.strip()]
    kind, G, h, K = lines[0][0], lines[0][1], int(lines[0][2]), int(lines[0][3]); t = T(h, 0)
    if kind == 'STRATEGY':
        node = {}; start = None
        for r in lines[1:]:
            if r[0] == 'START': start = r
            else: node[int(r[1])] = r
        def follow(spec, c, r, j):       # start of a round, cop c, robber r (r != c); -> next product state
            if spec[0] == 'NOGL': return (int(spec[1]), c, r, j)
            need(spec[0] == 'GL', "bad spec"); need(j >= 1, "glimpse without budget")
            tab = {int(a.split(':')[0]): int(a.split(':')[1]) for a in spec[1:]}
            a = r if G == 'E' else t.toward(c, r)
            need(a in tab, "true answer %d not handled" % a)
            return (tab[a], c, r, j - 1)
        def stepfn(st):
            i, c, r, j = st
            need(i in node, "missing node %d" % i); row = node[i]; need(row[2] == 'MOVE', "bad node")
            c2 = int(row[3]); need(c2 == c or c2 in t.nb[c], "illegal cop move")
            if c2 == r: return []
            need(row[4] != 'WIN', "node %d claims capture but robber at %d is free" % (i, r))
            return [follow(row[4:], c2, r2, j) for r2 in t.nb[r] + [r] if r2 != c2]
        c0 = int(start[1])
        N, cnt = product_check([follow(start[2:], c0, r, K) for r in t.verts if r != c0], stepfn)
        return "ACCEPT strategy %s(h=%d,k=%d): every robber caught within %d rounds (real-game product states %d)" % (G, h, K, N, cnt)
    else:
        # L = states "cop at c about to MOVE (glimpse decision for this round already taken), j glimpses left, belief S"
        L = set()
        for r in lines[1:]:
            c, j, S = int(r[0]), int(r[1]), int(r[2], 16)
            need(S and not S >> c & 1 and 0 <= j <= K and c in t.nb, "bad state"); L.add((c, j, S))
        def round_start_lost(c, S, j):   # cop may move blind, or glimpse (adversary picks the answer) and then move
            if (c, j, S) not in L: return False
            return j == 0 or any((c, j - 1, P) in L for P in t.answers(G, c, S).values())
        for c in t.verts: need(round_start_lost(c, t.ALL & ~(1 << c), K), "opening at %d not covered" % c)
        for (c, j, S) in L:
            for c2 in t.nb[c] + [c]:
                S1 = S & ~(1 << c2); need(S1, "capture possible at %r" % ((c, j, S),))
                need(round_start_lost(c2, t.expand(S1, c2), j), "not closed at %r move %d" % ((c, j, S), c2))
        return "ACCEPT losing %s(h=%d,k=%d): closed set of %d states" % (G, h, K, len(L))

for fn in sys.argv[1:]:
    try:
        first = open(fn).readline()
        res = check_A(fn) if first.startswith('#') else check_B(fn)
    except Reject as e: res = "REJECT: %s" % e
    except Exception as e: res = "REJECT (malformed): %r" % e
    print(fn.split('latent-mining-11/')[-1], "->", res[:230])
