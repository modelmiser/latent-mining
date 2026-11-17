# Referee checks for PART 0 (C1,C2,C3) and Theorem 1 pieces.
import sys, itertools, random
from collections import Counter
from lib import *
rng=random.Random(20261117)
def allloops(L):
    for t in itertools.product('01',repeat=L): yield ''.join(t)
def necklaces(L):
    for v in allloops(L):
        if all(v<=v[i:]+v[:i] for i in range(1,L)): yield v
print("== C1 / C3: all universal loops n=3 L=8..13, n=4 L=16..19 (necklaces), random Euler loops n=5..8 e=0..6")
def c1c3(u,n,stats,falsev=False):
    L=len(u); e=L-2**n; mult=N(u,n)
    surplus=Counter({c:mult[c]-1 for c in mult if mult[c]>1})
    for k in range(1,n+1):
        Nk=N(u,k)
        for f in itertools.product('01',repeat=k):
            f=''.join(f)
            if Nk[f] < 2**(n-k)+(1 if falsev else 0): stats['c1bad']+=1
            for j in range(0,n-k+1):
                sp=sum(mu for c,mu in surplus.items() if c[j:j+k]==f)
                if Nk[f]!=2**(n-k)+sp: stats['c1exact_bad']+=1
            stats['c1']+=1
    T=2**(n-1); r=rho(u); a=(r-T)/2
    ok = (r-T)%2==0 and 0<=a<=e/2
    if falsev: ok = (r==T)
    if not ok: stats['c3bad']+=1
    stats['c3']+=1
for fv in (False,True):
    st=Counter()
    for n,Ls in ((3,range(8,14)),(4,range(16,20))):
        for L in Ls:
            for v in necklaces(L):
                if universal(v,n): c1c3(v,n,st,fv); st['loops']+=1
    for n in (5,6,7,8):
        for e in range(0,7):
            for _ in range(6):
                c1c3(euler_universal(n,random_extra(n,e,rng),rng),n,st,fv); st['loops']+=1
    print(("FALSE VARIANT (N_f>=2^(n-k)+1; rho==T): " if fv else "as stated: ")+str(dict(st)))
print("== C2: exhaustive, all loops of length k+2..11 (k=2..6), every cell, both edits; exact multiset identity")
def c2(v,c,k,variant=None):
    m=len(v); v=v[c+1:]+v[:c+1]   # edited cell now at index m-1
    bad=0
    # loss
    v1=v[:-1]; W=wins(v,k); W1=wins(v1,k)
    destroyed=[W[(m-1-j)%m] for j in range(k)]
    created=[W1[(m-2-j)%(m-1)] for j in range(k-1)]
    if N(v1,k)!=N(v,k)-Counter(destroyed)+Counter(created) or (N(v,k)-Counter(destroyed))+Counter(destroyed)!=N(v,k): bad+=1
    # doubling
    v2=v+v[-1]; W2=wins(v2,k)
    nd = k-1 if variant=='dbl_k-1' else k-2
    destroyed=[W[(m-1-j)%m] for j in range(1,nd+1)] if variant!='dbl_k-1' else [W[(m-1-j)%m] for j in range(0,k-1)]
    created=[W2[(m-1-j)%(m+1)] for j in range(k-1)]
    if N(v2,k)!=N(v,k)-Counter(destroyed)+Counter(created): bad+=2
    # inequality form: N_f(v') <= N_f(v)+c_f, c_f>=0, sum c_f <= k-1 : minimal c_f = max(0, N' - N)
    for vp in (v1,v2):
        a,b=N(vp,k),N(v,k)
        if sum(max(0,a[f]-b[f]) for f in a) > k-1: bad+=4
    return bad
for variant in (None,'dbl_k-1'):
    tot=Counter()
    for k in range(2,7):
        for L in range(k+2,12):
            for v in allloops(L):
                for c in range(L):
                    b=c2(v,c,k,variant); tot['cases']+=1
                    if b&1: tot['loss_bad']+=1
                    if b&2: tot['dbl_bad']+=1
                    if b&4: tot['ineq_bad']+=1
    print(("FALSE VARIANT (doubling destroys k-1 windows): " if variant else "as stated: ")+str(dict(tot)))
tot=Counter()
for L in range(3,9):
    for v in allloops(L):
        for c in range(L):
            a,b=N(dbl(v,c),1),N(v,1); tot['cases']+=1
            if sum(max(0,a[f]-b[f]) for f in a)>0: tot['ineq_bad']+=1
print("k=1 (outside the lemma; sum c_f <= k-1 = 0 must fail): "+str(dict(tot)))
tot=Counter()
for k in range(2,6):
    for L in (k,k+1):
        for v in allloops(L):
            for c in range(L):
                b=c2(v,c,k); tot[('L=k' if L==k else 'L=k+1','cases')]+=1
                for bit,name in ((1,'loss_bad'),(2,'dbl_bad'),(4,'ineq_bad')):
                    if b&bit: tot[('L=k' if L==k else 'L=k+1',name)]+=1
print("below the stated length k+2 (information only): "+str(dict(tot)))

print("== Theorem 1 steps 3-6: my own enumeration of mechanic options on 3-window excess vectors")
W3=[''.join(t) for t in itertools.product('01',repeat=3)]
def after_loss(x,ctx):
    o,p,c,q,r=ctx; y=dict(x)
    for f in (o+p+c,p+c+q,c+q+r): y[f]-=1
    for f in (o+p+q,p+q+r): y[f]+=1
    return y
