#!/usr/bin/env python3
"""Independent witness checker, straight from the problem definition (sets, no arc model).
Witness file format (plain text):
  line 1:  n k
  then one line per round t = 1,2,...:  "t: i>j i>j ..."   (links destroyed in round t; may be empty)
Rounds beyond the file are treated as destruction-free.
Prints: LEGAL/ILLEGAL and the first round at which some process is complete.
usage: check.py file [expected_first_completion]
"""
import sys

def check(path):
    with open(path) as fh:
        lines = [l.strip() for l in fh if l.strip() and not l.startswith('#')]
    n, k = map(int, lines[0].split())
    Ds = []
    for idx, l in enumerate(lines[1:], 1):
        head, _, rest = l.partition(':')
        if int(head) != idx:
            return n, k, 'ILLEGAL: round numbering', None
        D = set()
        for tok in rest.split():
            i, j = map(int, tok.split('>'))
            if not (0 <= i < n and 0 <= j < n) or (j - i) % n not in (1, n - 1):
                return n, k, 'ILLEGAL: %s is not a ring link' % tok, None
            if n == 2:
                return n, k, 'ILLEGAL', None
            D.add((i, j))
        Ds.append(D)
    prev = set()
    for t, D in enumerate(Ds, 1):
        if len(D) > k:
            return n, k, 'ILLEGAL: round %d destroys %d > k' % (t, len(D)), None
        if D & prev:
            return n, k, 'ILLEGAL: round %d repeats a link of round %d' % (t, t - 1), None
        prev = D
    K = [{i} for i in range(n)]
    t = 0
    while True:
        t += 1
        D = Ds[t - 1] if t <= len(Ds) else set()
        new = [set(s) for s in K]
        for i in range(n):
            for j in ((i + 1) % n, (i - 1) % n):
                if (i, j) not in D:
                    new[j] |= K[i]
        K = new
        if any(len(s) == n for s in K):
            return n, k, 'LEGAL', t

if __name__ == '__main__':
    n, k, status, first = check(sys.argv[1])
    print('n=%d k=%d %s first_completion=%s' % (n, k, status, first))
    if len(sys.argv) > 2:
        ok = status == 'LEGAL' and first == int(sys.argv[2])
        print('EXPECTED %s: %s' % (sys.argv[2], 'OK' if ok else 'MISMATCH'))
        sys.exit(0 if ok else 1)
