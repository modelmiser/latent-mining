#!/usr/bin/env python3
# For strand length a: scan all sets Z of necklace classes; LP feasibility of the strand system. usage: zscan.py n a
import sys, numpy as np, pickle
from scipy.optimize import linprog
n=int(sys.argv[1]); a=int(sys.argv[2]); full=(1<<n)-1
def canon(w): return min(((w<<d)|(w>>(a-d)))&((1<<a)-1) for d in range(a))
CAN=[canon(w) for w in range(1<<a)]
classes=sorted(set(CAN)); cid={u:k for k,u in enumerate(classes)}; C=len(classes)
choices=[(0,0),(0,1)]+[(i,b) for i in range(1,n+1) for b in (0,1)]
sigs={}
def rec(p,c,D):
    if p==a:
        if any(d==0 for d in D[1:]): return
        sig=[]
        for x in range(full+1):
            w=c
            for i in range(n):
                if (x>>i)&1: w^=D[i+1]
            sig.append(cid[CAN[w]])
        if sig[full] in sig[:full]: return
        sigs.setdefault(tuple(sig),(c,tuple(D))); return
    for (o,b) in (choices if p>0 else [(1,0),(1,1)]):
        D2=list(D)
        if o: D2[o]|=1<<p
        rec(p+1,c|(b<<p),D2)
rec(0,0,[0]*(n+1))
S=np.array(list(sigs)); G=len(S)
# full constraint matrix: rows (U,x)
A=np.zeros((C*(full-1),G))
for U in range(C):
    for x in range(1,full):
        A[U*(full-1)+x-1]=(S[:,x]==U).astype(float)-(S[:,0]==U)
out=open(f"zscan_n{n}_a{a}.txt","w")
nz_mask=np.zeros(G,dtype=np.int64)
for x in range(full): nz_mask|= (1<<S[:,x])
z_bit=(1<<S[:,full])
feas=0
for Z in range(1,(1<<C)-1):
    use=((z_bit&Z)!=0)&((nz_mask&Z)==0)
    g=int(use.sum())
    if g==0: continue
    Au=A[:,use]
    res=linprog(np.zeros(g),A_eq=np.vstack([Au,np.ones((1,g))]),b_eq=np.concatenate([np.zeros(Au.shape[0]),[1]]),bounds=(0,None),method="highs")
    if res.status==0:
        feas+=1; out.write(",".join(str(classes[k]) for k in range(C) if (Z>>k)&1)+f" usable={g}\n"); out.flush()
print("n",n,"a",a,"classes",C,"usable sigs",G,"feasible Z:",feas)
