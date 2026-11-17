# Referee's own branch-and-bound for "no packing of `target` sets exists".
# Every pruning is certified in exact integer arithmetic here: a node (chosen sets fixed, some sets forbidden) is closed
# only if integer weights w>=0 on the live short words satisfy w(S)>=K for EVERY still-available minimal set S and
# chosen + floor(sum(w)/K) < target.  HiGHS is used only as an LP (not MILP) oracle proposing w and a branching set;
# a wrong proposal can only make a node fail to close, never close it wrongly.  Sets come from ref3 'sets'
# (inclusion-minimal S(u,a,b); dominance is value-preserving).
import sys,subprocess,math,time
import numpy as np
from scipy.optimize import linprog
from scipy.sparse import csr_matrix
n,t,target,tl=int(sys.argv[1]),int(sys.argv[2]),int(sys.argv[3]),float(sys.argv[4]); K=10**6
sets=[list(map(int,l.split())) for l in subprocess.run(['./ref3','sets',str(n),str(t)],capture_output=True,text=True).stdout.strip().split('\n')]
NP=1<<(n-1); m=len(sets)
rows=[i for i,s in enumerate(sets) for p in s]; cols=[p for s in sets for p in s]
A=csr_matrix((np.ones(len(rows),dtype=np.int64),(rows,cols)),shape=(m,NP))
AT=A.T.tocsr()
t0=time.time(); nodes=0; closed=0; maxdepth=0
stack=[(0,np.ones(m,dtype=bool),0)]   # (chosen, available mask over sets, depth)
while stack:
    if time.time()-t0>tl: print(f'TIMEOUT n={n} t={t} target={target} nodes={nodes} open={len(stack)}'); sys.exit(2)
    ch,av,dep=stack.pop(); nodes+=1; maxdepth=max(maxdepth,dep)
    idx=np.nonzero(av)[0]
    if ch>=target: print('FOUND packing of size',ch); sys.exit(1)
    if len(idx)==0: closed+=1; continue           # bound = ch < target
    Asub=A[idx]
    r=linprog(np.ones(NP),A_ub=-Asub.astype(float),b_ub=-np.ones(len(idx)),bounds=(0,None),method='highs')
    w=np.array([max(0,math.ceil(x*K*(1+2e-6)+(2 if x>1e-9 else 0))) for x in r.x],dtype=np.int64)
    exact_ok = int((Asub@w).min())>=K and int(w.min())>=0
    if exact_ok and ch+int(w.sum())//K<target: closed+=1; continue
    x=-r.ineqlin.marginals                     # primal packing LP values (hint only)
    frac=np.minimum(x,1-x); j=int(np.argmax(frac))
    if frac[j]<1e-6: j=int(np.argmax(x))       # LP integral: branch on a chosen set
    s=idx[j]
    # child "s not used"
    av0=av.copy(); av0[s]=False; stack.append((ch,av0,dep+1))
    # child "s used": remove every set meeting s
    meet=np.zeros(m,dtype=bool)
    for p in sets[s]: meet[AT[p].indices]=True
    av1=av&~meet; stack.append((ch+1,av1,dep+1))
print(f'NONE n={n} t={t} target={target}: no packing of {target} sets; nodes={nodes} closed={closed} maxdepth={maxdepth} minimal_sets={m} time={time.time()-t0:.0f}s')
