# G4 inner count, redone with the full singleton condition w[-1]!=w[0]!=w[1] for d in {0,1} (the proof uses w[0]!=w[1] at d=0)
import itertools
from collections import Counter
st=Counter()
for k in range(2,9):
    for d in range(-(k-2),k):
        for p in range(-(k-1),0):
            u=p+d
            if not (-(k-1)<=u<=0): continue
            lo=min(p,u,-1); hi=max(p+k,u+k-1,1); cnt=cntL=0
            for bits in itertools.product((0,1),repeat=hi-lo+1):
                g=lambda c: bits[c-lo]
                if [g(c) for c in range(p,p+k+1) if c!=0]==[g(c) for c in range(u,u+k)]:
                    if g(-1)!=g(0)!=g(1): cnt+=1
                    if g(-1)!=g(0): cntL+=1
            if d in (0,1): st['d in {0,1}: pairs']+=1; st['d in {0,1}: solutions']+=cnt
            if d>=2:
                st['d>=2 pairs']+=1; st['d>=2 count > 2^(d-1)']+= cntL>2**(d-1); st['d>=2 count == 2^(d-1)']+= cntL==2**(d-1)
            if d<=-1:
                st['d<=-1 pairs']+=1; dd=1-d   # mirror: condition w[0]!=w[1]
                c2=0
                for bits in itertools.product((0,1),repeat=hi-lo+1):
                    g=lambda c: bits[c-lo]
                    if [g(c) for c in range(p,p+k+1) if c!=0]==[g(c) for c in range(u,u+k)] and g(0)!=g(1): c2+=1
                st['d<=-1 count > 2^(d\'-1)']+= c2>2**(dd-1)
print(dict(st))
