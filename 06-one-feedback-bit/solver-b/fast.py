# Fast: F(n,t) as a set-packing ILP (HiGHS via scipy.optimize.milp).
# Reduction: two outcome sets are disjoint iff their length-(n-1) parts
#   S(u,a,b) = uD(a) ∪ D(u)b  are disjoint (S nonempty for n>=1, and equal (u,a) => S's meet
#   when t<n; when t=n S=D(u) determines nothing else but (u,a)=(u,'') equal means same strategy).
# We do NOT rely on that argument in mode 'full' (uses whole outcome sets, all strategies);
# mode 'min' keeps only inclusion-minimal distinct S-sets (safe: a set in a packing may be
# replaced by any nonempty subset; the subset cannot already be present since it meets it).
import sys, numpy as np
from scipy.optimize import milp, LinearConstraint, Bounds
from scipy.sparse import lil_matrix
from common import *
def sets(n,t,mode):
    if mode=='full':
        return [(s,frozenset(outcome(*s))) for s in strategies(n,t)]
    seen={}
    for s in strategies(n,t):
        u,a,b=s
        S=frozenset({u+x for x in D(a)} | {x+b for x in D(u)})
        if S not in seen: seen[S]=s
    L=sorted(seen.items(), key=lambda kv: len(kv[0]))
    keep=[]
    for S,s in L:
        if not any(K<=S for K,_ in keep): keep.append((S,s))
    return [(s,S) for S,s in keep]
def solve(n,t,mode='min',tl=1200):
    L=sets(n,t,mode)
    pts=sorted(set().union(*[S for _,S in L])); idx={p:i for i,p in enumerate(pts)}
    A=lil_matrix((len(pts),len(L)))
    for j,(_,S) in enumerate(L):
        for p in S: A[idx[p],j]=1
    res=milp(c=-np.ones(len(L)), constraints=LinearConstraint(A.tocsr(),-np.inf,1),
             integrality=np.ones(len(L)), bounds=Bounds(0,1), options={'time_limit':tl,'mip_rel_gap':0})
    x=res.x
    fam=[L[j][0] for j in range(len(L)) if x is not None and x[j]>0.5]
    return res.status, len(fam), getattr(res,'mip_dual_bound',None), fam, len(L)
if __name__=="__main__":
    n0,n1,mode=int(sys.argv[1]),int(sys.argv[2]),sys.argv[3]
    ts=[int(x) for x in sys.argv[4].split(',')] if len(sys.argv)>4 else None
    for n in range(n0,n1+1):
        for t in (ts if ts else range(n+1)):
            st,v,db,fam,m=solve(n,t,mode)
            print(f"n={n} t={t} status={st} F={v} dualbound={db} nsets={m}",flush=True)
            if st==0 or fam:
                with open(f"wit_{mode}_n{n}_t{t}.txt","w") as f:
                    f.write(f"# n={n} t={t} size={v} status={st} (0=proved optimal) ; lines: u a b ('-' = empty)\n")
                    for u,a,b in fam: f.write(f"{u or '-'} {a or '-'} {b or '-'}\n")
