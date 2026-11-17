# which periodic 1-patterns P have max-thinning waste density <= 2/3 per 1 ?  (O-safe stage-1 layouts)
import itertools, sys
from core import *
from fractions import Fraction
res={}
for p in range(1,13):
    for bits in itertools.product([0,1],repeat=p):
        if bits[0]==0 or sum(bits)==0: continue
        reps=3*12   # make length multiple of 3 periods
        cells=[i for i in range(p*reps) if bits[i%p]]
        lay=tuple((c,1) for c in cells)
        W=phi(lay)
        n=len(cells)
        d=Fraction(W,n)
        if d<=Fraction(2,3)+Fraction(3,n):
            s=''.join('1' if b else '.' for b in bits)
            # canonical: skip if pattern is repetition of shorter
            if any(p%q==0 and s==s[:q]*(p//q) for q in range(1,p)): continue
            print(p,s,'ones/period',sum(bits),'W/1s=%.4f'%float(d),'span/M=%.3f'%(p/sum(bits)))
