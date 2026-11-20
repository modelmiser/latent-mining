#!/usr/bin/env python3
import math
from math import comb
D={}
for l in open('D_all.txt'):
    m,d,c,w=map(int,l.split()); D[m]=(d,c,w)
N=[int(l.split()[1]) for l in open('nt22.txt')]
def lbN(m):
    for t,v in enumerate(N):
        if v>=m: return t
bad=[m for m in D if not (math.ceil(math.log2(m)-1e-12)<=D[m][0]<=2*(m.bit_length()-1))]
print("easy bounds ceil(log2 m)<=D<=2floor(log2 m) violations:",bad)
print("N-bound violations (D(m) < min{t:N(t)>=m}):",[m for m in D if D[m][0]<lbN(m)])
print("gap D(m)-min{t:N(t)>=m} histogram:",{g:sum(1 for m in D if D[m][0]-lbN(m)==g) for g in range(0,6)})
# first m and last m for each D value
first={};last={}
for m in sorted(D):
    d=D[m][0]; first.setdefault(d,m); last[d]=m
print("d: first m with D=d, last m<=4100 with D=d, min{t..} thresholds N(d-1)+1")
for d in sorted(first): print(d,first[d],last[d],N[d-1]+1 if d>=1 else 1)
r=[(D[m][0]/math.log2(m),m) for m in D]
for lo in (2,100,400,1000,2000,3000):
    s=[x for x in r if x[1]>=lo]; print("m>=%d: min ratio %.4f at m=%d, max %.4f at m=%d"%(lo,min(s)[0],min(s)[1],max(s)[0],max(s)[1]))
print("N ratios:",["%.4f"%(N[t+1]/N[t]) for t in range(len(N)-1)])
print("N(t)^(1/t):",["%.4f"%(N[t]**(1/t)) for t in range(1,len(N))])
# G table checks
G={}
for l in open('nt_G.txt'):
    n,g=map(int,l.split()); G[n]=g
S={}
for l in open('slow_G.txt'):
    n,g=map(int,l.split()); S[n]=g
print("slow vs fast G mismatches (n with slow G<=12):",[n for n in S if n in G and G[n]!=S[n]])
ex=[n for n in G if G[n]>=0 and G[n]==math.ceil(math.log2(n)) and bin(n).count('1')>2]
ex2=[n for n in G if bin(n).count('1')<=2 and G[n]!=(n.bit_length()-1 + (bin(n).count('1')-1))]
print("n<=2^14: G=ceil(log2 n) with popcount>2:",ex[:5]," popcount<=2 but G!=ceil:",ex2[:5])
print("binary-method bound G<=floor(log2 n)+popcount-1 violations:",[n for n in G if G[n]>n.bit_length()+bin(n).count('1')-2][:5])
# rigorous upper bound U(t) on N(t)
F=[0,1]
for i in range(4000): F.append(F[-1]+F[-2])
def U(t): return sum(min(2**j*comb(s+j+1,2*j+1),2**(s-j)*F[j+2]) for s in range(t+1) for j in range(s+1))
print("U(t)>=N(t) check:",all(U(t)>=N[t] for t in range(len(N))),[U(t) for t in range(8)])
for t in (100,200,400,800,1600): print("t=%d log2(U(t))/t = %.5f"%(t,math.log2(U(t))/t))
# exponent: max_delta min(delta+(1+delta)H(2delta/(1+delta)), 1-delta*(1-log2 phi))
H=lambda x:0 if x<=0 or x>=1 else -x*math.log2(x)-(1-x)*math.log2(1-x)
phi=(1+5**.5)/2
best=max((min(d+(1+d)*H(2*d/(1+d)),1-d*(1-math.log2(phi))),d) for d in [i/1e6 for i in range(1,400000)])
print("gamma=%.6f at delta=%.6f  lambda<=%.5f  L>=%.5f"%(best[0],best[1],2**best[0],1/best[0]))
