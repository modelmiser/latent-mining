#!/usr/bin/env python3
"""Explicit "two opposing combs" strategy (proofs.txt, Theorem L): shows F(n,k) >= n - ceil(n/k')  with k' = min(k,n).
 m = k' chains per direction, tops spaced with cyclic gaps <= delta = ceil(n/m).
 A-chain c, step j (j>=1), round 2j-1 : destroy link (x+1 -> x) with x = P_c - j
 B-chain c, step j (j>=1), round 2j   : destroy link (x-1 -> x) with x = Q_c + j
 Q_c is chosen so that at the last round T = n - delta - 1 the B-top sits one to the right of the A-top.
usage: gen.py n k [outfile]   (witness in check.py format; first completion must be >= T+1 = n - delta)
"""
import sys

def strategy(n, k):
    m = min(k, n)
    delta = -(-n // m)
    T = n - delta - 1
    P = [(c * n) // m for c in range(m)]          # cyclic gaps are floor/ceil of n/m <= delta
    # A-top of chain c at time t: position P_c - 1 - floor(t/2);  B-top: Q_c + ceil(t/2)
    # want  Q_c + ceil(T/2) = P_c - 1 - floor(T/2) + 1   =>  Q_c = P_c - T
    Q = [(p - T) % n for p in P]
    rounds = []
    for t in range(1, T + 1):
        D = []
        if t % 2 == 1:
            j = (t + 1) // 2
            for p in P:
                x = (p - j) % n
                D.append(((x + 1) % n, x))
        else:
            j = t // 2
            for q in Q:
                x = (q + j) % n
                D.append(((x - 1) % n, x))
        rounds.append(sorted(set(D)))
    return T, rounds

if __name__ == '__main__':
    n = int(sys.argv[1]); k = int(sys.argv[2])
    T, rounds = strategy(n, k)
    out = open(sys.argv[3], 'w') if len(sys.argv) > 3 else sys.stdout
    out.write('%d %d\n' % (n, k))
    for t, D in enumerate(rounds, 1):
        out.write('%d: %s\n' % (t, ' '.join('%d>%d' % l for l in D)))
