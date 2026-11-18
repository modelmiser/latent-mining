#!/usr/bin/env python3
"""Lemma 8 on abstract path profiles, with no trees.  All quantities are in THIRDS (integers).

A profile is what the flipped path of a round of rule L looks like from the weights' point of view:
  k = h(x), the length of the path x = w_0 -> ... -> w_k;
  for i < k, the out-neighbour c_i of w_i off the path: its height a_i >= k-i-1 and whether its slots are equal;
  the end w_k: no out-neighbour, or one, z, with a height and the same flag;
  the other endpoint y: its height b >= k and the same flag.
Rule L (2 h(x) + eps(x) <= 2 h(y) + eps(y)): if b = k and the slots of y are equal then the slots of x are equal,
that is, a_0 = k-1 when k >= 1, and w_0 has no out-neighbour when k = 0.
For every profile with k <= KMAX, a_i <= k-i-1+SPREAD, b <= k+SPREAD and height of z <= SPREAD, the program
computes the weights of w_0..w_k before and after the round from the definitions (the formula with the maximum,
and the new heights step by step) and checks that the total gain is at least 3k.  It does not use the carry pi.
Exit code 1 on any violation.     usage: third_paths.py KMAX SPREAD     (KMAX >= 0, SPREAD >= 0)"""
import sys, itertools
if len(sys.argv) != 3: sys.exit(__doc__)
KMAX = int(sys.argv[1]); SP = int(sys.argv[2])
if KMAX < 0 or SP < 0: sys.exit('need KMAX >= 0 and SPREAD >= 0')
def g3(P, Q): d = Q - P; return 3 * Q + (4 if d == 0 else 2 if d == 1 else 1)
def gam(h, eq): return 3 * h + (1 if eq else 2)
def w(children, slots):                       # children: list of (height, slots equal?); slots: sorted own slots
    if not children: return 0
    gc = [gam(*c) for c in children]; return 1 + sum(gc) - max(gc + [g3(*slots)])
worst = None; cnt = 0; bad = 0
for k in range(0, KMAX + 1):
    arange = [range(k - i - 1, k - i + SP) for i in range(k)]
    for a in itertools.product(*arange):
        for ce in itertools.product((True, False), repeat=k):
            for b in range(k, k + SP + 1):
                for ye in (True, False):
                    if b == k and ye and k >= 1 and a[0] != k - 1: continue
                    for z in [None] + [(zh, ze) for zh in range(0, SP + 1) for ze in (True, False)]:
                        if k == 0 and b == 0 and ye and z is not None: continue
                        zq = -1 if z is None else z[0]
                        def eq_before(i):          # are the slots of w_i equal, before the round
                            return (a[i] == k - i - 1) if i < k else (z is None)
                        before = 0
                        for i in range(k):
                            before += w([(k - i - 1, eq_before(i + 1)), (a[i], ce[i])], (k - i - 1, a[i]))
                        before += w([] if z is None else [z], (-1, zq))
                        after = 0; hn = []; eqs = []
                        s0 = tuple(sorted(((a[0] if k >= 1 else zq), b)))
                        if k >= 1: after += w([(a[0], ce[0]), (b, ye)], s0)
                        else: after += w(([] if z is None else [z]) + [(b, ye)], s0)
                        hn.append(s0[0] + 1); eqs.append(s0[0] == s0[1])
                        for i in range(1, k):
                            si = tuple(sorted((a[i], hn[i - 1])))
                            after += w([(a[i], ce[i]), (hn[i - 1], eqs[i - 1])], si)
                            hn.append(si[0] + 1); eqs.append(si[0] == si[1])
                        if k >= 1:
                            sk = tuple(sorted((zq, hn[k - 1])))
                            after += w(([] if z is None else [z]) + [(hn[k - 1], eqs[k - 1])], sk)
                        cnt += 1; sl = after - before - 3 * k
                        if sl < 0:
                            bad += 1
                            if bad <= 8: print('VIOLATION k', k, 'a', a, 'c flags', ce, 'b', b, 'y eq', ye, 'z', z, 'slack', sl)
                        worst = sl if worst is None else min(worst, sl)
    print('k <=', k, 'profiles', cnt, 'violations', bad, 'least gain minus 3k', worst, flush=True)
print('ALL OK' if not bad else 'FAILED')
sys.exit(1 if bad else 0)
