#!/usr/bin/env python3
"""LP-dual upper-bound certificates.  A weight y(w) >= 0 on words w of length n-1 with
sum_{w in S(u,a,b)} y(w) >= 1 for EVERY strategy proves F(n,t) <= floor(sum_w y(w))
(disjoint short-outcome sets each absorb weight >= 1).
Writes cert_n_t.txt ('word num/den' lines) when floor(total) equals the known value F.
usage: lp.py n F0 F1 ... Fn   (the claimed values, only used to decide whether cert is tight)"""
import sys, math
from fractions import Fraction
import numpy as np
from scipy.optimize import linprog
from scipy.sparse import lil_matrix
from fast import family

DEN = 5040

def cert(n, t):
    fam = family(n, t, dom=True)
    keys = list(fam)
    U = 1 << (n - 1)
    A = lil_matrix((len(keys), U))
    for j, S in enumerate(keys):
        for x in S:
            A[j, x] = -1
    res = linprog(c=np.ones(U), A_ub=A.tocsr(), b_ub=-np.ones(len(keys)), bounds=(0, None), method='highs')
    y = [Fraction(math.ceil(max(v, 0) * DEN - 1e-7), DEN) for v in res.x]
    # repair: make exactly feasible
    for S in keys:
        s = sum(y[x] for x in S)
        if s < 1:
            x = max(S, key=lambda e: y[e]); y[x] += 1 - s
    return res.fun, y

if __name__ == '__main__':
    n = int(sys.argv[1]); Fs = [int(x) for x in sys.argv[2:]]
    for t in range(n + 1):
        val, y = cert(n, t)
        tot = sum(y)
        tight = math.floor(tot) == Fs[t]
        print(f"n={n} t={t} LP={val:.4f} rational_total={float(tot):.4f} F={Fs[t]} cert_tight={tight}", flush=True)
        if tight:
            with open(f"cert_{n}_{t}.txt", "w") as f:
                f.write(f"# n={n} t={t}: weights on words of length {n-1}; total={tot} ; proves F<={math.floor(tot)}\n")
                for x in range(1 << (n - 1)):
                    if y[x]:
                        f.write(f"{format(x, '0%db' % (n-1))} {y[x].numerator}/{y[x].denominator}\n")
