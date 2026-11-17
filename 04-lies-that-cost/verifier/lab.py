#!/usr/bin/env python3
"""Referee: labelled-state fixed point (no multiset symmetry, all 2^n subsets as questions).
State = tuple over x of remaining obligation, -1 = dead.  Q = INF iff root outside the least fixed point."""
import sys
k=int(sys.argv[1]); n=int(sys.argv[2])
root=(0,)*n
def step(st,S,ans):
    alive=[x for x in range(n) if st[x]>=0]
    ins=sum(1 for x in alive if S>>x&1); ell=min(ins,len(alive)-ins)
    new=[]
    for x in range(n):
        r=st[x]
        if r<0: new.append(-1)
        elif bool(S>>x&1)==ans: new.append(max(r-1,0))
        elif r==0: new.append(k*ell)
        else: new.append(-1)
    return tuple(new)
ids={root:0}; states=[root]; trans=[]
i=0
while i<len(states):
    st=states[i]; i+=1
    al=sum(1 for r in st if r>=0)
    if al==1: trans.append(None); continue
    tl=[]
    for S in range(1<<n):
        ch=[]
        for ans in (True,False):
            d=step(st,S,ans)
            if all(r<0 for r in d): continue
            if d not in ids: ids[d]=len(states); states.append(d)
            ch.append(ids[d])
        tl.append(ch)
    trans.append(tl)
val=[0 if t is None else -1 for t in trans]
rnd=0
while True:
    rnd+=1
    add=[i for i,t in enumerate(trans) if val[i]<0 and any(all(val[c]>=0 for c in ch) for ch in t)]
    if not add: break
    for i in add: val[i]=rnd
print("lab k=%d n=%d labelled_states=%d Q=%s"%(k,n,len(states),"INF" if val[0]<0 else val[0]),flush=True)
