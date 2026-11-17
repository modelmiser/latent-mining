import sys,random
import game
from game import *
F,n,nw=game.solve(); V,opts,idx,st=game.G
inv=[None]*len(idx)
for s,i in idx.items(): inv[i]=s
def show(C,pos):
    return sorted(pos+i-OFF for i in range(C.bit_length()) if C>>i&1)
random.seed(int(sys.argv[4]) if len(sys.argv)>4 else 0)
i=min(st,key=lambda j:V[j]); pos=0; print("F",F)
print("req 0 FAULT cache",show(inv[i][1],0),"V",V[i])
steps=0
while V[i]>0 and steps<60:
    steps+=1
    # adversary: choose option maximizing; prefer fault options, random tie
    best=[]
    for oi,(fl,nx) in enumerate(opts[i]):
        t=fl+min(V[j] for j in nx)
        best.append((t,fl,oi))
    mx=max(b[0] for b in best)
    cands=[b for b in best if b[0]==mx]
    fc=[b for b in cands if b[1]==1]
    t,fl,oi=random.choice(fc or cands)
    # find direction: recompute
    d=None
    cnt=0
    for dd in (1,-1):
        if game.wf_step(inv[i][0],dd) is not None:
            if cnt==oi: d=dd
            cnt+=1
    pos+=d
    nx=opts[i][oi][1]
    mn=min(V[j] for j in nx)
    j=random.choice([j for j in nx if V[j]==mn])
    i=j
    wf=inv[i][0]
    print("req",pos,"FAULT" if fl else "hit  ","cache",show(inv[i][1],pos),"V",V[i],"opt",min(f for C,f in wf), "wf",[(show(C,pos),f) for C,f in wf][:6])
