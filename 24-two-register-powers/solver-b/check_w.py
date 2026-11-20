#!/usr/bin/env python3
# Independent checker for upper-bound witnesses. usage: check_w.py witnessfile
# Lines "W m r g path": replays path on INTEGERS from (1,1); requires len(path)==g and a register ≡ r (mod m).
# Lines "D m d ...": requires d == max g over the m residues and all m residues present.
import sys
W={};D={}
for l in open(sys.argv[1]):
    p=l.split()
    if p[0]=="D": D[int(p[1])]=int(p[2])
    elif p[0]=="W":
        m,r,g=int(p[1]),int(p[2]),int(p[3]); path="" if p[4]=="-" else p[4]
        a=b=1
        for ch in path:
            if ch=="A": a=2*a
            elif ch=="B": b=2*b
            elif ch=="C": a=a+b
            elif ch=="D": b=a+b
            else: sys.exit("bad move")
        if len(path)!=g or (a%m!=r and b%m!=r): sys.exit("REJECT m=%d r=%d"%(m,r))
        W.setdefault(m,{})[r]=g
for m,d in D.items():
    if sorted(W[m])!=list(range(m)) or max(W[m].values())!=d: sys.exit("REJECT table m=%d"%m)
print("ACCEPT: D(m) <=",len(D),"claimed values, m in",min(D),"..",max(D))
