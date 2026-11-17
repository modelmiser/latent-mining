# LP (not ILP) to get bounding weights for the referee's own branch-and-bound. The weights are re-verified
# exactly (integers) inside ref.cpp against ALL strategies, so soundness does not depend on this file.
import sys,subprocess,math
import numpy as np
from scipy.optimize import linprog
from scipy.sparse import lil_matrix
n,t=int(sys.argv[1]),int(sys.argv[2]); K=10**6
sets=[list(map(int,l.split())) for l in subprocess.run(['./ref','sets',str(n),str(t)],capture_output=True,text=True).stdout.strip().split('\n')]
NP=1<<(n-1); A=lil_matrix((len(sets),NP))
for i,s in enumerate(sets):
    for p in s: A[i,p]=-1
r=linprog(np.ones(NP),A_ub=A.tocsr(),b_ub=-np.ones(len(sets)),bounds=(0,None),method='highs')
w=[max(0,math.ceil(x*K*(1+2e-6)+ (2 if x>1e-9 else 0))) for x in r.x]
print('LP value',r.fun,'scaled total',sum(w)/K,file=sys.stderr)
with open(f'w_{n}_{t}.txt','w') as f:
    f.write(f'{K}\n')
    for p,x in enumerate(w):
        if x: f.write(f'{p} {x}\n')
