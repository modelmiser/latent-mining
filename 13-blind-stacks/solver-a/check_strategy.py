#!/usr/bin/env python3
"""Independent checker (imports nothing of the solver).  Plays a policy table
against EVERY arrival order under the raw rules of the problem and counts the
orders beaten.  Usage: check_strategy.py policy.txt [lost_orders_out.txt]
Exit status 0 iff the count equals the claimed_beaten in the file header."""
import sys
from itertools import permutations
pol = {}; k = n = claimed = None
for line in open(sys.argv[1]):
    if line.startswith("#"):
        for t in line[1:].split():
            a, b = t.split("=")
            if a == "k": k = int(b)
            if a == "n": n = int(b)
            if a == "claimed_beaten": claimed = int(b)
        continue
    a, u, m = line.split()
    pol[(a, int(u))] = m
def play(order):
    stacks = [[] for _ in range(k)]      # items = arrival indices
    refused = set()                      # items refused since last successful OUT
    arrived = 0; nxt = 1                 # nxt = smallest number not yet output
    while True:
        # normal form: attempt OUT on every top not yet refused in this phase
        progress = True
        while progress:
            progress = False
            for i in range(k):
                if stacks[i] and stacks[i][-1] not in refused:
                    it = stacks[i][-1]
                    if order[it] == nxt:          # legal OUT: executed
                        stacks[i].pop(); nxt += 1; refused = set()
                    else:                         # refused
                        refused.add(it)
                    progress = True
        if nxt == n + 1: return True
        key = "|".join("".join("K" if x in refused else "U" for x in s) or "-" for s in stacks)
        m = pol.get((key, n - arrived))
        if m is None: return False                # strategy gives up
        if m == "IN":
            if len(stacks[0]) >= 2 or arrived >= n: return False   # refused forever
            stacks[0].append(arrived); arrived += 1
        else:
            i = int(m[1:]) - 1
            if not stacks[i] or len(stacks[i + 1]) >= 2: return False
            stacks[i + 1].append(stacks[i].pop())
lost = []; beaten = 0
for order in permutations(range(1, n + 1)):
    if play(order): beaten += 1
    else: lost.append(order)
print("k=%d n=%d beaten=%d of %d claimed=%s %s" % (k, n, beaten, beaten + len(lost), claimed, "OK" if beaten == claimed else "MISMATCH"))
if len(sys.argv) > 2:
    with open(sys.argv[2], "w") as fh:
        fh.write("# k=%d n=%d arrival orders NOT beaten by %s (%d)\n" % (k, n, sys.argv[1], len(lost)))
        for o in lost: fh.write(" ".join(map(str, o)) + "\n")
sys.exit(0 if beaten == claimed else 1)
