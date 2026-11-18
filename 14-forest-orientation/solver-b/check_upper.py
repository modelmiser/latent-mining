#!/usr/bin/env python3
"""check_upper.py FILE n b
FILE lists positions with claimed bounds 'b | trees'. Checks (1) the start position (n singletons) is listed with bound <= b,
(2) closure: for every listed position (bound beta) and EVERY legal Adversary pair {u,v} there is a legal Algorithm reply
(a normal reply, which is a legal reply) of cost c <= beta leading to a single tree or to a listed position with bound <= beta-c.
This proves T(n) <= b with no lemma. Uses the tree text / canonical form of check_cert.py (own code, orientations explicit)."""
import sys
sys.argv_saved=sys.argv; 
import importlib.util
src=open(__file__.replace('check_upper.py','check_cert.py')).read().replace("\nmain()\n","\n")
ns={}; exec(compile(src,'check_cert_lib','exec'),ns)
graph,enc,canon=ns['graph'],ns['enc'],ns['canon']
def all_replies(sA,sB):
    """for every pair (u in A, v in B): list of (cost, merged canonical tree)"""
    nA,EA=graph(sA,0); nB,EB=graph(sB,nA); n=nA+nB; E=EA|EB
    out={v:0 for v in range(n)}; outn={v:[] for v in range(n)}; adj0={v:set() for v in range(n)}
    for a,b in E: out[a]+=1; outn[a].append(b); adj0[a].add(b); adj0[b].add(a)
    def paths(x):
        if out[x]<=1: return [[x]]
        res=[]; st=[[x]]
        while st:
            p=st.pop()
            for w in outn[p[-1]]:
                (res if out[w]<=1 else st).append(p+[w])
        return res
    P={x:paths(x) for x in range(n)}
    allr=[]; seen={}
    for u in range(nA):
        for v in range(nA,n):
            key=(enc(adj0,out,u,-1),enc(adj0,out,v,-1))
            if key in seen: continue          # same orbit pair => same replies (only a speed-up)
            seen[key]=1; rs=[]
            for x,y in ((u,v),(v,u)):
                for p in P[x]:
                    E2=set(E)
                    for a,b in zip(p,p[1:]): E2.remove((a,b)); E2.add((b,a))
                    E2.add((x,y)); adj={w:set() for w in range(n)}; o2={w:0 for w in range(n)}
                    for a,b in E2: adj[a].add(b); adj[b].add(a); o2[a]+=1
                    assert max(o2.values())<=2
                    rs.append((len(p)-1,canon(list(range(n)),adj,o2)))
            allr.append(rs)
    return allr
def main():
    fn,n,b=sys.argv[1],int(sys.argv[2]),int(sys.argv[3]); S={}
    for line in open(fn):
        if line.startswith('#') or not line.strip(): continue
        a,t=line.split('|'); S[tuple(sorted(t.split()))]=int(a)
    start=tuple(["(2)"]*n)
    if S.get(start,10**9)>b: print("FAILED: start not listed with bound <=",b); sys.exit(1)
    cache={}; bad=0
    for state,beta in S.items():
        if len(state)<2: continue
        ds=sorted(set(state))
        for i,sA in enumerate(ds):
            for sB in ds[i:]:
                if sA==sB and state.count(sA)<2: continue
                rest=list(state); rest.remove(sA); rest.remove(sB)
                if (sA,sB) not in cache: cache[(sA,sB)]=all_replies(sA,sB)
                for rs in cache[(sA,sB)]:
                    ok=False
                    for c,t in rs:
                        if c>beta: continue
                        if not rest: ok=True; break
                        if S.get(tuple(sorted(rest+[t])),10**9)<=beta-c: ok=True; break
                    if not ok: bad+=1
    print("n=%d b=%d : %s (positions=%d, violations=%d)"%(n,b,"VERIFIED T(n) <= b" if bad==0 else "FAILED",len(S),bad))
    sys.exit(0 if bad==0 else 1)
main()
