#!/usr/bin/env python3
"""Finite checks for Theorem 5 of NOTE.md, T(n) <= (n-1)/3 (rule L: flip a shortest path from an endpoint x
with 2 h(x) + eps(x) <= 2 h(y) + eps(y)).  All quantities are in THIRDS, so everything is an integer.

A vertex c has height h(c) and a flag eps(c) = 0 if its two slot values are equal, 1 otherwise.
  gamma(c) = 3 h(c) + 1 + eps(c).
Weight of a vertex v:
  no out-neighbour:                        0
  one out-neighbour c:                     eps(c) if h(c) = 0,  1 if h(c) >= 1
  two, of heights p <= q, flags e_p, e_q:  3p - 1 + e_p + e_q   if q = p
                                           3p + 1 + e_p + e_q   if q = p + 1
                                           3p + 2 + e_p         if q >= p + 2
Phi = sum of the weights.

Checked here, over ranges given by R (default 14):
 (0) the table above equals  1 + sum gamma(c) - max(gamma(c), g(slots of v)),  g(P,Q) = 3Q + (4, 2, 1 for Q-P = 0, 1, >= 2);
 (M) the weight never decreases when the slots of an out-neighbour increase;
 (P) the three path inequalities with the carry pi:
       start:    gain(x)    >= 2 + pi(state_0)
       interior: gain(w_i)  >= 3 + pi(state_i) - pi(state_(i-1))
       end:      gain(w_k)  >= 1 - pi(state_(k-1))
     state after w_i = (o, e, f): o = new height of w_i minus old height, e = its new slots are equal,
     f = the old slots of the next path vertex are equal;  pi = -1 if f is false and (o, e) is one of
     (0, False), (1, True), (1, False), (2, True);  pi = 0 otherwise.
     Also the case k = 0 (nothing flipped): gain >= 0.
Exit code 1 on any failure.     usage: third_check.py [R]"""
import sys
R = int(sys.argv[1]) if len(sys.argv) > 1 else 14
if R < 4: sys.exit('R must be at least 4 (smaller ranges do not contain the finite case lists)')
fail = 0
def g3(P, Q): d = Q - P; return 3 * Q + (4 if d == 0 else 2 if d == 1 else 1)
def weight(ch):                                  # ch: list of (height, eps) of the out-neighbours
    if not ch: return 0
    if len(ch) == 1: return ch[0][1] if ch[0][0] == 0 else 1
    (p, ep), (q, eq) = sorted(ch)
    if q == p: return 3 * p - 1 + ep + eq
    if q == p + 1: return 3 * p + 1 + ep + eq
    return 3 * p + 2 + ep
def weight_max(ch):                              # the same thing as a formula with a maximum
    if not ch: return 0
    gc = [3 * h + 1 + e for h, e in ch]; sl = sorted([h for h, e in ch] + [-1] * (2 - len(ch)))
    return 1 + sum(gc) - max(gc + [g3(*sl)])
# (0)
n0 = 0
for h1 in range(0, R):
    for e1 in (0, 1):
        n0 += 1; fail += weight([(h1, e1)]) != weight_max([(h1, e1)])
        for h2 in range(0, R):
            for e2 in (0, 1): n0 += 1; fail += weight([(h1, e1), (h2, e2)]) != weight_max([(h1, e1), (h2, e2)])
print('(0) table = formula: %d classes, failures so far %d' % (n0, fail))
# (M): slots (P, Q) of an out-neighbour -> (height, eps) = (P + 1, [P != Q]); raise one slot by one
def he(s): return (s[0] + 1, int(s[0] != s[1]))
SL = [(p, q) for q in range(-1, R) for p in range(-1, q + 1)]
def ups(s):
    p, q = s; yield (p, q + 1)
    if p + 1 <= q: yield (p + 1, q)
nm = 0
for s in SL:
    for s2 in ups(s):
        nm += 1; fail += weight([he(s2)]) < weight([he(s)])
        for t in SL: nm += 1; fail += weight([he(s2), he(t)]) < weight([he(s), he(t)])
print('(M) monotone: %d comparisons, failures so far %d' % (nm, fail))
# (P)
def pi(o, e, f): return -1 if (not f) and (o, e) in ((0, False), (1, True), (1, False), (2, True)) else 0
B = (True, False); L = 3 * R; npth = 0
# interior: old out-neighbours w_(i+1) (height L, old slots equal? = ne) and c (height L + al, flag ce);
#           new out-neighbours c and w_(i-1) (height L + 2 + o, new slots equal? = e1)
for o in range(0, R):
    for e1 in B:
        for al in range(0, R):
            for ce in (0, 1):
                for ne in B:
                    before = weight([(L, int(not ne)), (L + al, ce)]); after = weight([(L + al, ce), (L + 2 + o, int(not e1))])
                    o2 = min(al, o + 2); e2 = (al == o + 2)
                    npth += 1; fail += (after - before) < 3 + pi(o2, e2, ne) - pi(o, e1, al == 0)
# start: x has old out-neighbours w_1 (height L, ne) and c (height L + al, ce); new: c and y (height L + 1 + be, flag ye).
#        rule L: if be = 0 and y's slots are equal then x's slots are equal, that is al = 0.
for al in range(0, R):
    for ce in (0, 1):
        for be in range(0, R):
            for ye in B:
                if be == 0 and ye and al != 0: continue
                for ne in B:
                    before = weight([(L, int(not ne)), (L + al, ce)]); after = weight([(L + al, ce), (L + 1 + be, int(not ye))])
                    o0 = min(al, be + 1); e0 = (al == be + 1)
                    npth += 1; fail += (after - before) < 2 + pi(o0, e0, ne)
# end: w_k has height 0; old out-neighbours: none, or z (height zh, flag ze); new: the same plus w_(k-1) (height 1 + o, e1)
for o in range(0, R):
    for e1 in B:
        for z in [None] + [(zh, ze) for zh in range(0, R) for ze in (0, 1)]:
            old = [] if z is None else [z]
            npth += 1; fail += weight(old + [(1 + o, int(not e1))]) - weight(old) < 1 - pi(o, e1, z is None)
# k = 0: x has height 0 and gains the out-neighbour y (height be, flag); rule L: if be = 0 and y has no out-neighbour then x has none
for be in range(0, R):
    for ye in B:
        for z in [None] + [(zh, ze) for zh in range(0, R) for ze in (0, 1)]:
            if be == 0 and ye and z is not None: continue
            old = [] if z is None else [z]
            npth += 1; fail += weight(old + [(be, int(not ye))]) - weight(old) < 0
print('(P) path inequalities: %d cases, failures so far %d' % (npth, fail))
print('ALL OK' if not fail else 'FAILED')
sys.exit(1 if fail else 0)
