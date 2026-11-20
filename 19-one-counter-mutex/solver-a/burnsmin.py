#!/usr/bin/env python3
"""For n = 2..5: least b for which SOME weight vector lets weighted Burns decode; writes the witness and checks it."""
import itertools, sys
from burnsgen import gen
from checker import check
for n in range(2, int(sys.argv[1]) + 1):
    for b in range(2, 40):
        found = None
        for w in itertools.product(range(1, b), repeat=n):
            t = gen(b, list(w))
            if t: found = (w, t); break
        if found:
            fn = "w_burns_n%d_b%d.txt" % (n, b); open(fn, 'w').write(found[1])
            print("n=%d least b for weighted Burns = %d weights %s ->" % (n, b, found[0]), end=' ', flush=True)
            check(fn); break
