#!/usr/bin/env python3
"""Second, separately written simulator (set-of-seen-configurations for never-fire; no Brent).
Reads lines 'RULE S=s N=n d=digits' on stdin, recomputes N for each, reports mismatches.
With --exact-max M: also asserts every rule has N == M.
Digits: entries (Q,1),(Q,2),(G,0),(G,1),(G,2),(U1,0)...; values 0=Q 1=G 2=F 3..=unnamed."""
import sys

def table(s, digs):
    states = [0, 1] + list(range(3, s))
    d = {(0, 0): 0}
    it = iter(digs)
    for x in states:
        for k in range(3):
            if (x, k) == (0, 0):
                continue
            d[(x, k)] = next(it)
    assert next(it, None) is None
    return d

def fires(d, n):
    c = (1,) + (0,) * (n - 1)
    seen = {c}
    t = 0
    while True:
        t += 1
        new = []
        for i, x in enumerate(c):
            k = 0
            if i - 1 >= 0 and c[i - 1] != x: k += 1
            if i + 1 < n and c[i + 1] != x: k += 1
            new.append(d[(x, k)])
        c = tuple(new)
        if 2 in c:
            return t if all(v == 2 for v in c) else 0
        if c in seen:          # exact repeat of an F-free configuration => never fires
            return 0
        seen.add(c)

def score(d, nmax=20):
    times = []
    for n in range(2, nmax + 1):
        t = fires(d, n)
        if not t:
            return n - 1, times
        times.append(t)
    return nmax, times

def main():
    exact = None
    if len(sys.argv) == 3 and sys.argv[1] == '--exact-max':
        exact = int(sys.argv[2])
    bad = tot = 0
    for line in sys.stdin:
        p = line.split()
        if not p or p[0] != 'RULE':
            continue
        s = int(p[1][2:]); N = int(p[2][2:]); digs = [int(ch) for ch in p[3][2:]]
        assert len(digs) == 3 * (s - 1) - 1
        N2, times = score(table(s, digs))
        tot += 1
        if N2 != N or (exact is not None and N2 != exact):
            bad += 1
            print('MISMATCH', line.strip(), 'python N =', N2)
        elif exact is not None:
            print(p[3][2:], 'N=%d' % N2, 'times', times)
    print('checked', tot, 'mismatches', bad)
    sys.exit(1 if bad else 0)

main()
