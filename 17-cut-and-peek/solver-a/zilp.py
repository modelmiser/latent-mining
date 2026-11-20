#!/usr/bin/env python3
# For each LP-feasible Z (from zscan file), ILP for the least number b of strands, with cutoff = best so far - 1.
# usage: zilp.py n a parity(0/1) budget_seconds per_Z_timelimit ; progress kept in zilp_n{n}_a{a}_p{parity}.log (resumable)
import sys, os, time, numpy as np
from scipy.optimize import milp, LinearConstraint, Bounds
n=int(sys.argv[1]); a=int(sys.argv[2]); par=int(sys.argv[3]); budget=float(sys.argv[4]); TL=float(sys.argv[5]); full=(1<<n)-1
t0=time.time()
def canon(w): return min(((w<<d)|(w>>(a-d)))&((1<<a)-1) for d in range(a))
CAN=[canon(w) for w in range(1<<a)]
def comp(w): return CAN[w^((1<<a)-1)]
def rev(w): return CAN[int(format(w,f'0{a}b')[::-1],2)]
Zs=[]
seen=set()
for l in open(f"zscan_n{n}_a{a}.txt"):
    z=frozenset(int(t) for t in l.split()[0].split(',')); g=int(l.split('=')[1])
    imgs=[z,frozenset(comp(u) for u in z),frozenset(rev(u) for u in z),frozenset(rev(comp(u)) for u in z)]
    key=min(tuple(sorted(i)) for i in imgs)
    if key in seen: continue
    seen.add(key); Zs.append((g,tuple(sorted(z))))
Zs.sort(reverse=True)
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
        if sig[full] in sig[:full]: return
        sigs.setdefault(tuple(sig),(c,tuple(D))); return
    for (o,b) in (choices if p>0 else [(1,0),(1,1)]):
        D2=list(D)
        if o: D2[o]|=1<<p
        rec(p+1,c|(b<<p),D2)
rec(0,0,[0]*(n+1))
S=np.array(list(sigs)); allsig=list(sigs)
logf=f"zilp_n{n}_a{a}_p{par}.log"; done={}
best=int(os.environ.get("BEST","23"))
if os.path.exists(logf):
    for l in open(logf):
        f=l.split(); done[f[0]]=f[1]
        if f[1].startswith("b="): best=min(best,int(f[1][2:]))
for other in (f"zilp_n{n}_a{a}_p{1-par}.log",):
    if os.path.exists(other):
        for l in open(other):
            f=l.split()
            if f[1].startswith("b="): best=min(best,int(f[1][2:]))
log=open(logf,"a")
for idx,(g,z) in enumerate(Zs):
    if idx%2!=par: continue
    zk=",".join(map(str,z))
    if zk in done: continue
    if time.time()-t0>budget: break
    Zset=set(z)
    use=[k for k,s in enumerate(allsig) if s[full] in Zset and not (set(s[:full])&Zset)]
    Su=S[use]; G=len(use); classes=sorted(set(CAN)-Zset)
    rows=[]
    for U in classes:
        for x in range(1,full): rows.append((Su[:,x]==U).astype(float)-(Su[:,0]==U))
    rows.append(np.ones(G)); A=np.array(rows)
    lo=[0]*(len(rows)-1)+[1]; hi=[0]*(len(rows)-1)+[best-1]
    res=milp(np.ones(G),constraints=LinearConstraint(A,lo,hi),integrality=np.ones(G),bounds=Bounds(np.zeros(G),np.full(G,best)),options={"time_limit":TL})
    if res.x is not None:
        mu=np.round(res.x).astype(int); b=int(mu.sum()); best=min(best,b)
        with open(f"strands_n{n}_a{a}_b{b}.txt","w") as f:
            for gi in np.nonzero(mu)[0]:
                c,D=sigs[allsig[use[gi]]]
                cw=''.join(str((c>>p)&1) for p in range(a)); ow=''.join(str(max([i for i in range(1,n+1) if (D[i]>>p)&1],default=0)) for p in range(a))
                for _ in range(mu[gi]): f.write(f"{cw} {ow}\n")
        log.write(f"{zk} b={b} status={res.status} usable={G}\n")
    else:
        log.write(f"{zk} {'none_below_'+str(best) if res.status==2 else 'timeout_cutoff_'+str(best)} usable={G}\n")
    log.flush()
print("orbits",len(Zs),"best",best,"elapsed",time.time()-t0)
