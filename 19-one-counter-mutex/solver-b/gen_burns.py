#!/usr/bin/env python3
"""gen_burns.py b w1 w2 ... wn  -> system file on stdout.
Burns' one-flag-per-process algorithm; flag of process i is the summand w_i in R.
Process i (0-based, lower index = higher priority):
 0 rem : read; lower-index flag up -> 2 else 3
 1 crit: add -w -> 0
 2 wait: read; lower-index flag up -> 2 else 3
 3     : add +w -> 4
 4     : read (own flag up); lower up -> 5 ; else higher up -> 6 ; else crit
 5     : add -w -> 2
 6     : read (own flag up); higher up -> 6 else crit
Decoding is by table; the generator refuses weights for which a needed decode is ambiguous."""
import sys, itertools
def sums(ws, b, nonempty=False):
    out = set()
    for k in range(len(ws)+1):
        for c in itertools.combinations(ws, k):
            if nonempty and k == 0: continue
            out.add(sum(c) % b)
    return out
def ok(b, w):
    n = len(w)
    for i in range(n):
        A = sums(w[:i], b); Ap = sums(w[:i], b, True)
        B = sums(w[i+1:], b); Bp = sums(w[i+1:], b, True)
        # lower-up decidable regardless of higher flags
        if B & {(x+y) % b for x in Ap for y in B}: return False
        # higher-up decidable when no lower flag is up (state 4 after the lower test)
        # and regardless of lower flags (state 6, lower flags may rise meanwhile)
        if A & {(x+y) % b for x in A for y in Bp}: return False
    return True
def gen(b, w):
    n = len(w); out = ["%d %d" % (n, b)]
    for i in range(n):
        Ap = sums(w[:i], b, True); B = sums(w[i+1:], b); A = sums(w[:i], b); Bp = sums(w[i+1:], b, True)
        lowup = {(x+y) % b for x in Ap for y in B}
        highup = {(x+y) % b for x in A for y in Bp}
        L = ["7"]
        r0 = ["2" if v in lowup else "3" for v in range(b)]
        L.append("R " + " ".join(r0)); L.append("A %d 0" % ((-w[i]) % b)); L.append("R " + " ".join(r0))
        L.append("A %d 4" % (w[i] % b))
        r4 = []; r6 = []
        for v in range(b):
            o = (v - w[i]) % b
            r4.append("5" if o in lowup else ("6" if o in highup else "1"))
            r6.append("6" if o in highup else "1")
        L.append("R " + " ".join(r4)); L.append("A %d 2" % ((-w[i]) % b)); L.append("R " + " ".join(r6))
        out += L
    return "\n".join(out) + "\n"
if __name__ == "__main__":
    if sys.argv[1] == "min":   # gen_burns.py min n : least b with admissible weights
        n = int(sys.argv[2])
        for b in range(2, 40):
            for w in itertools.product(range(1, b), repeat=n):
                if ok(b, list(w)): print("n=%d least b=%d weights=%s" % (n, b, w)); sys.exit(0)
    else:
        b = int(sys.argv[1]); w = [int(x) for x in sys.argv[2:]]
        if not ok(b, w): sys.exit("ambiguous weights")
        sys.stdout.write(gen(b, w))
