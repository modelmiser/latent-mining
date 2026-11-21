#!/usr/bin/env python3
"""Independent replay of a list (one integer per line) under Theorem 2's strategy, exact rationals.
Written from the theorem text, not from lagadv2.cpp.  Tie rule: argv[2] in L (leftmost), R, or H
(highest vertex first).  Prints max lag, max height - t/5 and the round at which each new lag
record was set."""
import sys, heapq
from fractions import Fraction as Fr
lst = [int(x) for x in open(sys.argv[1])]
tie = sys.argv[2] if len(sys.argv) > 2 else 'L'
k = {}; F = {}; eta = {}
def lag(x): return Fr(3*k.get(x,0) + k.get(x-1,0) + k.get(x+1,0), 5) - F.get(x,0)
heap = []   # entries (-lag, tiekey, x); validated lazily
def key(x): return x if tie == 'L' else -x if tie == 'R' else -eta.get(x,0)
def push(x):
    l = lag(x)
    if eta.get(x,0) >= 2 and l >= 1: heapq.heappush(heap, (-l, key(x), x))
maxlag = Fr(0); maxexc = Fr(-10**9); passes = 0
for t, a in enumerate(lst, 1):
    k[a] = k.get(a,0)+1; eta[a] = eta.get(a,0)+1
    for z in (a-1, a, a+1):
        l = lag(z); assert l >= 0
        if l > maxlag: maxlag = l; print("round", t, "lag record", float(l), "at vertex", z)
        push(z)
    maxexc = max(maxexc, eta[a] - Fr(t,5))
    f = None
    while heap:
        l, kk, x = heap[0]
        if eta.get(x,0) >= 2 and lag(x) == -l and key(x) == kk: f = x; break
        heapq.heappop(heap)
    if f is None: passes += 1; continue
    heapq.heappop(heap)
    F[f] = F.get(f,0)+1; eta[f] -= 2
    for z in (f-1, f+1): eta[z] = eta.get(z,0)+1
    for z in (f-1, f, f+1): push(z)
    maxexc = max(maxexc, max(eta[f-1], eta[f+1]) - Fr(t,5))
print("length", len(lst), "tie", tie, "max lag", float(maxlag), "max height - t/5", float(maxexc), "passes", passes)
