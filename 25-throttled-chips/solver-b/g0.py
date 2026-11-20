# gap bound g0(m): least g such that min_{a+b=g-2} T(a)+T(b) > m-2, T(0)=0, T(a)=(a+1)(a+2)/2
def T(a): return 0 if a==0 else (a+1)*(a+2)//2
def g0(m):
    F=m-2; g=2
    while min(T(a)+T(g-2-a) for a in range(g-1))<=F: g+=1
    return g
if __name__=='__main__':
    print([(m,g0(m)) for m in range(2,25)])
