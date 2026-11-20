#!/usr/bin/env python3
"""Independent checker for strategy certificates of the Delayer/Hastener gossip game.
usage: check.py n D|H file claim
 D: verifies that Delayer's listed strategy forces >= claim calls against EVERY Hastener reply.
 H: verifies that Hastener's listed strategy ends within <= claim calls against EVERY Delayer choice.
Positions are matched up to relabelling of agents; this checker uses its own canonical form
(iterated colour refinement, then minimum over permutations inside colour classes) and
transports the stored move through the relabelling.  Shares no code with the solvers."""
import sys
from itertools import permutations, product
sys.setrecursionlimit(100000)
n=int(sys.argv[1]); side=sys.argv[2]; fn=sys.argv[3]; claim=int(sys.argv[4])
FULL=(1<<n)-1

def legal(K,a,b):
    return a!=b and not ((K[b]>>a)&1 and (K[a]>>b)&1)
def call(K,a,b):
    L=list(K); u=K[a]|K[b]; L[a]=u; L[b]=u; return tuple(L)

def canon(K):
    """return (code, perm) with perm[old]=new, code = relabelled state tuple; code is a true
    canonical form (colours are isomorphism-invariant, min over all colour-preserving orders)."""
    col=[(bin(K[i]).count('1'), sum((K[j]>>i)&1 for j in range(n))) for i in range(n)]
    for _ in range(3):
        col=[(col[i], tuple(sorted(col[j] for j in range(n) if (K[i]>>j)&1)),
                      tuple(sorted(col[j] for j in range(n) if (K[j]>>i)&1))) for i in range(n)]
        ranks={c:r for r,c in enumerate(sorted(set(col)))}
        col=[ranks[c] for c in col]
    classes={}
    for i,c in enumerate(col): classes.setdefault(c,[]).append(i)
    groups=[classes[c] for c in sorted(classes)]
    best=None
    for combo in product(*[permutations(g) for g in groups]):
        order=[x for g in combo for x in g]      # order[new]=old
        perm=[0]*n
        for new,old in enumerate(order): perm[old]=new
        code=tuple(sum(1<<perm[j] for j in range(n) if (K[old]>>j)&1) for old in order)
        if best is None or code<best[0]: best=(code,perm)
    return best

table={}
for line in open(fn):
    p=line.split()
    if not p: continue
    K=tuple(int(x,16) for x in p[:n])
    code,perm=canon(K)
    if side=='D':
        table[code]=perm[int(p[n])]
    else:
        table[code]={perm[int(x.split(':')[0])]:perm[int(x.split(':')[1])] for x in p[n:]}

memo={}
def val(K):
    """D: minimum number of further calls over all Hastener replies; H: maximum over Delayer choices."""
    if all(k==FULL for k in K): return 0
    code,perm=canon(K)
    if code in memo: return memo[code]
    if code not in table: raise SystemExit("REJECT: reachable position missing from certificate: %s"%(K,))
    K=code
    if side=='D':
        a=table[code]
        bs=[b for b in range(n) if legal(K,a,b)]
        if not bs: raise SystemExit("REJECT: named agent not active")
        r=min(1+val(call(K,a,b)) for b in bs)
    else:
        r=0
        for a in range(n):
            if any(legal(K,a,b) for b in range(n)):
                if a not in table[code]: raise SystemExit("REJECT: no reply listed for active agent")
                b=table[code][a]
                if not legal(K,a,b): raise SystemExit("REJECT: illegal reply")
                r=max(r,1+val(call(K,a,b)))
    memo[code]=r
    return r
v=val(tuple(1<<i for i in range(n)))
ok = v>=claim if side=='D' else v<=claim
print("%s n=%d side=%s strategy guarantees %s %d ; claim %s %d ; positions used %d"%("ACCEPT" if ok else "REJECT",n,side,">=" if side=='D' else "<=",v,">=" if side=='D' else "<=",claim,len(memo)))
sys.exit(0 if ok else 1)
