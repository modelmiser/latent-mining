#!/usr/bin/env python3
"""Checker for an Adversary strategy table, written from the game definition
(chain/3-question.txt).  Shares no code with gen.py.

usage: check_adv.py L n table.txt

A position is the list of present balls, NEWEST FIRST; a ball is (receiver, other bin).
Bins are named by first appearance when the list is read newest-first (receiver before
other); bins holding no present ball and offered in no present ball have no name: they are
interchangeable, and there are n - m of them when m bins are named.  Two positions that
differ by renaming bins are the same position of the game (the rules do not mention names).

The table gives one move per position: 'E' or 'O a b'; a name >= m means an unnamed bin
(two different names >= m mean two different unnamed bins).

ACCEPT means: starting from the empty position, if Adversary plays the table's move, then
whatever Rule answers, (1) the position reached either has a bin of load L or is in the table,
(2) every table move used is legal on n bins, and (3) no position can repeat along a play
(the graph of reachable positions is acyclic).  The graph is finite, so every play ends, and
it can only end with a bin of load L.  Hence Adversary forces load L on n bins.
"""
import sys

L = int(sys.argv[1]); n = int(sys.argv[2]); path = sys.argv[3]


def parse_state(txt):
    balls = []
    for tok in txt.split():
        r, o = tok.split('.')
        balls.append((int(r), int(o)))
    return tuple(balls)


def text(s):
    return ' '.join('%d.%d' % b for b in s)


def renamed(balls):
    """rename bins by first appearance, newest ball first, receiver before other"""
    name = {}
    out = []
    for (r, o) in balls:
        for x in (r, o):
            if x not in name:
                name[x] = len(name)
        out.append((name[r], name[o]))
    return tuple(out)


def component_of(balls):
    """map bin -> representative of its connected component in the offer graph;
    also verifies that the offer graph is a forest"""
    rep = {}

    def root(x):
        rep.setdefault(x, x)
        while rep[x] != x:
            x = rep[x]
        return x
    for (r, o) in balls:
        a, b = root(r), root(o)
        if a == b:
            raise SystemExit('REJECT: offer graph has a cycle in %r' % (balls,))
        rep[a] = b
    return root


table = {}
with open(path) as f:
    for line in f:
        line = line.rstrip('\n')
        st, mv = line.split('|')
        s = parse_state(st)
        if renamed(s) != s:
            raise SystemExit('REJECT: line not in canonical naming: ' + line)
        if st in table:
            raise SystemExit('REJECT: duplicate position: ' + line)
        table[st] = mv        # keyed by the canonical text (memory); parsed again when used

WHITE, GREY, BLACK = 0, 1, 2
colour = {}
maxbins = 0
nwins = 0


def successors(s):
    """positions after the table's move at s (list); a winning answer is returned as None"""
    global maxbins, nwins
    if text(s) not in table:
        raise SystemExit('REJECT: position not in table: %r' % (s,))
    mv = table[text(s)]
    m = 1 + max([max(b) for b in s], default=-1)        # number of named bins
    if m > n:
        raise SystemExit('REJECT: more than n bins in %r' % (s,))
    if mv == 'E':
        if not s:
            raise SystemExit('REJECT: Expire on the empty position')
        return [renamed(s[:-1])]                         # the oldest ball is the last one
    parts = mv.split()
    if len(parts) != 3 or parts[0] != 'O':
        raise SystemExit('REJECT: bad move %r' % mv)
    a, b = int(parts[1]), int(parts[2])
    if a == b or a < 0 or b < 0:
        raise SystemExit('REJECT: offer of equal or negative bins at %r' % (s,))
    unnamed_used = len({x for x in (a, b) if x >= m})
    if m + unnamed_used > n:
        raise SystemExit('REJECT: offer needs more than n bins at %r' % (s,))
    maxbins = max(maxbins, m + unnamed_used)
    root = component_of(s)
    if a < m and b < m and root(a) == root(b):
        raise SystemExit('REJECT: offer inside one component at %r: %s' % (s, mv))
    # an unnamed bin is alone in its component; two different unnamed bins are different components
    out = []
    for (r, o) in ((a, b), (b, a)):
        load = 1 + sum(1 for (x, _) in s if x == r)
        if load >= L:
            nwins += 1
            out.append(None)
        else:
            t = renamed(((r, o),) + s)
            component_of(t)                              # forest check of the new position
            out.append(t)
    return out


start = ()
colour[text(start)] = GREY
stack = [(start, iter(successors(start)))]
while stack:
    s, it = stack[-1]
    nxt = None
    for t in it:
        if t is None:
            continue
        c = colour.get(text(t), WHITE)
        if c == GREY:
            raise SystemExit('REJECT: a play can return to position %r' % (t,))
        if c == WHITE:
            nxt = t
            break
    if nxt is None:
        colour[text(s)] = BLACK
        stack.pop()
    else:
        colour[text(nxt)] = GREY
        stack.append((nxt, iter(successors(nxt))))

for key in colour:
    s = parse_state(key)
    if any(sum(1 for (x, _) in s if x == r) >= L for r in {x for (x, _) in s}):
        raise SystemExit('REJECT: internal: stored position with load L')
print('ACCEPT L=%d n=%d: reachable positions=%d (table lines=%d), winning answers=%d, most bins in use=%d'
      % (L, n, len(colour), len(table), nwins, maxbins))
