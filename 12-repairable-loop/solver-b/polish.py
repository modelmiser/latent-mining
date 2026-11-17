#!/usr/bin/env python3
"""polish.py n file : read loops (last whitespace token of each line that is a 01-string), keep the repairable ones,
then repeatedly try to shorten: delete one cell, or delete one cell and flip/insert elsewhere nearby. Heuristic; prints
every repairable loop found that is shorter than the input. Uses its own bitmask checker (cross-checked by explain.py)."""
import sys, itertools
n=int(sys.argv[1]); FULL=(1<<(1<<n))-1
def cov(v):
    m=len(v); x=0; mask=0; W=(1<<n)-1
    for j in range(n-1,0,-1): x=(x<<1)|v[(-j)%m]
    for i in range(m):
        x=((x<<1)|v[i])&W; mask|=1<<x
    return mask
def fix(v):
    c=cov(v)
    if c==FULL: return True
    if bin(FULL&~c).count("1")>n: return False
    m=len(v)
    for k in range(m):
        if k>0 and v[k]==v[k-1]: continue
        if cov(v[:k]+v[k+1:])==FULL or cov(v[:k+1]+v[k:])==FULL: return True
    return False
def rep(w):
    if cov(w)!=FULL:
        if not fix(w): return False
    L=len(w)
    for k in range(L):
        if not fix(w[:k]+w[k+1:]): return False
        if (k==0 or w[k]!=w[k-1]) and not fix(w[:k+1]+w[k:]): return False
    return True
seeds=[]
for line in open(sys.argv[2]):
    t=line.split()
    if t and set(t[-1])<= set("01") and len(t[-1])>8: seeds.append(tuple(int(c) for c in t[-1]))
seeds=[w for w in seeds if rep(w)]
print("repairable seeds:",[len(w) for w in seeds]); sys.stdout.flush()
seen=set(); frontier=seeds; best=min(len(w) for w in seeds) if seeds else 0
while frontier:
    nxt=[]
    for w in frontier:
        L=len(w)
        for k in range(L):
            v=w[:k]+w[k+1:]
            cands=[v]
            for j in range(L-1): cands.append(v[:j]+(1-v[j],)+v[j+1:])
            for c in cands:
                if c in seen: continue
                seen.add(c)
                if rep(c):
                    print(len(c),"".join(map(str,c))); sys.stdout.flush(); nxt.append(c)
                    break
            if nxt and len(nxt)>=3: break
        if nxt and len(nxt)>=3: break
    frontier=nxt[:3]
