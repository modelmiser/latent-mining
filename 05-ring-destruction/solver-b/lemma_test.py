#!/usr/bin/env python3
"""Randomised test of the lemmas used in proofs.txt, against the set-based definition:
 (E) K_t(i) = {i-L..i+R} with the extent recursion;  (H) deficit a_i(t)=t-R_i(t) <= ceil(t/2);
 (I) a_i(t) <= a_{i-1}(t)+1 [process coords: alpha(i) <= alpha(i-1)+1];
 (Phi) sum over tops of alpha <= number of destructions of links (i+1 -> i) so far (needs k<n)."""
import random,sys
random.seed(int(sys.argv[1]) if len(sys.argv)>1 else 1)
trials=0
for _ in range(3000):
    n=random.randint(3,12); k=random.randint(1,n-1)
    K=[{i} for i in range(n)]; R=[0]*n; L=[0]*n; prev=set(); hitsA=hitsB=0
    links=[((i+1)%n,i) for i in range(n)]+[((i-1)%n,i) for i in range(n)]
    for t in range(1,2*n):
        # only useful links, to make the test bite
        useful=[l for l in links if not K[l[0]]<=K[l[1]] and l not in prev]
        D=set(random.sample(useful,min(len(useful),random.randint(0,k))))
        new=[set(x) for x in K]
        for i in range(n):
            for j in ((i-1)%n,(i+1)%n):
                if (j,i) not in D: new[i]|=K[j]
        R2=[R[i] if ((i+1)%n,i) in D else max(R[i],R[(i+1)%n]+1) for i in range(n)]
        L2=[L[i] if ((i-1)%n,i) in D else max(L[i],L[(i-1)%n]+1) for i in range(n)]
        hitsA+=sum(1 for (s,d) in D if s==(d+1)%n); hitsB+=sum(1 for (s,d) in D if s==(d-1)%n)
        K,R,L,prev=new,R2,L2,D
        for i in range(n):
            assert K[i]=={(i+j)%n for j in range(-L[i],R[i]+1)},"E"
        if any(len(x)==n for x in K):
            assert any(R[i]+L[i]>=n-1 for i in range(n)); break
        assert all(R[i]+L[i]<=n-2 for i in range(n))
        a=[t-R[i] for i in range(n)]; b=[t-L[i] for i in range(n)]
        assert all(0<=x<=(t+1)//2 for x in a+b),"H"
        assert all(a[i]<=a[i-1]+1 for i in range(n)) and all(b[i]<=b[(i+1)%n]+1 for i in range(n)),"I"
        assert sum(a[i] for i in range(n) if a[(i+1)%n]<=a[i])<=hitsA,"Phi A"
        assert sum(b[i] for i in range(n) if b[i-1]<=b[i])<=hitsB,"Phi B"
        trials+=1
print("lemma checks passed on",trials,"positions")
