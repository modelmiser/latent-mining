# Test: (1) P/R-components == fibres of red();  (2) J' is inductive under every F-move
# from every config of every component whose anchor set satisfies J'(f)  (all anchor sets in window,
# reachable or not).  usage: induct.py q N
import sys, itertools
q=int(sys.argv[1]); N=int(sys.argv[2]); U=2**(q-2); MODE=sys.argv[3] if len(sys.argv)>3 else 'J2'
def red(S):
    S=list(S)
    ch=True
    while ch:
        ch=False
        r=q-len(S)
        for i,x in enumerate(S):
            prev=S[i-1] if i else 0
            if x-prev<=2**r:
                del S[i]; ch=True; break
    return tuple(S)
def G(A):
    # least f>=0 for which the invariant holds.  Modes:
    #  J1 : a_j <= (f+3)U - 2^(q-1-j) for every rank j              (J')
    #  J2 : a_1 <= (f+2)U and a_m <= (f+3)U - 2^(q-m-1)              (J'', the one proved)
    #  M1 : J2 with a_1 <= (f+2)U - 1          (mutant, must be violated)
    #  M2 : J2 with a_m <= (f+3)U - 2^(q-m-1) - 1   (mutant: not even true at f=1? must be violated)
    #  M3 : only a_1 <= (f+2)U                 (mutant: too weak to be inductive, must be violated)
    if not A: return 0
    m=len(A); f=1
    while True:
        if MODE=='J1': ok=all(2*a <= 2*(f+3)*U - 2**(q-j) for j,a in enumerate(A,1))
        elif MODE=='J2': ok=A[0]<=(f+2)*U and 2*A[-1] <= 2*(f+3)*U - 2**(q-m)
        elif MODE=='M1': ok=A[0]<=(f+2)*U-1 and 2*A[-1] <= 2*(f+3)*U - 2**(q-m)
        elif MODE=='M2': ok=A[0]<=(f+2)*U and 2*A[-1] <= 2*(f+3)*U - 2**(q-m) - 2
        elif MODE=='M3': ok=A[0]<=(f+2)*U
        if ok: return f
        f+=1
def comp(A):
    seen={A}; st=[A]
    while st:
        S=st.pop(); s=set(S)
        nb=[]
        if len(S)<q:
            if 1 not in s: nb.append(tuple(sorted(S+(1,))))
            for x in S:
                if x+1 not in s: nb.append(tuple(sorted(S+(x+1,))))
        for x in S:
            if x==1 or x-1 in s: nb.append(tuple(y for y in S if y!=x))
        for T in nb:
            if T not in seen: seen.add(T); st.append(T)
    return seen
def anchorsets():
    for m in range(0,q):
        r=q-m
        def rec(pref):
            if len(pref)==m: yield tuple(pref); return
            lo=(pref[-1] if pref else 0)+2**r+1
            for a in range(lo,N+1):
                yield from rec(pref+[a])
        yield from rec([])
nA=0; nS=0; nF=0; bad=0; badcomp=0; worst=-9; hist={}
for A in anchorsets():
    nA+=1
    C=comp(A); g=G(A)
    for S in C:
        nS+=1
        if red(S)!=A: badcomp+=1
        s=set(S)
        for z in S:
            if z>=2 and z-1 not in s:
                nF+=1
                A2=red(tuple(y for y in S if y!=z))
                if len(A2)>=q: bad+=1; print("m=q anchor set?!",A,S,z,A2)
                d=G(A2)-g
                hist[d]=hist.get(d,0)+1
                if d>1:
                    bad+=1
                    if bad<15: print("VIOLATION",A,"G",g,"S",S,"remove",z,"->",A2,"G",G(A2))
print("mode",MODE,"q",q,"N",N,"anchor sets",nA,"configs",nS,"F-moves",nF,"comp/red mismatches",badcomp,"violations",bad)
print("histogram of G(A')-G(A):",sorted(hist.items()))
