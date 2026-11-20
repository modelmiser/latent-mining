#!/usr/bin/env python3
"""Slow independent solvers.
(1) brute(n,t): enumerate EVERY (Q,E) on n cells (feasible for n<=2) and test with the definition.
(2) lazy(n,t): depth-first over the history tree in plain DFS order, assigning E(c,op) lazily, no
    weight pruning, no symmetry breaking, no compatibility lemma: only Q-conflicts prune.
usage: slow.py [root_empty 0/1]"""
import sys, itertools
sys.setrecursionlimit(100000)
ROOT_EMPTY = len(sys.argv) > 1 and sys.argv[1] == '1'

def seqs(t):
    out = []
    for L in range(t + 1):
        for seq in itertools.product(range(3), repeat=L):
            d = 0; ok = True
            for op in seq:
                d += -1 if op == 2 else 1
                if d < 0: ok = False; break
            if ok: out.append(seq)
    return out

def survives(n, t, Q, E, S):
    for seq in S:
        c = 0; st = []
        for op in seq:
            if op == 2: st.pop()
            else: st.append(op)
            c = E[c][op]
            if Q[c] != (st[-1] if st else 2): return False
    return True

def brute(n, t):
    N = 1 << n; S = seqs(t)
    sup = [[d for d in range(N) if d & c == c] for c in range(N)]
    per_c = [list(itertools.product(sup[c], repeat=3)) for c in range(N)]
    for E in itertools.product(*per_c):
        for Q in itertools.product(range(3), repeat=N):
            if ROOT_EMPTY and Q[0] != 2: continue
            if survives(n, t, Q, E, S): return True
    return False

def lazy(n, t):
    N = 1 << n
    E = {}; Q = {}
    if ROOT_EMPTY: Q[0] = 2
    # explicit list of pending (config, stack, depth) in DFS order
    def go(todo):
        if not todo: return True
        c, st, d = todo[-1]; rest = todo[:-1]
        if d == t: return go(rest)
        # expand this node: all three ops, one after another
        return expand(c, st, d, 0, rest)
    def expand(c, st, d, op, rest):
        if op == 3: return go(rest)
        if op == 2 and not st: return expand(c, st, d, 3, rest)
        st2 = st[:-1] if op == 2 else st + (op,)
        top = st2[-1] if st2 else 2
        if (c, op) in E:
            cands = [E[(c, op)]]; fixed = True
        else:
            cands = [x for x in range(N) if x & c == c]; fixed = False
        for c2 in cands:
            if c2 in Q and Q[c2] != top: continue
            newq = c2 not in Q
            if newq: Q[c2] = top
            if not fixed: E[(c, op)] = c2
            if expand(c, st, d, op + 1, rest + [(c2, st2, d + 1)]): return True
            if not fixed: del E[(c, op)]
            if newq: del Q[c2]
        return False
    return go([(0, (), 0)])

if __name__ == '__main__':
    print("root_empty =", ROOT_EMPTY)
    for n in (1, 2):
        for t in range(0, n + 2):
            print("brute n=%d t=%d %s" % (n, t, brute(n, t)), flush=True)
    for n in (1, 2, 3, 4):
        for t in range(0, n + 1):
            print("lazy  n=%d t=%d %s" % (n, t, lazy(n, t)), flush=True)
