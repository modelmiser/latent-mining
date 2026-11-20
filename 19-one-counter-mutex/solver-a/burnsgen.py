#!/usr/bin/env python3
"""burnsgen.py b w1 w2 ... wn  -> prints Burns' one-flag algorithm with flag weights w_i mod b
in checker.py format, or exits 2 if some process cannot decode (any lower flag?, any higher flag?)
from the sum of the others' weighted flags."""
import sys, itertools
def gen(b, w):
    n = len(w); out = ["n %d b %d s 7" % (n, b)]
    for i in range(n):
        dec = {}
        others = [j for j in range(n) if j != i]
        for bits in itertools.product([0, 1], repeat=n - 1):
            v = sum(x * w[j] for x, j in zip(bits, others)) % b
            cls = (any(x and j < i for x, j in zip(bits, others)), any(x and j > i for x, j in zip(bits, others)))
            if dec.setdefault(v, cls) != cls: return None
        def rd(f, own):   # successor list indexed by R
            return ' '.join(str(f(*dec[(R - own) % b]) if (R - own) % b in dec else -1) for R in range(b))
        p = "p %d q " % (i + 1)
        out += [p + "0 read " + rd(lambda L, H: 3 if L else 2, 0),
                p + "1 add %d 0" % ((-w[i]) % b),
                p + "2 add %d 4" % (w[i] % b),
                p + "3 read " + rd(lambda L, H: 3 if L else 2, 0),
                p + "4 read " + rd(lambda L, H: 5 if L else (6 if H else 1), w[i]),
                p + "5 add %d 3" % ((-w[i]) % b),
                p + "6 read " + rd(lambda L, H: 6 if H else 1, w[i])]
    return '\n'.join(out) + '\n'
if __name__ == '__main__':
    b = int(sys.argv[1]); w = [int(x) for x in sys.argv[2:]]
    t = gen(b, w)
    if t is None: sys.exit(2)
    sys.stdout.write(t)
