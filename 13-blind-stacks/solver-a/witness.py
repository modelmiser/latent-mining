#!/usr/bin/env python3
"""Dump an optimal strategy for (k,n) as a plain-text policy table.
Line format:  <stacks> <u> <MOVE>
 stacks: k fields joined by '|', each a string over {U,K} bottom->top ('-' if empty);
         K = item already refused an OUT since the last successful OUT, U = not.
 u: number of unarrived items.  MOVE: IN, or S<i> (SHIFT i, 1-based).
Convention (normal form): before consulting the table, Sorter attempts OUT on every
stack whose top is U (repeat until every top is K)."""
import sys
from fast import make
k = int(sys.argv[1]); n = int(sys.argv[2])
f = make(k)
enc = lambda st: "|".join("".join("UK"[x] for x in s) or "-" for s in st)
seen = {}
def walk(st, u):
    if (st, u) in seen: return
    for i in range(k):
        s = st[i]
        if s and s[-1] == 0:
            seen[(st, u)] = None
            walk(tuple(tuple(0 for _ in (t[:-1] if j == i else t)) for j, t in enumerate(st)), u)
            walk(st[:i] + (s[:-1] + (1,),) + st[i+1:], u)
            return
    opts = []
    if u > 0 and len(st[0]) < 2:
        opts.append(("IN", (st[0] + (0,),) + st[1:], u - 1))
    for i in range(k - 1):
        if st[i] and len(st[i+1]) < 2:
            l = list(st); l[i+1] = st[i+1] + (st[i][-1],); l[i] = st[i][:-1]
            opts.append(("S%d" % (i + 1), tuple(l), u))
    if not opts or f(st, u) == 0:
        seen[(st, u)] = None; return
    m, s2, u2 = max(opts, key=lambda o: f(o[1], o[2]))
    seen[(st, u)] = m
    walk(s2, u2)
walk(tuple(() for _ in range(k)), n)
print("# k=%d n=%d claimed_beaten=%d" % (k, n, f(tuple(() for _ in range(k)), n)))
for (st, u), m in sorted(seen.items(), key=lambda x: (-x[0][1], enc(x[0][0]))):
    if m: print(enc(st), u, m)
