# Referee: closed-set conditions, written from the two proofs' statements.
import sys
def split_ok(S):
    s=set(S); return all(u==1 or any((u-p) in s for p in s if p<u) for u in s) and (1 in s or not any(u>=2 for u in s))
def net_case(s,u,v):
    z=u-v
    if z in s: return 1
    if any((z-p) in s for p in s if p<z): return 2
    if any((u-c) in s and (v-c) in s for c in range(1,v)): return 3
    return 0
def closedB(S):   # solver B: (C) and (S) with U=1
    s=set(S)
    if not all(u==1 or any((u-p) in s for p in s if p<u) for u in s): return False
    return all(net_case(s,u,v) in (1,2) for u in s for v in s if u>v)
def closedA(S):   # solver A: split, net with third option
    s=set(S)
    if not all(u==1 or any((u-p) in s for p in s if p<u) for u in s): return False
    return all(net_case(s,u,v)!=0 for u in s for v in s if u>v)
def quad(M): return sorted(set(range(1,M+1))|{c*M for c in range(1,M+1)})
def cubic(M): return sorted(set(range(1,M+1))|{c*M for c in range(1,M+1)}|{M*M-j for j in range(0,M+1)}|{c*M*M for c in range(1,M+1)})
def Kmax(m,cond3):
    best=[0,None]
    def ext(S):
        s=set(S)
        if len(S)==m:
            if S[-1]>best[0]: best[0]=S[-1]; best[1]=list(S)
            return
        top=S[-1]
        # crude bound: each later element at most doubles
        if top*(1<<(m-len(S)))<=best[0]: return
        for t in range(2*top,top,-1):
            if not any((t-p) in s for p in S): continue
            s2=s|{t}; ok=True
            for v in S:
                c=net_case(s2,t,v)
                if c==0 or (c==3 and not cond3): ok=False;break
            if ok: ext(S+[t])
    ext([1]); return best
if __name__=="__main__":
    print("cubic family of solver A, Corollary 1b: M, |S|, K, closed under B's (C)+(S), closed under A's def, formula |S|=4M-3")
    for M in range(2,13):
        S=cubic(M); print(M,len(S),S[-1],closedB(S),closedA(S),len(S)==4*M-3, "quad same size K=%d"%(((len(S)+1)//2)**2))
    print("quadratic family: ",[(M,closedB(quad(M))) for M in range(2,13)])
    print("deliberately broken: cubic(4) minus 13:",closedB([x for x in cubic(4) if x!=13]),closedA([x for x in cubic(4) if x!=13])," {1,2,4,8}:",closedB([1,2,4,8]),closedA([1,2,4,8]))
    for name,W in [("B|W|=10 proofs",[1,2,3,4,7,11,15,17,18,36]),("B s22",[1,2,4,6,7,8,16,24,32,40]),("B 8",[1,2,3,6,9,11,12,24]),("B 9",[1,2,4,5,10,12,15,16,32]),("B 7",[1,2,3,6,8,9,18]),("B 6",[1,2,4,6,7,14]),
                   ("A 10",[1,2,4,5,10,12,17,20,21,42]),("A 11",[1,2,3,6,9,11,12,24,26,38,50]),("A 12",[1,2,3,6,9,12,14,15,30,32,47,62]),("A 13",[1,2,3,6,9,12,13,14,15,30,45,60,75])]:
        print(name,W,"closedB",closedB(W),"closedA",closedA(W))
    mm=int(sys.argv[1]) if len(sys.argv)>1 else 9
    for m in range(1,mm+1):
        print("m=%d  Kmax under B-cond: %s   under A-cond: %s"%(m,Kmax(m,False),Kmax(m,True)),flush=True)
