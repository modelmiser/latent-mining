"""EXPLORATORY: do periodic (n,p) patterns survive the POINTWISE idealised system
   D(tau) = sum_{s=tau}^{tau+d-1} (n_s-p_s) <= d-1+g_tau  for all tau,  avg n>=1/2, avg p<=-1/2 ?
(g from the 'full corridor' bottom-state model in lp_cycles.py).  A survivor = the lemma set is insufficient."""
import itertools, sys
sys.dont_write_bytecode=True
import lp_cycles as M
moves=[(n,p) for n in (-1,0,1) for p in (-1,0,1)]
def survivors(d,L):
    out=[]
    for pat in itertools.product(moves,repeat=L):
        if sum(n for n,p in pat)*2<L or sum(p for n,p in pat)*2>-L: continue
        sp=sn=(1,1); ok=True; gs=None
        for rep in range(8):
            gs=[]
            for n,p in pat:
                r=M.gval(sp,sn,n,p)
                if r is None: ok=False;break
                g,sp,sn=r; gs.append(g)       # g for time tau = (index of this tick)+1
            if not ok: break
        if not ok: continue
        c=[n-p for n,p in pat]
        for i in range(L):                     # tick i just happened; tau=i+1
            D=sum(c[(i+1+k)%L] for k in range(d))
            if D>d-1+gs[i]: ok=False;break
        if ok: out.append(pat)
    return out
if __name__=="__main__":
    M.CAP=9
    for d in (3,5,7,9,11):
        for L in range(1,7):
            s=survivors(d,L)
            print(f"d={d} period={L}: survivors={len(s)}", s[:2] if s else "")
