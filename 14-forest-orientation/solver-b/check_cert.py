#!/usr/bin/env python3
"""check_cert.py CERTFILE n k
Independent checker (works on explicit edge orientations, not charges) that the adversary strategy in
CERTFILE forces total payment >= k on n vertices against every NORMAL-FORM reply of Algorithm
(normal form = Lemma 1 of proofs.txt: either orient the new edge out of an endpoint of outdegree<=1 and
flip nothing, or flip one directed path from a saturated endpoint x through saturated vertices to the first
unsaturated vertex and orient the new edge out of x).
Tree text: '(' digit children ')' , digit = 2 - outdegree. Orientation is recovered by leaf peeling and verified.
A state is the multiset of canonical tree strings (min over centre rootings, children sorted)."""
import sys
sys.setrecursionlimit(100000)

def parse(s):
    par=[]; dig=[]; st=[]; i=0
    while i<len(s):
        if s[i]=='(':
            v=len(par); par.append(st[-1] if st else -1); dig.append(int(s[i+1])); st.append(v); i+=2
        else:
            st.pop(); i+=1
    assert not st
    return par,dig

def orient(par,dig):
    """returns set of directed edges (tail,head) with outdeg(v)=2-dig[v]; unique for a tree; asserts validity"""
    n=len(par); adj=[set() for _ in range(n)]
    for v in range(1,n): adj[v].add(par[v]); adj[par[v]].add(v)
    need=[2-d for d in dig]; assert all(0<=x<=2 for x in need)
    edges=set(); leaves=[v for v in range(n) if len(adj[v])==1]; removed=[False]*n; left=n
    while left>1:
        v=leaves.pop()
        if removed[v]: continue
        (w,)=adj[v]
        if need[v]==1: edges.add((v,w))
        elif need[v]==0: edges.add((w,v)); need[w]-=1; assert need[w]>=0
        else: raise AssertionError("invalid outdegrees")
        removed[v]=True; left-=1; adj[w].discard(v)
        if len(adj[w])==1: leaves.append(w)
    last=[v for v in range(n) if not removed[v]][0]; assert need[last]==0
    return edges

def enc(adj,out,x,p):
    return "("+str(2-out[x])+"".join(sorted(enc(adj,out,y,x) for y in adj[x] if y!=p))+")"

def canon(vs,adj,out):
    deg={v:len(adj[v]) for v in vs}; layer=[v for v in vs if deg[v]<=1]; rem=len(vs); gone=set()
    while rem>2:
        nx=[]
        for x in layer:
            gone.add(x); rem-=1
            for y in adj[x]:
                if y not in gone:
                    deg[y]-=1
                    if deg[y]==1: nx.append(y)
        layer=nx
    return min(enc(adj,out,r,-1) for r in layer)

def graph(tstr,off):
    par,dig=parse(tstr); E=orient(par,dig); n=len(par)
    return n,{(a+off,b+off) for a,b in E}

def replies(sA,ru,sB,rv):
    """all normal-form replies after Adversary names u in tree sA (rooted string ru) and v in sB (rv).
    yields (cost, canonical string of merged tree)"""
    nA,EA=graph(sA,0); nB,EB=graph(sB,nA); n=nA+nB; E=EA|EB
    def info(E):
        adj={v:set() for v in range(n)}; out={v:0 for v in range(n)}; outn={v:[] for v in range(n)}
        for a,b in E: adj[a].add(b); adj[b].add(a); out[a]+=1; outn[a].append(b)
        return adj,out,outn
    adj,out,outn=info(E)
    us=[x for x in range(nA) if enc(adj,out,x,-1)==ru]; vs=[x for x in range(nA,n) if enc(adj,out,x,-1)==rv]
    assert us and vs, "named vertex not found"
    u=us[0]; v=vs[0]; res=[]
    for x,y in ((u,v),(v,u)):
        if out[x]<=1:
            paths=[[x]]
        else:
            paths=[]; stack=[[x]]
            while stack:
                p=stack.pop()
                for w in outn[p[-1]]:
                    if out[w]<=1: paths.append(p+[w])
                    else: stack.append(p+[w])
        for p in paths:
            E2=set(E)
            for a,b in zip(p,p[1:]): assert (a,b) in E2; E2.remove((a,b)); E2.add((b,a))
            E2.add((x,y))
            adj2,out2,_=info(E2); assert max(out2.values())<=2
            res.append((len(p)-1,canon(list(range(n)),adj2,out2)))
    return res

def main():
    fn,n,k=sys.argv[1],int(sys.argv[2]),int(sys.argv[3])
    cert={}
    for line in open(fn):
        if line.startswith('#') or not line.strip(): continue
        a,b,c,d=[x.strip() for x in line.split('|')]
        cert[tuple(sorted(b.split()))]=(int(a),)+tuple(c.split())+tuple(d.split())
    memo={}; rc={}
    def ok(state,k):
        if k<=0: return True
        if len(state)<=1: return False
        if (state,k) in memo: return memo[(state,k)]
        e=cert.get(state)
        if e is None or e[0]<k: print("no certificate entry for state",state,"k=",k); memo[(state,k)]=False; return False
        _,sA,ru,sB,rv=e; rest=list(state)
        rest.remove(sA); rest.remove(sB)     # raises if the two trees are not both present
        key=(sA,ru,sB,rv)
        if key not in rc: rc[key]=replies(sA,ru,sB,rv)
        r=True
        for cost,t in rc[key]:
            if cost>=k: continue
            if not ok(tuple(sorted(rest+[t])),k-cost): r=False; break
        memo[(state,k)]=r; return r
    start=tuple(["(2)"]*n)
    good=ok(start,k)
    print("n=%d k=%d : %s  (entries=%d, positions checked=%d)"%(n,k,"VERIFIED adversary forces >= k" if good else "FAILED",len(cert),len(memo)))
    sys.exit(0 if good else 1)
main()
