#!/usr/bin/env python3
"""Finite checks for Theorem 2 of NOTE.md:  T(n) <= (9/16) n - log2(n)  for n >= 19.

f(1) = 0,  f(n) = max over 1 <= a <= n/2 of  f(a) + f(n-a) + c(a),
c(a) = floor(log2(a+1)) - 1   (an upper bound on what the greedy rule pays
when the smaller tree has a vertices).  T(n) <= f(n) for every n.

psi(n) = log2(n) if n is a power of 2, and log2(n+1) otherwise.
The induction in NOTE.md Section 5 needs, with N = 64:
  (i)   f(n) <= (9/16) n - psi(n)                    for N <= n <= 2N - 2;
  (ii)  f(a) + c(a) + log2(1 + (a+1)/N) <= (9/16) a  for 1 <= a <= N - 1.
The statement of the theorem below N needs
  (iii) f(n) <= (9/16) n - log2(n)                   for 19 <= n <= N - 1.
All are checked in integer arithmetic:  x <= (9/16) m - log2(num/den)  with
integer x is  2^(9m - 16x) * den^16 >= num^16.

Also checked: f(n) <= 9n/16 for n <= 63, and that below 19 the theorem's
inequality fails for f exactly at n = 6, 9, 18 (both used in the note's
remarks). Informational: the claim checked directly up to M, f at powers of 2
against 9 * 2^k / 16 - k, and the maximum of f(n)/n.

M below 126 is raised to 126.

Run:  python3 greedy_recurrence.py [M]     (M: how far to tabulate, default 3000)
"""
import sys

N = 64


def table(M):
    c = [0] + [(a + 1).bit_length() - 2 for a in range(1, M + 1)]
    f = [0, 0]
    for n in range(2, M + 1):
        f.append(max(f[a] + f[n - a] + c[a] for a in range(1, n // 2 + 1)))
    return f, c


def le(x, m, num, den):
    """x <= (9/16) m - log2(num/den), exactly; requires num >= den > 0."""
    assert num >= den > 0
    e = 9 * m - 16 * x
    return e >= 0 and 2 ** e * den ** 16 >= num ** 16


def psi_arg(n):
    """psi(n) = log2(psi_arg(n))."""
    return n if n & (n - 1) == 0 else n + 1


def main():
    M = max(int(sys.argv[1]) if len(sys.argv) > 1 else 3000, 2 * N - 2)
    f, c = table(M)
    print("f(1..126) =", f[1:127])
    i = all(le(f[n], n, psi_arg(n), 1) for n in range(N, 2 * N - 1))
    ii = all(le(f[a] + c[a], a, N + a + 1, N) for a in range(1, N))
    iii = all(le(f[n], n, n, 1) for n in range(19, N))
    print("(i)   base cases 64..126:", "OK" if i else "FAIL")
    print("(ii)  small parts 1..63:", "OK" if ii else "FAIL")
    print("(iii) theorem for 19..63:", "OK" if iii else "FAIL")
    direct = all(le(f[n], n, psi_arg(n), 1) for n in range(N, M + 1))
    print(f"direct: f(n) <= 9n/16 - psi(n) for 64 <= n <= {M}:", "OK" if direct else "FAIL")
    small = all(16 * f[n] <= 9 * n for n in range(1, N))
    print("f(n) <= 9n/16 for 1 <= n <= 63:", "OK" if small else "FAIL")
    fails = [n for n in range(1, 19) if not le(f[n], n, n, 1)]
    print("n < 19 with f(n) > 9n/16 - log2(n):", fails, "OK" if fails == [6, 9, 18] else "FAIL")
    k = 4
    pw = True
    while 2 ** k <= M:
        pw = pw and 16 * f[2 ** k] == 9 * 2 ** k - 16 * k
        k += 1
    print(f"f(2^k) = 9*2^k/16 - k for 4 <= k <= {k - 1}:", "OK" if pw else "FAIL")
    r, at = max((f[n] / n, n) for n in range(1, M + 1))
    print(f"max f(n)/n for n <= {M}: {r:.6f} at n = {at};  f(1000) =", f[1000] if M >= 1000 else "-")
    sys.exit(0 if i and ii and iii and direct and pw and small and fails == [6, 9, 18] else 1)


if __name__ == "__main__":
    main()
