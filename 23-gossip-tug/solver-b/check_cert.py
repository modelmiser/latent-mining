#!/usr/bin/env python3
# Independent checker for Delayer lower-bound certificates (see header line of the certificate).
# Usage: check_cert.py n T file    -> verifies that entry 0 is the initial position and claims >= T.
import sys
n=int(sys.argv[1]); T=int(sys.argv[2]); FULL=(1<<n)-1
D="0123456789ab"
E=[]
for line in open(sys.argv[3]):
    if line.startswith('#') or not line.strip(): continue
    p=line.split(); t=int(p[0]); a=D.index(p[1]); K=[int(x,16) for x in p[2:2+n]]; kids=p[2+n:]
    E.append((t,a,K,kids))
def fail(i,msg): print(f"FAIL entry {i}: {msg}"); sys.exit(1)
def legal(K,a,b): return a!=b and not ((K[a]>>b)&1 and (K[b]>>a)&1)
for i,(t,a,K,kids) in enumerate(E):
    if len(K)!=n or any(not (K[x]>>x)&1 or K[x]>FULL for x in range(n)): fail(i,"bad state")
    if t<1: fail(i,"t<1")
    bs=[b for b in range(n) if legal(K,a,b)]
    if not bs: fail(i,"named agent not active")
    if len(kids)!=len(bs): fail(i,"wrong number of replies")
    for b,kid in zip(bs,kids):
        f=kid.split(':')
        if D.index(f[0])!=b: fail(i,"reply label mismatch")
        C=list(K); C[a]=C[b]=K[a]|K[b]
        if f[1]=='L':
            nonfull=sum(1 for x in C if x!=FULL)
            if not (t-1<=0 or (nonfull+1)//2>=t-1): fail(i,"leaf bound not met")
        else:
            j=int(f[1]); perm=[D.index(c) for c in f[2]]
            if sorted(perm)!=list(range(n)): fail(i,"not a permutation")
            if not (0<=j<len(E)): fail(i,"bad index")
            R=[0]*n
            for x in range(n):
                m=0
                for y in range(n):
                    if (C[x]>>y)&1: m|=1<<perm[y]
                R[perm[x]]=m
            if R!=E[j][2]: fail(i,"child does not match referenced entry")
            if E[j][0]<t-1: fail(i,"referenced entry claims too little")
t,a,K,kids=E[0]
if K!=[1<<x for x in range(n)]: fail(0,"entry 0 is not the initial position")
if t<T: fail(0,"root claims less than T")
print(f"OK: n={n}: Delayer forces >= {t} calls; {len(E)} entries checked")
