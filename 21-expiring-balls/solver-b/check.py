# check.py n L mode file : independent checker of witness files (own move generation, all n bins explicit).
import sys
n,L,mode=int(sys.argv[1]),int(sys.argv[2]),int(sys.argv[3]); fn=sys.argv[4]
def parse(t): return () if t=='-' else tuple((int(x[0]),int(x[1])) for x in t.split(','))
def canon(s):
    mp={}; out=[]
    for r,o in s:
        for x in (r,o):
            if x not in mp: mp[x]=len(mp)+1
        out.append((mp[r],mp[o]))
    return tuple(out)
def maxload(s):
    c={}
    for r,o in s: c[r]=c.get(r,0)+1
    return max(c.values()) if c else 0
def comp(s):
    c={x:x for x in range(1,n+1)}
    for r,o in s:
        a,b=c[r],c[o]
        assert a!=b, "not a forest"
        for x in c:
            if c[x]==b: c[x]=a
    return c
def removals(s):
    if mode==0: return [s[1:]] if s else []
    if mode==2: return [s[:j]+s[j+1:] for j in range(len(s))]
    return []
lines=[l.split() for l in open(fn) if not l.startswith('#')]
if lines and len(lines[0])==1:   # Rule safe set
    S={parse(l[0]) for l in lines}
    assert () in S, "empty state missing"
    for s in S:
        assert s==canon(s), "non-canonical"
        assert maxload(s)<L, "load reached in "+str(s)
        for t in removals(s): assert canon(t) in S, "removal leaves set: "+str(s)
        c=comp(s)
        for u in range(1,n+1):
            for v in range(u+1,n+1):
                if c[u]!=c[v]:
                    assert canon(s+((u,v),)) in S or canon(s+((v,u),)) in S, "offer %d,%d unanswerable in %s"%(u,v,s)
    print("OK: Rule safe set of %d states verified: Adversary cannot force load %d on %d bins (mode %d)"%(len(S),L,n,mode))
else:
    R={}; M={}
    for st,rk,mv in lines: s=parse(st); assert s not in R; R[s]=int(rk); M[s]=mv
    assert () in R, "empty state missing"
    for s in R:
        assert s==canon(s); mv=M[s]
        if mv=='T': assert maxload(s)>=L, "bad terminal "+str(s); continue
        assert maxload(s)<L or True
        if mv[0]=='E':
            j=int(mv[1:]); assert j<len(s) and (mode==2 or (mode==0 and j==0)), "illegal removal"
            succ=[s[:j]+s[j+1:]]
        else:
            u,v=int(mv[1]),int(mv[2]); c=comp(s)
            assert 1<=u<=n and 1<=v<=n and u!=v and c[u]!=c[v], "illegal offer in "+str(s)
            succ=[s+((u,v),), s+((v,u),)]
        for t in succ:
            t=canon(t); assert t in R, "successor missing: %s -> %s"%(s,t)
            assert R[t]<R[s], "rank not decreasing: %s -> %s"%(s,t)
    print("OK: Adversary strategy of %d states verified: forces load %d on %d bins (mode %d)"%(len(R),L,n,mode))
