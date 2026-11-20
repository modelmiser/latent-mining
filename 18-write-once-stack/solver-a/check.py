#!/usr/bin/env python3
"""Independent witness checker, straight from the definition.
usage: check.py witness.txt [--root-empty]
File: 'n N t T' line, then lines 'config Q E(PUSH0) E(PUSH1) E(POP)'; unlisted configs: Q=EMPTY, E=stay.
Enumerates every legal operation sequence of length <= T and checks Q after every operation.
--root-empty additionally requires Q(0^n)=EMPTY (alternative reading)."""
import sys, itertools

def load(fn):
    Q, E = {}, {}
    n = t = None
    for line in open(fn):
        line = line.split('#')[0].split()
        if not line: continue
        if line[0] == 'n':
            n, t = int(line[1]), int(line[3]); continue
        c, qv, e0, e1, ep = line
        assert c not in Q, "duplicate config line"
        Q[c] = qv; E[c] = (e0, e1, ep)
    return n, t, Q, E

def check(n, t, Q, E, root_empty=False):
    for c, (qv, es) in ((c, (Q[c], E[c])) for c in Q):
        assert len(c) == n and set(c) <= set('01'), "bad config " + c
        assert qv in ('0', '1', 'EMPTY')
        for e in es:
            assert len(e) == n and set(e) <= set('01')
            assert all(not (a == '1' and b == '0') for a, b in zip(c, e)), "E not monotone at " + c
    if root_empty:
        assert Q.get('0' * n, 'EMPTY') == 'EMPTY', "Q(0^n) != EMPTY"
    count = 0
    for L in range(t + 1):
        for seq in itertools.product(range(3), repeat=L):
            c = '0' * n; st = []; ok = True
            for op in seq:
                if op == 2:
                    if not st: ok = False; break
                    st.pop()
                else:
                    st.append(str(op))
                c = E[c][op] if c in E else c
                want = st[-1] if st else 'EMPTY'
                got = Q.get(c, 'EMPTY')
                if got != want:
                    return False, "fails on %s: Q=%s, top=%s" % (seq, got, want)
            if ok: count += 1
    return True, "survives all %d legal sequences of length <= %d" % (count, t)

if __name__ == '__main__':
    n, t, Q, E = load(sys.argv[1])
    ok, msg = check(n, t, Q, E, '--root-empty' in sys.argv)
    print(sys.argv[1], "n=%d t=%d" % (n, t), "OK" if ok else "REJECTED", msg)
    sys.exit(0 if ok else 1)
