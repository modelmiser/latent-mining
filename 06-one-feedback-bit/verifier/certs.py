# Referee check of both solvers' dual certificates, exact arithmetic, own definition code.
import glob,re,sys,os,itertools,math
from fractions import Fraction
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from chk import D,O
B=''
claim={}
for ln in "2:2 2 2|3:2 4 2 2|4:4 4 4 4 4|5:6 8 8 8 6 6|6:10 12 13 12 12 10 10|7:16 20 21 22 20 20 16 16|8:30 32 37 38 38 35 34 30 30".split('|'):
    n,v=ln.split(':')
    for t,x in enumerate(v.split()): claim[(int(n),t)]=int(x)
W=lambda k:[''.join(p) for p in itertools.product('01',repeat=k)]
def loadc(f,scale):
    y={}
    for ln in open(f):
        ln=ln.split('#')[0].split()
        if not ln: continue
        if ln[0] in y: return None
        y[ln[0]]=Fraction(ln[1])/scale
    return y
def cert_ok(n,t,y,bound):
    if y is None or any(len(w)!=n-1 or v<0 or set(w)-set('01') for w,v in y.items()): return False,'malformed'
    for u in W(t):
        for a in W(n-t):
            for b in W(n-t):
                S={w for w in O(u,a,b) if len(w)==n-1}
                if sum(y.get(w,0) for w in S)<1: return False,'strategy %s under-weighted'%((u,a,b),)
    tot=sum(y.values())
    if tot>=bound+1: return False,'total %s too large'%float(tot)
    return True,float(tot)
if __name__=="__main__":
    bad=0;c=0;seen=set()
    for f,sc in [(x,1) for x in sorted(glob.glob(B+'solver-a/cert_*.txt'))]+[(x,10**6) for x in sorted(glob.glob(B+'solver-b/cert_*.txt'))]:
        m=re.search(r'(\d+)_t?(\d+)\.txt$',f); n,t=int(m.group(1)),int(m.group(2)); c+=1; seen.add((n,t))
        ok,msg=cert_ok(n,t,loadc(f,sc),claim[(n,t)])
        if not ok: bad+=1; print('PROBLEM',f,msg)
    print('certificates checked',c,'problems',bad)
    print('entries n<=8 with no certificate from either solver:',sorted(k for k in claim if k not in seen and k[0]>=3))
    y=loadc(B+'solver-a/cert_5_0.txt',1)
    print('A(5)<=6 cert vs bound 5 (must fail):',cert_ok(5,0,y,5))
    z=dict(y); z['0000']=Fraction(1,2); print('weight lowered (must fail):',cert_ok(5,0,z,6))
    z=dict(y); z['0101']=Fraction(-1,3); print('negative weight (must fail):',cert_ok(5,0,z,6))
    print('cert for (5,0) applied to (5,2) (must fail):',cert_ok(5,2,y,6))
    for f,sc,n,t in [('solver-a/bad_cert_6_1.txt',1,6,1),('solver-b/BAD_cert_n6_t1.txt',10**6,6,1)]:
        print(f,cert_ok(n,t,loadc(B+f,sc),12))
