# slow.py n L mode : direct implementation of the definition, no cleverness.
# State = tuple of balls in arrival order, ball = (u,v,w) with u<v, w the receiver, bins 0..n-1 labelled (NO symmetry reduction).
# Adversary-win set = least fixpoint, computed by naive repeated sweeps over all states reachable from empty.
import sys, itertools
n,L,mode=int(sys.argv[1]),int(sys.argv[2]),int(sys.argv[3])
def comps(s):
    c=list(range(n))
    for (u,v,w) in s:
        a,b=c[u],c[v]
        c=[a if x==b else x for x in c]
    return c
def maxload(s):
    return max([sum(1 for b in s if b[2]==x) for x in range(n)])
def adv_moves(s):
    """list of options; each option is a tuple of successor states among which Rule chooses (1 for removals)"""
    out=[]
    if mode==0 and s: out.append((s[1:],))
    if mode==2:
        for j in range(len(s)): out.append((s[:j]+s[j+1:],))
    c=comps(s)
    for u in range(n):
        for v in range(u+1,n):
            if c[u]!=c[v]: out.append((s+((u,v,u),), s+((u,v,v),)))
    return out
states={(): None}; todo=[()]
while todo:
    s=todo.pop()
    if maxload(s)>=L: states[s]=[]; continue
    states[s]=adv_moves(s)
    for opt in states[s]:
        for t in opt:
            if t not in states: states[t]=None; todo.append(t)
win={s for s in states if maxload(s)>=L}
ch=True
while ch:
    ch=False
    for s,mv in states.items():
        if s not in win and any(all(t in win for t in opt) for opt in mv):
            win.add(s); ch=True
print("slow n=%d L=%d mode=%d labelled_states=%d RESULT: Adversary %s force load %d"%(n,L,mode,len(states),"CAN" if () in win else "CANNOT",L))
