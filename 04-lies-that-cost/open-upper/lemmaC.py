# Literal test of Lemma C (majority halving).  Candidates are labelled, each with its own obligation.
# For every multiset of initial obligations in 0..R on m candidates, EVERY choice of S with |S|=floor(m/2)
# at every halving step, and every legal responder, the questioner must be down to one alive candidate
# within 3*ceil(log2 m) questions.  usage: lemmaC.py K MMAX R
import sys, itertools, math
from functools import lru_cache
K=int(sys.argv[1]); MMAX=int(sys.argv[2]); R=int(sys.argv[3])
INF=10**9
@lru_cache(None)
def halve(c, t):          # c: sorted tuple of (side, r); t questions already asked in this triple
    sides={s for s,_ in c}
    if len(sides)<=1: return solve(tuple(sorted(r for _,r in c)))
    if t==3: return INF   # Lemma C says this cannot happen
    nS=sum(1 for s,_ in c if s==1); ell=min(nS,len(c)-nS)
    worst=0
    for ans in (0,1):
        new=[]
        for s,r in c:
            if s==ans: new.append((s,max(r-1,0)))
            elif r==0: new.append((s,K*ell))
        if not new: continue          # illegal answer
        worst=max(worst,halve(tuple(sorted(new)),t+1))
    return 1+worst
@lru_cache(None)
def solve(o):
    m=len(o)
    if m<=1: return 0
    worst=0
    for S in itertools.combinations(range(m),m//2):
        c=tuple(sorted((1 if i in S else 0,o[i]) for i in range(m)))
        worst=max(worst,halve(c,0))
    return worst
bad=0;tested=0
for m in range(2,MMAX+1):
    bound=3*math.ceil(math.log2(m)); mx=0
    for o in itertools.combinations_with_replacement(range(R+1),m):
        v=solve(o); tested+=1; mx=max(mx,v)
        if v>bound:
            bad+=1
            if bad<=5: print("FAIL k=%d obligations=%s needs %s > %d"%(K,o,v if v<INF else 'INF',bound))
    print("k=%d m=%d bound=%d worst=%s"%(K,m,bound,mx if mx<INF else 'INF'))
print("lemmaC k=%d m<=%d R=%d multisets=%d failures=%d"%(K,MMAX,R,tested,bad))
