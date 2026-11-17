# fix.py K NMAX -- exact game value (including INFINITY) on the full, uncapped obligation-multiset
# state space, by least-fixed-point iteration. Independent of fast.cpp (no horizon cap, no pruning).
# State: sorted tuple of remaining obligations of alive candidates. Substates of the root reachable only.
import sys, itertools
K=int(sys.argv[1]); NMAX=int(sys.argv[2])
def moves(st):
    # distinct questions: for each obligation value choose how many go in S
    vals=sorted(set(st)); cnt=[st.count(v) for v in vals]; tot=len(st)
    res=[]
    for a in itertools.product(*[range(c+1) for c in cnt]):
        sa=sum(a); ell=min(sa,tot-sa)
        Y=[];N=[]
        for v,c,x in zip(vals,cnt,a):
            Y+= [max(v-1,0)]*x; N+=[max(v-1,0)]*(c-x)
            if v==0: Y+=[K*ell]*(c-x); N+=[K*ell]*x
        res.append((tuple(sorted(Y)),tuple(sorted(N))))
    return res
for n in range(2,NMAX+1):
    root=(0,)*n
    graph={}; stack=[root]
    while stack:
        s=stack.pop()
        if s in graph: continue
        if len(s)==1: graph[s]=[]; continue
        graph[s]=moves(s)
        for Y,N in graph[s]:
            for t in (Y,N):
                if len(t)>0 and t not in graph: stack.append(t)
    val={s:0 for s in graph if len(s)==1}
    q=0
    while True:
        q+=1; new=[]
        for s,mv in graph.items():
            if s in val: continue
            for Y,N in mv:
                if (len(Y)==0 or Y in val) and (len(N)==0 or N in val): new.append(s); break
        if not new: break
        for s in new: val[s]=q
    print("k=%d n=%d Q=%s states=%d"%(K,n,val.get(root,"INF"),len(graph)),flush=True)
    if root not in val and len(sys.argv)>3:
        # dump the trap set: states never winnable, reachable from root while staying non-winnable
        with open("%s_k%d_n%d.txt"%(sys.argv[3],K,n),"w") as f:
            f.write("k %d n %d\n"%(K,n))
            seen=set(); st=[root]
            while st:
                s=st.pop()
                if s in seen: continue
                seen.add(s)
                for Y,N in graph[s]:
                    for t in (Y,N):
                        if len(t)>0 and t not in val and t not in seen: st.append(t)
            for s in sorted(seen): f.write(" ".join(map(str,s))+"\n")
