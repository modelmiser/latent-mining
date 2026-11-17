#!/usr/bin/env python3
# Independent checker for witness files written by fast.cpp.  usage: check.py file
#  strat_G_hH_kK.txt : cop strategy table; verifies it captures against every adversary answer, with no cycles.
#  lose_G_hH_kK.txt  : closed set L of states; verifies every start (c, V\c, K) is in L, no state has empty S,
#                      every blind move from a state in L lands in L (and does not clear S), and for j>0 the listed
#                      bad glimpse answer is a legal answer after which every cop move lands in L at j-1.
#                      => the cop can never leave L, so never wins with K glimpses.
import sys,re
fn=sys.argv[1]
m=re.search(r'(strat|lose)_([ED])_h(\d+)_k(\d+)',fn); kind,G,h,K=m.group(1),m.group(2),int(m.group(3)),int(m.group(4))
n=2**(h+1)-1                      # vertices 1..n (heap), bit v
nb={v:[u for u in (v//2,2*v,2*v+1) if 1<=u<=n] for v in range(1,n+1)}
def expand(S,c):
    R=S
    for v in range(1,n+1):
        if S>>v&1:
            for u in nb[v]: R|=1<<u
    return R&~(1<<c)
def branch(c,u):
    seen={u}; st=[u]
    while st:
        x=st.pop()
        for y in nb[x]:
            if y!=c and y not in seen: seen.add(y); st.append(y)
    return sum(1<<v for v in seen)
ALL=sum(1<<v for v in range(1,n+1))
def fail(msg): print("REJECT",fn,":",msg); sys.exit(1)
lines=[l.split() for l in open(fn) if not l.startswith('#')]
if kind=='strat':
    start=int(re.search(r'start=(\d+)',open(fn).readline()).group(1))
    tab={}
    for t in lines:
        key=(int(t[0]),int(t[1]),int(t[2],16))
        tab[key]=('M',int(t[4])) if t[3]=='M' else ('G',{int(a.split(':')[0]):int(a.split(':')[1]) for a in t[4:]})
    state={}  # 1 on stack, 2 done
    def visit(j,c,S):
        st=[(j,c,S,0)]
        # iterative DFS with cycle detection
        stack=[((j,c,S),None)]
        while stack:
            key,it=stack[-1]
            if it is None:
                if state.get(key)==2: stack.pop(); continue
                if state.get(key)==1: fail("cycle at %s"%(key,))
                if key not in tab: fail("state not in table %s"%(key,))
                state[key]=1
                j,c,S=key; act=tab[key]; succ=[]
                if c<1 or c>n or S>>c&1 or S==0: fail("bad state")
                if act[0]=='M':
                    c2=act[1]
                    if c2!=c and c2 not in nb[c]: fail("illegal move")
                    S1=S&~(1<<c2)
                    if S1: succ.append((j,c2,expand(S1,c2)))
                else:
                    if j<=0: fail("glimpse without budget")
                    if G=='E': answers=[(v,1<<v) for v in range(1,n+1) if S>>v&1]
                    else: answers=[(u,S&branch(c,u)) for u in nb[c] if S&branch(c,u)]
                    for a,A in answers:
                        if a not in act[1]: fail("answer %d not handled at %s"%(a,key))
                        c2=act[1][a]
                        if c2!=c and c2 not in nb[c]: fail("illegal move")
                        A1=A&~(1<<c2)
                        if A1: succ.append((j-1,c2,expand(A1,c2)))
                stack[-1]=(key,iter(succ))
            else:
                try: stack.append((next(it),None))
                except StopIteration: state[key]=2; stack.pop()
    visit(K,start,ALL&~(1<<start))
    print("ACCEPT",fn,": cop wins %s(h=%d,k=%d) from start %d; %d states reachable"%(G,h,K,start,len(state)))
else:
    L={}
    for t in lines: L[(int(t[0]),int(t[1]),int(t[2],16))]=int(t[3])
    for c in range(1,n+1):
        if (K,c,ALL&~(1<<c)) not in L: fail("start %d missing"%c)
    for (j,c,S),bad in L.items():
        if S==0 or S>>c&1: fail("bad state")
        for c2 in [c]+nb[c]:
            S1=S&~(1<<c2)
            if S1==0: fail("blind capture possible at %s"%((j,c,S),))
            if (j,c2,expand(S1,c2)) not in L: fail("blind move leaves L at %s -> %d"%((j,c,S),c2))
        if j>0:
            if G=='E':
                if not S>>bad&1: fail("illegal answer")
                A=1<<bad
            else:
                if bad not in nb[c]: fail("illegal answer")
                A=S&branch(c,bad)
                if A==0: fail("illegal answer (empty)")
            for c2 in [c]+nb[c]:
                A1=A&~(1<<c2)
                if A1==0: fail("capture after glimpse at %s"%((j,c,S),))
                if (j-1,c2,expand(A1,c2)) not in L: fail("glimpse move leaves L at %s ans %d -> %d"%((j,c,S),bad,c2))
    print("ACCEPT",fn,": cop cannot win %s(h=%d,k=%d); closed losing set of %d states"%(G,h,K,len(L)))
