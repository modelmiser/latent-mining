#!/usr/bin/env python3
# Writes the recursive witness M_r (cycle length W_r = W_{r-1}(W_{r-1}+1), W_0=1) as sylvester_r<r>.txt
import sys
def build(r,base):
    """returns (nodes dict path->(kind,val), path of free leaf, number of indices used) using indices base+1.."""
    if r==0: return {'':('L',0)},'',0
    a=base+1
    n0,f0,c0=build(r-1,base+1)
    n1,f1,c1=build(r-1,base+1+c0)
    nodes={'':('I',a)}
    for p,v in n0.items(): nodes['0'+p]=v
    nodes['0'+f0]=('L',a)            # inner free leaf sets the flag
    for p,v in n1.items(): nodes['1'+p]=v
    return nodes,'1'+f1,1+c0+c1
for r in range(1,int(sys.argv[1])+1):
    nodes,_,c=build(r,0); W=1
    for _ in range(r): W=W*(W+1)
    with open('sylvester_r%d.txt'%r,'w') as f:
        f.write('r %d\nN %d\nstart %s\n'%(r,W,' '.join('0'*c)))
        for p in sorted(nodes,key=lambda q:(len(q),q)): f.write('%s %s %d\n'%(p or '-',nodes[p][0],nodes[p][1]))
