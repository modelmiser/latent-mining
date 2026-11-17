#!/usr/bin/env python3
"""Independent checker (uses only slow.py's definition-level code).
witnesses.txt lines:  n  word  YES|NO     (YES = claimed repairable, NO = claimed not repairable)
For YES lines it also prints, for every adversary move, one mechanic answer (strategy), to strategy_n<n>.txt"""
import sys
from slow import universal, repairable, edits
def moves(v):
    m=len(v); out=[('pass',-1,tuple(v))]
    if m>=2:
        for k in range(m): out.append(('loss',k,tuple(v[:k]+v[k+1:])))
    for k in range(m): out.append(('double',k,tuple(v[:k+1]+v[k:])))
    return out
ok=True
for line in open(sys.argv[1] if len(sys.argv)>1 else 'witnesses.txt'):
    line=line.split('#')[0].split()
    if len(line)<3: continue
    n=int(line[0]); v=tuple(int(c) for c in line[1]); exp=line[2]=='YES'
    r=repairable(v,n)
    print(n,line[1],'len',len(v),'universal' if universal(v,n) else 'not-universal','repairable' if r else 'NOT repairable','OK' if r==exp else 'MISMATCH')
    ok&= (r==exp)
    if r and exp:
        with open(f'strategy_n{n}_L{len(v)}.txt','w') as f:
            f.write(f'# n={n} w={line[1]}\n# adversary move -> mechanic move (cells 0-indexed in the loop they act on) -> w\'\'\n')
            for (a,k,w1) in moves(v):
                for (b,j,w2) in moves(w1):
                    if universal(w2,n):
                        f.write(f'{a} {k} -> {b} {j} -> {"".join(map(str,w2))}\n'); break
sys.exit(0 if ok else 1)
