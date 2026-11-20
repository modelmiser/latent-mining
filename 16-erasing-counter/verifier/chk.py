#!/usr/bin/env python3
# Referee witness checker. Reads solver-a format ("r/N/start/path I idx/path L idx") or solver-b format (s-expression, start, N).
# Validates the tree against the problem definition, then follows the orbit: start must return first at exactly step N.
# usage: chk.py r file [fmt-b-start-override]
import sys,re
def fail(m): print("REJECT:",m); sys.exit(1)
def parse_a(lines):
    r=N=None; start=None; nodes={}
    for ln in lines:
        t=ln.split()
        if not t: continue
        if t[0]=='r': r=int(t[1])
        elif t[0]=='N': N=int(t[1])
        elif t[0]=='start': start=[int(x) for x in t[1:]]
        else:
            p='' if t[0]=='-' else t[0]
            if not re.fullmatch('[01]*',p): fail("bad path "+p)
            if p in nodes: fail("duplicate node "+p)
            nodes[p]=(t[1],int(t[2]))
    def build(p):
        if p not in nodes: fail("missing node at '%s'"%p)
        k,v=nodes[p]
        if k=='I': return ('I',v,build(p+'0'),build(p+'1'))
        if k=='L': return ('L',v)   # 0 = empty
        fail("bad kind")
    tree=build('')
    cnt=[0]
    def count(t):
        cnt[0]+=1
        if t[0]=='I': count(t[2]); count(t[3])
    count(tree)
    if cnt[0]!=len(nodes): fail("unreachable node lines")
    return r,N,start,tree
def parse_b(lines):
    toks=re.findall(r'\(|\)|\[|\]|\d+',lines[0]); pos=[0]
    def rd():
        t=toks[pos[0]]; pos[0]+=1
        if t=='(':
            i=int(toks[pos[0]]); pos[0]+=1; a=rd(); b=rd()
            if toks[pos[0]]!=')': fail("syntax")
            pos[0]+=1; return ('I',i,a,b)
        if t=='[':
            if toks[pos[0]]==']': pos[0]+=1; return ('L',0)
            i=int(toks[pos[0]]); pos[0]+=1
            if toks[pos[0]]!=']': fail("syntax")
            pos[0]+=1; return ('L',i)
        fail("syntax")
    tree=rd()
    if pos[0]!=len(toks): fail("trailing tokens")
    start=[int(c) for c in lines[1].strip()]; N=int(lines[2])
    return None,N,start,tree
def validate(t,r,d,path):
    if t[0]=='L':
        if t[1]!=0 and t[1] not in path: fail("leaf writes %d not on its path %s"%(t[1],path))
        return
    i=t[1]
    if not (1<=i<=d): fail("index %d out of range"%i)
    if i in path: fail("index %d repeated on a path"%i)
    if len(path)>=r: fail("path longer than r")
    validate(t[2],r,d,path+[i]); validate(t[3],r,d,path+[i])
def idxset(t):
    return set() if t[0]=='L' else {t[1]}|idxset(t[2])|idxset(t[3])
def step(tree,s):
    t=tree; P=0
    while t[0]=='I':
        b=1<<(t[1]-1); P|=b; t=t[3] if s&b else t[2]
    s&=~P
    if t[1]: s|=1<<(t[1]-1)
    return s
def main():
    r=int(sys.argv[1]); lines=open(sys.argv[2]).read().strip().split('\n')
    if lines[0].lstrip()[:1] in '([': r2,N,start,tree=parse_b(lines)
    else:
        r2,N,start,tree=parse_a(lines)
        if r2!=r: fail("file says r=%s"%r2)
    d=2**r-1
    if len(start)!=d or any(x not in (0,1) for x in start): fail("start is not in {0,1}^d")
    if N<1: fail("N<1")
    validate(tree,r,d,[])
    s0=sum(b<<i for i,b in enumerate(start)); s=s0; minw=bin(s0).count('1')
    for k in range(1,N+1):
        s=step(tree,s)
        if s==s0:
            if k<N: fail("returns early at step %d"%k)
            break
        w=bin(s).count('1')
        if w<minw: minw=w
    else: fail("no return to start at step N=%d"%N)
    print("ACCEPT r=%d N=%d indices=%d start_weight=%d min_weight_on_cycle=%d"%(r,N,len(idxset(tree)),sum(start),minw))
main()
