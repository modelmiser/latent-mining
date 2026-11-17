#!/usr/bin/env python3
"""Solve W(k,m) exactly by state abstraction + least-fixpoint value iteration.
usage: game.py k m [prune=1] [dump]
State = (offline work function, Online cache, position), all translated so the
last request sits at bit OFF.  prune=0: no dominance pruning anywhere (all sub-caches
kept for offline, all legal moves for Online).  prune=1: offline entries (C,f) dropped
when some (C',f') has f'+|C\\C'|<=f; Online restricted to inclusion-maximal C'.
"""
import sys
from itertools import combinations
k=int(sys.argv[1]); m=int(sys.argv[2]); PR=int(sys.argv[3]) if len(sys.argv)>3 else 1
OFF=4*m+1; B=1<<OFF
pc=lambda x: bin(x).count('1')
def subsets_moves(C,q,maximal):
    """all C' with B in C', C' subset of C|B|q, |C'|<=k"""
    U=C|B|q; rest=U&~B
    bits=[1<<i for i in range(rest.bit_length()) if rest>>i&1]
    out=[]
    if maximal:
        ex=max(0,len(bits)+1-k)
        for ev in combinations(bits,ex):
            out.append(U&~sum(ev))
    else:
        for r in range(0,min(k-1,len(bits))+1):
            for keep in combinations(bits,r):
                out.append(B|sum(keep))
    return out
def shift(x,d):  # d=+1: request moved right, so masks shift right by 1 to recenter
    return x>>1 if d==1 else x<<1
def wf_step(wf,d):
    """wf: tuple of (mask,f); request the page at OFF+d; return recentered new wf (or None)"""
    new={}
    for C,f in wf:
        C=shift(C,d)
        if C&B:
            if new.get(C,99)>f: new[C]=f
        elif f<m:
            for q in (B>>1,B<<1):
                for C2 in subsets_moves(C,q,PR):
                    if new.get(C2,99)>f+1: new[C2]=f+1
    if not new: return None
    items=list(new.items())
    if PR:
        keep=[]
        for C,f in items:
            dom=False
            for C2,f2 in items:
                if C2!=C and f2+pc(C&~C2)<=f: dom=True;break
            if not dom: keep.append((C,f))
        items=keep
    return tuple(sorted(items))
def solve():
    wf0=wf_step(((0,0),), 0) if False else None
    # round 1: request page 0 at bit OFF from empty cache
    new={}
    for q in (B>>1,B<<1):
        for C2 in subsets_moves(0,q,PR): new[C2]=1
    items=list(new.items())
    if PR: items=[(C,f) for C,f in items if not any(C2!=C and f2+pc(C&~C2)<=f for C2,f2 in items)]
    wf0=tuple(sorted(items))
    starts=[(wf0,C) for q in (B>>1,B<<1) for C in subsets_moves(0,q,PR)]
    idx={}; opts=[]; stack=[]
    def get(s):
        if s not in idx:
            idx[s]=len(opts); opts.append(None); stack.append(s)
        return idx[s]
    st=[get(s) for s in starts]
    wfcache={}
    while stack:
        s=stack.pop(); wf,C=s; o=[]
        for d in (1,-1):
            key=(wf,d)
            if key not in wfcache: wfcache[key]=wf_step(wf,d)
            w2=wfcache[key]
            if w2 is None: continue
            Cs=shift(C,d)
            if Cs&B: o.append((0,(get((w2,Cs)),)))
            else:
                nx=set()
                for q in (B>>1,B<<1):
                    for C2 in subsets_moves(Cs,q,PR): nx.add(C2)
                o.append((1,tuple(get((w2,c)) for c in nx)))
        opts[idx[s]]=o
    n=len(opts); V=[0]*n; CAP=4*m+5
    changed=True; it=0
    while changed:
        changed=False; it+=1
        for i in range(n-1,-1,-1):
            v=0
            for fl,nx in opts[i]:
                t=fl+min(V[j] for j in nx)
                if t>v: v=t
            if v>V[i]:
                V[i]=v; changed=True
                if v>CAP: return None,n,len(wfcache)
    global G; G=(V,opts,idx,st)
    return 1+min(V[i] for i in st), n, len(wfcache)
if __name__=="__main__":
    F,n,nw=solve()
    print(f"k={k} m={m} prune={PR} F={F} states={n} wfsteps={nw}",flush=True)
