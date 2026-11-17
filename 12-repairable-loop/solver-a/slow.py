#!/usr/bin/env python3
"""Direct, slow implementation of the definition. Loops are tuples of 0/1."""
import sys, itertools
def covers(v, n):
    m = len(v); s = set()
    for i in range(m):
        s.add(tuple(v[(i+j) % m] for j in range(n)))
    return s
def universal(v, n):
    return len(covers(v, n)) == 2**n
def edits(v):
    """all results of one legal edit or pass"""
    m = len(v); out = [tuple(v)]
    if m >= 2:
        for k in range(m): out.append(tuple(v[:k] + v[k+1:]))
    for k in range(m): out.append(tuple(v[:k+1] + v[k:]))
    return out
def repairable(v, n):
    v = tuple(v)
    for w1 in edits(v):
        if not any(universal(w2, n) for w2 in edits(w1)):
            return False
    return True
def canon(v):
    m = len(v); return min(tuple(v[i:] + v[:i]) for i in range(m))
def all_repairable(n, L):
    res = set()
    for v in itertools.product((0,1), repeat=L):
        if canon(v) != v: continue
        if repairable(v, n): res.add(v)
    return sorted(res)
if __name__ == "__main__":
    n = int(sys.argv[1]); Lmax = int(sys.argv[2])
    for L in range(1, Lmax+1):
        r = all_repairable(n, L)
        print(f"n={n} L={L} repairable necklaces: {len(r)}", ''.join(map(str, r[0])) if r else '', flush=True)
        if len(r) and len(sys.argv) > 3:
            for v in r: print('  ', ''.join(map(str, v)), 'universal' if universal(v, n) else 'NOT-universal')
