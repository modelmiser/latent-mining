#!/usr/bin/env python3
"""Referee's own checker, written from the problem statement.
usage: vcheck.py FILE [FILE...]     (auto-detects solver-a / solver-b file format)
An automaton is a list of states; state = None (undefined), ('R',[succ...]) or ('A',c,succ);
succ -1 = undefined branch.  State 0 = rem, 1 = crit.  Undefined things must be unreachable.
"""
import sys
from itertools import product


def parse(fn):
    toks = []
    for line in open(fn):
        line = line.split('#')[0].split()
        if line:
            toks.append(line)
    if toks[0][0] == 'n':  # solver-a format
        n, b, s = int(toks[0][1]), int(toks[0][3]), int(toks[0][5])
        auts = [dict() for _ in range(n)]
        for w in toks[1:]:
            i, q = int(w[1]) - 1, int(w[3])
            if q in auts[i]:
                raise ValueError('duplicate state')
            if w[4] == 'read':
                t = [int(x) for x in w[5:]]
                if len(t) != b:
                    raise ValueError('read arity')
                auts[i][q] = ('R', t)
            elif w[4] == 'add':
                auts[i][q] = ('A', int(w[5]), int(w[6]))
            elif w[4] == 'unreached':
                auts[i][q] = None
            else:
                raise ValueError('bad line')
        out = []
        for a in auts:
            k = max(a) + 1
            out.append([a.get(q) for q in range(k)])
        return n, b, s, out
    flat = [x for w in toks for x in w]
    pos = 0
    n, b = int(flat[0]), int(flat[1])
    pos = 2
    out = []
    for _ in range(n):
        k = int(flat[pos]); pos += 1
        a = []
        for _ in range(k):
            t = flat[pos]; pos += 1
            if t == 'R':
                a.append(('R', [int(x) for x in flat[pos:pos + b]])); pos += b
            elif t == 'A':
                a.append(('A', int(flat[pos]), int(flat[pos + 1]))); pos += 2
            else:
                raise ValueError('bad token ' + t)
        out.append(a)
    if pos != len(flat):
        raise ValueError('trailing tokens')
    return n, b, None, out


def structural(n, b, s, auts):
    for i, a in enumerate(auts):
        k = len(a)
        if k < 2:
            return 'process %d: needs rem and crit' % i
        if s is not None and k > s:
            return 'process %d: more than s states' % i
        if a[0] is None or a[1] is None:
            return 'process %d: rem/crit undefined' % i
        for q, st in enumerate(a):
            if st is None:
                continue
            if st[0] == 'A':
                if not (0 <= st[1] < b):
                    return 'process %d state %d: add constant out of range' % (i, q)
                succ = [st[2]]
                if st[2] == -1:
                    return 'process %d state %d: add without successor' % (i, q)
            else:
                if len(st[1]) != b:
                    return 'process %d state %d: read arity' % (i, q)
                succ = [x for x in st[1] if x != -1]
            for x in succ:
                if not (0 <= x < k):
                    return 'process %d state %d: successor out of range' % (i, q)
                if q == 0 and x == 0:
                    return 'process %d: rem has rem as successor' % i
                if q == 1 and x != 0:
                    return 'process %d: crit has a non-rem successor' % i
                if q != 1 and x == 0:
                    return 'process %d state %d: non-crit state leads to rem' % (i, q)
            if q == 1 and st[0] == 'R' and len(succ) != b:
                pass  # undefined crit branches are only a problem if reached
    return None


def step(auts, b, cfg, p):
    """cfg = (states tuple, R).  Returns new cfg or None if undefined."""
    st, r = cfg
    a = auts[p][st[p]]
    if a is None:
        return None
    if a[0] == 'A':
        nq, nr = a[2], (r + a[1]) % b
    else:
        nq, nr = a[1][r], r
    if nq == -1:
        return None
    return (st[:p] + (nq,) + st[p + 1:], nr)


def sccs(nodes, edges):
    """Kosaraju.  edges: dict node -> list of (label, node).  Yields lists of nodes."""
    order, seen = [], set()
    for root in nodes:
        if root in seen:
            continue
        seen.add(root)
        stack = [(root, iter(edges[root]))]
        while stack:
            x, it = stack[-1]
            for _, y in it:
                if y not in seen:
                    seen.add(y)
                    stack.append((y, iter(edges[y])))
                    break
            else:
                order.append(x)
                stack.pop()
    rev = {x: [] for x in nodes}
    for x in nodes:
        for _, y in edges[x]:
            rev[y].append(x)
    comp = {}
    for root in reversed(order):
        if root in comp:
            continue
        comp[root] = root
        todo = [root]
        while todo:
            x = todo.pop()
            for y in rev[x]:
                if y not in comp:
                    comp[y] = root
                    todo.append(y)
    return comp


def check(n, b, s, auts):
    """Returns ('OK', info) or (reason, info)."""
    e = structural(n, b, s, auts)
    if e:
        return 'ILLEGAL: ' + e, None
    init = (tuple([0] * n), 0)
    seen = {init}
    todo = [init]
    while todo:
        c = todo.pop()
        if sum(1 for q in c[0] if q == 1) >= 2:
            return 'SAFETY', c
        for p in range(n):
            d = step(auts, b, c, p)
            if d is None:
                return 'UNDEFINED entry is reachable', (c, p)
            if d not in seen:
                seen.add(d)
                todo.append(d)
    for M in range(1, 1 << n):
        mem = [p for p in range(n) if M >> p & 1]
        nodes = [c for c in seen
                 if all(q != 1 for q in c[0])
                 and all((c[0][p] != 0) == bool(M >> p & 1) for p in range(n))]
        ns = set(nodes)
        edges = {c: [] for c in nodes}
        for c in nodes:
            for p in mem:
                d = step(auts, b, c, p)
                if d in ns:
                    edges[c].append((p, d))
        comp = sccs(nodes, edges)
        movers = {}
        for c in nodes:
            for p, d in edges[c]:
                if comp[d] == comp[c]:
                    movers.setdefault(comp[c], set()).add(p)
        for root, mv in movers.items():
            if len(mv) == len(mem):
                return 'PROGRESS', (root, mem)
    return 'OK', len(seen)


if __name__ == '__main__':
    for fn in sys.argv[1:]:
        try:
            n, b, s, auts = parse(fn)
        except Exception as ex:  # noqa
            print('%-60s PARSE ERROR %s' % (fn, ex))
            continue
        res, info = check(n, b, s, auts)
        print('%-60s n=%d b=%d states=%s  %s  %s' % (fn, n, b, [len(a) for a in auts], res, info))
