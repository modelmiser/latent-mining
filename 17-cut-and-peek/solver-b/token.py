# Abstract "token" relaxation (exploration only): g in Z^d, permutations p1,p2,p3 of coordinates,
# delta_i = p_i g - g.  Want: g+delta_i+delta_k a permutation of g for all pairs, and
# g+delta_1+delta_2+delta_3 having NO entry in common with g.  Reports the first solutions per (d, maxval).
import itertools, sys
from collections import Counter
def run(d,V):
    found=0
    for g in itertools.combinations_with_replacement(range(V+1),d):
        if g[0]!=0 or len(set(g))==1: continue
        G=Counter(g); Gs=set(g)
        dels=sorted(set(tuple(g[p[c]]-g[c] for c in range(d)) for p in itertools.permutations(range(d))))
        dels=[x for x in dels if any(x)]
        ok={}
        for a in range(len(dels)):
            for b in range(a,len(dels)):
                ok[(a,b)]=Counter(g[c]+dels[a][c]+dels[b][c] for c in range(d))==G
        for a in range(len(dels)):
            for b in range(a,len(dels)):
                if not ok[(a,b)]: continue
                for c3 in range(b,len(dels)):
                    if ok[(a,c3)] and ok[(b,c3)]:
                        fin=[g[c]+dels[a][c]+dels[b][c]+dels[c3][c] for c in range(d)]
                        if not (set(fin)&Gs):
                            found+=1
                            if found<=3: print("d=%d g=%s deltas=%s %s %s final=%s"%(d,g,dels[a],dels[b],dels[c3],fin)); sys.stdout.flush()
    print("d=%d V=%d solutions=%d"%(d,V,found)); sys.stdout.flush()
for d,V in [(2,6),(3,6),(4,6),(5,5),(6,4)]: run(d,V)
