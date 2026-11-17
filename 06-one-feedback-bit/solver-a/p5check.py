#!/usr/bin/env python3
"""Brute-force check of P5 (min |S| = r(u)+r(a)-1, attained iff b in u_t D(a)),
P2 (S = D(ub) at t=n-1), P3 (S(u,a,a) = D(ua)) for n <= NMAX."""
import sys, itertools
D = lambda s: {s[:i] + s[i+1:] for i in range(len(s))}
r = lambda s: sum(1 for i in range(len(s)) if i == 0 or s[i] != s[i-1])
W = lambda k: [''.join(p) for p in itertools.product('01', repeat=k)]
S = lambda u, a, b: {u + x for x in D(a)} | {x + b for x in D(u)}
for n in range(2, int(sys.argv[1]) + 1):
    for t in range(0, n + 1):
        for u in W(t):
            for a in W(n - t):
                assert S(u, a, a) == D(u + a)
                if 1 <= t <= n - 1:
                    for b in W(n - t):
                        k = len(S(u, a, b)); lo = r(u) + r(a) - 1
                        good = b in {u[-1] + x for x in D(a)}
                        assert k == (lo if good else lo + 1), (u, a, b)
                        if t == n - 1:
                            assert S(u, a, b) == D(u + b)
    print("n", n, "ok", flush=True)
