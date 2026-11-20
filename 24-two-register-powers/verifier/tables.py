#!/usr/bin/env python3
# referee: (1) ordered-pair direct enumeration N(0..12); (2) D(m) from definition 1 (integer values); comparisons.
import math,sys
S=""
# (1)
lev={(1,1)}; G={}
Ns=[]
for t in range(13):
    for a,b in lev:
        G.setdefault(a,t); G.setdefault(b,t)
    Ns.append(len(G))
    lev={s for a,b in lev for s in ((2*a,b),(a,2*b),(a+b,b),(a,a+b))}
print("python ordered-pair N(0..12):",Ns)
levN=[int(l.split()[3]) for l in open("lev22.out") if l.startswith("t ")]
print("C++ N(0..22):",levN)
assert levN[:13]==Ns
V={}
for l in open("vals.txt"):
    n,g=map(int,l.split()); V[n]=g
assert all(V[n]==g for n,g in G.items()) and all(G.get(n)==g for n,g in V.items() if g<=12)
print("vals.txt G agrees with python for all n with G<=12:",len(G))
# compare solver G tables
for name,fn in (("A nt_G",S+"solver-a/nt_G.txt"),("A slow_G",S+"solver-a/slow_G.txt"),("B g_15",S+"solver-b/g_15.txt"),("B g_12",S+"solver-b/g_12.txt")):
    d=0;c=0;unk=0
    for l in open(fn):
        p=l.split()
        if len(p)<2: continue
        n,g=int(p[0]),int(p[1])
        if g<0 or g>17:
            if n in V: d+=1
            unk+=1; continue
        c+=1
        if V.get(n)!=g: d+=1
    print(name,"entries with G in 0..17 compared:",c,"other entries:",unk,"mismatches:",d)
A_N=[int(l.split()[1]) for l in open(S+"solver-a/nt22.txt")]
print("A N == mine:",A_N==levN)
B_N=[int(l.split()[3]) for l in open(S+"solver-b/gbfs21.out") if l.startswith("t ")]
print("B N == mine[:22]:",B_N==levN[:22])
# (2) definition 1
byn=sorted(V.items())
my={}
for m in range(2,4101):
    g=[99]*m
    for n,gg in byn:
        r=n%m
        if gg<g[r]: g[r]=gg
    D=max(g)
    if D==99: print("UNCOVERED m",m); continue
    my[m]=(D,g.count(D),g.index(D))
dm={}
for l in open("dmod_all.out"):
    p=list(map(int,l.split())); dm[p[0]]=tuple(p[1:])
print("def1 vs def2 (mod BFS) mismatches (all 3 columns):",[m for m in my if my[m]!=dm.get(m)][:10], "count m:",len(my),len(dm))
A={}
for l in open(S+"solver-a/D_all.txt"):
    p=list(map(int,l.split())); A[p[0]]=tuple(p[1:])
print("A D_all range",min(A),max(A),"mismatch vs mine (D,#worst,first worst):",[m for m in A if A[m]!=my.get(m)][:10])
B={}
for l in open(S+"solver-b/D_table.txt"):
    p=list(map(int,l.split())); B[p[0]]=p[1]
print("B D_table range",min(B),max(B),"mismatch vs mine:",[m for m in B if B[m]!=my[m][0]][:10])
for fn in ("solver-a/slow_D.txt","solver-a/D_400.txt"):
    X={int(l.split()[0]):tuple(map(int,l.split()[1:])) for l in open(S+fn)}
    print(fn,len(X),"mismatch:",[m for m in X if X[m]!=my[m][:len(X[m])]][:5])
print("D(2..40):",[my[m][0] for m in range(2,41)])
print("D(400)=",my[400])
def T(m):
    for t,v in enumerate(levN):
        if v>=m: return t
for hi in (4096,4100):
    h={}
    for m in range(2,hi+1): h[my[m][0]-T(m)]=h.get(my[m][0]-T(m),0)+1
    print("gap D-T histogram m<=%d:"%hi,h,"gap-2 m:",[m for m in range(2,hi+1) if my[m][0]-T(m)==2])
viol=[m for m in my if not (math.ceil(math.log2(m)-1e-12)<=my[m][0]<=2*(m.bit_length()-1))]
print("easy bound violations:",viol)
first={};last={}
for m in sorted(my): first.setdefault(my[m][0],m); last[my[m][0]]=m
print("first/last:",[(d,first[d],last[d]) for d in sorted(first)])
for lo,hi in ((64,128),(256,512),(1024,2048),(2048,4096),(400,4101)):
    x=min((my[m][0]/math.log2(m),m) for m in range(lo,hi)); print("min ratio [%d,%d):"%(lo,hi),round(x[0],4),x[1])
print("ratios N(t+1)/N(t):",[round(levN[t+1]/levN[t],4) for t in range(15,22)])
# worst residue file of A
bad=0;cnt=0
gcache={}
for l in open(S+"solver-a/worst_residue_m400.txt"):
    m,D,r=map(int,l.split()); cnt+=1
    g=min(gg for n,gg in byn if n%m==r)
    if D!=my[m][0] or g!=D: bad+=1
print("A worst_residue lines:",cnt,"bad:",bad)
import pickle; pickle.dump((my,levN),open("my.pkl","wb"))
