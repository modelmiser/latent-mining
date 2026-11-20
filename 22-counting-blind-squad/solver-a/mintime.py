# min firing time over all rules synchronising a single given n (lazy DFS, exhaustive)
import sys
from check import names, run
def mint(s,n):
    S=names(s); d={('Q',0):'Q'}; best=[None,None]
    def rec():
        try: t=run(d,n)
        except KeyError as e:
            key=e.args[0]
            for v in S:
                d[key]=v; rec()
            del d[key]; return
        if t>0 and (best[0] is None or t<best[0]): best[0]=t; best[1]=dict(d)
    rec(); return best
for s in (3,4,5):
    for n in range(2,8):
        b=mint(s,n); print('s=%d n=%d min single-n firing time %s (n-1=%d, 2n-2=%d) %s'%(s,n,b[0],n-1,2*n-2,sorted(b[1].items()) if b[1] and n==4 and s==4 else ''))
