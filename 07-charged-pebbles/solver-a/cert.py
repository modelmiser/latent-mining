#!/usr/bin/env python3
# Closed-set certificates for the upper bound.
#   cert.py dump p k file   : BFS (canonical states, no position cap needed: placements are only at node 1 or top-adjacent) and dump reachable set
#   cert.py check p k file  : independently verify: start state in set, set closed under ALL legal moves, report max clean n in set.
# line format: "pos:charge,pos:charge|h1,h2"  (board sorted by pos, hand charges sorted)
import sys
def succ(s):
    board,hand=s; occ={q for q,_ in board}; out=[]
    for c in set(hand):
        h=list(hand); h.remove(c)
        for i in {1}|{q+1 for q in occ}:
            if i not in occ: out.append((tuple(sorted(board+((i,c),))),tuple(h)))
    for j,(q,c) in enumerate(board):
        rest=board[:j]+board[j+1:]
        if q==1 or q-1 in occ: out.append((rest,tuple(sorted(hand+(c,)))))
        elif c>=1: out.append((rest,tuple(sorted(hand+(c-1,)))))
    return out
def fmt(s): return ",".join("%d:%d"%x for x in s[0])+"|"+",".join(map(str,s[1]))
def parse(l):
    b,h=l.strip().split("|")
    return (tuple(tuple(map(int,x.split(":"))) for x in b.split(",") if x),tuple(int(x) for x in h.split(",") if x))
mode,p,k,fn=sys.argv[1],int(sys.argv[2]),int(sys.argv[3]),sys.argv[4]
start=((),(k,)*p)
if mode=="dump":
    seen={start}; st=[start]
    while st:
        s=st.pop()
        for t in succ(s):
            if t not in seen: seen.add(t); st.append(t)
    open(fn,"w").write("\n".join(sorted(fmt(s) for s in seen))+"\n"); print("dumped",len(seen))
else:
    S={parse(l) for l in open(fn)}
    if start not in S: sys.exit("REJECT: start missing")
    best=0
    for s in S:
        for t in succ(s):
            if t not in S: sys.exit("REJECT: not closed: %s -> %s"%(fmt(s),fmt(t)))
        if len(s[0])==1: best=max(best,s[0][0][0])
    print("CERT OK p=%d k=%d: %d states closed; max clean n = %d ; formula %d"%(p,k,len(S),best,(p*k+2)*2**(p-2)))
