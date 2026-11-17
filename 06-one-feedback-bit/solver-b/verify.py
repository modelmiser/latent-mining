#!/usr/bin/env python3
# Independent witness checker (shares no code with common.py / fast.py).
# usage: verify.py n t file    -- file lines "u a b" ('-' = empty string), '#' comments.
# Enumerates outcomes by simulating the channel: for every deletion position (or none),
# the sender sends u, then a or b according to whether the deletion hit the first t bits.
import sys
def outcomes(u,a,b,n,t):
    assert len(u)==t and len(a)==n-t and len(b)==n-t and set(u+a+b)<={'0','1'}
    res=set()
    for d in [None]+list(range(n)):          # adversary's choice
        fb = d is not None and d < t         # feedback bit
        sent = u + (b if fb else a)
        res.add(''.join(c for i,c in enumerate(sent) if i!=d))
    return res
n,t=int(sys.argv[1]),int(sys.argv[2])
fam=[]
for line in open(sys.argv[3]):
    line=line.split('#')[0].split()
    if not line: continue
    fam.append(tuple('' if x=='-' else x for x in line))
used={}
ok=True
for s in fam:
    for o in outcomes(*s,n,t):
        if o in used:
            print("COLLISION",o,used[o],s); ok=False
        used[o]=s
print(f"n={n} t={t} strategies={len(fam)} distinct={len(set(fam))}", "PAIRWISE DISJOINT: OK" if ok and len(set(fam))==len(fam) else "REJECTED")
sys.exit(0 if ok else 1)
