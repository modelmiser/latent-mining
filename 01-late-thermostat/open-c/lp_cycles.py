"""EXPLORATORY: find periodic (n,p) patterns that defeat the idealised averaging argument
(avg n >= 1/2, avg p <= -1/2, avg g >= 1 in the 'full corridor' bottom-state model)."""
import itertools, sys
sys.dont_write_bytecode=True
CAP=7
def step(st,p):
    f,r=st; m=min(f,r)
    if p==0:  nf,nr=m,max(1,f-1)
    elif p==-1: nf,nr=max(1,m-1),max(1,f-2)
    else: nf,nr=m+1,f
    if nf>CAP or nr>CAP: return None
    return (nf,nr)
def arrivals(st,p):
    f,r=st; out=set()
    for a in range(f,CAP+4):
        for w in (-1,0,1): out.add(a+p+w)
    for a in range(r,CAP+4): out.add(a+p)
    return out
def e(st): return min(st[0],st[1]+1)
def gval(sp,sn,n,p):
    sp2=step(sp,p); sn2=step(sn,-n)
    if sp2 is None or sn2 is None: return None
    Bs=arrivals(sp,p); As={-v for v in arrivals(sn,-n)}
    g=e(sp2)+e(sn2)
    for be in Bs:
        for al in As:
            if be>=-1 and al<=1 and ((be>0 and al>0) or (be==0 and al==0) or (be<0 and al<0)): g=min(g,be-al)
    return g,sp2,sn2
moves=[(n,p) for n in (-1,0,1) for p in (-1,0,1)]
if __name__=="__main__":
  for L in range(1,7):
      for pat in itertools.product(moves,repeat=L):
          if sum(n for n,p in pat)*2<L or sum(p for n,p in pat)*2>-L: continue
          sp=sn=(1,1); ok=True
          for rep in range(6):          # settle to the periodic regime
              tot=0
              for n,p in pat:
                  r=gval(sp,sn,n,p)
                  if r is None: ok=False;break
                  g,sp,sn=r; tot+=g
              if not ok: break
          if ok and tot>=L: print(L,pat,"sum g per period",tot,"state",sp,sn)
