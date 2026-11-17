#!/usr/bin/env python3
# Independent checker for cert_n*_t*.txt.  usage: certcheck.py n t F file
# Verifies with exact integers: for EVERY strategy (u,a,b), the weights on its length-(n-1)
# outcomes sum to >= K, and total weight < (F+1)*K.  Hence no F+1 pairwise disjoint outcome sets.
import sys
from itertools import product
n,t,F=map(int,sys.argv[1:4]); K=10**6; w={}
for line in open(sys.argv[4]):
    line=line.split('#')[0].split()
    if line: w[line[0]]=int(line[1])
assert all(len(p)==n-1 and v>=0 for p,v in w.items())
bad=0;cnt=0
B=lambda m:[''.join(p) for p in product('01',repeat=m)]
for u in B(t):
    for a in B(n-t):
        for b in B(n-t):
            outs=set()
            for d in range(n):
                sent=u+(b if d<t else a)
                outs.add(sent[:d]+sent[d+1:])
            cnt+=1
            if sum(w.get(o,0) for o in outs)<K: bad+=1
tot=sum(w.values())
ok = bad==0 and tot<(F+1)*K
print(f"n={n} t={t} strategies={cnt} violated={bad} total={tot}/{K} claim F<={F}:", "CERTIFIED" if ok else "REJECTED")
sys.exit(0 if ok else 1)
