# are the four s=7 optimal classes (solver-a/dfs7.txt) the same space-time diagrams up to a renaming of states?
R=["0 3 1 6 1 0 5 4 1 2 5 3 4 1 0 3 5 1","0 4 3 5 3 0 6 3 0 1 5 3 2 1 4 4 1 3","0 4 5 2 3 4 1 5 0 3 1 5 6 5 0 4 3 5","0 4 5 4 3 5 6 5 0 3 6 5 1 5 0 2 3 4"]
def tab(line):
    v=list(map(int,line.split())); d={}; i=0
    for x in [0,1,3,4,5,6]:
        for k in range(3): d[(x,k)]=v[i]; i+=1
    return d
def diag(d,n):
    c=[1]+[0]*(n-1); out=[tuple(c)]
    while True:
        nx=[]
        for i in range(n):
            x=c[i]; k=(i>0 and c[i-1]!=x)+(i<n-1 and c[i+1]!=x); nx.append(d[(x,k)])
        out.append(tuple(nx))
        if 2 in nx: return out
        c=nx
D=[[diag(tab(r),n) for n in range(2,13)] for r in R]
for t0 in (0,1):
  for j in range(1,4):
    m={}; ok=True; inj=True
    for da,db in zip(D[0],D[j]):
        if len(da)!=len(db): ok=False; break
        for ra,rb in zip(da[t0:],db[t0:]):
            for x,y in zip(ra,rb):
                if m.setdefault(x,y)!=y: ok=False
    if ok: inj=len(set(m.values()))==len(m)
    print('from time',t0,'class 1 vs class',j+1,'consistent renaming:',ok,'injective:',inj,m if ok else '')
