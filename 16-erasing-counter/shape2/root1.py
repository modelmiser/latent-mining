#!/usr/bin/env python3
"""count the states with the root bit set on the longest cycle of each tree in a file"""
import sys
from sim import parse, cycles, legal, depth, show
for l in open(sys.argv[1]):
    if l.strip() and not l.startswith('#'):
        t = parse(l); assert legal(t)
        cs, _ = cycles(t); c = max(cs, key=len)
        print('depth %d  longest cycle %d  states with root bit = 1: %d  with root bit = 0: %d   %s'
              % (depth(t), len(c), sum(s[t[1]] for s in c), sum(1 - s[t[1]] for s in c), show(t)))
