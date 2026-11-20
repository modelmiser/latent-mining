#!/usr/bin/env python3
"""Simulate the 'lag' Firer strategy of proofs.txt (Theorem 2) against lists and report
max lag and max height - t/5.  Pure sanity check of the proof's invariants (cheap, single core).
Usage: strategy_sim.py [trials] [length] [width]"""
import sys, random
from fractions import Fraction as Fr
def run(lst):
    k={}; F={}; eta={}
    worst_h=Fr(-99); worst_l=Fr(0); theta_ok=True
    for t,a in enumerate(lst,1):
        k[a]=k.get(a,0)+1; eta[a]=eta.get(a,0)+1
        def lag(x): return Fr(3*k.get(x,0)+k.get(x-1,0)+k.get(x+1,0),5)-F.get(x,0)
        sites=set(k)|{x-1 for x in k}|{x+1 for x in k}
        L={x:lag(x) for x in sites}
        assert all(v>=0 for v in L.values())
        M=max(L.values()); worst_l=max(worst_l,M)
        worst_h=max(worst_h,max(eta.values())-Fr(t,5))
        cand=[x for x in sites if eta.get(x,0)>=2 and L[x]>=1]
        if M>=6: assert any(L[x]==M for x in cand), "max-lag site not ready"
        if cand:
            x=max(cand,key=lambda y:L[y])
            eta[x]-=2; eta[x-1]=eta.get(x-1,0)+1; eta[x+1]=eta.get(x+1,0)+1; F[x]=F.get(x,0)+1
            L[x]-=1
        worst_h=max(worst_h,max(eta.values())-Fr(t,5))
        theta=sum(max(v-6,0)**2 for v in L.values())
        assert theta<=t
    return worst_l,worst_h
if __name__=="__main__":
    trials=int(sys.argv[1]) if len(sys.argv)>1 else 200
    n=int(sys.argv[2]) if len(sys.argv)>2 else 400
    w=int(sys.argv[3]) if len(sys.argv)>3 else 5
    random.seed(1); wl=0; wh=-99
    for tr in range(trials):
        mode=tr%4
        if mode==0: lst=[random.randrange(w) for _ in range(n)]
        elif mode==1: lst=[0]*n
        elif mode==2: lst=[(i%w) for i in range(n)]
        else:
            lst=[];p=0
            for i in range(n):
                if random.random()<0.1: p=random.randrange(w)
                lst.append(p)
        a,b=run(lst); wl=max(wl,a); wh=max(wh,b)
    print("max lag seen",float(wl),"max (height - t/5) seen",float(wh))
