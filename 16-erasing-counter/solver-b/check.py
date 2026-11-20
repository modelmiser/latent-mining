#!/usr/bin/env python3
"""Independent checker. File: line1 tree  ( (i T0 T1) | [] | [i] ), line2 start state as d chars (position 1 first), line3 claimed N.
Validates the tree against the definition for depth r, then follows the orbit: start must return after exactly N steps, not earlier."""
import sys,re
r=int(sys.argv[1]); d=2**r-1
L=open(sys.argv[2]).read().split('\n'); toks=re.findall(r'[()\[\]]|\d+',L[0]); start=L[1].strip(); N=int(L[2])
pos=0
def rd(path):
    global pos
    t=toks[pos]; pos+=1
    if t=='[':
        if toks[pos]==']': pos+=1; return None
        v=int(toks[pos]); pos+=2
        if v not in path: sys.exit('REJECT: leaf writes index %d not on its path'%v)
        return (v,)
    if t!='(' : sys.exit('REJECT: parse')
    i=int(toks[pos]); pos+=1
    if not 1<=i<=d or i in path: sys.exit('REJECT: bad or repeated index %d'%i)
    if len(path)>=r: sys.exit('REJECT: path longer than r')
    a=rd(path+[i]); b=rd(path+[i])
    if toks[pos]!=')': sys.exit('REJECT: parse')
    pos+=1
    return (i,a,b)
T=rd([])
if pos!=len(toks) or len(start)!=d: sys.exit('REJECT: parse/len')
def step(s):
    t=T; m=0
    while t is not None and len(t)==3:
        b=1<<(t[0]-1); m|=b; t=t[2] if s&b else t[1]
    s&=~m
    if t is not None: s|=1<<(t[0]-1)
    return s
s0=sum(1<<i for i,c in enumerate(start) if c=='1'); s=s0
for k in range(1,N+1):
    s=step(s)
    if s==s0:
        if k==N: print('ACCEPT: r=%d cycle of length exactly %d'%(r,N)); sys.exit(0)
        sys.exit('REJECT: returns already after %d steps'%k)
sys.exit('REJECT: no return after %d steps'%N)
