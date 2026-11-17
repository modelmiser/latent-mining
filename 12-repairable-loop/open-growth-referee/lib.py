# Referee library: literal definitions, written independently of open-growth/*.py
import random, itertools
from collections import Counter
def wins(v,k):
    m=len(v); vv=v+v[:k] if m>=k else (v*(k//m+2))
    return [vv[i:i+k] for i in range(m)]
def N(v,k): return Counter(wins(v,k))
def universal(v,n): return len(v)>=2**n and len(set(wins(v,n)))==2**n
def loss(v,c): return v[:c]+v[c+1:]
def dbl(v,c): return v[:c+1]+v[c]+v[c+1:]
def moves(v):
    yield ('pass',-1,v)
    if len(v)>=2:
        for c in range(len(v)): yield ('loss',c,loss(v,c))
    for c in range(len(v)): yield ('dbl',c,dbl(v,c))
def answers(v,n): return [(t,c) for t,c,x in moves(v) if universal(x,n)]
def answerable(v,n): return any(universal(x,n) for t,c,x in moves(v))
def repairable(v,n): return all(answerable(x,n) for t,c,x in moves(v))
def runs(v):
    """cyclic runs as list of (letter,length,startcell); needs >=2 runs"""
    m=len(v); st=[i for i in range(m) if v[i]!=v[i-1]]
    if not st: return [(v[0],m,0)]
    out=[]
    for a,b in zip(st,st[1:]+[st[0]+m]): out.append((v[a],b-a,a))
    return out
def rho(v): return sum(1 for i in range(len(v)) if v[i]!=v[i-1])
def S(v): return sum(1 for i in range(len(v)) if v[i-1]!=v[i]!=v[(i+1)%len(v)])
def singles(v): return [i for i in range(len(v)) if v[i-1]!=v[i]!=v[(i+1)%len(v)]]
def euler_universal(n,extra,rng):
    """random universal loop: Euler circuit of de Bruijn graph (nodes = (n-1)-words) plus the n-words in `extra` (must be balanced)"""
    edges={}
    for w in itertools.product('01',repeat=n):
        w=''.join(w); edges.setdefault(w[:-1],[]).append(w[-1])
    for w in extra: edges[w[:-1]].append(w[-1])
    for k in edges: rng.shuffle(edges[k])
    start='0'*(n-1); stack=[start]; circ=[]
    while stack:
        u=stack[-1]
        if edges[u]:
            b=edges[u].pop(); stack.append((u+b)[1:])
        else: circ.append(stack.pop())
    circ.reverse()
    v=''.join(x[-1] for x in circ[1:])
    assert len(v)==2**n+len(extra) and universal(v,n)
    return v
def periodic_edges(p,n): # n-windows of the periodic word p^inf : a closed walk of length len(p)
    s=p*(n+2); return [s[i:i+n] for i in range(len(p))]
def random_extra(n,e,rng):
    ex=[]
    while len(ex)<e:
        r=e-len(ex); cand=['0','1']+(['01'] if r>=2 else [])+(['001','011'] if r>=3 else [])+(['0001','0011','0111'] if r>=4 else [])
        ex+=periodic_edges(rng.choice(cand),n)
    return ex
def load(fn):
    return [l.split()[0] for l in open(fn) if l.strip() and set(l.split()[0])<=set('01')]
