#!/usr/bin/env python3
# Subgroup-R family: m = a*b, R = multiples of b. Row splits into b strands (cyclic words of length a);
# M(x) = union over strands of all a rotations. Search (MILP) for a multiset of strand gadgets with
# class counts constant on AND-0 inputs and 111-classes disjoint from AND-0 classes.
# usage: strands.py n a [weak]
import sys, itertools, numpy as np
from scipy.optimize import milp, LinearConstraint, Bounds
from scipy.sparse import lil_matrix
n=int(sys.argv[1]); a=int(sys.argv[2]); weak=len(sys.argv)>3
full=(1<<n)-1
def canon(w):
    best=w
    for d in range(1,a):
        r=((w<<d)|(w>>(a-d)))&((1<<a)-1)
        if r<best: best=r
    return best
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
        sig=tuple(sig)
        if sig not in sigs: sigs[sig]=(c,tuple(D))
        return
    opts=choices if p>0 else [(1,0),(1,1)]
    for (o,b) in opts:
        D2=list(D); 
        if o: D2[o]|=1<<p
        rec(p+1,c|(b<<p),D2)
rec(0,0,[0]*(n+1))
allsig=list(sigs)
if not weak:
    use=[s for s in allsig if s[full] not in s[:full]]
else: use=allsig
classes=sorted(set(CAN))
cid={u:k for k,u in enumerate(classes)}
print(f"n={n} a={a} gadget signatures={len(allsig)} usable={len(use)} classes={len(classes)}")
if not use: print("RESULT infeasible (no usable gadget)"); sys.exit()
G=len(use); C=len(classes); B=1
nv=G+C  # mu_g integer, z_U binary
rows=[];lo=[];hi=[]
A=lil_matrix((C*(full-1)+ (G*2 if not weak else 1) +1, nv))
r=0
for U in classes:
    for x in range(1,full):
        for g,s in enumerate(use):
            v=(s[x]==U)-(s[0]==U)
            if v: A[r,g]=v
        lo.append(0);hi.append(0);r+=1
if not weak:
    for g,s in enumerate(use):
        # mu_g <= B*z[s[full]]
        A[r,g]=1; A[r,G+cid[s[full]]]=-B; lo.append(-np.inf); hi.append(0); r+=1
    # for each class U: sum_{g uses U at AND-0} mu_g <= BIG*(1-z_U)  (aggregate)
    A2=lil_matrix((C,nv)); 
    for g,s in enumerate(use):
        for U in set(s[:full]): A2[cid[U],g]=1
    for U in classes: A2[cid[U],G+cid[U]]=1
    A=A[:r]
    from scipy.sparse import vstack
    A=vstack([A.tocsr(),A2.tocsr()]); lo+= [-np.inf]*C; hi+=[1]*C; r+=C
    row=lil_matrix((1,nv)); row[0,:G]=1
    A=vstack([A,row.tocsr()]); lo.append(1); hi.append(1)
else:
    # weak: some class count differs at 111: try each class and sign separately below
    A=A[:r]
cobj=np.zeros(nv); cobj[:G]=1
integ=np.ones(nv)
if not weak: integ[:G]=0
ub=np.array([B]*G+[1]*C); lb=np.zeros(nv)
if not weak:
    res=milp(cobj,constraints=LinearConstraint(A.tocsr(),lo,hi),integrality=integ,bounds=Bounds(lb,ub),options={"time_limit":1200})
    print("status",res.status,res.message)
    if res.x is not None:
        mu=res.x[:G].copy(); mu[mu<1e-9]=0
        print("RESULT feasible (rational weights; scale to integers), support=",np.count_nonzero(mu))
        for g in np.nonzero(mu)[0]:
            c,D=sigs[use[g]]; print(" mult",mu[g]," c=",format(c,f'0{a}b')[::-1]," D=",[format(d,f'0{a}b')[::-1] for d in D[1:]]," sig=",use[g])
    else: print("RESULT infeasible" if res.status==2 else "RESULT unknown")
else:
    from scipy.sparse import vstack
    found=False
    for U in classes:
        for sgn in (1,-1):
            row=lil_matrix((1,nv))
            for g,s in enumerate(use):
                v=(s[full]==U)-(s[0]==U)
                if v: row[0,g]=sgn*v
            AA=vstack([A.tocsr(),row.tocsr()])
            res=milp(cobj,constraints=LinearConstraint(AA,lo+[1],hi+[np.inf]),integrality=integ,bounds=Bounds(lb,ub),options={"time_limit":300})
            if res.x is not None:
                mu=np.round(res.x[:G]).astype(int); found=True
                print("WEAK feasible class",U,"b=",mu.sum())
                for g in np.nonzero(mu)[0]:
                    c,D=sigs[use[g]]; print(" mult",mu[g]," c=",format(c,f'0{a}b')[::-1]," D=",[format(d,f'0{a}b')[::-1] for d in D[1:]])
                break
        if found: break
    if not found: print("WEAK infeasible")
