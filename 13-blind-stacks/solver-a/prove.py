#!/usr/bin/env python3
"""Computer-assisted induction (see proofs.txt, Theorem 3).
For k=2: checks, over ALL flag states s (49 of them) and ALL actions a, on a base
window of levels u, that
  (i)  Q(s,a,u) = 5 Q(s,a,u-1) - 5 Q(s,a,u-2)
  (ii) there is a fixed action a*(s) with d(u)=Q(s,a*,u)-Q(s,a,u) satisfying
       d(u-1) >= 0 and d(u) >= lam2*d(u-1), lam2=(5-sqrt5)/2   (exact test)
For k=1 the same with recurrence x(u)=2x(u-1) and d(u)>=0.
Q(s,a,u) = value of action a in state s with u unarrived items."""
import sys, itertools
from fast import make
k = int(sys.argv[1]); U0 = int(sys.argv[2]); U1 = int(sys.argv[3])
f = make(k)
stk = [(), (0,), (1,), (0,0), (0,1), (1,0), (1,1)]
states = list(itertools.product(stk, repeat=k))
def actions(st, u):
    """list of (name, value)"""
    for i in range(k):
        s = st[i]
        if s and s[-1] == 0:
            return [("probe", f(st, u))]
    out = []
    if u > 0 and len(st[0]) < 2:
        out.append(("IN", f((st[0] + (0,),) + st[1:], u - 1)))
    for i in range(k - 1):
        if st[i] and len(st[i+1]) < 2:
            n = list(st); n[i+1] = st[i+1] + (st[i][-1],); n[i] = st[i][:-1]
            out.append(("SHIFT%d" % (i+1), f(tuple(n), u)))
    if not out: out.append(("none", f(st, u)))
    return out
def ge_lam2(d1, d0):
    # d1 >= (5-sqrt5)/2 * d0  <=>  A + sqrt5*B >= 0, A=2d1-5d0, B=d0
    A = 2*d1 - 5*d0; B = d0
    if A >= 0 and B >= 0: return True
    if A < 0 and B < 0: return False
    if A >= 0: return A*A >= 5*B*B      # B<0
    return 5*B*B >= A*A                 # A<0,B>=0
for u in range(0, U1 + 1): f(tuple(() for _ in range(k)), u)   # warm cache bottom-up
bad = 0; nchk = 0
for st in states:
    acts = {u: actions(st, u) for u in range(U0 - 2, U1 + 1)}
    names = [a for a, _ in acts[U0]]
    for u in range(U0 - 2, U1 + 1):
        assert [a for a, _ in acts[u]] == names
    Q = {a: {u: dict(acts[u])[a] for u in acts} for a in names}
    for u in range(U0, U1 + 1):
        assert f(st, u) == max(Q[a][u] for a in names)
        for a in names:
            nchk += 1
            if k == 2: ok = Q[a][u] == 5*Q[a][u-1] - 5*Q[a][u-2]
            else:      ok = Q[a][u] == 2*Q[a][u-1]
            if not ok: bad += 1; print("REC FAIL", st, a, u)
    # fixed best action
    found = False
    for astar in names:
        good = True
        for a in names:
            for u in range(U0, U1 + 1):
                d0 = Q[astar][u-1] - Q[a][u-1]; d1 = Q[astar][u] - Q[a][u]
                if d0 < 0 or d1 < 0: good = False
                if k == 2 and not ge_lam2(d1, d0): good = False
        if good: found = True; break
    if not found: bad += 1; print("DOMINANCE FAIL", st)
print("k=%d window u=%d..%d states=%d checks=%d failures=%d" % (k, U0, U1, len(states), nchk, bad))
