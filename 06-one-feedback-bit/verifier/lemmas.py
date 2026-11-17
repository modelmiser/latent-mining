# mechanical tests of the solvers' lemmas
import itertools,math,sys,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from chk import D
W=lambda k:[''.join(p) for p in itertools.product('01',repeat=k)]
r=lambda s: sum(1 for i in range(len(s)) if i==0 or s[i]!=s[i-1])
bad=0;cases=0
for n in range(2,10):
    for t in range(1,n):
        for u in W(t):
            for a in W(n-t):
                Da=D(a); best=None; arg=set()
                for b in W(n-t):
                    S={u+x for x in Da}|{x+b for x in D(u)}; cases+=1
                    attained = len(S)==r(u)+r(a)-1
                    if len(S)<r(u)+r(a)-1: bad+=1
                    if attained != (b in {u[-1]+x for x in Da}): bad+=1   # solver-a's "exactly when"
                    # P2/P5: t=n-1 => S = D(ub)
                    if t==n-1 and S!=D(u+b): bad+=1
                    # solver-b P6 inclusion: c.S(u',a,b) subset S
                    if t>=2:
                        S2={u[0]+u[1:]+x for x in Da}|{u[0]+x+b for x in D(u[1:])}
                        if not S2<=S: bad+=1
                    # P3: b=a => S = D(ua)
                    if b==a and S!=D(u+a): bad+=1
print('P5/P7 min |S| = r(u)+r(a)-1 iff b in u_t D(a); P2; P3; P6-inclusion: cases',cases,'violations',bad)
# solver-a P6: claimed "#words with r(w)-1 < m is at most 2^n n^-2 (Hoeffding)", m = n/2 - sqrt(n ln n)
worst=0;hw=0;hn=None
for n in range(3,3001):
    m=n/2-math.sqrt(n*math.log(n)); N=n-1
    tail=sum(math.comb(N,k) for k in range(0,N+1) if k<m)/2**N if m>0 else 0.0
    worst=max(worst,tail*n*n)
    s=(N/2)-m; h=math.exp(-2*s*s/N)*n*n
    if h>hw: hw=h;hn=n
print('exact tail * n^2, max over 3<=n<=3000: %.4g ; Hoeffding bound * n^2 max: %.4f at n=%d'%(worst,hw,hn))
# table relations
T={2:[2,2,2],3:[2,4,2,2],4:[4,4,4,4,4],5:[6,8,8,8,6,6],6:[10,12,13,12,12,10,10],7:[16,20,21,22,20,20,16,16],8:[30,32,37,38,38,35,34,30,30]}
v=0
for n in range(3,9):
    for t in range(1,n+1):
        if T[n][t]>2*T[n-1][t-1]: v+=1
    if T[n][1]!=2*T[n-1][0] or T[n][n-1]!=T[n][0] or min(T[n])<T[n][0]: v+=1
print('table relations (P6-b, F(n,1)=2A(n-1), F(n,n-1)=A(n), F>=A(n)) violations:',v)
