#!/usr/bin/env python3
# Slow, obvious solver: backtrack over E/Q on demand while walking every legal sequence (DFS over the sequence tree).
# No symmetry breaking, no compatibility or chain pruning. usage: slow.py n t L|S
import sys
sys.setrecursionlimit(100000)
n=int(sys.argv[1]);t=int(sys.argv[2]);mode=sys.argv[3]
N=1<<n
E={};Q={}
if mode=='S': Q[0]=2
def top(st): return st[-1] if st else 2
nodes=0
# agenda: list of (config, stack tuple, steps) to expand; process as explicit stack with continuation
def solve(agenda):
    global nodes
    if not agenda: return True
    c,st,s=agenda[-1]; rest=agenda[:-1]
    if s==t: return solve(rest)
    # expand the three ops one at a time: items (c,st,s,opindex)
    return expand(c,st,s,0,rest)
def expand(c,st,s,op,rest):
    global nodes
    if op==3: return solve(rest)
    if op==2 and not st: return expand(c,st,s,3,rest)
    st2=st[:-1] if op==2 else st+(op,)
    want=top(st2)
    if (c,op) in E:
        c2=E[(c,op)]
        if c2 in Q:
            if Q[c2]!=want: return False
            return expand(c,st,s,op+1,rest+[(c2,st2,s+1)])
        Q[c2]=want
        if expand(c,st,s,op+1,rest+[(c2,st2,s+1)]): return True
        del Q[c2]; return False
    free=(N-1)&~c
    a=free
    while True:
        c2=c|a; nodes+=1
        E[(c,op)]=c2
        if c2 in Q:
            if Q[c2]==want and expand(c,st,s,op+1,rest+[(c2,st2,s+1)]): return True
        else:
            Q[c2]=want
            if expand(c,st,s,op+1,rest+[(c2,st2,s+1)]): return True
            del Q[c2]
        del E[(c,op)]
        if a==0: break
        a=(a-1)&free
    return False
ok=solve([(0,(),0)])
print("slow n=%d t=%d mode=%s result=%s nodes=%d"%(n,t,mode,"SAT" if ok else "UNSAT",nodes))
