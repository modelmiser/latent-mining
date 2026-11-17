# Independent cross-check (different representation + algorithm: least fixpoint of
# adversary-winning set over layouts as tuples of (start,size)). usage: check.py G M H
import sys, itertools
G, M, H = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
def layouts(pos=0, live=0):
    if pos >= H: yield (); return
    for rest in layouts(pos+1, live): yield rest
    for s in (1,2,3):
        if pos+s <= H and live+s <= M:
            for rest in layouts(pos+s, live+s): yield ((pos,s),)+rest
ALL = set(layouts())
def occ(L):
    o=set()
    for (p,s) in L: o.update(range(p,p+s))
    return o
def places(L, s):
    o=occ(L)
    return [tuple(sorted(L+((i,s),))) for i in range(H-s+1) if not any(c in o for c in range(i,i+s))]
def moves(L, budget):
    res={L}
    if budget<=0: return res
    for b in L:
        if b[1]<=budget and b[1]<=2:
            rest=tuple(x for x in L if x!=b)
            for L2 in places(rest,b[1]): res |= moves(L2, budget-b[1])
    return res
lose=set()   # adversary-winning positions (adversary to move)
changed=True
while changed:
    changed=False
    for L in ALL:
        if L in lose: continue
        live=sum(s for _,s in L); bad=False
        for s in (1,2,3):
            if live+s<=M and all(L2 in lose for L2 in places(L,s)): bad=True
        if not bad:
            for b in L:
                if G=='O' and b[1]!=1: continue
                rest=tuple(x for x in L if x!=b)
                opts = moves(rest,b[1]-1) if G=='F' else {rest}
                if all(L2 in lose for L2 in opts): bad=True; break
        if bad: lose.add(L); changed=True
print(G,M,H,"states",len(ALL),"ADVERSARY_WINS" if () in lose else "MANAGER_WINS")
