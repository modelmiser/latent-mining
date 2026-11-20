#!/usr/bin/env python3
"""Exploration only (not a result): print one optimal line with features, and how often simple
Delayer rules pick an optimal agent over all positions reachable under optimal play."""
import sys
from functools import lru_cache
n=int(sys.argv[1]); full=(1<<n)-1
def legal(K,a,b): return a!=b and not ((K[b]>>a)&1 and (K[a]>>b)&1)
def call(K,a,b):
    L=list(K);u=K[a]|K[b];L[a]=u;L[b]=u;return tuple(L)
def canon(K):
    # cheap normalisation (sort by invariants); any relabelling is safe
    inv=[(bin(K[i]).count('1'),sum((K[j]>>i)&1 for j in range(n)),i) for i in range(n)]
    order=[i for _,_,i in sorted(inv)]
    perm=[0]*n
    for new,old in enumerate(order): perm[old]=new
    return tuple(sum(1<<perm[j] for j in range(n) if (K[old]>>j)&1) for old in order)
@lru_cache(maxsize=None)
def V(K):
    if all(k==full for k in K): return 0
    return max(W(K,a) for a in range(n) if any(legal(K,a,b) for b in range(n)))
def W(K,a):
    return min(1+V(canon(call(K,a,b))) for b in range(n) if legal(K,a,b))
def feat(K,a):
    return "|K|=%d knownby=%d partners=%d"%(bin(K[a]).count('1'),sum((K[j]>>a)&1 for j in range(n)),sum(legal(K,a,b) for b in range(n)))
K=canon(tuple(1<<i for i in range(n)))
print("A=",V(K))
step=0
while V(K):
    v=V(K)
    acts=[a for a in range(n) if any(legal(K,a,b) for b in range(n))]
    opt=[a for a in acts if W(K,a)==v]
    step+=1
    print("step",step,"rem",v,"state",[format(k,'0%db'%n) for k in K])
    for a in acts: print("    ",a,feat(K,a),"W=",W(K,a),"*" if a in opt else "")
    a=opt[0]
    bs=[b for b in range(n) if legal(K,a,b)]
    b=min(bs,key=lambda b:(1+V(canon(call(K,a,b))),-bin(K[a]|K[b]).count('1')))
    print("   Delayer",a,"Hastener",b,[ (b2,1+V(canon(call(K,a,b2)))) for b2 in bs])
    K=canon(call(K,a,b))
