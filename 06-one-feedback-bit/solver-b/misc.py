# belief 4: min over b of |uD(a) ∪ D(u)b| == r(u)+r(a)-1 for 1<=t<=n-1 ; and attained by b=u_t a'
from common import *
def r(s): return sum(1 for i in range(len(s)) if i==0 or s[i]!=s[i-1])
for n in range(2,9):
    ok=True
    for t in range(1,n):
        for u in strs(t):
            for a in strs(n-t):
                m=min(len({u+x for x in D(a)}|{x+b for x in D(u)}) for b in strs(n-t))
                b=u[-1]+a[1:]
                m2=len({u+x for x in D(a)}|{x+b for x in D(u)})
                if not (m==m2==r(u)+r(a)-1): ok=False
    print("n",n,"min|S| = r(u)+r(a)-1 for all interior t:",ok,flush=True)
