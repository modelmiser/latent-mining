#!/usr/bin/env python3
# Slow direct reference: pebbles keep identities (no interchangeability trick), state = tuple of (pos,charge) per pebble, pos 0 = hand.
# usage: slow.py N c1,c2,...   (N = position cap; result exact iff maxocc < N)
import sys
from collections import deque
N=int(sys.argv[1]); ch=tuple(int(x) for x in sys.argv[2].split(','))
start=tuple((0,c) for c in ch)
seen={start}; dq=deque([start]); best=0; maxocc=0
while dq:
    s=dq.popleft()
    occ={pos for pos,c in s if pos}
    if occ:
        maxocc=max(maxocc,max(occ))
        if len(occ)==1: best=max(best,max(occ))
    nxt=[]
    for j,(pos,c) in enumerate(s):
        if pos==0:
            for i in range(1,N+1):
                if i not in occ and (i==1 or (i-1) in occ):
                    nxt.append(s[:j]+((i,c),)+s[j+1:])
        else:
            if pos==1 or (pos-1) in occ: nxt.append(s[:j]+((0,c),)+s[j+1:])
            elif pos>=2 and c>=1: nxt.append(s[:j]+((0,c-1),)+s[j+1:])
    for t in nxt:
        if t not in seen: seen.add(t); dq.append(t)
print("charges=%s N=%d C=%d maxocc=%d states=%d %s"%(sys.argv[2],N,best,maxocc,len(seen),"EXACT" if maxocc<N else "CAP-BINDING"))
