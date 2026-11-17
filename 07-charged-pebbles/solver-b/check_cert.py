#!/usr/bin/env python3
"""Independent checker for an UPPER-BOUND certificate in the REAL game.
Usage: check_cert.py p k n certfile
certfile: one state per line:  'node:charge node:charge ... | handcharge handcharge ...'
The checker verifies (straight from the rules, sharing no code with fast.cpp):
  1. the start state (empty board, p pebbles of charge k in hand) is in the set;
  2. the set is closed under every legal move P, R, F (on the unbounded line);
  3. no state in the set is clean at a node > n.
Then every reachable state is in the set, hence C(p,k) <= n.  Prints OK or the first failure."""
import sys


def parse(line):
    b, h = line.split("|")
    board = tuple(sorted((int(x.split(":")[0]), int(x.split(":")[1])) for x in b.split()))
    hand = tuple(sorted(int(x) for x in h.split()))
    return board, hand


def succ(state):
    board, hand = state
    d = dict(board)
    res = []
    cand = {1} | {i + 1 for i in d}
    for c in set(hand):
        h = list(hand)
        h.remove(c)
        for i in cand:
            if i not in d:      # i-1 is 0 or pebbled by construction
                res.append((tuple(sorted(board + ((i, c),))), tuple(h)))
    for i, c in board:
        rest = tuple(x for x in board if x[0] != i)
        if i == 1 or (i - 1) in d:
            res.append((rest, tuple(sorted(hand + (c,)))))
        elif c >= 1:
            res.append((rest, tuple(sorted(hand + (c - 1,)))))
    return res


def check(p, k, n, lines):
    S = set(parse(l) for l in lines if l.strip() and l[0] != '#')
    if ((), tuple([k] * p)) not in S:
        return "start state missing"
    for s in S:
        if len(s[0]) + len(s[1]) != p:
            return "bad state %r" % (s,)
        if len(s[0]) == 1 and s[0][0][0] > n:
            return "state clean beyond n: %r" % (s,)
        for t in succ(s):
            if t not in S:
                return "not closed: %r -> %r" % (s, t)
    return "OK %d states, closed, no clean state beyond %d" % (len(S), n)


if __name__ == "__main__":
    p, k, n = map(int, sys.argv[1:4])
    r = check(p, k, n, open(sys.argv[4]).read().split("\n"))
    print(r)
    sys.exit(0 if r.startswith("OK") else 1)
