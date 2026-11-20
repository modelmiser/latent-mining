#!/usr/bin/env python3
# Family "R inside a subgroup" (see strand3.cpp / proofs L5), any number of strands: LP feasibility for every
# Ra (0 in Ra, up to translation) and every set Z of classes allowed at input 1^n.  usage: zscanR.py n a
import sys, numpy as np
from scipy.optimize import linprog
n=int(sys.argv[1]); a=int(sys.argv[2]); full=(1<<n)-1; AM=(1<<a)-1
def rot(w,j): return ((w<<j)|(w>>(a-j)))&AM if j else w
choices=[(0,0),(0,1)]+[(i,b) for i in range(1,n+1) for b in (0,1)]
gad=[]
def rec(p,c,D):
    if p==a:
        if any(d==0 for d in D[1:]): return
        rows=[]
        for x in range(full+1):
            w=c
            for i in range(n):
                if (x>>i)&1: w^=D[i+1]
            rows.append(w)
        gad.append(rows); return
    for (o,b) in (choices if p>0 else [(1,0),(1,1)]):
        D2=list(D)
        if o: D2[o]|=1<<p
        rec(p+1,c|(b<<p),D2)
rec(0,0,[0]*(n+1))
gad=np.array(gad)
tot=0
for R in range(1,AM+1,2):
    if bin(R).count('1')<2: continue
    if any((rot(R,d)&1) and rot(R,d)<R for d in range(1,a)): continue
    ids={}; cl=np.zeros(AM+1,dtype=int); wins=[]
    for w in range(AM+1):
        ws=tuple(sorted(rot(w,j)&R for j in range(a)))
        if ws not in ids: ids[ws]=len(ids); wins.append(set(ws))
        cl[w]=ids[ws]
    C=len(ids)
    conf=[sum(1<<v for v in range(C) if wins[u]&wins[v]) for u in range(C)]
    S=np.unique(cl[gad],axis=0)
    # drop gadgets whose 1^n class conflicts with one of its AND-0 classes
    keep=[k for k,s in enumerate(S) if not any((conf[s[full]]>>s[x])&1 for x in range(full))]
    S=S[keep]; G=len(S)
    if R==AM and __import__('os').environ.get('SKIPFULL'): continue
    if G==0: print(f"Ra={R:0{a}b}[::-1 bits] classes={C} usable=0: infeasible for every b"); continue
    A=np.zeros((C*(full-1),G))
    for U in range(C):
        for x in range(1,full): A[U*(full-1)+x-1]=(S[:,x]==U).astype(float)-(S[:,0]==U)
    nzmask=np.zeros(G,dtype=np.int64)
    for x in range(full): nzmask|=(1<<S[:,x])
    zbit=(1<<S[:,full]); feas=[]
    for Z in range(1,1<<C):
        cz=0
        for u in range(C):
            if (Z>>u)&1: cz|=conf[u]
        use=((zbit&Z)!=0)&((nzmask&cz)==0); g=int(use.sum())
        if g==0: continue
        Au=A[:,use]
        res=linprog(np.zeros(g),A_eq=np.vstack([Au,np.ones((1,g))]),b_eq=np.concatenate([np.zeros(Au.shape[0]),[1]]),bounds=(0,None),method="highs")
        if res.status==0: feas.append(Z)
    tot+=len(feas)
    print(f"Ra={''.join(str((R>>q)&1) for q in range(a))} classes={C} usable={G} LP-feasible Z sets={len(feas)}", flush=True)
print(f"n={n} a={a}: total feasible (Ra,Z) = {tot}")
