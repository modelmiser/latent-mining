#!/usr/bin/env python3
# Independent checker. usage: verify.py witnessfile
# File: "r R" / "N n" / "start b1..bd" / then one line per node: "<path> I <index>" or "<path> L <R>"
# where <path> is the 0/1 string from the root ("-" = root) and R=0 means the empty set.
import sys
def load(fn):
    nodes={}; r=N=None; start=None
    for ln in open(fn):
        t=ln.split()
        if not t: continue
        if t[0]=='r': r=int(t[1])
        elif t[0]=='N': N=int(t[1])
        elif t[0]=='start': start=[int(x) for x in t[1:]]
        else: nodes['' if t[0]=='-' else t[0]]=(t[1],int(t[2]))
    return r,N,start,nodes
def check(fn):
    r,N,start,nodes=load(fn); d=2**r-1
    assert len(start)==d and all(b in (0,1) for b in start), "bad start"
    # structural checks
    def rec(p,onpath):
        assert p in nodes, "missing node "+p
        k,x=nodes[p]
        if k=='L':
            assert x==0 or x in onpath, "leaf R not on path at "+repr(p)
            return 1
        assert k=='I' and 1<=x<=d, "bad index"
        assert x not in onpath, "index repeated on path at "+repr(p)
        assert len(onpath)<r, "path too deep at "+repr(p)
        return 1+rec(p+'0',onpath+[x])+rec(p+'1',onpath+[x])
    cnt=rec('',[])
    assert cnt==len(nodes), "stray nodes"
    def f(s):
        p=''; P=[]
        while nodes[p][0]=='I':
            i=nodes[p][1]; P.append(i); p+=str(s[i-1])
        R=nodes[p][1]; s=list(s)
        for i in P: s[i-1]=1 if i==R else 0
        return s
    s=f(start); k=1
    while s!=start:
        assert k<N, "no return within N steps"
        s=f(s); k+=1
    assert k==N, "returns early at %d"%k
    return r,N
if __name__=='__main__':
    try:
        r,N=check(sys.argv[1]); print("OK r=%d cycle length %d"%(r,N))
    except AssertionError as e:
        print("REJECTED:",e); sys.exit(1)
