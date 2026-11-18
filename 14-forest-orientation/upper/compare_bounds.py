#!/usr/bin/env python3
"""Compare the two upper bounds of NOTE.md: the recurrence f(n) (Theorem 2) and U(n) = max(0, (n-3)//2) (Theorem 3).
usage: compare_bounds.py [N=4000].  Exit code 1 unless the statements quoted in NOTE.md hold up to N."""
import sys
N = int(sys.argv[1]) if len(sys.argv) > 1 else 4000
if N < 130: sys.exit('N must be at least 130')
c = lambda a: (a + 1).bit_length() - 2            # floor(log2(a+1)) - 1
f = [0, 0]
for n in range(2, N + 1): f.append(max(f[a] + f[n - a] + c(a) for a in range(1, n // 2 + 1)))
U = lambda n: max(0, (n - 3) // 2)
ok = all(f[n] <= U(n) for n in range(1, 96)) and all(U(n) <= f[n] for n in range(94, N + 1)) \
     and U(96) < f[96] and all(U(n) < f[n] for n in range(126, N + 1))
print('f(n) <= U(n) for n <= 95; U(n) <= f(n) for 94 <= n <= %d; U < f at 96 and for 126 <= n <= %d:' % (N, N), 'OK' if ok else 'FAILED')
print('n with f(n) < U(n):', [n for n in range(1, N + 1) if f[n] < U(n)])
print('n <= 130 with U(n) < f(n):', [n for n in range(1, 131) if U(n) < f[n]])
sys.exit(0 if ok else 1)
