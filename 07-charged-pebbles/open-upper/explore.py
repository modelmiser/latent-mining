import sys
from collections import deque
def costs(q,N,fmax):
    # 0-1 BFS over configs (sorted tuples) with nodes<=N
    start=()
    dist={start:0}
    layer=[start]
    f=0
    while layer and f<=fmax:
        dq=deque(layer); nxt=[]
        while dq:
            S=dq.popleft()
            if dist[S]!=f: continue
            s=set(S)
            # P
            if len(S)<q:
                for i in ([1] if 1 not in s else [])+[x+1 for x in S if x+1 not in s and x+1<=N]:
                    T=tuple(sorted(S+(i,)))
                    if T not in dist or dist[T]>f:
                        dist[T]=f; dq.append(T)
            for x in S:
                T=tuple(y for y in S if y!=x)
                if x==1 or (x-1) in s:
                    if T not in dist or dist[T]>f:
                        dist[T]=f; dq.append(T)
                else:
                    if T not in dist:
                        dist[T]=f+1; nxt.append(T)
        layer=[T for T in nxt if dist[T]==f+1]
        f+=1
    return dist
def reduce(S,q):
    S=list(S)
    ch=True
    while ch:
        ch=False
        r=q-len(S)
        for i,x in enumerate(S):
            prev=S[i-1] if i else 0
            if x-prev<=2**r:
                del S[i]; ch=True; break
    return tuple(S)
if __name__=="__main__":
    q=int(sys.argv[1]);N=int(sys.argv[2]);fm=int(sys.argv[3])
    d=costs(q,N,fm)
    anc={}
    bad=0
    for S,c in d.items():
        A=reduce(S,q)
        if A in anc and anc[A]!=c: bad+=1
        anc[A]=min(anc.get(A,99),c)
    print("configs",len(d),"anchorsets",len(anc),"inconsistent",bad)
    for A in sorted(anc,key=lambda A:(len(A),A)):
        print(anc[A],A)
