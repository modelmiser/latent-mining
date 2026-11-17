#!/usr/bin/env python3
"""Independent lower-bound checker: plays a strategy file against EVERY arrival order by the literal rules.
usage: check_strategy.py k n strategy_file [lostfile]
The Sorter side only uses: counts, its own record of which items were refused since the last output, and r.
Prints number of orders beaten."""
import sys
from itertools import permutations
k=int(sys.argv[1]); n=int(sys.argv[2])
tab={}
for line in open(sys.argv[3]):
    r,st,mv=[x.strip() for x in line.split('|')]
    tab[(int(r),st)]=mv
def play(order):
    stacks=[[] for _ in range(k)]   # each entry [value, flag]
    nxt=0; mn=1; steps=0
    while mn<=n:
        key=(n-nxt,' ; '.join(''.join(str(f) for v,f in s) if s else '-' for s in stacks))
        mv=tab.get(key)
        if mv is None: return False
        steps+=1
        if steps>10*n*k+100: return False
        if mv=='IN':
            if len(stacks[0])>=2 or nxt>=n: return False   # refused -> strategy would repeat forever
            stacks[0].append([order[nxt],0]); nxt+=1
        elif mv.startswith('SHIFT'):
            i=int(mv[5:])-1
            if not stacks[i] or len(stacks[i+1])>=2: return False
            stacks[i+1].append(stacks[i].pop())
        else:
            i=int(mv[3:])-1
            if not stacks[i]: return False
            if stacks[i][-1][0]==mn:
                stacks[i].pop(); mn+=1
                for s in stacks:
                    for it in s: it[1]=0
            else:
                if stacks[i][-1][1]==1: return False  # repeated refused attempt: infinite loop
                stacks[i][-1][1]=1
    return True
won=0; lost=[]
for p in permutations(range(1,n+1)):
    if play(p): won+=1
    else: lost.append(p)
print('k=%d n=%d beaten=%d of %d'%(k,n,won,len(lost)+won))
if len(sys.argv)>4:
    open(sys.argv[4],'w').write('\n'.join(' '.join(map(str,p)) for p in lost)+'\n')
