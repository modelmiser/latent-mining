# Referee generator of the one-weightless-state weight protocol (solver A, Theorem 1') from a set S, and
# a local certificate check of the properties the proof uses.  usage: gen.py out.txt s1 s2 ...
import sys
from closed import net_case
def build(S):
    S=sorted(S); K=S[-1]; s=set(S)
    order=[K]+[u for u in S if u!=K]
    st={}; names=[]
    for u in order:
        st[u]=len(names); names.append(u); st[-u]=len(names); names.append(-u)
    n=len(names); names.append(0)
    rules={}
    def tok(x): return n if x==0 else st[x]
    def setr(p,q,r,t): rules[(p,q)]=(r,t); rules[(q,p)]=(t,r)
    for u in S:
        for v in S:
            if u==v: setr(st[u],st[-v],n,n); continue
            big,small=max(u,v),min(u,v); sg=1 if u>v else -1   # sign of the survivor
            c=net_case(s,big,small); z=big-small
            if c==1: a,b=z,0
            elif c==2:
                p=min(p for p in s if p<z and (z-p) in s); a,b=p,z-p
            else: raise SystemExit("not closed (sum of <=2 needed) at %d,%d"%(big,small))
            setr(st[u],st[-v],tok(sg*a),tok(sg*b))
    for u in S:
        if u>=2:
            p=min(p for p in s if p<u and (u-p) in s); setr(st[-u],n,st[-p],st[-(u-p)])
    w=''.join('A' if x>=0 else 'B' for x in names)
    return names,w,rules
def cert(names,w,rules):
    # properties used by the proof: (a) weight conserved (b) (sum|w|, #weightless) drops lexicographically
    # (c) every (+,-) pair legal both orders (d) (-u,n) legal for u>=2 both orders (e) outputs by sign, n -> A
    n=len(names)
    for (p,q),(r,t) in rules.items():
        assert (r,t)!=(p,q)
        assert names[p]+names[q]==names[r]+names[t],("conservation",p,q)
        a0=abs(names[p])+abs(names[q]); a1=abs(names[r])+abs(names[t])
        z0=(names[p]==0)+(names[q]==0); z1=(names[r]==0)+(names[t]==0)
        assert (a1,z1)<(a0,z0),("decrease",p,q)
    for p in range(n):
        for q in range(n):
            if names[p]*names[q]<0: assert (p,q) in rules,("pm legal",p,q)
            if names[p]<=-2 and names[q]==0: assert (p,q) in rules and (q,p) in rules,("split legal",p)
    assert all((w[i]=='A')==(names[i]>=0) for i in range(n))
    assert names[0]==max(names) and names[1]==-names[0]
    return True
if __name__=="__main__":
    S=list(map(int,sys.argv[2:])); names,w,rules=build(S)
    print("states",len(names),"K",max(S),"certificate",cert(names,w,rules))
    with open(sys.argv[1],'w') as f:
        f.write("# one-weightless protocol, S=%s names=%s\n%d 0 1\n%s\n"%(S,names,len(names),w))
        for (p,q),(r,t) in sorted(rules.items()): f.write("%d %d %d %d\n"%(p,q,r,t))
