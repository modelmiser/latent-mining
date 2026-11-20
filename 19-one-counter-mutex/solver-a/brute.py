#!/usr/bin/env python3
"""brute.py n b s : enumerate EVERY tuple of fully specified automata (no laziness, no symmetry
reduction, add 0 included, crit as add c) and count solutions with checker.check_prog.
Cross-check for synth.cpp on the cases small enough."""
import sys, itertools
from checker import check_prog
n, b, s = map(int, sys.argv[1:4])
non = list(range(1, s))
acts = [('read', list(t)) for t in itertools.product(non, repeat=b)] + [('add', c, t) for c in range(b) for t in non]
procs = []
for crit in range(b):
    for combo in itertools.product(acts, repeat=s - 1):
        d = {1: ('add', crit, 0), 0: combo[0]}
        for k, q in enumerate(range(2, s)): d[q] = combo[k + 1]
        procs.append(d)
cnt = tot = 0
for tup in itertools.product(procs, repeat=n):
    prog = {(i, q): a for i, d in enumerate(tup) for q, a in d.items()}
    tot += 1
    # unreachable states are simply never consulted by the checker
    if check_prog(n, b, s, prog, verbose=False): cnt += 1
print("brute n=%d b=%d s=%d: %d tuples, %d solutions" % (n, b, s, tot, cnt))
