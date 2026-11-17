#!/usr/bin/env python3
# explore blind-reachable states on T_h from all full starts (and optionally U-start); print clean sets
import sys
h=int(sys.argv[1]); mode=sys.argv[2] if len(sys.argv)>2 else 'full'
n=2**(h+1)-1
nb={v:[u for u in (v//2,2*v,2*v+1) if 1<=u<=n] for v in range(1,n+1)}
depth={v:v.bit_length()-1 for v in range(1,n+1)}
def expand(S,c):
    R=set(S)
    for v in S: R.update(nb[v])
    R.discard(c); return frozenset(R)
V=frozenset(range(1,n+1))
if mode=='full': starts=[(c,V-{c}) for c in range(1,n+1)]
else: starts=[(1,frozenset(v for v in V if depth[v] in (1,2)))]
seen=set(starts); st=list(starts)
while st:
    c,S=st.pop()
    for c2 in [c]+nb[c]:
        S1=S-{c2}
        t=(c2,expand(S1,c2))
        if t not in seen: seen.add(t); st.append(t)
print("h",h,mode,"reachable blind states",len(seen), "min |S|",min(len(S) for c,S in seen))
# clean sets by height of vertices
from collections import Counter
cnt=Counter()
worst=0
for c,S in seen:
    clean=V-S-{c}
    hs=tuple(sorted(h-depth[v] for v in clean))
    cnt[(h-depth[c],hs)]+=1
for k,v in sorted(cnt.items()): print(" cop height",k[0],"clean heights",k[1],"x",v)
