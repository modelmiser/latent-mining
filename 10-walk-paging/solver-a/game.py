#!/usr/bin/env python3
"""W(k,m) solver.  mode 'slow': definition, no pruning.  mode 'fast': domination pruning.
usage: game.py k m mode"""
import sys
from itertools import combinations
def popc(x): return bin(x).count('1')
def subsets_containing(base, pbit, k):
    # all C' subset of base, containing pbit, |C'|<=k
    others=[1<<i for i in range(base.bit_length()) if (base>>i)&1 and (1<<i)!=pbit]
    out=[]
    for r in range(0,min(k-1,len(others))+1):
        for comb in combinations(others,r):
            out.append(pbit|sum(comb))
    return out
def maximal_containing(base, pbit, k):
    n=popc(base)
    if n<=k: return [base]
    others=[1<<i for i in range(base.bit_length()) if (base>>i)&1 and (1<<i)!=pbit]
    return [pbit|sum(comb) for comb in combinations(others,k-1)]
def fault_options(C, pbit, k, slow):
    res=set()
    for qbit in (pbit>>1, pbit<<1):
        base=C|pbit|qbit
        res.update(subsets_containing(base,pbit,k) if slow else maximal_containing(base,pbit,k))
    return res
def online_options(C,pbit,k,slow):
    if slow: return fault_options(C,pbit,k,True)
    l,r=pbit>>1,pbit<<1
    if (C&l) and not (C&r): qs=[r]
    elif (C&r) and not (C&l): qs=[l]
    elif (C&l) and (C&r): qs=[l]
    else: qs=[l,r]
    res=set()
    for q in qs: res.update(maximal_containing(C|pbit|q,pbit,k))
    return res
def prune(d):
    items=sorted(d.items(), key=lambda t:(t[1],-popc(t[0])))
    keep=[]
    for C,c in items:
        dom=False
        for C2,c2 in keep:
            if c2+popc(C&~C2)<=c: dom=True;break
        if not dom: keep.append((C,c))
    return keep
def wf_step(wf,pbit,k,m,slow):
    d={}
    for C,c in wf:
        if C&pbit:
            if d.get(C,99)>c: d[C]=c
        elif c+1<=m:
            for C2 in fault_options(C,pbit,k,slow):
                if d.get(C2,99)>c+1: d[C2]=c+1
    if not d: return None
    if slow: return tuple(sorted(d.items()))
    return tuple(sorted(prune(d)))
def solve(k,m,slow,cap=None,verbose=False):
    if cap is None: cap=2*m+3
    OFF=2*m+3
    p0=1<<OFF
    wf0=wf_step(((0,0),),p0,k,m,slow)
    starts=[(p0,wf0,oc) for oc in online_options(0,p0,k,slow)]
    succ={}  # state -> list of (cost, [succ states])
    stack=list(starts); seen=set(starts)
    wfcache={}
    while stack:
        S=stack.pop()
        pb,wf,oc=S
        mv=[]
        for p in (pb>>1,pb<<1):
            key=(wf,p)
            if key not in wfcache: wfcache[key]=wf_step(wf,p,k,m,slow)
            w2=wfcache[key]
            if w2 is None: continue
            if oc&p: ss=[(p,w2,oc)]; cost=0
            else: ss=[(p,w2,o2) for o2 in online_options(oc,p,k,slow)]; cost=1
            mv.append((cost,ss))
            for s in ss:
                if s not in seen: seen.add(s); stack.append(s)
        succ[S]=mv
    V={S:0 for S in succ}
    changed=True; it=0
    while changed:
        changed=False; it+=1
        for S,mv in succ.items():
            v=0
            for cost,ss in mv:
                x=cost+min(V[s] for s in ss)
                if x>v: v=x
            if v>cap: v=cap
            if v>V[S]: V[S]=v; changed=True
    F=1+min(V[s] for s in starts)
    if verbose: print("states",len(succ),"iters",it,file=sys.stderr)
    return F,len(succ),V,succ,starts
if __name__=="__main__":
    k=int(sys.argv[1]); m=int(sys.argv[2]); slow=sys.argv[3]=='slow'
    F,n,_,_,_=solve(k,m,slow)
    print(k,m,sys.argv[3],"F=",F,"states",n, "(cap %d)"%(2*m+4), flush=True)
