# definition-level reference: explicit agents, ordered pairs of distinct agents; graph on multisets.
import sys
def load(fn):
    L=[l.split('#')[0].strip() for l in open(fn)]; L=[l for l in L if l]
    s,qA,qB=map(int,L[0].split()); w=L[1]
    d={(p,q):(p,q) for p in range(s) for q in range(s)}
    for l in L[2:]:
        p,q,r,t=map(int,l.split()); d[(p,q)]=(r,t)
    return s,qA,qB,w,d
def wins(s,qA,qB,w,d,a,b,x):
    maj='A' if a>b else 'B'
    init=tuple(sorted([qA]*a+[qB]*b+[x]))
    E={}; todo=[init]
    while todo:
        c=todo.pop()
        if c in E: continue
        out=set()
        for i in range(len(c)):
            for j in range(len(c)):
                if i!=j and d[(c[i],c[j])]!=(c[i],c[j]):
                    l=list(c); l[i],l[j]=d[(c[i],c[j])]; out.add(tuple(sorted(l)))
        E[c]=out; todo.extend(out)
    for c in E:
        if all(w[p]==maj for p in c): continue
        if not E[c]: return False
        # on a cycle? reachable from itself in >=1 step
        seen=set(); st=list(E[c])
        while st:
            u=st.pop()
            if u==c: return False
            if u in seen: continue
            seen.add(u); st.extend(E[u])
    return True
def thresh(P,N):
    s=P[0]; best=1
    for n in range(2,N+1):
        for a in range(n+1):
            b=n-a
            if abs(a-b)<2: continue
            for x in range(s):
                if not wins(*P,a,b,x): return best,(a,b,x)
        best=n
    return best,None
if __name__=="__main__":
    print(thresh(load(sys.argv[1]),int(sys.argv[2])))
