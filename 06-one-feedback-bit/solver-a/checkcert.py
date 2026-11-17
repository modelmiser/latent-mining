#!/usr/bin/env python3
"""Standalone check of an LP-dual certificate, by the textual definition, exact fractions.
usage: checkcert.py n t file bound
Verifies: weights >= 0; for every strategy (u,a,b) the weight of its length-(n-1)
outcomes is >= 1; floor(total weight) <= bound.  Then F(n,t) <= bound."""
import sys, itertools, math
from fractions import Fraction

def D(s):
    return {s[:i] + s[i+1:] for i in range(len(s))}

def main(n, t, path, bound):
    y = {}
    for line in open(path):
        if line.startswith('#') or not line.strip():
            continue
        w, v = line.split()
        assert len(w) == n - 1 and w not in y
        y[w] = Fraction(v)
        assert y[w] >= 0
    W = lambda k: [''.join(p) for p in itertools.product('01', repeat=k)]
    for u in W(t):
        for a in W(n - t):
            for b in W(n - t):
                S = {u + x for x in D(a)} | {x + b for x in D(u)}
                if sum(y.get(w, 0) for w in S) < 1:
                    print(f"REJECT: strategy {(u, a, b)} has weight < 1"); return False
    tot = sum(y.values())
    if math.floor(tot) > bound:
        print(f"REJECT: total {tot} gives only F <= {math.floor(tot)}"); return False
    print(f"OK n={n} t={t}: total weight {tot} (= {float(tot):.4f}) proves F({n},{t}) <= {math.floor(tot)}")
    return True

if __name__ == '__main__':
    sys.exit(0 if main(int(sys.argv[1]), int(sys.argv[2]), sys.argv[3], int(sys.argv[4])) else 1)
