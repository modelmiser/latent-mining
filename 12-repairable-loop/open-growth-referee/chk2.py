# Referee checks: Theorem 1 structure (literal brute force) and Theorem 2 lemmas G1-G4, (**), arithmetic.
import sys, itertools, random, glob, re
from collections import Counter
from fractions import Fraction
from lib import *
rng=random.Random(4711)
B=''
W3=[''.join(t) for t in itertools.product('01',repeat=3)]
# ---------- repairable loops (my own brute-force lists; n=5,6 from solver files, re-verified here by lib.repairable)
R={3:[],4:[],5:[],6:[]}
for n,Ls in ((3,range(10,17)),(4,range(18,23))):
    for L in Ls: R[n]+=load('rep_n%d_L%d.txt'%(n,L))
c5=set(load(B+'solver-b/n5L34.txt'))
R[5]=[v for v in sorted(c5) if repairable(v,5)]
c6=set()
for fn in glob.glob(B+'solver-a/*.txt')+glob.glob(B+'solver-b/*.txt'):
    try: txt=open(fn,errors='ignore').read(400000)
    except Exception: continue
    for s in re.findall(r'(?<![01])[01]{67,90}(?![01])',txt): c6.add(s)
c6=sorted(c6); rng.shuffle(c6)
for v in c6:
    if len(R[6])>=40: break
    if repairable(v,6): R[6].append(v)
print("repairable loops:",{n:len(v) for n,v in R.items()},"| candidates n=5:",len(c5),"n=6 strings seen:",len(c6))
print("  lengths n=6:",sorted(Counter(len(v) for v in R[6]).items()))
print("  non-universal repairable:",{n:sum(1 for v in R[n] if not universal(v,n)) for n in R})

print("== Theorem 1, consistency on real loops of length 2^n+2 (n=4,5; the proof itself needs n>=7)")
for n in (4,5):
    st=Counter()
    for v in R[n]:
        if len(v)!=2**n+2: continue
        st['loops']+=1
        x={f:N(v,3)[f]-2**(n-3) for f in W3}
        if S(v)!=2**(n-2)+2: st['S_bad']+=1
        if rho(v)!=2**(n-1)+2: st['rho_bad']+=1
        if x!={f:(1 if f in('010','101') else 0) for f in W3}: st['vec_bad']+=1
        Rr=runs(v); k=len(Rr)
        for i in range(k):
            if Rr[i][1]==1 and Rr[i-1][1]>=2 and Rr[(i+1)%k][1]>=2:
                st['iso']+=1
                if sorted((Rr[i-1][1],Rr[(i+1)%k][1]))!=[2,n]: st['iso_not_{n,2}']+=1
    print(" n=%d %s"%(n,dict(st)))

print("== Theorem 1 steps 5,6,7 literally, on loops 'de Bruijn + pair' (which have the forced 3-excess), brute-force replies")
def dpair(n):
    D=euler_universal(n,[],rng); m=len(D)
    c=rng.choice([i for i in range(m) if D[i-1]+D[i]+D[(i+1)%m]=='101'])
    w1=dbl(D,c); m1=len(w1)
    c=rng.choice([i for i in range(m1) if ''.join(w1[(i+j)%m1] for j in range(-1,3))=='1001'])
    return w1[:c+1]+'1'+w1[c+1:]
