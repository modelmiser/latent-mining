# u1.py -- U1(n) by exhaustive search on the one-lie game: state (a,b) = (#never lied, #lied once)
# and comparison with the formula quoted in the problem (Pelc).
import sys
from functools import lru_cache
sys.setrecursionlimit(10000)
@lru_cache(None)
def win(a,b,q):
    if a+b<=1: return True
    if q==0: return False
    if a*(q+1)+b>2**q: return False   # volume bound (safe: standard counting argument)
    for x in range(a+1):
        for y in range(b+1):
            # S holds x of a, y of b. yes: (x, y+(a-x)); no: (a-x, (b-y)+x)
            if win(x,y+a-x,q-1) and win(a-x,b-y+x,q-1): return True
    return False
def U1(n):
    q=0
    while not win(n,0,q): q+=1
    return q
def pelc(n):
    q=0
    while True:
        if (n%2==0 and n*(q+1)<=2**q) or (n%2==1 and n*(q+1)+(q-1)<=2**q): return q
        q+=1
if __name__=="__main__":
    N=int(sys.argv[1])
    bad=0
    for n in range(2,N+1):
        u,p=U1(n),pelc(n)
        if u!=p: bad+=1
        print(n,u,p,"" if u==p else "MISMATCH")
    print("mismatches",bad)
