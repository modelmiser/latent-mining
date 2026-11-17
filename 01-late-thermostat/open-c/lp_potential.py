"""EXPLORATORY (not a proof): idealised 'full corridor' bottom-state model.
State of one corridor: (f,r) = min free level, min rest level (all higher levels assumed present).
Search (LP) for rho and a potential Phi with  g' - 1 + rho*(c-1) + Phi(s') - Phi(s) <= -eps  on all transitions.
eps>0 would give the steady-state contradiction IN THE IDEALISED MODEL."""
import itertools, numpy as np
from scipy.optimize import linprog
CAP=7
def step(st,p):
    f,r=st; m=min(f,r)
    if p==0:  nf,nr=m,max(1,f-1)
    elif p==-1: nf,nr=max(1,m-1),max(1,f-2)
    else: nf,nr=m+1,f
    if nf>CAP or nr>CAP: return None   # bottom pushed past the top of the band: clock loses, transition illegal
    return (nf,nr)
def arrivals(st,p):
    """levels reachable at t+1 by plays in the corridor at t (levels>=f free, >=r rest), incl. exits (<=0)."""
    f,r=st; out=set()
    for a in range(f,CAP+4):
        for w in (-1,0,1): out.add(a+p+w)
    for a in range(r,CAP+4): out.add(a+p)
    return out
states=set(); fr=[(1,1)]
while fr:
    s=fr.pop()
    if s in states: continue
    states.add(s)
    for p in (-1,0,1):
        if step(s,p) is not None: fr.append(step(s,p))
states=sorted(states); print("single-corridor states:",states)
joint=[(a,b) for a in states for b in states]; idx={s:i for i,s in enumerate(joint)}
def e(st): return min(st[0],st[1]+1)
rows=[];rhs=[]
nv=len(joint)+3  # Phi..., rho_n, rho_p, eps
for (sp,sn) in joint:
    for n in (-1,0,1):
        for p in (-1,0,1):
            sp2=step(sp,p); sn2=step(sn,-n)   # negative corridor mirrored: control -n acts like p on |x|
            if sp2 is None or sn2 is None: continue
            Bs=arrivals(sp,p); As={-v for v in arrivals(sn,-n)}
            g=e(sp2)+e(sn2)
            for be in Bs:
                for al in As:
                    if be>=-1 and al<=1 and ((be>0 and al>0) or (be==0 and al==0) or (be<0 and al<0)):
                        g=min(g,be-al)
            c=n-p
            row=np.zeros(nv); row[idx[(sp2,sn2)]]+=1; row[idx[(sp,sn)]]-=1; row[-3]=n-0.5; row[-2]=-p-0.5; row[-1]=1
            rows.append(row); rhs.append(1-g)
cost=np.zeros(nv); cost[-1]=-1
bounds=[(-200,200)]*len(joint)+[(-20,20),(-20,20),(0,1)]
res=linprog(cost,A_ub=np.array(rows),b_ub=np.array(rhs),bounds=bounds)
print("status",res.status,res.message)
print("eps=",res.x[-1],"rho_n,rho_p=",res.x[-3],res.x[-2])
if res.x[-1]>1e-9:
    for s in joint: print(s,round(res.x[idx[s]],3))
