#!/usr/bin/env python3
# referee: mechanical tests of solver-A P4/P5/P6 and solver-B lemma/(*), plus the constants.
import math
from math import comb
from fractions import Fraction
import pickle
my,levN=pickle.load(open("my.pkl","rb"))
F=[0,1]
for i in range(6000): F.append(F[-1]+F[-2])
# ---------- A: P4, P5, P6 on all words of length <= SMAX (ordered states, tagged by j = #additions)
SMAX=15
lev={(1,1,0)}
okP4=okP5=okP6=True; tight=0
vals_upto=set()
for s in range(SMAX+1):
    byj={}
    for a,b,j in lev: byj.setdefault(j,[]).append((a,b))
    for j,sts in byj.items():
        k=s-j
        mx=max(max(a,b) for a,b in sts)
        if mx>2**k*F[j+2]: okP4=False; print("P4 FAIL",s,j,mx)
        if mx==2**k*F[j+2]: tight+=1
        if len(set(sts))>2**j*comb(k+2*j+1,2*j+1): okP5=False; print("P5 FAIL",s,j)
        av={a for a,b in sts}; bv={b for a,b in sts}
        if av!=bv: okP6=False; print("P6 symmetry FAIL",s,j)
        if len(av)>min(2**j*comb(s+j+1,2*j+1),2**(s-j)*F[j+2]): okP6=False; print("P6 FAIL",s,j)
        vals_upto|=av
    assert len(vals_upto)==levN[s]
    lev={x for a,b,j in lev for x in ((2*a,b,j),(a,2*b,j),(a+b,b,j+1),(a,a+b,j+1))}
print("A P4 (size bound) ok:",okP4,"tight cases:",tight,"; P5 (class bound) ok:",okP5,"; P6 per-(s,j) ok:",okP6,"; lengths <=",SMAX)
def U(t): return sum(min(2**j*comb(s+j+1,2*j+1),2**(s-j)*F[j+2]) for s in range(t+1) for j in range(s+1))
print("U(0..7):",[U(t) for t in range(8)],"U>=N for t<=22:",all(U(t)>=levN[t] for t in range(23)))
def lg2big(n): 
    b=n.bit_length(); return b-1+math.log2(n>>(b-60)) -59 if b>60 else math.log2(n)
for t in (100,200,400,800,1600): print("log2 U(%d)/t = %.5f"%(t,lg2big(U(t))/t))
# gamma by bisection on the crossing, and shape checks
H=lambda x:0.0 if x<=0 or x>=1 else -x*math.log2(x)-(1-x)*math.log2(1-x)
lphi=math.log2((1+5**.5)/2)
f1=lambda d:d+(1+d)*H(2*d/(1+d)); f2=lambda d:1-d*(1-lphi)
lo,hi=0.01,0.3
for _ in range(200):
    mid=(lo+hi)/2
    if f1(mid)<f2(mid): lo=mid
    else: hi=mid
print("A crossing d*=%.9f f1=%.9f f2=%.9f  2^gamma=%.6f 1/gamma=%.6f"%(lo,f1(lo),f2(lo),2**f2(lo),1/f2(lo)))
grid=[i/10**6 for i in range(0,10**6+1)]
print("A max over d in [0,1] of min(f1,f2) on 1e6 grid: %.9f"%max(min(f1(d),f2(d)) for d in grid))
print("A f1 increasing on [0,d*]:",all(f1(grid[i+1])>=f1(grid[i]) for i in range(int(lo*10**6))))
# ---------- B: lemma in exact integers.  2^(4 Phi) = y^3*max(4x,y)/4 =: Q/4 for x<=y.
def Q(x,y): return y**3*max(4*x,y)
def typ(x,y): 
    z=[(x,2*y),(2*x,y),(x,x+y),(y,x+y)]; return [(min(p),max(p)) for p in z]
worst=[Fraction(0)]*4; ok=True
R=600
for y in range(1,R+1):
    for x in range(1,y+1):
        q=Q(x,y)
        for i,(u,v) in enumerate(typ(x,y)):
            ratio=Fraction(Q(u,v),q)
            if ratio>worst[i]: worst[i]=ratio
            if ratio>(16 if i==0 else 8): ok=False; print("B lemma FAIL",x,y,i)
print("B lemma exact on all 1<=x<=y<=%d: ok=%s ; sup of 2^(4 dPhi) per type A,B,C,D = %s (allowed 16,8,8,8)"%(R,ok,[float(w) for w in worst]))
# (*) on every type-word of length <= JMAX: M^4 <= 2^(4j-k+2); track (state,k) sets
JMAX=16
lev={(1,1,0)}; ok=True; slackmin=None
for j in range(JMAX+1):
    for x,y,k in lev:
        if y**4>2**(4*j-k+2): ok=False; print("(*) FAIL",x,y,j,k)
    if j==JMAX: break
    nx=set()
    for x,y,k in lev:
        for i,(u,v) in enumerate(typ(x,y)): nx.add((u,v,k+(i!=0)))
    # keep only max k per state (worst case for (*)), safe: larger k is the stronger test and successors inherit
    best={}
    for u,v,k in nx:
        if best.get((u,v),-1)<k: best[(u,v)]=k
    lev={(u,v,k) for (u,v),k in best.items()}
print("B (*) exact on all type-words of length <=",JMAX,":",ok)
# does type-word determine the same states as the real moves?  (unordered) check to length 10
a={(1,1)}; b={(1,1)}
for j in range(10):
    a={(min(p),max(p)) for x,y in a for p in ((2*x,y),(x,2*y),(x+y,y),(x,x+y))}
    b={p for x,y in b for p in typ(x,y)}
    assert a==b
print("B types = real moves on unordered states (len<=10): True")
# constants
for eta in (0.044,0.0445,0.045,0.0455):
    p=4*eta; print("B eta=%.4f: H(4eta)+4eta*log2(3)=%.5f  1-eta=%.4f"%(eta,H(p)+p*math.log2(3),1-eta))
lo,hi=0.03,0.06
for _ in range(100):
    mid=(lo+hi)/2
    if H(4*mid)+4*mid*math.log2(3)<1-mid: lo=mid
    else: hi=mid
print("B method optimum (same c=rho=1/4): eta*=%.6f exponent %.6f  L>=%.5f"%(lo,1-lo,1/(1-lo)))
print("2^0.956=%.6f 1/0.956=%.6f"%(2**0.956,1/0.956))
# B finite bound: N(t) <= 2^((1-eta)t) + 2(t+1) sum_{k<=K} C(t,k)3^k ; first t where bound <= 2*2^(0.956 t), and where it beats 2^t
def Bbound_log2(t):
    K=math.floor(4*0.044*t+2); Sx=sum(comb(t,k)*3**k for k in range(K+1)); v=2*(t+1)*Sx+(1<<int(0.956*t+1))  # int ceil of first term (over-estimate)
    return lg2big(v)
t_ok=None
for t in list(range(10,3000,10))+list(range(3000,20001,500)):
    lb=Bbound_log2(t)
    if lb<=0.956*t+1: 
        if t_ok is None: t_ok=t
    else: t_ok=None
print("B: explicit bound <= 2^(0.956t+1) holds from t≈",t_ok,"(sampled grid to 20000)")
first_nontriv=next(t for t in range(10,20000,10) if Bbound_log2(t)<t)
print("B: explicit bound first below 2^t at t≈",first_nontriv)
firstA=next(t for t in range(1,3000) if U(t)<2**t)
print("A: U(t) first below 2^t at t=",firstA)
