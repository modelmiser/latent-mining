#!/usr/bin/env python3
"""Checker written from the definition in chain/3-question.txt.

Tree text format (same as upper/PROOFS.md):  (i T0 T1) = internal node with index i, 0-child T0,
1-child T1;  [] = empty leaf;  [i] = leaf writing index i.

usage: sim.py 'TREE' [more trees...]     (or  sim.py -f FILE, one tree per line, '#' comments)
For each tree: legality (per the definition), depth, L(T), every cycle length of f_T (full
functional graph over {0,1}^idx(T)), the longest cycle, and the verdict  longest <= L(T) ?
Exit status 1 if some tree is illegal.
"""
import sys


def parse(txt):
    toks = txt.replace('(', ' ( ').replace(')', ' ) ').replace('[', ' [ ').replace(']', ' ] ').split()
    pos = [0]

    def rec():
        t = toks[pos[0]]; pos[0] += 1
        if t == '[':
            t2 = toks[pos[0]]; pos[0] += 1
            if t2 == ']':
                return ('l', None)
            assert toks[pos[0]] == ']'; pos[0] += 1
            return ('l', int(t2))
        assert t == '(', t
        i = int(toks[pos[0]]); pos[0] += 1
        a = rec(); b = rec()
        assert toks[pos[0]] == ')'; pos[0] += 1
        return ('n', i, a, b)
    tr = rec()
    assert pos[0] == len(toks), 'trailing tokens'
    return tr


def show(t):
    if t[0] == 'l':
        return '[]' if t[1] is None else '[%d]' % t[1]
    return '(%d %s %s)' % (t[1], show(t[2]), show(t[3]))


def legal(t, path=()):
    """no index twice on a path; a leaf writes nothing or an index of its own path"""
    if t[0] == 'l':
        return t[1] is None or t[1] in path
    if t[1] in path:
        return False
    return legal(t[2], path + (t[1],)) and legal(t[3], path + (t[1],))


def depth(t):
    return 0 if t[0] == 'l' else 1 + max(depth(t[2]), depth(t[3]))


def nodes(t):
    return 0 if t[0] == 'l' else 1 + nodes(t[2]) + nodes(t[3])


def L(t):
    return 1 if t[0] == 'l' else L(t[3]) * (L(t[2]) + 1)


def idx(t):
    return set() if t[0] == 'l' else {t[1]} | idx(t[2]) | idx(t[3])


def step(t, s):
    """s: dict index -> bit (only indices of the tree matter). Returns (new dict, leaf)."""
    read = []
    while t[0] == 'n':
        read.append(t[1]); t = t[3] if s[t[1]] else t[2]
    o = dict(s)
    for u in read:
        o[u] = 0
    if t[1] is not None:
        o[t[1]] = 1
    return o, t


def cycles(t):
    """all cycles of f_t on {0,1}^idx(t): list of lists of states (tuples in sorted-index order)"""
    ix = sorted(idx(t)); n = len(ix)
    N = 1 << n

    def dec(c):
        return {ix[j]: (c >> j) & 1 for j in range(n)}

    def enc(d):
        return sum(d[ix[j]] << j for j in range(n))
    nxt = [enc(step(t, dec(c))[0]) for c in range(N)]
    color = [0] * N
    out = []
    for c0 in range(N):
        if color[c0]:
            continue
        path = []; c = c0
        while color[c] == 0:
            color[c] = 1; path.append(c); c = nxt[c]
        if color[c] == 1:
            cyc = path[path.index(c):]
            out.append([dec(x) for x in cyc])
        for x in path:
            color[x] = 2
    return out, ix


def report(txt):
    t = parse(txt)
    ok = legal(t)
    print('tree   :', show(t))
    print('legal  :', ok, ' depth', depth(t), ' internal nodes', nodes(t), ' indices', len(idx(t)))
    if not ok:
        return False
    cs, ix = cycles(t)
    lens = sorted(len(c) for c in cs)
    m = lens[-1]
    print('L(T)   :', L(t))
    print('cycles :', len(cs), ' longest', m, ' lengths>1:', [x for x in lens if x > 1])
    # independent re-walk of the longest cycle, from the definition, to confirm its length
    c = max(cs, key=len); s0 = c[0]; s = step(t, s0)[0]; k = 1
    while s != s0:
        s = step(t, s)[0]; k += 1
    assert k == m
    print('P1     :', 'HOLDS (longest %d <= L %d)' % (m, L(t)) if m <= L(t) else
          'VIOLATED (longest %d > L %d)' % (m, L(t)))
    print()
    return True


if __name__ == '__main__':
    a = sys.argv[1:]
    if a and a[0] == '-f':
        a = [l.strip() for l in open(a[1]) if l.strip() and not l.startswith('#')]
    allok = True
    for x in a:
        allok &= report(x)
    sys.exit(0 if allok else 1)
