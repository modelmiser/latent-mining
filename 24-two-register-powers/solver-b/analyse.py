import math
D={}
for fn in ("dm1100.out","dm4096.out"):
    for l in open(fn):
        p=l.split()
        if p[0]=="D": D[int(p[1])]=(int(p[2]),int(p[3]),int(p[4]))
N=[int(l.split()[3]) for l in open("gbfs21.out") if l.startswith("t ")]
def T(m):
    return next(t for t in range(len(N)) if N[t]>=m)
ms=sorted(D)
print("m range",ms[0],ms[-1],"N=",N)
# checks of easy bounds
bad=[m for m in ms if not (math.ceil(math.log2(m))<=D[m][0]<=2*math.floor(math.log2(m)))]
print("violations of ceil(log2 m)<=D<=2floor(log2 m):",bad)
print("violations of D>=T(m):",[m for m in ms if D[m][0]<T(m)])
gap={}
for m in ms: gap[D[m][0]-T(m)]=gap.get(D[m][0]-T(m),0)+1
print("histogram of D(m)-T(m):",sorted(gap.items()))
# first/last m for each D value, min ratio
first={};last={}
for m in ms:
    d=D[m][0]; first.setdefault(d,m); last[d]=m
print("d: first m with D=d, last m with D<=d (largest m s.t. all m'<=m have D<=d), N(d)")
for d in sorted(first):
    mx=max(m for m in ms if all(D[k][0]<=d for k in ms if k<=m)) if D[ms[0]][0]<=d else None
    print(d,first[d],last[d],mx,N[d] if d<len(N) else None)
print("record-low D/log2 m (running min over m>=16):")
best=9
for m in ms:
    if m<16: continue
    r=D[m][0]/math.log2(m)
    if r<best+1e-12:
        best=r; print(m,D[m][0],round(r,4))
for lo in (16,64,256,1024,2048):
    sub=[m for m in ms if lo<=m<2*lo]
    if sub:
        print("m in [%d,%d): min ratio %.4f at %s, max ratio %.4f, mean D %.3f"%(lo,2*lo,min(D[m][0]/math.log2(m) for m in sub),min(sub,key=lambda m:D[m][0]/math.log2(m)),max(D[m][0]/math.log2(m) for m in sub),sum(D[m][0] for m in sub)/len(sub)))
print("D(2..40):",[D[m][0] for m in range(2,41)])
with open("D_table.txt","w") as f:
    for m in ms: f.write("%d %d\n"%(m,D[m][0]))
