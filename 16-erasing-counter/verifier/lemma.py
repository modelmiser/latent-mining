# mechanical test of solver-a Lemma 2 (a),(b),(c), the problem's weight claims, and A's Remark 3 bound, on all r<=2 trees and random r=3 trees
import random,sys
def gen(path,dl,d):
    out=[('L',0)]+[('L',p) for p in path]
    if dl:
        for i in range(1,d+1):
            if i in path: continue
            ch=gen(path+[i],dl-1,d)
            out+=[('I',i,a,b) for a in ch for b in ch]
    return out
def rnd(path,dl,d):
    if dl==0 or random.random()<0.12: return ('L',random.choice([0]+path))
    i=random.choice([x for x in range(1,d+1) if x not in path])
    return ('I',i,rnd(path+[i],dl-1,d),rnd(path+[i],dl-1,d))
def step(tree,s):
    t=tree;P=0;c=0
    while t[0]=='I':
        b=1<<(t[1]-1);P|=b
        if s&b: c+=1;t=t[3]
        else: t=t[2]
    o=s&~P
    if t[1]: o|=1<<(t[1]-1)
    return o,c,t[1]
def test(tree,r,d,st):
    Z=0;t=tree;k=0
    while t[0]=='I': Z|=1<<(t[1]-1);k+=1;t=t[2]
    p0=t[1]
    n=1<<d;f=[0]*n;cr=[False]*n
    for s in range(n):
        o,c,wr=step(tree,s);f[s]=o
        dw=bin(o).count('1')-bin(s).count('1')
        assert dw in (1-c,-c) and dw<=1-c            # (a) / problem claim
        if dw>0: assert c==0 and s&Z==0 and wr==p0; cr[s]=True
        if not cr[s]: assert bin(o&Z).count('1')<=bin(s&Z).count('1')   # (b)
    seen=[0]*n
    for s in range(n):
        x=s
        while not seen[x]: seen[x]=s+1;x=f[x]
        if seen[x]!=s+1: continue
        cyc=[];y=x
        while True:
            cyc.append(y);y=f[y]
            if y==x: break
        if any(cr[y] for y in cyc):
            assert all(bin(y&Z).count('1')<=1 for y in cyc),(tree,cyc)   # (c) first half
        else:
            assert len(cyc)<=2**k                                         # (c) second half
            if len(cyc)>1: st['nocreate_max']=max(st['nocreate_max'],len(cyc))
        if n-1 in cyc and len(cyc)>1:
            assert len(cyc)<=2**(2**(r-1)); st['allones_max']=max(st['allones_max'],len(cyc))
        assert len(cyc)<=n-1 or r==1
st={'nocreate_max':0,'allones_max':0}
for r in (1,2):
    d=2**r-1;T=gen([],r,d)
    for t in T: test(t,r,d,st)
    print("r",r,"trees",len(T),"ok",st)
random.seed(5)
for _ in range(int(sys.argv[1])): test(rnd([],3,7),3,7,st)
print("r 3 random ok",st)
