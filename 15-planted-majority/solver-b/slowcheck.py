#!/usr/bin/env python3
"""Slow, direct implementation of the problem definition. Independent of the C++ search.
Protocol file format (plain text):
  line 1: s
  line 2: qA qB           (state indices)
  line 3: w as a string over {A,B}, one letter per state
  then s*s lines 'p q p2 q2' meaning d(p,q)=(p2,q2); missing lines = identity.
Usage: slowcheck.py file N   -> prints first failing instance with a reason, or PASS up to N.
Agents are kept as explicit ordered pairs of distinct agents, as in the text."""
import sys, itertools
def load(fn):
    L=[l.split('#')[0].strip() for l in open(fn)]; L=[l for l in L if l]
    s=int(L[0]); qA,qB=map(int,L[1].split()); w=L[2]; assert len(w)==s and qA!=qB
    d={(p,q):(p,q) for p in range(s) for q in range(s)}
    for l in L[3:]:
        p,q,p2,q2=map(int,l.split()); d[(p,q)]=(p2,q2)
    return s,qA,qB,w,d
def succ(c,d):
    # c: sorted tuple of agent states. returns list of successor multisets (legal moves), via ordered pairs of distinct agents
    out=[]; n=len(c)
    for i in range(n):
        for j in range(n):
            if i==j: continue
            p,q=c[i],c[j]; r=d[(p,q)]
            if r==(p,q): continue
            l=list(c); l[i]=r[0]; l[j]=r[1]; out.append(tuple(sorted(l)))
    return out
def check(s,qA,qB,w,d,a,b,x):
    maj='A' if a>b else 'B'
    init=tuple(sorted([qA]*a+[qB]*b+[x]))
    R={init:None}; stack=[init]
    while stack:
        c=stack.pop(); sc=succ(c,d); R[c]=sc
        for t in sc:
            if t not in R: R[t]=None; stack.append(t)
    good=lambda c: all(w[p]==maj for p in c)
    for c,sc in R.items():
        if not good(c):
            if not sc: return ('bad terminal',c)
            if c in sc: return ('bad self-loop',c)
    # non-good config on a cycle: can c reach itself?
    for c in R:
        if good(c): continue
        seen=set(); st=list(R[c])
        while st:
            t=st.pop()
            if t==c: return ('bad cycle through',c)
            if t in seen: continue
            seen.add(t); st.extend(R[t])
    return None
def G_upto(P,N):
    s=P[0]
    for n in range(2,N+1):
        for a in range(n+1):
            b=n-a
            if abs(a-b)<2: continue
            for x in range(s):
                r=check(*P,a,b,x)
                if r: return (n,a,b,x,r)
    return None
if __name__=='__main__':
    P=load(sys.argv[1]); N=int(sys.argv[2]); r=G_upto(P,N)
    print('PASS all instances with 2<=a+b<=%d'%N if r is None else 'FAIL at a+b=%d: (a,b,x)=(%d,%d,%d) %s %s'%(r[0],r[1],r[2],r[3],r[4][0],r[4][1]))
