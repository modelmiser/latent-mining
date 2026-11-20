# closed-form count of the layouts fast*.cpp must visit for n=3 (independent of the C++ recursion)
from math import factorial as F, comb
from fractions import Fraction
def leaves(m):
    tot=Fraction(0)
    for s0 in range(2,m+1,2):
        for s1 in range(2,m+1,2):
            for s2 in range(2,m+1,2):
                h=m-s0-s1-s2
                if h<0: continue
                arr=Fraction(F(m-1),F(s0-1)*F(s1)*F(s2)*F(h))*Fraction(s1,s1+s2)
                tot+=arr*comb(s0-1,s0//2)*comb(s1,s1//2)*comb(s2,s2//2)*2**h
    return tot
for m in range(6,16): print(m,leaves(m))
