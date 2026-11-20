#!/usr/bin/env python3
"""Slow, direct implementation of the definition. Tree text format:
   leaf: [] or [i]      internal: (i T0 T1)     positions 1..d
Exhaustive over ALL trees (no symmetry) for small r."""
import sys
def parse(txt):
    toks=txt.replace('(',' ( ').replace(')',' ) ').replace('[',' [ ').replace(']',' ] ').split()
    pos=0
    def rd():
        nonlocal pos
        t=toks[pos]; pos+=1
        if t=='[':
            if toks[pos]==']': pos+=1; return ('L',None)
            v=int(toks[pos]); assert toks[pos+1]==']'; pos+=2; return ('L',v)
        assert t=='('
        i=int(toks[pos]); pos+=1
        a=rd(); b=rd(); assert toks[pos]==')'; pos+=1
        return ('N',i,a,b)
    t=rd(); assert pos==len(toks); return t
def validate(t,r,d,path=()):
    if t[0]=='L':
        assert len(path)<=r, "too deep"
        assert t[1] is None or t[1] in path, "leaf writes unread index"
        return
    assert 1<=t[1]<=d and t[1] not in path, "bad/repeated index"
    validate(t[2],r,d,path+(t[1],)); validate(t[3],r,d,path+(t[1],))
def step(t,s):   # s: tuple of bits, s[i-1] = position i
    P=[]
    while t[0]=='N':
        P.append(t[1]); t=t[3] if s[t[1]-1] else t[2]
    s=list(s)
    for i in P: s[i-1]=0
    if t[1] is not None: s[t[1]-1]=1
    return tuple(s)
def longest_cycle(t,d):
    import itertools
    best=0;bs=None;seen={}
    for s0 in itertools.product((0,1),repeat=d):
        if s0 in seen: continue
        order={};s=s0
        while s not in seen and s not in order:
            order[s]=len(order); s=step(t,s)
        if s in order:
            L=len(order)-order[s]
            if L>best: best=L;bs=s
        for x in order: seen[x]=1
    return best,bs
def show(t):
    if t[0]=='L': return '[]' if t[1] is None else '[%d]'%t[1]
    return '(%d %s %s)'%(t[1],show(t[2]),show(t[3]))
def trees(d,depth,path=()):
    yield ('L',None)
    for i in path: yield ('L',i)
    if depth==0: return
    for i in range(1,d+1):
        if i in path: continue
        subs=list(trees(d,depth-1,path+(i,)))
        for a in subs:
            for b in subs: yield ('N',i,a,b)
if __name__=='__main__':
    if sys.argv[1]=='enum':
        r=int(sys.argv[2]); d=2**r-1; best=0;n=0;hist={}
        for t in trees(d,r):
            n+=1; L,s=longest_cycle(t,d); hist[L]=hist.get(L,0)+1
            if L>best: best=L;bt=t;bs=s
        print('r',r,'trees',n,'D',best,'witness',show(bt),'start',''.join(map(str,bs)),'hist',sorted(hist.items()))
    else:  # check r file : file has tree text on line 1; prints longest cycle
        r=int(sys.argv[2]); d=2**r-1
        t=parse(open(sys.argv[3]).readline()); validate(t,r,d)
        print('valid; longest cycle',longest_cycle(t,d))
