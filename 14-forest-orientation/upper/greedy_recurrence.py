#!/usr/bin/env python3
"""Finite checks for Theorem 2 of NOTE.md:  T(n) <= (4/7) n - log2(n+1)  for n >= 16.

f(1) = 0,  f(n) = max over 1 <= a <= n/2 of  f(a) + f(n-a) + c(a),
c(a) = floor(log2(a+1)) - 1   (the most the greedy rule pays when the smaller
tree has a vertices).  T(n) <= f(n) for every n.

The induction in NOTE.md Section 5 needs, with N = 16:
  (i)  f(n) <= (4/7) n - log2(n+1)                 for N <= n <= 2N - 2;
  (ii) f(a) + c(a) + log2(1 + a/(N+1)) <= (4/7) a  for 1 <= a <= N - 1.
Both are checked in integer arithmetic:  x <= (4/7) m - log2(y)  with integer x
is  2^(4m - 7x) >= y^7  (y rational; cleared of denominators below).

Run:  python3 greedy_recurrence.py [M]     (M: how far to tabulate, default 3000)
"""
import sys

N = 16


def table(M):
    c = [0] + [(a + 1).bit_length() - 2 for a in range(1, M + 1)]
    f = [0, 0]
    for n in range(2, M + 1):
        f.append(max(f[a] + f[n - a] + c[a] for a in range(1, n // 2 + 1)))
    return f, c


def le(x, m, num, den):
    """x <= (4/7) m - log2(num/den), exactly; requires num >= den > 0."""
    assert num >= den > 0
    e = 4 * m - 7 * x
    return e >= 0 and 2 ** e * den ** 7 >= num ** 7


def main():
    M = int(sys.argv[1]) if len(sys.argv) > 1 else 3000
    f, c = table(max(M, 2 * N))
    print("f(1..30) =", f[1:31])
    print("c(1..15) =", c[1:16])
    i = all(le(f[n], n, n + 1, 1) for n in range(N, 2 * N - 1))
    ii = all(le(f[a] + c[a], a, N + 1 + a, N + 1) for a in range(1, N))
    print("(i)  base cases 16..30:", "OK" if i else "FAIL")
    print("(ii) small parts 1..15:", "OK" if ii else "FAIL")
    direct = all(le(f[n], n, n + 1, 1) for n in range(N, M + 1))
    print(f"direct: f(n) <= 4n/7 - log2(n+1) for 16 <= n <= {M}:", "OK" if direct else "FAIL")
    r, at = max((f[n] / n, n) for n in range(1, M + 1))
    print(f"max f(n)/n for n <= {M}: {r:.6f} at n = {at};  f(1000) =", f[1000] if M >= 1000 else "-")
    sys.exit(0 if i and ii and direct else 1)


if __name__ == "__main__":
    main()
