#!/usr/bin/env python3
"""Referee checker, written from the problem statement.
usage: vcheck.py FILE MODE [T_OVERRIDE]
MODE: L = nothing checked before first op; S = also require Q(start) == top(init stack).
Accepts both solvers' formats. Returns 0 iff scheme survives every legal sequence of <= t ops.
Monotonicity is checked for EVERY listed E entry. Simulation keeps the full real stack."""
import sys
OPS = ('PUSH0', 'PUSH1', 'POP')
def parse(path):
    Q = {}; E = {}; n = t = None; init = ''
    for raw in open(path):
        line = raw.split('#')[0].split()
        if not line: continue
        if line[0] == 'n':
            n = int(line[1])
            if len(line) >= 4 and line[2] == 't': t = int(line[3])
        elif line[0] == 't': t = int(line[1])
        elif line[0] == 'init': init = '' if line[1] == '-' else line[1]
        elif line[0] == 'Q':
            if line[1] in Q: raise ValueError('dup Q ' + line[1])
            Q[line[1]] = line[2]
        elif line[0] == 'E':
            if (line[1], line[2]) in E: raise ValueError('dup E')
            if line[2] not in OPS: raise ValueError('bad op')
            E[(line[1], line[2])] = line[3]
        else:
            if len(line) != 5: raise ValueError('bad line ' + raw)
            c, q, e0, e1, ep = line
            if c in Q: raise ValueError('dup config ' + c)
            Q[c] = q; E[(c, 'PUSH0')] = e0; E[(c, 'PUSH1')] = e1; E[(c, 'POP')] = ep
    return n, t, init, Q, E
def verify(n, t, init, Q, E, mode):
    if n is None or t is None: return 'no n/t'
    if set(init) - set('01'): return 'bad init'
    def okc(c): return len(c) == n and set(c) <= set('01')
    for c, q in Q.items():
        if not okc(c): return 'bad config ' + c
        if q not in ('0', '1', 'EMPTY'): return 'bad Q value'
    for (c, op), d in E.items():
        if not okc(c) or not okc(d): return 'bad config in E'
        for a, b in zip(c, d):
            if a == '1' and b != '1': return 'not monotone: %s %s %s' % (c, op, d)
    def q(c): return Q.get(c, 'EMPTY')
    def top(s): return s[-1] if s else 'EMPTY'
    start = '0' * n
    if mode == 'S' and q(start) != top(init): return 'Q(start) wrong'
    nseq = [0]
    def rec(c, st, left):
        nseq[0] += 1
        if left == 0: return None
        for op in OPS:
            if op == 'POP':
                if not st: continue
                st2 = st[:-1]
            else: st2 = st + op[4]
            c2 = E.get((c, op), c)
            if q(c2) != top(st2): return 'wrong top after stack %r -> %r at config %s (Q=%s)' % (st, st2, c2, q(c2))
            r = rec(c2, st2, left - 1)
            if r: return r
        return None
    r = rec(start, init, t)
    return r if r else ('OK', nseq[0])
if __name__ == '__main__':
    n, t, init, Q, E = parse(sys.argv[1])
    if len(sys.argv) > 3: t = int(sys.argv[3])
    r = verify(n, t, init, Q, E, sys.argv[2])
    if isinstance(r, tuple):
        print('OK', sys.argv[1], 'n=%d t=%d init=%r mode=%s seqs=%d' % (n, t, init, sys.argv[2], r[1])); sys.exit(0)
    print('REJECT', sys.argv[1], 'n=%d t=%d' % (n, t), r); sys.exit(1)
