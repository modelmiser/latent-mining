#!/usr/bin/env python3
# ILP for strands with a FIXED set Z of "all-ones-only" necklace classes. usage: strands_ilp.py n a Z(comma list of canonical words) [timelimit]
import sys, numpy as np
from scipy.optimize import milp, LinearConstraint, Bounds
from scipy.sparse import lil_matrix
n=int(sys.argv[1]); a=int(sys.argv[2]); Z=set(int(t) for t in sys.argv[3].split(',')); TL=float(sys.argv[4]) if len(sys.argv)>4 else 600
full=(1<<n)-1
def canon(w):
    return min(((w<<d)|(w>>(a-d)))&((1<<a)-1) for d in range(a))
CAN=[canon(w) for w in range(1<<a)]
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
            sig.append(CAN[w])
        if sig[full] in Z and not (set(sig[:full])&Z):
            sig=tuple(sig)
            if sig not in sigs: sigs[sig]=(c,tuple(D))
        return
    for (o,b) in (choices if p>0 else [(1,0),(1,1)]):
        D2=list(D)
        if o: D2[o]|=1<<p
        rec(p+1,c|(b<<p),D2)
rec(0,0,[0]*(n+1))
use=list(sigs); G=len(use); classes=sorted(set(CAN)-Z); cid={u:k for k,u in enumerate(classes)}
print("usable for this Z:",G)
A=lil_matrix((len(classes)*(full-1)+1,G)); r=0; lo=[];hi=[]
for U in classes:
    for x in range(1,full):
        for g,s in enumerate(use):
            v=(s[x]==U)-(s[0]==U)
            if v: A[r,g]=v
        lo.append(0);hi.append(0);r+=1
A[r,:]=1; lo.append(1); hi.append(np.inf)
res=milp(np.ones(G),constraints=LinearConstraint(A.tocsr(),lo,hi),integrality=np.ones(G),bounds=Bounds(np.zeros(G),np.full(G,20)),options={"time_limit":TL,"disp":False})
print("status",res.status,res.message)
if res.x is not None:
    mu=np.round(res.x).astype(int); b=int(mu.sum())
    print("b=",b,"m=",a*b)
    with open(f"strands_n{n}_a{a}_b{b}.txt","w") as f:
        for g in np.nonzero(mu)[0]:
            c,D=sigs[use[g]]
            cw=''.join(str((c>>p)&1) for p in range(a)); ow=''.join(str(max([i for i in range(1,n+1) if (D[i]>>p)&1],default=0)) for p in range(a))
            for _ in range(mu[g]): f.write(f"{cw} {ow}\n"); print(cw,ow)
