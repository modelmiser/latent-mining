#!/usr/bin/env python3
"""Sawtooth strategy (proofs.txt Theorem L). usage: construct.py n k [outfile]
T = n-1-ceil(n/k). Prints the strategy; nobody is complete through round T,
so F(n,k) >= T+1, provided T >= ceil(n/k)."""
import sys
def strategy(n,k):
    kk=min(k,n)
    g=-(-n//kk)
    T=n-1-g
    if T<1 or T<g: return None
    s=[ (j*n)//kk for j in range(kk)]          # process-coordinate ramp bases at time T, gaps <= g
    rounds=[]
    for t in range(1,T+1):
        D=[]
        if t%2==1:
            v=(t+1)//2
            # alpha ramp j: final process position s_j+v gets deficit >= v. frame x = i+t is invariant,
            # so the hit at round t is on link (i+1 -> i) with i = s_j+v+T-t
            for sj in s:
                i=(sj+v+T-t)%n; D.append(((i+1)%n,i))
        else:
            v=t//2
            # beta ramp j: final process position e_j-v gets deficit >= v, e_j = s_{j-1}+T+1; frame y=i-t
            for j in range(kk):
                e=s[j-1]+T+1
                i=(e-v-T+t)%n; D.append(((i-1)%n,i))
        rounds.append(D)
    return T,rounds
if __name__=="__main__":
    n=int(sys.argv[1]);k=int(sys.argv[2])
    r=strategy(n,k)
    if r is None: print("construction not applicable"); sys.exit(2)
    T,rounds=r
    out=open(sys.argv[3],'w') if len(sys.argv)>3 else sys.stdout
    out.write("# sawtooth strategy n=%d k=%d: no process complete through round %d\n"%(n,k,T))
    for t,D in enumerate(rounds,1): out.write(str(t)+"".join(" %d>%d"%l for l in D)+"\n")
