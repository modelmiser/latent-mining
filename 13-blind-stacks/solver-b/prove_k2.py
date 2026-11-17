#!/usr/bin/env python3
"""Computer-assisted proof that N(2,n) = 5 N(2,n-1) - 5 N(2,n-2) for all n >= 5. See proofs.txt (Theorem 3).
Self-contained (own move generator; does not import fast.py). Exact integer arithmetic only.
"""
import itertools, sys
OPTS=[(),(0,),(1,),(0,0),(0,1),(1,0),(1,1)]
STS=list(itertools.product(OPTS,repeat=2))
def moves(st, rpos):
    out=[]
    a,b=st
    if rpos and len(a)<2: out.append(('IN',[(((a+(0,)),b),-1)]))
    if a and len(b)<2: out.append(('SHIFT1',[((a[:-1],b+(a[-1],)),0)]))
    if a and a[-1]==0:
        out.append(('OUT1',[((tuple(0 for _ in a[:-1]),tuple(0 for _ in b)),0),((a[:-1]+(1,),b),0)]))
    if b and b[-1]==0:
        out.append(('OUT2',[((tuple(0 for _ in a),tuple(0 for _ in b[:-1])),0),((a,b[:-1]+(1,)),0)]))
    return out
# topological order within one r: successors (same r) have: an item moved forward, or fewer items, or more flags.
def rank(st):
    a,b=st
    return (len(a)+len(b), len(a), -sum(a)-sum(b))   # smaller = later in play
ORDER=sorted(STS,key=rank)
for st in STS:
    for rp in (0,1):
        for nm,succ in moves(st,rp):
            for s,dr in succ:
                if dr==0: assert rank(s)<rank(st)
def full_dp(R):
    V=[]
    for r in range(R+1):
        cur={}
        for st in ORDER:
            if r==0 and st==((),()): cur[st]=1; continue
            best=0
            for nm,succ in moves(st,r>0):
                best=max(best,sum((cur[s] if dr==0 else V[r-1][s]) for s,dr in succ))
            cur[st]=best
        V.append(cur)
    return V
M=130
V=full_dp(M)
# policy: argmax at r=M (first maximiser)
PI={}
for st in STS:
    best=-1
    for nm,succ in moves(st,True):
        v=sum((V[M][s] if dr==0 else V[M-1][s]) for s,dr in succ)
        if v>best: best=v; PI[st]=nm
r0=1  # claim: policy PI is optimal for every r >= r0+1 ... we check slack for r>=1 (W_0 = V_0)
# W by policy
W=[V[0]]
for r in range(1,M+1):
    cur={}
    for st in ORDER:
        mv=dict(moves(st,True))
        cur[st]=sum((cur[s] if dr==0 else W[r-1][s]) for s,dr in mv[PI[st]]) if st in PI else 0
    W.append(cur)
def ge_beta(x1,x0):
    # is x1 - beta*x0 >= 0, beta=(5-sqrt5)/2  <=> (2*x1-5*x0) + sqrt5*x0 >= 0
    a=2*x1-5*x0; b=x0
    if a>=0 and b>=0: return True
    if a<0 and b<0: return False
    if a>=0: return a*a>=5*b*b   # b<0
    return 5*b*b>=a*a            # a<0,b>=0
nslack=0; J0=8
for st in STS:
    mv=moves(st,True)
    if not mv: continue
    d=dict(mv)
    for nm,succ in mv:
        if nm==PI[st]: continue
        s=[None]+[sum((W[r][x] if dr==0 else W[r-1][x]) for x,dr in d[PI[st]]) - sum((W[r][x] if dr==0 else W[r-1][x]) for x,dr in succ) for r in range(1,M+1)]
        # s[r], r=1..M
        assert all(s[r]>=0 for r in range(1,J0+2)), (st,nm)
        zeros=[s[r+2]-5*s[r+1]+5*s[r] for r in range(J0,M-1)]
        assert len(zeros)>=49+49 and all(z==0 for z in zeros), (st,nm)
        assert ge_beta(s[J0+1],s[J0]), (st,nm,s[:12])
        assert all(x>=0 for x in s[1:])  # redundant direct check on the computed range
        nslack+=1
# value sequences
for st in STS:
    w=[W[r][st] for r in range(M+1)]
    assert all(W[r][st]==V[r][st] for r in range(M+1))
    assert all(w[r+2]-5*w[r+1]+5*w[r]==0 for r in range(J0,M-1))
a=[W[n][((),())] for n in range(M+1)]
assert a[1:5]==[1,2,6,21]
assert all(a[n]==5*a[n-1]-5*a[n-2] for n in range(5,M+1))
F=[0,1]; L=[2,1]
for i in range(2,M+2): F.append(F[-1]+F[-2]); L.append(L[-1]+L[-2])
for n in range(3,M+1):
    j=n//2
    cf = 3*5**(j-1)*F[n] if n%2 else (3*5**(j-2)*L[n] if j>=2 else None)
    assert cf==a[n], n
print("OK: %d slack sequences certified; policy optimal for all r>=1; N(2,n) recurrence and closed form checked to n=%d"%(nslack,M))
print("policy:",{str(k):v for k,v in PI.items()})
print("N(2,n), n=1..20:",a[1:21])
