# test whether W_L = {positive integers <= 2^L with at most 2 nonzero signed binary digits} satisfies
# (C) or the mixed-progress variant (C'): exists x'<x in W with x'-z in W  (z=x-y).
import sys
def build(L,t=2):
    W={0}
    for _ in range(t):
        W={w+s*(1<<a) for w in W for a in range(L+1) for s in (1,-1)}|W
    return sorted(w for w in W if 1<=w<=(1<<L))
for L in range(2,11):
    W=build(L); S=set(W); two=S|{a+b for a in W for b in W}
    badC=0; badM=0; ex=None
    for x in W:
        for y in W:
            if y>=x: continue
            z=x-y
            if z in two: continue
            badC+=1
            if not any((xp-z) in S for xp in W if z<xp<x):
                badM+=1; ex=ex or (x,y)
    print('L=%d K=%d |W|=%d  pairs failing (C): %d  failing (C) and mixed: %d %s'%(L,1<<L,len(W),badC,badM,ex))
