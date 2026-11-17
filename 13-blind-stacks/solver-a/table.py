import sys, math
from fast import N
for k in range(1, 9):
    print(k, [N(k, n) for n in range(1, 13)], "deficit at n=k+2:", math.factorial(k+2) - N(k, k+2), "N(k,k+1)==(k+1)!:", N(k,k+1)==math.factorial(k+1), flush=True)
a = [N(2, n) for n in range(0, 301)]
assert all(a[n] == 5*a[n-1] - 5*a[n-2] for n in range(5, 301)); print("k=2 recurrence holds n=5..300; fails at n=4:", a[4], 5*a[3]-5*a[2])
U = [0, 1]
for j in range(2, 300): U.append(5*U[-1] - 5*U[-2])
assert all(a[n] == 21*U[n-3] - 30*U[n-4] for n in range(4, 300)); print("closed form N(2,n)=21U(n-3)-30U(n-4) ok n=4..299")
# gf check: (1-5x+5x^2)*A = x-3x^2+x^3+x^4
c = [a[n] - 5*a[n-1] + 5*(a[n-2] if n >= 2 else 0) for n in range(1, 12)]; print("numerator coeffs x^1..:", c)
