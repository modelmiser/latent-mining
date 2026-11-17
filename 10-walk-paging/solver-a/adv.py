#!/usr/bin/env python3
"""Explicit adversary strategy ADV (see proofs.txt, Theorem L) played against EVERY Online behaviour.
Computes min over Online of total faults (Dijkstra, 0/1 weights) and asserts every adversary move is
legal (exact opt via work function).  usage: adv.py kmax mmax [slow]   ('slow' = unpruned Online moves)
Option env BREAK=1: sabotaged adversary (always prefers a CACHED frontier) -- must give a smaller value."""
import sys, os, heapq
from game import wf_step, online_options
BREAK=os.environ.get('BREAK')=='1'
def decide(st,C,k,m):
    """st=(pos,a,b,phase,dir,nb,fresh,free)  C=set of pages. returns (request or None, newstate)"""
    pos,a,b,phase,d,nb,fresh,free=st
    j=k//2
    if phase==0: O=range(a,b+1)
    elif d==-1: O=range(a,min(b,a+k-1)+1)
    else: O=range(max(a,b-k+1),b+1)
    miss=[x for x in O if x not in C]
    if miss:
        t=min(miss,key=lambda x:(abs(x-pos),x))
        p=pos+(1 if t>pos else -1)
        return p,(p,a,b,phase,d,nb,False,free)
    v=b-a+1
    if phase==0:
        if v==2*m: return None,st
        la=(a-1) in C; rb=(b+1) in C
        if BREAK: side=-1 if la else (1 if rb else -1)
        else: side=-1 if not la else (1 if not rb else -1)
        if v==k-1:
            phase=1; d=side; nb=j; free=1 if k%2==0 else 0; fresh=False
            st=(pos,a,b,phase,d,nb,fresh,free)
        else:
            end=a if side==-1 else b
            if pos!=end:
                p=pos+(1 if end>pos else -1); return p,(p,a,b,phase,d,nb,fresh,free)
            p=pos+side
            return p,(p,min(a,p),max(b,p),phase,d,nb,fresh,free)
    # steady
    end=a if d==-1 else b
    if not fresh and not free and nb==m: return None,st
    if pos!=end:
        p=pos+(1 if end>pos else -1); return p,(p,a,b,phase,d,nb,fresh,free)
    p=pos+d; a2,b2=min(a,p),max(b,p)
    if free: return p,(p,a2,b2,phase,d,nb,False,0)
    if fresh: return p,(p,a2,b2,phase,d,nb,False,0)      # second page of a pair block
    return p,(p,a2,b2,phase,d,nb+1,True,0)               # opens a new block
def play(k,m,slow=False):
    OFF=2*m+3
    def mask(C): return sum(1<<(x+OFF) for x in C)
    wf0=wf_step(((0,0),),1<<OFF,k,m,False)
    st0=(0,0,0,0,0,0,False,0)
    pq=[]; dist={}; cnt=0
    for oc in online_options(0,1<<OFF,k,slow):
        n=(st0,wf0,oc); dist[n]=1; heapq.heappush(pq,(1,cnt,n)); cnt+=1
    best=None; nodes=0
    while pq:
        dcur,_,n=heapq.heappop(pq)
        if dist[n]<dcur: continue
        nodes+=1
        st,wf,oc=n
        C={i-OFF for i in range(oc.bit_length()) if (oc>>i)&1}
        p,st2=decide(st,C,k,m)
        if p is None: best=dcur; break      # Dijkstra: first terminal popped is the minimum
        pb=1<<(p+OFF)
        w2=wf_step(wf,pb,k,m,False)
        assert w2 is not None, ("ILLEGAL adversary move",k,m,st,p)
        if oc&pb: nxt=[(0,oc)]
        else: nxt=[(1,o2) for o2 in online_options(oc,pb,k,slow)]
        for c,o2 in nxt:
            n2=(st2,w2,o2); d2=dcur+c
            if d2<dist.get(n2,1<<30): dist[n2]=d2; heapq.heappush(pq,(d2,cnt,n2)); cnt+=1
    return best,nodes
if __name__=="__main__":
    K=int(sys.argv[1]); M=int(sys.argv[2]); slow=len(sys.argv)>3
    for m in range(1,M+1):
        row=[]
        for k in range(2,K+1):
            v,n=play(k,m,slow); t=max(m+1,2*m-k//2+1)
            row.append("%s%s"%(v,"" if v==t else "(!=%d)"%t))
        print("ADV%s m=%d k=2..%d:"%(" slow" if slow else "",m,K)," ".join(row),flush=True)
