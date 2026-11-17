from fractions import Fraction
def bm(s):
    """Berlekamp-Massey over Q. returns C with s[i] = sum_{j>=1} C[j-1]*s[i-j]"""
    s = [Fraction(x) for x in s]
    C=[Fraction(1)]; B=[Fraction(1)]; L=0; m=1; b=Fraction(1)
    for n in range(len(s)):
        d = s[n] + sum(C[i]*s[n-i] for i in range(1,L+1))
        if d == 0: m += 1
        elif 2*L <= n:
            T = C[:]
            C = C + [Fraction(0)]*(len(B)+m-len(C))
            for i in range(len(B)): C[i+m] -= d/b*B[i]
            L = n+1-L; B = T; b = d; m = 1
        else:
            C = C + [Fraction(0)]*(len(B)+m-len(C))
            for i in range(len(B)): C[i+m] -= d/b*B[i]
            m += 1
    return [-c for c in C[1:L+1]]