for n in (4,5,7,8):
    st=Counter()
    for rep in range({4:60,5:60,7:14,8:5}[n]):
        w=dpair(n); m=len(w); st['loops']+=1
        assert {f:N(w,3)[f]-2**(n-3) for f in W3}=={f:(1 if f in('010','101') else 0) for f in W3}
        Rr=runs(w); k=len(Rr); rep_ok=True
        for i in range(k):
            a,l,s=Rr[i]
            if l!=1: continue
            p,q=Rr[i-1][1],Rr[(i+1)%k][1]
            if p>=2 and q>=2:           # isolated singleton
                wp=loss(w,s); ans=answers(wp,n)
                if not ans: rep_ok=False
                if a=='1':
                    st['iso1']+=1
                    for t,c in ans:   # step 5
                        st['iso1_answers']+=1
                        mm=len(wp)
                        if not (t=='dbl' and wp[c-1]+wp[c]+wp[(c+1)%mm]=='101'): st['step5_reply_bad']+=1
                        w2=dbl(wp,c) if t=='dbl' else (loss(wp,c) if t=='loss' else wp)
                        lg=[ll for aa,ll,ss in runs(w2) if aa=='0' and ll>=n-1]
                        if lg!=[n+2]: st['step5_run_bad']+=1
                    if p>=3 and q>=3:
                        st['iso1_pq>=3']+=1
                        if ans: st['STEP7_BAD(answerable)']+=1
                    if ans and sorted((p,q))!=[2,n]: st['answerable_not_{n,2}']+=1
                    if ans: st['iso1_answerable']+=1
            if a=='1' and p==1 and q==1 and Rr[i-2][1]>=1:   # middle of three consecutive singletons: step 6
                wp=loss(w,s); mm=len(wp); st['mid']+=1
                for t,c in answers(wp,n):
                    st['mid_answers']+=1
                    z=''.join(wp[(c+j)%mm] for j in range(-2,3))
                    if not (t=='loss' and wp[c]=='0' and (z[1:5]=='1001' or z[0:4]=='1001')): st['step6_reply_bad']+=1
        if n<=5:
            full=repairable(w,n); st['repairable']+=full
    print(" n=%d %s"%(n,dict(st)))
print(" (n=4,5: D+pair loops can be repairable, so 'answerable' is reachable by this checker; n>=7: none with p,q>=3 may be)")

# ---------------- Theorem 2 ----------------
def g3data(w,s,k):
    m=len(w); v=w[s+1:]+w[:s+1]; Wv=wins(v,k); v1=v[:-1]; W1=wins(v1,k)
    through=[Wv[(m-1-j)%m] for j in range(k)]
    junction=[W1[(m-2-j)%(m-1)] for j in range(k-1)]
    F=set(through); J=sum(1 for z in junction if z in F)
    return F,J
def g4bound(L,distinct,n,k,variant=None):
    m=n-k; tot=0
    for d in range(2,k):
        ex=(min(m,d) if variant=='min' else max(m,d))-1-(1 if variant=='half' else 0)
        tot+=(k-d)*Fraction(2)**ex
    rep=0 if variant=='norepeat' else 2*(k-1)*(L-distinct)
    return 2*tot+rep
print("== G1,G2,G3 on every repairable loop (all singletons, all k=2..n), universal and non-universal separately")
for variant in (None,'rhs2','nojunction','k-2'):
    st=Counter()
    for n in R:
        for w in R[n]:
            L=len(w); e=L-2**n; uni=universal(w,n); tag='U' if uni else 'N'
            for k in range(2,n+1):
                b=2**(n-k); Nk=N(w,k); x=lambda f:Nk[f]-b
                allw=[''.join(t) for t in itertools.product('01',repeat=k)]
                st['G12 cases '+tag]+=1
                if sum(x(f) for f in allw)!=e: st['G1bad']+=1
                neg=sum(max(0,-x(f)) for f in allw)
                if neg>(k-2 if variant=='k-2' else k-1): st['G2bad '+tag]+=1
                if sum(x(f) for f in allw if x(f)>=1)>e+k-1: st['G2bbad']+=1
                for s in singles(w):
                    F,J=g3data(w,s,k); st['G3 cases '+tag]+=1
                    lhs=sum(x(f) for f in F)
                    if variant=='rhs2': bad = lhs < 2-J
                    elif variant=='nojunction': bad = not any(x(f)>=1 for f in F)
                    else: bad = (lhs < 1-J) or (J==0 and not any(x(f)>=1 for f in F))
                    if bad: st['G3bad '+tag]+=1
    print({None:'as stated: ','rhs2':'FALSE VARIANT G3 (right side 2 - #): ','nojunction':'FALSE VARIANT G3 (every singleton has a P-window): ','k-2':'FALSE VARIANT G2 (k-2): '}[variant]+str(dict(sorted(st.items()))))
st=Counter()
for n in (3,4,5,6,7):
    for rep in range(30):
        w=euler_universal(n,random_extra(n,rng.randrange(0,3),rng),rng); b=None
        for k in range(2,n+1):
            Nk=N(w,k)
            for s in singles(w):
                F,J=g3data(w,s,k); st['cases']+=1
                if sum(Nk[f]-2**(n-k) for f in F)<1-J: st['viol']+=1
print("G3 inequality on NON-repairable loops (random universal, e<=2) -- must be violated somewhere:",dict(st))

