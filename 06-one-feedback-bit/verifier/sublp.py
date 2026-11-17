# For each frontier subproblem (chosen count + live point mask) compute LP-dual weights on the residual instance.
# Output is re-verified exactly by ref2 'sub' (weights >= K on every available minimal set), so this is untrusted.
import sys,subprocess,math
import numpy as np
from scipy.optimize import linprog
from scipy.sparse import csr_matrix
n,t,target,fin,fout=int(sys.argv[1]),int(sys.argv[2]),int(sys.argv[3]),sys.argv[4],sys.argv[5]; K=10**6
sets=[list(map(int,l.split())) for l in subprocess.run(['./ref3','sets',str(n),str(t)],capture_output=True,text=True).stdout.strip().split('\n')]
masks=[sum(1<<p for p in s) for s in sets]
out=open(fout,'w'); nsub=0; lpcut=0
for ln in open(fin):
    f=ln.split(); ch=int(f[0]); live=sum(int(f[1+i],16)<<(64*i) for i in range(4)); nsub+=1
    av=[i for i,m in enumerate(masks) if m&~live==0]
    pts=sorted({p for i in av for p in sets[i]}); idx={p:j for j,p in enumerate(pts)}
    rows=[];cols=[]
    for r,i in enumerate(av):
        for p in sets[i]: rows.append(r); cols.append(idx[p])
    A=csr_matrix((-np.ones(len(rows)),(rows,cols)),shape=(len(av),len(pts)))
    r=linprog(np.ones(len(pts)),A_ub=A,b_ub=-np.ones(len(av)),bounds=(0,None),method='highs')
    w=[max(0,math.ceil(x*K*(1+2e-6)+(2 if x>1e-9 else 0))) for x in r.x]
    if ch+sum(w)//K<target: lpcut+=1
    out.write(' '.join(f[:5])+f' {K} '+' '.join(f'{p}:{x}' for p,x in zip(pts,w) if x)+'\n')
print('subproblems',nsub,'cut by LP weights at their root',lpcut,file=sys.stderr)
