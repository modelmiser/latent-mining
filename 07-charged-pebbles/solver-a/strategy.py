#!/usr/bin/env python3
# Generates the general lower-bound strategy for any p>=2,k>=0: clean at (pk+2)*2^(p-2). usage: strategy.py p k > file
import sys
p,k=int(sys.argv[1]),int(sys.argv[2])
out=[]
ch=[k]*p
def rev(base,q,pebs,undo=False):
    # reversible moves taking {ground=base} to clean at base+2^(q-1) using pebble ids pebs (len q); pebs[0] ends on top.
    # returns move list of (op,node,pebble)
    if q==1: return [('P',base+1,pebs[0])]
    h=1<<(q-2)
    a=rev(base,q-1,pebs[1:])            # pebs[1] lands at base+h
    b=rev(base+h,q-1,[pebs[0]]+pebs[2:]) # pebs[0] lands at base+2h
    return a+b+inv(a)
def inv(m): return [(('R' if op=='P' else 'P'),n,x) for op,n,x in reversed(m)]
moves=rev(0,p,list(range(p)))  # pebble 0 at 2^(p-1)
m=1<<(p-1); base=0
for j in range(p*k):
    top=(base+1)%p
    others=[top]+[x for x in range(p) if x!=top and x!=base]
    moves+=rev(m,p-1,others)
    moves.append(('F',m,base))
    m+=1<<(p-2); base=top
print("# p=%d k=%d general strategy, clean at n=%d ; %d moves"%(p,k,m,len(moves)))
for op,n,x in moves:
    if op=='P': print("P %d %d"%(n,ch[x]))
    elif op=='R': print("R %d"%n)
    else: print("F %d"%n); ch[x]-=1