print("== G4: bad singletons <= bound, ANY loop of length > 2n+2")
def nbad(w,k):
    return sum(1 for s in singles(w) if g3data(w,s,k)[1]>0)
def necklaces(L):
    for t in itertools.product('01',repeat=L):
        v=''.join(t)
        if all(v<=v[i:]+v[:i] for i in range(1,L)): yield v
def g4run(loops_n,label):
    for variant in (None,'half','norepeat','min'):
        st=Counter()
        for w,n in loops_n:
            L=len(w); dist=len(set(wins(w,n)))
            for k in range(2,n+1):
                nb=nbad(w,k); bd=g4bound(L,dist,n,k,variant); st['cases']+=1
                if nb>bd: st['viol']+=1
                if nb==bd and nb>0: st['attained(>0)']+=1
        print("  %s %s %s"%(label,{None:'as stated','half':'FALSE VARIANT 2^(max-2)','norepeat':'FALSE VARIANT no repeat term','min':'FALSE VARIANT min for max'}[variant],dict(st)))
ex=[(w,n) for n,Ls in ((3,range(9,16)),(4,range(11,17)),(5,range(13,17)),(6,range(15,17))) for L in Ls for w in necklaces(L)]
g4run(ex,"exhaustive necklaces n=3 L=9..15, n=4 L=11..16, n=5 L=13..16, n=6 L=15..16 (%d loops):"%len(ex))
rl=[]
for n in range(5,11):
    for rep in range(12):
        L=rng.randrange(2*n+3,160); rl.append((''.join(rng.choice('01') for _ in range(L)),n))
        pat=''.join(rng.choice('01') for _ in range(rng.randrange(2,10)))
        w=(pat*40)[:rng.randrange(2*n+3,120)]; w=list(w)
        for _ in range(rng.randrange(0,4)): w[rng.randrange(len(w))]=rng.choice('01')
        rl.append((''.join(w),n))
        # concatenation of periodic chunks (many squares)
        w=''
        while len(w)<2*n+3+rng.randrange(0,150):
            pat=''.join(rng.choice('01') for _ in range(rng.randrange(1,8))); w+=pat*rng.randrange(1,6)
        rl.append((w,n))
    for rep in range(3):
        u=euler_universal(n,random_extra(n,rng.randrange(0,4),rng),rng); rl.append((u,n))
        c=rng.randrange(len(u)); rl.append((loss(u,c),n)); rl.append((dbl(u,c),n))
rl+=[(w,n) for n in R for w in R[n][:150]]
g4run(rl,"random / periodic / chunked / (edited) universal n=5..10 / repairable (%d loops):"%len(rl))
sm=[(w,n) for n in (3,4,5) for L in range(max(4,n+2),2*n+3) for w in necklaces(L)]
st=Counter()
for w,n in sm:
    for k in range(2,n+1):
        st['cases']+=1
        if nbad(w,k)>g4bound(len(w),len(set(wins(w,n))),n,k): st['viol']+=1
print("  at lengths n+2..2n+2 (outside the hypothesis, information):",dict(st))

print("== G4 inner count: #factors on [p,p+k+d-1] with word(J_p)=word(W_u) and w[-1]!=w[0]  (claimed <= 2^(d-1)), all k<=8")
mx=Counter(); viol=0; eqbad=0
for k in range(2,9):
    for d in range(-(k-2),k):
        for p in range(-(k-1),0):
            u=p+d
            if not (-(k-1)<=u<=0): continue
            lo=min(p,u); hi=max(p+k,u+k-1); Lf=hi-lo+1; cnt=0
            for bits in itertools.product((0,1),repeat=Lf):
                g=lambda c: bits[c-lo]
                J=[g(c) for c in range(p,p+k+1) if c!=0]; Wd=[g(c) for c in range(u,u+k)]
                eq=(J==Wd)
                eqs=all(g(c+d)==g(c) for c in range(p,0)) and all(g(c+d-1)==g(c) for c in range(1,p+k+1))
                if eq!=eqs: eqbad+=1
                if eq and g(-1)!=g(0): cnt+=1
            if d in (0,1):
                if cnt: viol+=1
            elif d>=2:
                assert Lf==k+d
                if cnt>2**(d-1): viol+=1
                mx[cnt==2**(d-1)]+=1
                if cnt>2**(d-2): mx['exceeds 2^(d-2) (false variant fails)']+=1
