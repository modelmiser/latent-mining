#!/usr/bin/env python3
# Direct, obvious implementation of the definitions. No symmetry, no pruning.
# usage: slow.py T MMAX   -> prints N(t) t<=T (integer BFS over ordered pairs) and D(m), m<=MMAX (BFS on Z_m x Z_m)
import sys
T=int(sys.argv[1]); MMAX=int(sys.argv[2])
def moves(a,b): return [(2*a,b),(a,2*b),(a+b,b),(a,a+b)]
# integer BFS
G={1:0}; seen={(1,1)}; front=[(1,1)]
N=[1]
for t in range(1,T+1):
    nf=[]
    for (a,b) in front:
        for s in moves(a,b):
            if s not in seen:
                seen.add(s); nf.append(s)
                for v in s:
                    if v not in G: G[v]=t
    front=nf; N.append(len(G))
print("N", *N)
print("G", *[G.get(n,-1) for n in range(1,65)])
def Dm(m):
    g=[-1]*m; g[1%m]=0
    seen={(1%m,1%m)}; front=[(1%m,1%m)]; t=0; left=m-1 if m>1 else 0
    while left:
        t+=1; nf=[]
        for (a,b) in front:
            for (x,y) in moves(a,b):
                s=(x%m,y%m)
                if s not in seen:
                    seen.add(s); nf.append(s)
                    for v in s:
                        if g[v]<0: g[v]=t; left-=1
        front=nf
        assert front
    return max(g),g
for m in range(2,MMAX+1):
    d,g=Dm(m)
    print("D",m,d)
