#!/usr/bin/env python3
"""Dump witness files for (k, n): strategy_k{k}_n{n}.txt and cert_k{k}_n{n}.txt
strategy line:  r | s1 ; s2 ; ... | MOVE     (stack = flags bottom->top, '-' = empty; flag 1 = refused since last output)
cert line:      r | s1 ; ... | VALUE         (claimed optimal count from that information state)
Only states reachable under SOME play are listed in cert; strategy lists states reachable under the strategy."""
import sys
from fast import V, moves, best_move
k=int(sys.argv[1]); n=int(sys.argv[2])
def fmt(st): return ' ; '.join(''.join(map(str,s)) if s else '-' for s in st)
start=(tuple(() for _ in range(k)), n)
# strategy-reachable
seen={start}; stack=[start]; lines=[]
while stack:
    st,r=stack.pop()
    if r==0 and not any(st): continue
    bm=best_move(st,r)
    if bm is None or V(st,r)==0: continue
    lines.append('%d | %s | %s'%(r,fmt(st),bm))
    for nm,succ in moves(st,r):
        if nm==bm:
            for s in succ:
                if s not in seen: seen.add(s); stack.append(s)
open('strategy_k%d_n%d.txt'%(k,n),'w').write('\n'.join(sorted(lines))+'\n')
seen={start}; stack=[start]; lines=[]
while stack:
    st,r=stack.pop()
    lines.append('%d | %s | %d'%(r,fmt(st),V(st,r)))
    for nm,succ in moves(st,r):
        for s in succ:
            if s not in seen: seen.add(s); stack.append(s)
open('cert_k%d_n%d.txt'%(k,n),'w').write('\n'.join(sorted(lines))+'\n')
print(k,n,V(*start),len(lines))
