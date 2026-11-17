import sys,os,collections,glob
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
import ref
from ref import *
BASE='.'
# 1. broken witnesses
n,k,R=parse(BASE+'/solver-b/surv_n10_k4.txt')
print('good:',check(n,k,R,extra=20)[:2])
b1=[list(r) for r in R]; b1[1]=b1[1][:3]+[b1[0][0]]; print('repeat link    :',check(n,k,b1)[:2])
b2=[list(r) for r in R]; b2[2]=b2[2]+[(5,6)]; print('over budget    :',check(n,k,b2)[:2])
b3=[list(r) for r in R]; b3[0][0]=(0,2); print('non-link       :',check(n,k,b3)[:2])
b4=[list(r) for r in R]; b4[3]=b4[3][:3]; print('one hit removed:',check(n,k,b4,extra=20)[:2],'(must complete before 7)')
b5=[list(r) for r in R]+[[(1,0),(2,1),(3,2),(4,3)]]; print('extended round :',check(n,k,b5,extra=20)[:2],'(must not reach 8)')
print('k understated  :',check(n,3,R)[:2])
# 2. A's Theorem U vs every computed value (table from the referee's own runs + solvers' large-k values)
F={}
for n in range(3,13):
    for k in (1,2): F[(n,k)]=n//2
vals={3:[2,2],4:[2,3],5:[3,3],6:[4,4],7:[4,5],8:[5,6],9:[6,6],10:[6,7],11:[7,8],12:[8,9]}
for n,v in vals.items(): F[(n,3)],F[(n,4)]=v
big={3:[2,2],4:[3,3,3,4],5:[4]*6,6:[4,5,5,5,5,5,5,6],7:[5,5,6,6,6,6,6,6,6,6],8:[6,6,6,7,7,7,7,7,7,7,7,8]}
for n,v in big.items():
    for j,x in enumerate(v): F[(n,5+j)]=x
badA=badB=0; openB=[]; openA=[]
for (n,k),f in sorted(F.items()):
    for t in range(1,f):
        if deadA(n,k,t): badA+=1; print('A-U REFUTED: claims dead at survivable t',n,k,t)
        if k<n and boundB(n,k,t): badB+=1; print('B-U REFUTED',n,k,t)
    if n>k>=3:
        if not deadA(n,k,f): openA.append((n,k))
        if not boundB(n,k,f): openB.append((n,k))
print('A dead at survivable t:',badA,' B:',badB); print('entries (n>k>=3, n<=12) where A-U fails to exclude t=F:',openA); print('same for B-U:',openB)
# all k>=n entries: is A's inequality (as coded, without the n>k step) consistent? report which it decides
print('k>=n entries where deadA(n,k,F) is True:',[(n,k) for (n,k),f in sorted(F.items()) if k>=n and deadA(n,k,f)])
# 3. corollary sweep, own evaluation
bad=0;cnt=0
for k in range(3,41):
    for n in range(k+1,601):
        t0=n-cdiv(n,k); cnt+=1
        if deadA(n,k,t0) is not True or (t0>=2 and deadA(n,k,t0-1)): bad+=1
print('corollary sweep pairs',cnt,'bad',bad)
# B bracket: how often does B-U leave a gap
gap=0
for k in range(3,13):
    for n in range(k+1,61):
        if not boundB(n,k,n-cdiv(n,k)): gap+=1
print('B-U open pairs, 3<=k<=12,k<n<=60:',gap)
# 4. mutation tests of the lemma tester (each must FAIL)
plays=[]
for path in glob.glob(BASE+'/solver-b/surv_n*_k[34].txt')+glob.glob(BASE+'/solver-a/witgen/g_1*_[345].txt'):
    plays.append(parse(path))
def count_fail():
    c=collections.Counter(); st=collections.Counter()
    for n,k,R in plays:
        try: lemma_tests(n,k,R,st)
        except Fail as e: c[str(e)[:40]]+=1
    return dict(c)
print('baseline fails:',count_fail())
orig=ref.deadA
ref.deadA=lambda n,k,t: orig(n,k-1,t); print('mutant deadA with k-1:',count_fail()); ref.deadA=orig
ref.deadA=lambda n,k,t: orig(n,k,t+1) if (t+1)%2==t%2 else orig(n-1,k,t); print('mutant deadA with n-1:',count_fail()); ref.deadA=orig
origB=ref.boundB
ref.boundB=lambda n,k,T: origB(n,k-1,T); print('mutant boundB with k-1:',count_fail()); ref.boundB=origB
src=open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'ref.py')).read()
for a,b,nm in (("req(t>=2*y-1,","req(t>=2*y,",'creation round 2y'),("req(dl<=oth[xst]+h[c]-Dp,","req(dl<=oth[xst]+h[c]-Dp-1,",'(1) tightened by 1'),("req(B<=E,","req(B<E or E==0,",'B<E'),("if k<n: req(phi<=sum(hits[:t+1])","if k<n: req(phi<sum(hits[:t+1])",'Phi strict'),("req(dl<=h[c]-s+beta and dl<=w+eps+beta","req(dl<=h[c]-s and dl<=w+eps",'odd bounds without beta')):
    assert a in src
    g={}; exec(compile(src.replace(a,b),'mut','exec'),g)
    c=collections.Counter(); st=collections.Counter()
    for n,k,R in plays:
        try: g['lemma_tests'](n,k,R,st)
        except g['Fail'] as e: c[str(e)[:40]]+=1
    print('mutant',nm,':',dict(c))
