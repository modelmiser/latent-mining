#!/usr/bin/env python3
"""Fast F(n,t): set packing over the universe {0,1}^(n-1).

Reduction (safe, see proofs.txt L1): two strategies conflict iff they have the same
(u,a) or their short-outcome sets S(u,a,b) = u D(a) ∪ D(u) b intersect.
 * if n-t >= 1 then uD(a) is nonempty and lies in S, so "same (u,a)" already forces
   S to intersect;  if t = n then a=b=empty, S = D(u), strategies are just words u
   and two different words are different strategies.
 * So F(n,t) = max number of pairwise disjoint sets among the family {S(u,a,b)},
   EXCEPT that identical sets coming from different strategies still conflict
   (they intersect, sets are nonempty for n>=2) -> dedupe is safe.
 * Dominance: if S ⊂ S' (proper or equal) then S' can be dropped: in any packing
   S' may be replaced by S.  Safe in both directions (value unchanged).
Solved exactly with HiGHS (scipy.optimize.milp).  Usage: fast.py n [t] [--nodom]
Writes witness to wit_n_t.txt
"""
import sys, itertools, time
import numpy as np
from scipy.optimize import milp, LinearConstraint, Bounds
from scipy.sparse import lil_matrix

def D(s):
    return {s[:i] + s[i+1:] for i in range(len(s))}

def W(k):
    return [''.join(p) for p in itertools.product('01', repeat=k)]

def family(n, t, dom=True):
    """returns dict frozenset(short outcomes as ints) -> a representative strategy"""
    fam = {}
    for u in W(t):
        Du = D(u)
        for a in W(n - t):
            base = {u + x for x in D(a)}
            for b in W(n - t):
                S = frozenset(int(x, 2) for x in base | {x + b for x in Du})
                if S not in fam:
                    fam[S] = (u, a, b)
    if dom:
        keys = sorted(fam, key=len)
        masks = []
        kept = []
        for S in keys:
            m = 0
            for x in S: m |= 1 << x
            if any((k & m) == k for k in masks):
                continue
            masks.append(m); kept.append(S)
        fam = {S: fam[S] for S in kept}
    return fam

def solve(n, t, dom=True, tl=1400):
    fam = family(n, t, dom)
    keys = list(fam)
    U = 1 << (n - 1)
    A = lil_matrix((U, len(keys)))
    for j, S in enumerate(keys):
        for x in S:
            A[x, j] = 1
    res = milp(c=-np.ones(len(keys)), constraints=LinearConstraint(A.tocsr(), -np.inf, 1),
               integrality=np.ones(len(keys)), bounds=Bounds(0, 1),
               options={'time_limit': tl, 'mip_rel_gap': 0.0})
    x = res.x
    chosen = [fam[keys[j]] for j in range(len(keys)) if x is not None and x[j] > 0.5]
    return res, chosen, len(keys)

if __name__ == '__main__':
    n = int(sys.argv[1])
    dom = '--nodom' not in sys.argv
    args = [a for a in sys.argv[2:] if not a.startswith('--')]
    ts = [int(args[0])] if args else range(0, n + 1)
    for t in ts:
        t0 = time.time()
        res, chosen, m = solve(n, t, dom)
        bound = -res.mip_dual_bound if res.mip_dual_bound is not None else None
        print(f"n={n} t={t} sets={m} status={res.status} F={len(chosen)} dualbound={bound} "
              f"time={time.time()-t0:.1f}", flush=True)
        if dom:
            with open(f"wit_{n}_{t}.txt", "w") as f:
                f.write(f"# n={n} t={t} size={len(chosen)} status={res.status} dualbound={bound}\n")
                for u, a, b in sorted(chosen):
                    f.write(f"{u or '-'} {a or '-'} {b or '-'}\n")
