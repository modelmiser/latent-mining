# gdp.py : numeric evaluation of the proved greedy upper bound T(n) <= g(n),
# g(n) = max_{a<=b, a+b=n} g(a)+g(b)+kap(a), kap(a)=max(0,floor(log2(a+1))-1)   (Lemma 4 of proofs.txt)
N=3000
kap=[0]*(N+1)
for a in range(1,N+1): kap[a]=max(0,(a+1).bit_length()-2)
g=[0]*(N+1)
for n in range(2,N+1): g[n]=max(g[a]+g[n-a]+kap[a] for a in range(1,n//2+1))
best=max(range(1,N+1),key=lambda n:g[n]/n)
print("g:",[g[n] for n in range(1,33)])
print("max g(n)/n for n<=%d: %.4f at n=%d ; g(N)/N=%.4f"%(N,g[best]/best,best,g[N]/N))
