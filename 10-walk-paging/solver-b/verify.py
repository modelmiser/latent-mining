#!/usr/bin/env python3
"""Independent checker (shares no code with game.py) for the two explicit strategies.
  verify.py alg k m [variant]   : Online strategy ALG vs EVERY legal adversary (exhaustive over
                                  reachable (offline work function, window) states); reports max faults.
  verify.py adv k m [bonus]     : Adversary strategy ADV vs EVERY Online move sequence; checks each
                                  request is legal (opt<=m, brute force) and Online faults >= bound+bonus.
opt is computed from the UNPRUNED work function: dict {cache(frozenset): min faults}, all sub-caches kept.
Pages are absolute integers here (no translation).
"""
import sys
from itertools import combinations
sys.setrecursionlimit(100000)
mode=sys.argv[1]; k=int(sys.argv[2]); m=int(sys.argv[3]); extra=int(sys.argv[4]) if len(sys.argv)>4 else 0
h=k//2; BOUND=max(m+1,2*m+1-h)
def moves(C,p):
    """all legal C' after a fault at p from cache C (frozenset)"""
    out=set()
    for q in (p-1,p+1):
        U=sorted((C|{p,q})-{p})
        for r in range(0,min(k-1,len(U))+1):
            for keep in combinations(U,r): out.add(frozenset(keep)|{p})
    return out
def wf_step(wf,p):
    new={}
    for C,f in wf.items():
        if p in C:
            if new.get(C,99)>f: new[C]=f
        elif f<m:
            for C2 in moves(C,p):
                if new.get(C2,99)>f+1: new[C2]=f+1
    return new   # empty <=> opt>m
WF0={frozenset():0}
def key(wf): return frozenset(wf.items())

# ---------------- ALG ----------------
def alg_fault(W,p,variant):
    """W=(lo,hi) inclusive window or None; p adjacent outside. returns new window"""
    if W is None: return (p,p+1)
    lo,hi=W; d=1 if p>hi else -1
    assert p==hi+1 or p==lo-1
    size=hi-lo+1
    if size<=k-2 or variant==1:      # variant 1 (deliberately wrong): always prefetch
        lo,hi=min(lo,p,p+d),max(hi,p,p+d)
    else:
        lo,hi=min(lo,p),max(hi,p)
    while hi-lo+1>k:                 # evict far end
        if d==1: lo+=1
        else: hi-=1
    return (lo,hi)
def check_alg(variant):
    seen=set(); worst=0
    wf=wf_step(WF0,0); W=alg_fault(None,0,variant)
    stack=[(wf,W,0,1)]
    while stack:
        wf,W,p,f=stack.pop()
        # translate-normalise for the visited set
        kk=(frozenset((frozenset(x-p for x in C),c) for C,c in wf.items()),W[0]-p,W[1]-p,f)
        if kk in seen: continue
        seen.add(kk); worst=max(worst,f)
        if f>4*m+4: return worst,len(seen)
        for d in (1,-1):
            q=p+d; w2=wf_step(wf,q)
            if not w2: continue
            if W[0]<=q<=W[1]: stack.append((w2,W,q,f))
            else: stack.append((w2,alg_fault(W,q,variant),q,f+1))
    return worst,len(seen)

# ---------------- ADV ----------------
class Adv:
    __slots__=("s","H","queue","mid","cyc","e","init","stopped")
    def copy(self):
        a=Adv()
        for n in self.__slots__:
            v=getattr(self,n); setattr(a,n,list(v) if isinstance(v,list) else v)
        return a
def path(a,b):
    st=1 if b>a else -1
    return list(range(a+st,b+st,st))
def adv_next(A,C,p):
    """returns next request or None (stop). C = Online cache after serving p."""
    if A.queue: return A.queue.pop(0)
    if not A.init:
        A.init=True
        s=-1 if -1 not in C else 1          # side Online did not prefetch
        end=s*(2*min(h,m)-1)
        A.H=(min(0,end),max(0,end))
        if end!=0:
            A.queue=path(0,end)
            if m<h: A.cyc=0
            return A.queue.pop(0)
    a,b=A.H
    if A.mid:
        e=A.e; end=b if e==1 else a; far=a if e==1 else b
        o1,o2=end+e,end+2*e; full=(b-a+1==k)
        if o2 not in C:
            A.queue=[o2]
            lo,hi=min(a,o2),max(b,o2)
            drop=2 if full else 1
            if e==1: lo+=drop
            else: hi-=drop
        else:
            miss=[x for x in range(a,b+1) if x not in C]
            assert miss
            x=min(miss,key=lambda x:abs(x-o1))
            assert not (full and x==far)
            A.queue=path(o1,x)
            lo,hi=min(a,o1),max(b,o1)
            if full:
                if e==1: lo+=1
                else: hi-=1
        A.H=(lo,hi); A.mid=False; A.cyc+=1
        return A.queue.pop(0)
    if m<h or A.cyc==m-h: return None
    miss=[x for x in range(a,b+1) if x not in C]
    if miss:
        x=min(miss,key=lambda x:abs(x-p))
        A.queue=path(p,x); return A.queue.pop(0)
    e=1 if (b+1) not in C else -1
    assert ((b+1) if e==1 else (a-1)) not in C
    A.e=e; A.mid=True
    end=b if e==1 else a
    A.queue=path(p,end)+[end+e]
    return A.queue.pop(0)
def check_adv(bonus):
    target=BOUND+bonus
    seen=set(); stats=[0,0,10**9]   # nodes, finished plays, min faults at stop
    def rec(A,wf,C,p,f):
        stats[0]+=1
        if f>=target: return True
        kk=(A.s,A.H,tuple(A.queue),A.mid,A.cyc,A.e,A.init,C,p,f,key(wf))
        if kk in seen: return True
        seen.add(kk)
        A=A.copy()
        q=adv_next(A,C,p)
        if q is None:
            stats[1]+=1; stats[2]=min(stats[2],f)
            return f>=target
        assert abs(q-p)==1
        w2=wf_step(wf,q)
        assert w2, "ILLEGAL request: opt>m"
        if q in C: return rec(A,w2,C,q,f)
        for C2 in moves(C,q):
            if not rec(A,w2,C2,q,f+1): return False
        return True
    ok=True
    wf=wf_step(WF0,0)
    for C in moves(frozenset(),0):
        A=Adv(); A.s=0;A.H=None;A.queue=[];A.mid=False;A.cyc=0;A.e=0;A.init=False;A.stopped=False
        if not rec(A,wf,C,0,1): ok=False;break
    return ok,stats
if mode=="alg":
    w,n=check_alg(extra)
    print(f"ALG k={k} m={m} variant={extra} worst_faults={w} bound={BOUND} states={n} {'OK' if w<=BOUND else 'VIOLATION'}",flush=True)
else:
    ok,st=check_adv(extra)
    print(f"ADV k={k} m={m} target={BOUND+extra} nodes={st[0]} finished={st[1]} minfaults_at_stop={st[2]} {'OK' if ok else 'FAIL'}",flush=True)
