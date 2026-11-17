# end-to-end: exact minimal F-cost of every anchor set met by 0-1 BFS (explore.costs) vs invariant J''.
import sys, explore
q=int(sys.argv[1]);N=int(sys.argv[2]);fm=int(sys.argv[3]);U=2**(q-2)
d=explore.costs(q,N,fm)
bad=0;n=0;tight1=0;tight2=0
for S,c in d.items():
    A=explore.reduce(S,q)
    if not A: continue
    n+=1; m=len(A)
    ok = c>=1 and A[0]<=(c+2)*U and 2*A[-1]<=2*(c+3)*U-2**(q-m) and max(S)<=(c+4)*U-1
    if A[0]==(c+2)*U: tight1+=1
    if 2*A[-1]==2*(c+3)*U-2**(q-m): tight2+=1
    if not ok:
        bad+=1
        if bad<5: print("BAD",S,A,c)
print("q",q,"N",N,"fmax",fm,"configs with nonempty anchor set",n,"violations",bad,"tight(a_1)",tight1,"tight(a_m)",tight2)
