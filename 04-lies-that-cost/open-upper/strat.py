# Mechanical replay of the strategy in proofs.txt (Theorem 1).  For each n it walks every state the
# strategy can reach and asserts each step of the proof: nice (b >= a-1), weight <= 2^j, the prescribed
# question exists (b1,b2 >= 0), K*ell >= j-1 (so liars are blocked for good and the state is again (a,b)),
# and that a small state (a+b <= 2j-3) is covered by the table (j <= 14) or by Lemma C (j >= 15).
# usage: strat.py K NMAX [mutant]   mutant=1: break the b-split (control, must fail)
import sys, random, math
sys.setrecursionlimit(10000)
K=int(sys.argv[1]); NMAX=int(sys.argv[2]); MUT=int(sys.argv[3]) if len(sys.argv)>3 else 0
def P(n):
    q=1
    while n*(q+1)+((q-1) if n%2 else 0) > 2**q: q+=1
    return q
ROOT_TABLE={(2,3),(2,5),(3,3)}     # (k,n) settled by table.cpp root mode
memo={}; stats={'table':0,'lemmaC':0,'safe':0}
def check(a,b,j):
    key=(a,b,j)
    if key in memo: return
    memo[key]=1
    m=a+b
    assert m>=1 and b>=a-1, ("not nice",key)
    assert a*(j+1)+b<=2**j, ("weight",key)
    if m==1: return
    assert j>=1
    if m<=2*j-3:
        if j<=14: stats['table']+=1
        else:
            assert 3*math.ceil(math.log2(m))<=j, ("lemmaC bound",key); stats['lemmaC']+=1
        return
    if a%2==0:
        a1=a2=a//2; b1=(b+1)//2; b2=b//2
    else:
        a1=(a+1)//2; a2=(a-1)//2
        d=j-1 if (b-(j-1))%2==0 else j-2     # b2-b1
        if MUT: d=0 if b%2==0 else 1
        b1=(b-d)//2; b2=b-b1
        assert (b-d)%2==0
    assert b1>=0 and b2>=0, ("split",key)
    ell=min(a1+b1,a2+b2)
    assert K*ell>=j-1, ("unsafe",key,ell)
    stats['safe']+=1
    y=(a1,b1+a2); n_=(a2,b2+a1)
    assert a1*j+b1+a2<=2**(j-1) and a2*j+b2+a1<=2**(j-1), ("child weight",key)
    for c in (y,n_):
        if c[0]+c[1]>=1: check(c[0],c[1],j-1)
def root(n):
    q=P(n)
    if (K,n) in ROOT_TABLE: return
    a1=(n+1)//2; a2=n//2; ell=a2
    assert K*ell>=q-1, ("unsafe root",n,q)
    check(a1,a2,q-1); check(a2,a1,q-1)
for n in range(2,NMAX+1): root(n)
random.seed(1)
big=0
for e in range(18,200):
    for _ in range(6):
        root(random.randrange(2**e,2**(e+1))); big+=1
    for q in range(e,e+12):          # n just below and above each threshold of P
        for par in (0,1):
            lo,hi=2,2**(e+2)
            while lo<hi:             # least n of this parity class index with P(n)>q
                mid=(lo+hi)//2; nn=2*mid+par
                if P(nn)>q: hi=mid
                else: lo=mid+1
            for dn in (-1,0):
                nn=2*(lo+dn)+par
                if nn>=2: root(nn); big+=1
print("strat k=%d: n=2..%d and %d large n (to 2^200, incl. threshold n) all OK; states=%d %s"%(K,NMAX,big,len(memo),stats))
