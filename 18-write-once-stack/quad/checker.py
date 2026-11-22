#!/usr/bin/env python3
"""Checker written from the definition in chain/3-question.txt only.

A scheme is two black-box functions on configurations (ints, bit i = cell i):
    Qf(c)      -> 'E', 0 or 1
    Ef(c, op)  -> configuration, op in ('P0','P1','POP')
check(n, t, Qf, Ef) explores EVERY legal operation sequence of length <= t
(prefix-shared: the set of (configuration, stack) pairs after each number of
steps is the exact image of all sequences of that length, because the play is
a deterministic function of the sequence).  After every operation it checks
  * the new configuration is inside {0,1}^n and a superset of the old one,
  * Q(new configuration) == top of stack, or 'E' when the stack is empty.
Nothing is checked before the first operation (literal reading); with
strict=True also Q(0^n) == 'E'.
Returns (True, number_of_sequences_checked) or (False, counterexample).
"""


def check(n, t, Qf, Ef, strict=False):
    full = (1 << n) - 1
    if strict and Qf(0) != 'E':
        return False, ('Q(0^n) != EMPTY', [])
    # layer: dict (config, stack) -> (count of sequences, one witness sequence)
    layer = {(0, ()): (1, ())}
    total = 1  # the empty sequence
    for step in range(1, t + 1):
        nxt = {}
        for (c, st), (cnt, seq) in layer.items():
            for op in ('P0', 'P1', 'POP'):
                if op == 'POP':
                    if not st:
                        continue  # illegal
                    st2 = st[:-1]
                else:
                    st2 = st + (0 if op == 'P0' else 1,)
                c2 = Ef(c, op)
                seq2 = seq + (op,)
                if not isinstance(c2, int) or c2 < 0 or c2 > full:
                    return False, ('E leaves {0,1}^n', seq2)
                if c2 & c != c:
                    return False, ('E clears a cell', seq2)
                want = st2[-1] if st2 else 'E'
                got = Qf(c2)
                if got != want or (got == 'E') != (want == 'E'):
                    return False, ('Q=%r, top=%r' % (got, want), seq2)
                key = (c2, st2)
                if key in nxt:
                    nxt[key] = (nxt[key][0] + cnt, nxt[key][1])
                else:
                    nxt[key] = (cnt, seq2)
        layer = nxt
        total += sum(v[0] for v in layer.values())
    return True, total


def count_legal(t):
    """number of legal sequences of length <= t, computed independently"""
    # by depth
    cur = {0: 1}
    tot = 1
    for _ in range(t):
        nx = {}
        for d, c in cur.items():
            nx[d + 1] = nx.get(d + 1, 0) + 2 * c
            if d > 0:
                nx[d - 1] = nx.get(d - 1, 0) + c
        cur = nx
        tot += sum(cur.values())
    return tot


def table_scheme(path):
    """read a scheme file: lines 'config Q E(P0) E(P1) E(POP)' (config = n chars,
    cell 0 first; Q in E/0/1/EMPTY); unlisted configs: Q = E, E = stay.
    Any unparsable non-comment line is an error."""
    Qt, Et, n, t = {}, {}, None, None
    for line in open(path):
        line = line.split('#')[0].strip()
        if not line:
            continue
        f = line.split()
        if f[0] == 'n':
            n, t = int(f[1]), int(f[3])
            continue
        if len(f) != 5:
            raise ValueError('bad line: ' + line)
        cs = [int(x[::-1], 2) for x in (f[0], f[2], f[3], f[4])]
        for x in (f[0], f[2], f[3], f[4]):
            if len(x) != n or set(x) - set('01'):
                raise ValueError('bad config: ' + line)
        q = {'EMPTY': 'E', 'E': 'E', '0': 0, '1': 1}[f[1]]
        if cs[0] in Qt:
            raise ValueError('duplicate config: ' + line)
        Qt[cs[0]] = q
        Et[cs[0]] = {'P0': cs[1], 'P1': cs[2], 'POP': cs[3]}
    return n, t, (lambda c: Qt.get(c, 'E')), (lambda c, op: Et[c][op] if c in Et else c)


if __name__ == '__main__':
    import sys
    n, t, Qf, Ef = table_scheme(sys.argv[1])
    if len(sys.argv) > 2 and sys.argv[2].isdigit():
        t = int(sys.argv[2])
    ok, info = check(n, t, Qf, Ef, strict='strict' in sys.argv)
    print('n=%d t=%d %s %s (legal sequences by formula: %d)' % (
        n, t, 'SURVIVES' if ok else 'FAILS', info, count_legal(t)))
