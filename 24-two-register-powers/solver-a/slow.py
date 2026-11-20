#!/usr/bin/env python3
# Direct, obvious implementation of the definitions. No pruning at all.
# usage: slow.py N tmax   -> N(t), G(n) by level sets of integer states
#        slow.py D mmax   -> D(m) by level sets of states in Z_m x Z_m
import sys
def succ(a,b): return ((2*a,b),(a,2*b),(a+b,b),(a,a+b))
def run_N(tmax):
    level={(1,1)}; G={1:0}; out=[]
    for t in range(tmax+1):
        for (a,b) in level:
            G.setdefault(a,t); G.setdefault(b,t)
        out.append(len(G)); print("N",t,len(G),"levelstates",len(level),flush=True)
        if t<tmax:
            level={s for st in level for s in succ(*st)}
    with open("slow_G.txt","w") as f:
        for n in sorted(G): f.write("%d %d\n"%(n,G[n]))
def D_of(m):
    level={(1%m,1%m)}; seen=set(); allst=set(level); g={}; t=0
    while True:
        for (a,b) in level:
            g.setdefault(a,t); g.setdefault(b,t)
        if len(g)==m: break
        nxt=set()
        for (a,b) in level:
            for (c,d) in succ(a,b): nxt.add((c%m,d%m))
        # NOTE: level = ALL states reachable in exactly t steps (no visited pruning)
        level=nxt; t+=1
    D=max(g.values()); worst=[r for r in range(m) if g[r]==D]
    return D,worst
def run_D(mmax):
    with open("slow_D.txt","w") as f:
        for m in range(2,mmax+1):
            D,w=D_of(m); f.write("%d %d %d %d\n"%(m,D,len(w),w[0])); f.flush()
if sys.argv[1]=="N": run_N(int(sys.argv[2]))
else: run_D(int(sys.argv[2]))
