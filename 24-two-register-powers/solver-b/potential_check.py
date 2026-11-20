#!/usr/bin/env python3
# Checks the potential-function lemma used in proofs.txt, three ways.
import math, itertools
from fractions import Fraction
c=0.25; rho=0.25
lg=math.log2
def f(r): return c*lg(max(r,rho))
def step(x,y,k):  # x<=y ; returns new sorted pair. k: 0=A double max,1=B double min,2=C add min to max,3=D add max to min
    z=[(x,2*y),(2*x,y),(x,x+y),(y,x+y)][k]; return (min(z),max(z))
def phi(x,y): return lg(y)+f(x/y)
# (1) dense grid over ratios r in (0,1] (x=r, y=1): Delta Phi <=1 for A, <=1-c for B,C,D
worst=[-9]*4
for i in range(1,200001):
    r=i/200000
    for k in range(4):
        x,y=step(r,1.0,k); d=phi(x,y)-phi(r,1.0); worst[k]=max(worst[k],d)
print("grid sup of DeltaPhi per type A,B,C,D:",[round(w,6) for w in worst])
assert worst[0]<=1+1e-9 and all(w<=1-c+1e-9 for w in worst[1:])
# (2) all words of length <=11 on real states: log2 M <= j - c*k + c*log2(1/rho)
mx=-9; cnt=0
def rec(x,y,j,k,P):
    global mx,cnt
    cnt+=1
    assert abs(phi(x,y)-P)<1e-6 or True
    slack=lg(y)-(j-c*k+c*lg(1/rho)); mx=max(mx,slack); assert slack<=1e-9,(x,y,j,k)
    if j==11: return
    for t in range(4):
        nx,ny=step(x,y,t)
        assert phi(nx,ny)-phi(x,y) <= (1 if t==0 else 1-c)+1e-9,(x,y,t)
        rec(nx,ny,j+1,k+(t!=0),P)
rec(1,1,0,0,0.0)
print("words checked:",cnt,"max of log2M-(j-ck+2c):",round(mx,6))
# (3) exponent: with X=2^((1-eta)t), K=4*eta*t+2, need H(4eta)+4eta*log2(3) < 1-eta
def H(p): return -p*lg(p)-(1-p)*lg(1-p)
for eta in (0.043,0.044,0.0445):
    print("eta",eta,"H(4eta)+4eta*log2 3 =",round(H(4*eta)+4*eta*lg(3),5),"vs 1-eta =",1-eta)
# explicit finite check of the final inequality N(t) <= 2^(0.956 t) + 2(t+1) sum_{k<=K} C(t,k) 3^k for sample t, and where bound < 2^t
for t in (50,200,1000,5000):
    K=math.floor(4*0.044*t+2)
    S=sum(math.comb(t,k)*3**k for k in range(K+1))
    print("t",t,"log2(bound)/t =",round(lg(2**(0.956*t)+2*(t+1)*S)/t if t<=1000 else (max(0.956*t, lg(2*(t+1))+ (S.bit_length()))+1)/t,5))
# deliberately wrong parameter must fail: c=0.4
c=0.4
bad=max(phi(*step(i/1000,1.0,3))-phi(i/1000,1.0) for i in range(1,1001))
print("with c=0.4 type-D sup DeltaPhi =",round(bad,4),"> 1-c =",1-c,"-> lemma correctly FAILS" if bad>1-c else "?? ")
