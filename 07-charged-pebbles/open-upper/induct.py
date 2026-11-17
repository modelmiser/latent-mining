# Test: (1) P/R-components == fibres of red();  (2) J' is inductive under every F-move
# from every config of every component whose anchor set satisfies J'(f)  (all anchor sets in window,
# reachable or not).  usage: induct.py q N
import sys, itertools
q=int(sys.argv[1]); N=int(sys.argv[2]); U=2**(q-2)
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
    # least f such that J'(f) holds: a_j <= (f+3)U - 2^(q-1-j), f>=1 if A nonempty
    if not A: return 0
    g=1
    for j,a in enumerate(A,1):
        # a + 2^(q-1-j) <= (f+3)U ; 2^(q-1-j) may be 1/2 when j=q (not allowed here)
        num=a*2+ (2**(q-j))  # doubled
        f=-(-num//(2*U))-3
        g=max(g,f)
    return g
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
print("q",q,"N",N,"anchor sets",nA,"configs",nS,"F-moves",nF,"comp/red mismatches",badcomp,"violations",bad)
print("histogram of G(A')-G(A):",sorted(hist.items()))