def after_dbl(x,ctx):
    p,c,q=ctx; y=dict(x); y[p+c+q]-=1; y[p+c+c]+=1; y[c+c+q]+=1; return y
def mech_options(x):
    out=[('pass','',x)]
    for ctx in itertools.product('01',repeat=5): out.append(('loss',''.join(ctx),after_loss(x,ctx)))
    for ctx in itertools.product('01',repeat=3): out.append(('dbl',''.join(ctx),after_dbl(x,ctx)))
    return out
def ok(y): return all(val>=0 for val in y.values())
def vec(t,z): return {'010':1+t,'101':1-t,'011':-t,'110':-t,'001':t,'100':t,'000':z,'111':-z}
# step 3: the vector is the unique solution of the linear constraints
cnt=0
for xs in itertools.product(range(-3,4),repeat=8):
    x=dict(zip(W3,xs))
    if x['010']+x['101']!=2: continue
    if x['010']+x['011']!=1 or x['001']+x['101']!=1: continue   # x(01) two ways
    if x['100']+x['101']!=1 or x['010']+x['110']!=1: continue   # x(10)
    if (x['000']+x['001'])+(x['110']+x['111'])!=0: continue
    if x['000']+x['001']!=x['000']+x['100'] or x['110']+x['111']!=x['011']+x['111']: continue
    cnt+=1
    assert x==vec(x['010']-1,x['000']),x
print("step 3: every solution in [-3,3]^8 of the linear constraints has the stated form; solutions:",cnt)
surv=[];surv1=[]
for t in range(-6,7):
    for z in range(-6,7):
        a=any(ok(y) for _,_,y in mech_options(after_loss(vec(t,z),'00100')))
        b=any(ok(y) for _,_,y in mech_options(after_loss(vec(t,z),'11011')))
        if a and b: surv.append((t,z))
        if a: surv1.append((t,z))
print("step 4: (t,z) in [-6,6]^2 surviving both moves:",surv," | surviving the first move only (shows the check can fail):",len(surv1))
x0=vec(0,0)
print("step 5 options after deleting an isolated 1:",[(a,b) for a,b,y in mech_options(after_loss(x0,'00100')) if ok(y)])
y5=[y for a,b,y in mech_options(after_loss(x0,'00100')) if ok(y)]
print("   resulting excess:",[{k:v for k,v in y.items() if v} for y in y5], "(loss must end at total 0: none of the loss options listed means none feasible)")
print("step 6 options after deleting the middle of 10101:",[(a,b) for a,b,y in mech_options(after_loss(x0,'10101')) if ok(y)])
print("step 6 mirrored (middle of 01010):",[(a,b) for a,b,y in mech_options(after_loss(x0,'01010')) if ok(y)])

print("== Step 5 'Lemma Z': universal, length 2^n+2, 3-excess = (000:+2, rest 0) => exactly one 0-run >= n-1 and it has length n+2")
for fv in (False,True):
    st=Counter()
    def lz(v,n):
        x=N(v,3)
        if all(x[f]-2**(n-3)==(2 if f=='000' else 0) for f in W3):
            st['hyp']+=1
            long=[l for a,l,s in runs(v) if a=='0' and l>=n-1]
            if not (len(long)==1 and long[0]==(n+1 if fv else n+2)): st['bad']+=1
    for n in (3,4):
        for v in necklaces(2**n+2):
            if universal(v,n): st['univ']+=1; lz(v,n)
    for n in (5,6,7):
        for _ in range(40):
            lz(euler_universal(n,['0'*n,'0'*n],rng),n)
    print(("FALSE VARIANT (length n+1): " if fv else "as stated: ")+str(dict(st)))

print("== Step 0 (E1,E2,E3): random universal u0 of length 2^n+1,+2,+3; w = every inverse edit (resp. u0 itself)")
def E(w):
    R=runs(w); k=len(R); ls=[l for a,l,s in R]
    e1=any(ls[i]==1 and ls[(i+1)%k]==1 and ls[(i+2)%k]==1 for i in range(k)) and k>=4
    e2=(2 in ls) and k>=4
    iso=[R[i][0] for i in range(k) if ls[i]==1 and ls[i-1]>=2 and ls[(i+1)%k]>=2]
    e3=('1' in iso) and ('0' in iso)
    return e1,e2,e3
for n in (5,6,7,8,9):
    st=Counter()
    for rep in range(12 if n<9 else 4):
        for e in (1,2,3):
            u0=euler_universal(n,random_extra(n,e,rng),rng)
            if e==2: ws=[u0]
            elif e==1: ws=[u0[:i]+b+u0[i:] for i in range(len(u0)) for b in '01']
            else: ws=[loss(u0,i) for i in range(len(u0)) if u0[i]==u0[i-1] or u0[i]==u0[(i+1)%len(u0)]]
            for w in ws:
                assert len(w)==2**n+2
                e1,e2,e3=E(w); st['w']+=1
                if not e1: st['E1bad']+=1
                if not e2: st['E2bad']+=1
                if not e3: st['E3bad']+=1
    print("n=%d %s"%(n,dict(st)), "(proof claims n>=7 only; failures at n=5,6 show the test can fail)" if n<7 else "")
