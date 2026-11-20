#!/usr/bin/env python3
"""checker.py -- independent, slow, direct checker for mutual-exclusion automata.

File format (plain text):
  n N b B s S
  p I q J read t_0 ... t_{B-1}     (I = 1..N, J = state; 0 = rem, 1 = crit)
  p I q J add C T
  p I q J unreached                (state claimed never reached)
A successor -1 in a read means "claimed never needed".
Checks: structural rules, state count <= S, safety, progress (as in the problem).
Exit 0 and prints OK iff a solution; otherwise prints the reason (+ a trace).
Progress: a violation exists iff there is a reachable configuration c, with
nonempty active set S and no crit, lying on a closed walk that uses only
crit-free configurations with active set S and contains a step of every
process in S.  (After T nobody can return to rem, since rem is entered only
from crit; so the active set is constant on the recurrent part.)
"""
import sys
from collections import deque

def load(fn):
    n = b = s = None
    prog = {}
    for line in open(fn):
        w = line.split('#')[0].split()
        if not w: continue
        if w[0] == 'n':
            n, b, s = int(w[1]), int(w[3]), int(w[5]); continue
        assert w[0] == 'p' and w[2] == 'q'
        i, q = int(w[1]) - 1, int(w[3])
        assert (i, q) not in prog, "duplicate state"
        if w[4] == 'read':
            t = [int(x) for x in w[5:]]; assert len(t) == b
            prog[(i, q)] = ('read', t)
        elif w[4] == 'add':
            prog[(i, q)] = ('add', int(w[5]) % b, int(w[6]))
        else:
            assert w[4] == 'unreached'
    return n, b, s, prog

def check(fn, verbose=True):
    return check_prog(*load(fn), verbose=verbose)

def check_prog(n, b, s, prog, verbose=True):
    def fail(msg):
        if verbose: print("FAIL:", msg)
        return False
    # structural rules
    for (i, q), a in prog.items():
        if not (0 <= i < n and 0 <= q < s): return fail("state out of range")
        succs = [x for x in a[1] if x != -1] if a[0] == 'read' else [a[2]]
        for t in succs:
            if not (0 <= t < s): return fail("successor out of range")
            if q == 1 and t != 0: return fail("crit successor must be rem")
            if q != 1 and t == 0: return fail("rem as successor of non-crit state")
        if q == 1 and a[0] == 'read' and -1 in a[1]: return fail("crit read incomplete")
    init = (tuple([0] * n), 0)
    par = {init: None}
    dq = deque([init]); edges = {}
    while dq:
        c = dq.popleft(); st, R = c
        if sum(1 for x in st if x == 1) >= 2:
            tr = []; x = c
            while x is not None: tr.append(x); x = par[x] and par[x][0]
            return fail("SAFETY: two in crit at %s; schedule %s" % (c, sched(par, c)))
        for i in range(n):
            a = prog.get((i, st[i]))
            if a is None: return fail("needed state undefined: process %d state %d (config %s)" % (i + 1, st[i], c))
            if a[0] == 'read':
                t = a[1][R]; R2 = R
                if t == -1: return fail("needed read successor undefined: process %d state %d value %d" % (i + 1, st[i], R))
            else:
                t = a[2]; R2 = (R + a[1]) % b
            c2 = (st[:i] + (t,) + st[i + 1:], R2)
            edges[(c, i)] = c2
            if c2 not in par: par[c2] = (c, i); dq.append(c2)
    reach = list(par)
    # progress
    for S in range(1, 1 << n):
        mem = [i for i in range(n) if (S >> i) & 1]
        nodes = set(c for c in reach if all((c[0][i] != 0) == bool((S >> i) & 1) for i in range(n)) and 1 not in c[0])
        fw = {c: [(i, edges[(c, i)]) for i in mem if edges[(c, i)] in nodes] for c in nodes}
        bw = {c: [] for c in nodes}
        for c in nodes:
            for i, d in fw[c]: bw[d].append(c)
        done = set()
        for c in nodes:
            if c in done: continue
            def bfs(adj, proj):
                seen = {c}; q = deque([c])
                while q:
                    x = q.popleft()
                    for y in adj[x]:
                        y = proj(y)
                        if y not in seen: seen.add(y); q.append(y)
                return seen
            scc = bfs(fw, lambda e: e[1]) & bfs(bw, lambda e: e)
            done |= scc
            labels = set(i for x in scc for i, d in fw[x] if d in scc)
            if labels == set(mem):
                return fail("PROGRESS: crit-free recurrent set, active processes %s, %d configs, e.g. %s; reached by schedule %s"
                            % ([i + 1 for i in mem], len(scc), sorted(scc)[:12], sched(par, c)))
    if verbose: print("OK: n=%d b=%d, states used per process <= %d, %d reachable configurations" % (n, b, s, len(reach)))
    return True

def sched(par, c):
    out = []
    while par[c] is not None:
        out.append(par[c][1] + 1); c = par[c][0]
    return ''.join(map(str, reversed(out)))

if __name__ == '__main__':
    ok = True
    for fn in sys.argv[1:]:
        print(fn, end=': ')
        ok = check(fn) and ok
    sys.exit(0 if ok else 1)
