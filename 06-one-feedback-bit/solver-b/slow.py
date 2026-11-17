# Slow, obvious exact search: maximum family of pairwise disjoint outcome sets.
# Plain clique-style backtracking over ALL strategies with full outcome sets.
# Only pruning: |current| + |remaining candidates| <= best  (trivially safe).
import sys
from common import *
def F_slow(n,t):
    S=[(s,frozenset(outcome(*s))) for s in strategies(n,t)]
    best=[0,None]
    def rec(chosen,cands):
        if len(chosen)>best[0]:
            best[0]=len(chosen); best[1]=list(chosen)
        for i,(s,o) in enumerate(cands):
            if len(chosen)+len(cands)-i<=best[0]: return
            rest=[c for c in cands[i+1:] if not (c[1]&o)]
            chosen.append(s); rec(chosen,rest); chosen.pop()
    rec([],S)
    return best
if __name__=="__main__":
    N=int(sys.argv[1])
    for n in range(2,N+1):
        print(n,[F_slow(n,t)[0] for t in range(n+1)],flush=True)
