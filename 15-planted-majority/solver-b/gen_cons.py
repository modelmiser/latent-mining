#!/usr/bin/env python3
"""Conservative weight protocol C(W). Usage: gen_cons.py w1,w2,...  (positive integers; K=max)
States: P_w (output A) for w in W descending (idx 0 = P_K = qA), then N_w (output B) (first = N_K = qB), then wa (A), wb (B).
Rules (given for both orders of the pair, mirrored):
  (P_x,N_y): z=x-y; z=0 -> (wa,wb); z>0 -> (P_z,wa) if z in W else (P_z1,P_z2) with z1+z2=z; z<0 symmetric with N and wb.
  (P_x,wb) with x=x1+x2, x1,x2 in W -> (P_x1,P_x2);  (N_x,wa) -> (N_x1,N_x2).  Everything else identity.
Exits with an error if W lacks a needed representation (so it doubles as a checker of the conditions on W)."""
import sys
W=sorted(set(int(t) for t in sys.argv[1].split(',')),reverse=True); K=W[0]; WS=set(W); n=len(W); s=2*n+2
P={w:i for i,w in enumerate(W)}; Nn={w:n+i for i,w in enumerate(W)}; wa=2*n; wb=2*n+1
def two(z):
    for z1 in W:
        if z-z1 in WS and z1>=z-z1: return (z1,z-z1)
    return None
out=[]
def rule(p,q,p2,q2): out.append((p,q,p2,q2)); out.append((q,p,q2,p2))
unsplit=[]
for x in W:
    for y in W:
        z=x-y
        if z==0: rule(P[x],Nn[y],wa,wb)
        else:
            a=abs(z); S=P if z>0 else Nn; wk=wa if z>0 else wb
            if a in WS: r=(S[a],wk)
            else:
                t=two(a)
                if t is None: sys.exit('W not closed: %d-%d'%(x,y))
                r=(S[t[0]],S[t[1]])
            if z>0: rule(P[x],Nn[y],r[0],r[1])
            else: rule(P[x],Nn[y],r[1],r[0])
    t=two(x)
    if t is None: unsplit.append(x)
    else:
        rule(P[x],wb,P[t[0]],P[t[1]]); rule(Nn[x],wa,Nn[t[0]],Nn[t[1]])
U=max(unsplit)
print('# conservative protocol C(W), W=%s, K=%d, max unsplittable U=%d; proved to win all instances with (a+b)*U+1 <= K, i.e. a+b <= %d'%(sorted(W),K,U,(K-1)//U))
print(s); print(P[K],Nn[K]); print('A'*n+'B'*n+'AB')
for r in out: print(*r)
