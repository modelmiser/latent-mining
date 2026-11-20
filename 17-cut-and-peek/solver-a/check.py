#!/usr/bin/env python3
# Independent checker, straight from the problem statement.
# Witness file format (plain text, '#' comments):
#   n <n>
#   m <m>
#   owner <string of m digits: 0 = H, i = player i>
#   u0 <string of m chars 0/1: card laid when the owner's bit is 0 (or the h card on H); 1 = heart>
#   u1 <string of m chars 0/1: card laid when the owner's bit is 1 (must equal u0 on H)>
#   R <comma separated positions>
import sys
from collections import Counter
from itertools import product
def load(fn):
    d={}
    for line in open(fn):
        line=line.split('#')[0].strip()
        if line: k,v=line.split(None,1); d[k]=v.strip()
    return d
def check(fn):
    d=load(fn); n=int(d['n']); m=int(d['m']); owner=[int(ch) for ch in d['owner']]
    u0=d['u0']; u1=d['u1']; R=sorted(set(int(t) for t in d['R'].split(',')))
    assert len(owner)==len(u0)==len(u1)==m and n>=2 and m>=1
    assert all(0<=o<=n for o in owner) and all(any(o==i for o in owner) for i in range(1,n+1)), "partition"
    assert all(u0[p]==u1[p] for p in range(m) if owner[p]==0), "h must not depend on inputs"
    assert R and all(0<=q<m for q in R), "R"
    def M(x):
        w=[u0[p] if owner[p]==0 or x[owner[p]-1]==0 else u1[p] for p in range(m)]
        c=Counter()
        for j in range(m):
            after=[None]*m
            for p in range(m): after[(p+j)%m]=w[p]
            c[''.join(after[q] for q in R)]+=1
        return c
    xs=list(product((0,1),repeat=n)); one=tuple([1]*n); zero=tuple([0]*n)
    M0=M(zero)
    priv=all(M(x)==M0 for x in xs if x!=one)
    corr=not (set(M(one)) & set(M0))
    print(f"{fn}: n={n} m={m} |R|={len(R)} privacy={priv} correctness={corr} VALID={priv and corr}")
    return priv and corr
if __name__=="__main__":
    ok=[check(f) for f in sys.argv[1:]]
    sys.exit(0 if all(ok) else 1)
