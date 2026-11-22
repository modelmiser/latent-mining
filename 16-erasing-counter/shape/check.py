#!/usr/bin/env python3
"""Checker written from the definition in chain/3-question.txt.

usage: check.py FILE
FILE has lines:  tree <tree>   /  start <bits, position 1 first>  /  claim_length <N>  /  claim_L <L>
Tree syntax: (i T0 T1) internal node reading position i (1-based), 0-child first; [i] leaf writing i; [] empty leaf.
Checks: the tree is legal (no index twice on a path, leaf writes only an index of its own path),
reports its depth, computes L(T) (L(leaf)=1, L(node)=L(1-child)*(L(0-child)+1)),
iterates f from the start state and verifies the first return happens after exactly claim_length steps,
with all intermediate states pairwise distinct. Exit 0 only if every claim holds; prints VIOLATES_P1 if N > L.
"""
import sys


def parse(s):
    toks = s.replace('(', ' ( ').replace(')', ' ) ').replace('[', ' [ ').replace(']', ' ] ').split()
    pos = 0

    def node():
        nonlocal pos
        t = toks[pos]
        if t == '[':
            pos += 1
            if toks[pos] == ']':
                pos += 1
                return ('leaf', None)
            w = int(toks[pos])
            assert toks[pos + 1] == ']', 'bad leaf'
            pos += 2
            return ('leaf', w)
        assert t == '(', 'bad token ' + t
        i = int(toks[pos + 1])
        pos += 2
        a = node()
        b = node()
        assert toks[pos] == ')', 'missing )'
        pos += 1
        return ('node', i, a, b)

    t = node()
    assert pos == len(toks), 'trailing tokens'
    return t


def legal(t, path=()):
    """returns depth; raises on an illegal tree"""
    if t[0] == 'leaf':
        if t[1] is not None and t[1] not in path:
            raise ValueError('leaf writes %d, not on its path %s' % (t[1], path))
        return 0
    i = t[1]
    if i < 1:
        raise ValueError('index < 1')
    if i in path:
        raise ValueError('index %d twice on a path' % i)
    return 1 + max(legal(t[2], path + (i,)), legal(t[3], path + (i,)))


def L(t):
    if t[0] == 'leaf':
        return 1
    return L(t[3]) * (L(t[2]) + 1)


def step(t, s):
    """s: tuple of bits, s[i-1] is position i"""
    P = []
    while t[0] == 'node':
        P.append(t[1])
        t = t[3] if s[t[1] - 1] else t[2]
    o = list(s)
    for i in P:
        o[i - 1] = 0
    if t[1] is not None:
        o[t[1] - 1] = 1
    return tuple(o)


def main():
    d = {}
    for line in open(sys.argv[1]):
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        k, v = line.split(None, 1)
        d[k] = v
    t = parse(d['tree'])
    try:
        depth = legal(t)
    except ValueError as e:
        print('ILLEGAL TREE:', e)
        return 1
    s0 = tuple(int(c) for c in d['start'])
    assert all(c in (0, 1) for c in s0)

    def maxidx(t):
        return 0 if t[0] == 'leaf' else max(t[1], maxidx(t[2]), maxidx(t[3]))
    if len(s0) < maxidx(t):
        print('start state too short')
        return 1
    Lt = L(t)
    N = int(d['claim_length'])
    seen = {s0}
    s = s0
    ok = True
    for n in range(1, N + 1):
        s = step(t, s)
        if n < N:
            if s in seen:
                print('FAIL: state repeats after %d steps (claimed first return at %d)' % (n, N))
                ok = False
                break
            seen.add(s)
    if ok and s != s0:
        print('FAIL: no return to the start after %d steps' % N)
        ok = False
    print('depth', depth, 'indices', maxidx(t), 'L(T)', Lt)
    if 'claim_L' in d and int(d['claim_L']) != Lt:
        print('FAIL: claimed L %s but L(T) = %d' % (d['claim_L'], Lt))
        ok = False
    if 'claim_depth' in d and int(d['claim_depth']) != depth:
        print('FAIL: claimed depth %s but depth = %d' % (d['claim_depth'], depth))
        ok = False
    if ok:
        print('OK: cycle of length exactly', N)
        print('VIOLATES_P1 (N > L(T))' if N > Lt else 'consistent with P1 (N <= L(T))')
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
