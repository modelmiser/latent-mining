#!/usr/bin/env python3
"""Writes witnesses.txt: for each (k,m) the play ADV (adv.py) versus FILL-FAR (strat.py).
Each line: k m F | walk | Online fault count on the walk | opt(walk) recomputed with the UNPRUNED work function."""
from adv import decide
from strat import strat
from game import wf_step
def opt_slow(walk,k):
    m=len(walk); OFF=max(abs(x) for x in walk)+3
    wf=((0,0),)
    for p in walk: wf=wf_step(wf,1<<(p+OFF),k,m,True)
    return min(c for _,c in wf)
out=[]
for m in range(1,5):
    for k in range(2,8):
        st=(0,0,0,0,0,0,False,0); order=strat('fill_far',(),0,k); walk=[0]; faults=1
        while True:
            p,st=decide(st,set(order),k,m)
            if p is None: break
            walk.append(p)
            if p not in order: faults+=1; order=strat('fill_far',order,p,k)
        F=max(m+1,2*m-k//2+1); o=opt_slow(walk,k) if (k<=5 and len(walk)<=14) else None
        out.append("k=%d m=%d F=%d | walk %s | FILL-FAR faults %d | opt_unpruned %s"%(k,m,F," ".join(map(str,walk)),faults,o))
        assert faults==F and (o is None or o<=m)
open('witnesses.txt','w').write(__doc__+"\n".join(out)+"\n")
print("\n".join(out))
