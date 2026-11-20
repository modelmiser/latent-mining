#!/usr/bin/env python3
"""Independent witness checker (shares no code with fast.cpp / slow.py).
  check.py adv  n L file   : file is an Adversary strategy (rank|move|state lines)
  check.py rule n L file   : file is a Rule safe set (one state per line)
FIFO expiry only. State text: balls oldest first, 'receiver:other', bins named by
non-negative integers; any naming is accepted on input and renamed by first appearance.
Exit status 0 and the word OK only if the witness is valid.
"""
import sys


def norm(balls):
    m = {}
    out = []
    for w, o in balls:
        for x in (w, o):
            if x not in m:
                m[x] = len(m)
        out.append((m[w], m[o]))
    return tuple(out)


def parse(txt):
    txt = txt.strip()
    if not txt:
        return ()
    return tuple(tuple(int(x) for x in b.split(':')) for b in txt.split(' '))


def valid_state(st, n):
    """forest on <= n bins, no self loops"""
    p = {}
    def f(x):
        while p.setdefault(x, x) != x:
            x = p[x]
        return x
    for w, o in st:
        if w == o:
            return False
        a, b = f(w), f(o)
        if a == b:
            return False
        p[a] = b
    return len(p) <= n


def maxload(st):
    l = {}
    for w, o in st:
        l[w] = l.get(w, 0) + 1
    return max(l.values()) if l else 0


def comp(st, k):
    c = list(range(k + 2))
    for w, o in st:
        a, b = c[w], c[o]
        if a != b:
            c = [b if x == a else x for x in c]
    return c


def legal_offers(st, n):
    """all legal offers up to renaming of untouched bins: labels k, k+1 are fresh bins"""
    k = 1 + max([max(b) for b in st], default=-1)
    c = comp(st, k)
    res = []
    for a in range(k):
        for b in range(a + 1, k):
            if c[a] != c[b]:
                res.append((a, b))
        if k < n:
            res.append((a, k))
    if k + 2 <= n:
        res.append((k, k + 1))
    return res


def check_adv(n, L, fn):
    table = {}
    for line in open(fn):
        if line.startswith('#'):
            continue
        r, mv, s = line.rstrip('\n').split('|')
        st = parse(s)
        assert st == norm(st), 'state not in normal form'
        assert st not in table, 'duplicate state'
        table[st] = (int(r), mv)
    assert () in table, 'empty state missing'
    seen = set()
    stack = [()]
    while stack:
        st = stack.pop()
        if st in seen:
            continue
        seen.add(st)
        assert valid_state(st, n)
        r, mv = table[st]          # KeyError = strategy has no answer here
        if maxload(st) >= L:
            continue               # Adversary has achieved L
        assert r > 0, 'rank 0 without load L'
        if mv == 'E':
            assert st, 'expire on empty state'
            nxt = [norm(st[1:])]
        else:
            t, a, b = mv.split()
            assert t == 'O'
            a, b = int(a), int(b)
            assert (min(a, b), max(a, b)) in legal_offers(st, n), 'illegal offer'
            nxt = [norm(st + ((a, b),)), norm(st + ((b, a),))]   # both Rule answers
        for t in nxt:
            assert t in table, 'successor not covered'
            assert table[t][0] < r, 'rank does not decrease'
            stack.append(t)
    return len(seen)


def check_rule(n, L, fn):
    S = set()
    for line in open(fn):
        if line.startswith('#'):
            continue
        st = parse(line)
        assert st == norm(st)
        S.add(st)
    assert () in S, 'empty state missing'
    for st in S:
        assert valid_state(st, n)
        assert maxload(st) < L, 'load L present'
        if st:
            assert norm(st[1:]) in S, 'not closed under expire'
        for a, b in legal_offers(st, n):
            assert norm(st + ((a, b),)) in S or norm(st + ((b, a),)) in S, f'offer {a},{b} at {st} has no safe answer'
    return len(S)


if __name__ == '__main__':
    kind, n, L, fn = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), sys.argv[4]
    try:
        c = check_adv(n, L, fn) if kind == 'adv' else check_rule(n, L, fn)
    except (AssertionError, KeyError) as e:
        print('REJECTED', fn, repr(e))
        sys.exit(1)
    print(f'OK {kind} n={n} L={L} states_checked={c} {fn}')