print("  violations:",viol,"| equation/word-equality mismatches:",eqbad,"| (p,d>=2) pairs with count exactly 2^(d-1):",dict(mx))

print("== proof of (**), term by term, on every repairable loop")
st=Counter()
for n in R:
    for w in R[n]:
        L=len(w); e=L-2**n; dist=len(set(wins(w,n))); sg=singles(w)
        st['loops']+=1
        if len(sg)<2**(n-2)-2: st['S_bad']+=1
        if len(sg)<2**(n-2)-1: st['S below 2^(n-2)-1 (false variant "-1")']+=1
        if 2**n-dist>n-1: st['missing_bad']+=1
        for k in range(2,n+1):
            m=n-k; b=2**m; Nk=N(w,k); st['k-cases']+=1
            P={f for f in Nk if Nk[f]-b>=1}
            pw=sum(Nk[f] for f in P)
            if pw>(b+1)*(e+k-1): st['Pwindows_bad']+=1
            nb=0; good_cov=0
            Wv=wins(w,k)
            for s in sg:
                F,J=g3data(w,s,k)
                if J>0: nb+=1
                elif not any(Wv[(s-j)%L] in P for j in range(k)): st['nonbad_uncovered']+=1
            if len(sg)-nb>k*pw: st['count_bad']+=1
            rhs=k*(b+1)*(e+k-1)+g4bound(L,dist,n,k)
            if len(sg)>rhs: st['chain_bad']+=1
            rhs2=k*(b+1)*(e+k-1)+2*sum((k-d)*Fraction(2)**(max(m,d)-1) for d in range(2,k))+2*(k-1)*(e+n-1)
            if 2**(n-2)-2>rhs2: st['(**)_bad']+=1
print(" ",dict(st))

print("== arithmetic of (**)")
def rhs(n,k,e):
    m=n-k
    return k*(2**m+1)*(e+k-1)+2*sum((k-d)*2**(max(m,d)-1) for d in range(2,k))+2*(k-1)*(e+n-1)
def emin(n,k):
    # least integer e>=0 with (**)
    lo=2**(n-2)-2; a=rhs(n,k,0); slope=rhs(n,k,1)-a
    if a>=lo: return 0
    return -((a-lo)//slope)   # ceil((lo-a)/slope)
print(" n : best k, least e allowed by (**) | closed bound (2^(n-3)-19n^2)/(19n) | e from k=n-4")
first=None
for n in range(4,41):
    best=max((emin(n,k),k) for k in range(2,n+1))
    cb=Fraction(2**(n-3)-19*n*n,19*n)
    e4=emin(n,n-4) if n>=6 else None
    if best[0]>0 and first is None: first=n
    if n>=6:
        k=n-4
        # exact rearrangement:  e(19k-2) >= 2^(n-3) - 19n^2 + 117n - 64  claimed to follow from (**) with 2*sum <= 48(k-2)+2^(n-3)
        assert 2*sum((k-d)*2**(max(4,d)-1) for d in range(2,k)) <= 48*(k-2)+2**(n-3), (n,k)
        if k>=5: assert 2*sum((k-d)*2**(max(4,d)-1) for d in range(2,k)) == 2**(n-3)+16*k-48
        # for every e violating the rearranged inequality, (**) with the relaxed sum is violated too, and conversely
        num=2**(n-3)-19*n*n+117*n-64
        for e in range(0,60):
            relaxed = 2**(n-2)-2 <= k*17*(e+k-1)+48*(k-2)+2**(n-3)+2*(k-1)*(e+n-1)
            assert relaxed == (e*(19*k-2)>=num), (n,e)
        assert e4 >= cb, (n,e4,cb)            # closed bound is implied
    if n in (6,7,13,14,15,16,17,20,25,30,40): print(" %2d : k=%2d e>=%d | %s | %s"%(n,best[1],best[0],float(cb),e4))
print(" first n with a non-zero bound:",first)
for n in range(6,400):
    k=n-4; num=2**(n-3)-19*n*n+117*n-64
    assert Fraction(num,19*k-2) >= Fraction(2**(n-3)-19*n*n,19*n) or 2**(n-3)-19*n*n<0
print(" closed bound <= rearranged bound whenever the closed bound is >= 0: checked n=6..399; closed bound first positive at n =",min(n for n in range(4,60) if 2**(n-3)>19*n*n))
