#!/usr/bin/env python3
"""Worst case of a FIXED Online strategy against all legal adversary walks.
usage: strat.py name k m     names: fill_far (the proof strategy), fill_lru, lru, outward
Online state = tuple of pages in recency order (most recent last)."""
import sys
from game import wf_step
def strat(name,order,p,k):
    C=set(order)
    out=[x for x in (p+1,p-1) if x not in C]   # prefers +1 when both absent
    if name=='lru':
        new=list(order)+[p]
    elif name=='outward':
        new=list(order)+([out[0]] if out else [])+[p]
    elif name=='fill_lru':
        if len(C)<=k-2 and out: new=list(order)+[out[0],p]
        else: new=list(order)+[p]
    if name=='fill_far':
        # proof strategy: prefetch outward while |C|<=k-2, else load p only; evict the page farthest from p
        new=sorted(C|{p}|({out[0]} if (len(C)<=k-2 and out) else set()))
        while len(new)>k: new.remove(max(new,key=lambda x:abs(x-p)))
        return tuple(new)
    while len(new)>k: new.pop(0)
    return tuple(new)
def worst(name,k,m):
    OFF=2*m+3; cap=2*m+3
    wf0=wf_step(((0,0),),1<<OFF,k,m,False)
    S0=(0,wf0,strat(name,(),0,k))
    succ={}; stack=[S0]; seen={S0}
    while stack:
        S=stack.pop(); pos,wf,order=S; mv=[]
        for p in (pos-1,pos+1):
            w2=wf_step(wf,1<<(p+OFF),k,m,False)
            if w2 is None: continue
            if p in order:
                o2=tuple([x for x in order if x!=p]+[p]); c=0
            else: o2=strat(name,order,p,k); c=1
            T=(p,w2,o2); mv.append((c,T))
            if T not in seen: seen.add(T); stack.append(T)
        succ[S]=mv
    V={S:0 for S in succ}; ch=True
    while ch:
        ch=False
        for S,mv in succ.items():
            v=min(cap,max([0]+[c+V[T] for c,T in mv]))
            if v>V[S]: V[S]=v; ch=True
    return 1+V[S0],len(succ)
if __name__=="__main__":
    name=sys.argv[1]
    for m in range(1,int(sys.argv[3])+1):
        row=[]
        for k in range(2,int(sys.argv[2])+1):
            f,n=worst(name,k,m)
            target=max(m+1,2*m-k//2+1)
            row.append("%d%s"%(f,"" if f==target else "(!=%d)"%target))
        print(name,"m=%d"%m,"k=2..:"," ".join(row),flush=True)
