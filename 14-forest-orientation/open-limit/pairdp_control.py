#!/usr/bin/env python3
"""Control for the pair-game DP: with a deliberately FALSE rule (slot-1 options cost p+2 instead of p+1) the
DP value must exceed the known exact T(n) = B(n) for some n <= 16.  With the true rule it never does."""
import sys
src = open('pairdp_noprune.py').read().split("for n in range(int(sys.argv[1])")[0]
for label, s in (('true rule', src),
                 ('false rule', src.replace("res.append(p + 1 + max", "res.append(p + 2 + max")
                                   .replace("res.append(pp + 1 + max", "res.append(pp + 2 + max"))):
    sys.argv = ['x', '1', '1']
    ns = {}
    exec(s, ns)
    bad = [(n, ns['V'](ns['norm']({(-1, -1): n})), max(0, n // 4 - 1)) for n in range(1, 17)]
    bad = [b for b in bad if b[1] > b[2]]
    print(label, ': (n, DP value, exact T(n)) with DP > T:', bad[:5], '...' if len(bad) > 5 else '')
