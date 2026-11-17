# Upper-bound certificates from the LP dual: integer weights w(x)>=0 on x in {0,1}^(n-1), scale K,
# such that every strategy's length-(n-1) outcome set S has w(S) >= K and sum(w) < (F+1)*K.
# Then any packing has size <= sum(w)/K < F+1.  Weights are rounded UP so feasibility survives.
import sys, math, numpy as np
from scipy.optimize import linprog
from scipy.sparse import lil_matrix
from fast import sets
K=10**6
def cert(n,t,F):
    L=sets(n,t,'min'); pts=sorted(set().union(*[S for _,S in L])); idx={p:i for i,p in enumerate(pts)}
    A=lil_matrix((len(L),len(pts)))
    for j,(_,S) in enumerate(L):
        for p in S: A[j,idx[p]]=1
    r=linprog(np.ones(len(pts)),A_ub=-A.tocsr(),b_ub=-np.ones(len(L)),bounds=(0,None),method='highs')
    w=[int(math.ceil(x*K*(1+1e-7)+1e-3)) if x>1e-12 else 0 for x in r.x]
    # repair to exact feasibility on minimal sets (integer arithmetic)
    for _,S in L:
        d=K-sum(w[idx[p]] for p in S)
        if d>0: w[idx[min(S)]]+=d
    tot=sum(w); ok = tot < (F+1)*K
    if ok:
        with open(f"cert_n{n}_t{t}.txt","w") as f:
            f.write(f"# upper-bound certificate: n={n} t={t} claims F<={F}; K={K}; lines: point weight; sum={tot}\n")
            for p in pts:
                if w[idx[p]]: f.write(f"{p} {w[idx[p]]}\n")
    return r.fun, tot/K, ok
if __name__=="__main__":
    for line in open(sys.argv[1]):
        n,t,F=map(int,line.split())
        lp,tot,ok=cert(n,t,F)
        print(f"n={n} t={t} F={F} LP={lp:.6f} certsum={tot:.6f} {'CERT WRITTEN' if ok else 'LP bound not tight: no certificate'}",flush=True)
