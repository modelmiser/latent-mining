#!/usr/bin/env python3
"""Explicit strategy reaching clean at (p*k+2)*2^(p-2) (the lower bound), as a move list
in the format of check_witness.py.   Usage: gen_strategy.py p k > file
Strategy (see proofs.txt, Theorem 1):
  frontier(base, ids): with pebbles ids (r of them) put pebbles on base+2^(r-1), base+2^(r-1)+2^(r-2), ..., base+2^r-1
  clean(base, ids, topid): pebble topid ends alone on base+2^(r-1)   [r = len(ids)+1]
  phase 0: clean(0, all).  Then p*k rounds: with pebble A alone on m, pick B = a hand pebble of
  maximal charge, clean(m, others, B) puts B on m+2^(p-2); forget A."""
import sys
p, k = int(sys.argv[1]), int(sys.argv[2])
charge = [k] * p
out = []


def P(i, pid):
    out.append("P %d %d" % (i, charge[pid]))


def R(i, pid):
    out.append("R %d %d" % (i, charge[pid]))


def clean(base, ids, top, undo=False):
    """ids: helper pebbles, top: the pebble that ends on base+2^len(ids). Reversible moves only."""
    r = len(ids)
    if not undo:
        frontier(base, ids)
        P(base + 2 ** r, top)
        frontier(base, ids, undo=True)
    else:
        frontier(base, ids)
        R(base + 2 ** r, top)
        frontier(base, ids, undo=True)


def frontier(base, ids, undo=False):
    if not ids:
        return
    if not undo:
        clean(base, ids[1:], ids[0])
        frontier(base + 2 ** (len(ids) - 1), ids[1:])
    else:
        frontier(base + 2 ** (len(ids) - 1), ids[1:], undo=True)
        clean(base, ids[1:], ids[0], undo=True)


ids = list(range(p))
clean(0, ids[1:], 0)
A = 0
m = 2 ** (p - 1)
for rnd in range(p * k):
    hand = [i for i in ids if i != A]
    B = max(hand, key=lambda i: charge[i])
    clean(m, [i for i in hand if i != B], B)
    assert charge[A] >= 1
    out.append("F %d %d" % (m, charge[A]))
    charge[A] -= 1
    A = B
    m += 2 ** (p - 2)
print("# p=%d k=%d strategy, clean at %d" % (p, k, m))
print("\n".join(out))
