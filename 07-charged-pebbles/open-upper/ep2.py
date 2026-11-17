# EP2 test: for every config S (|S|<=q, nodes<=N) and every z in S, with A=red(S), A'=red(S-z):
#   red( A | A' | {z} ) == A
import sys, itertools
q=int(sys.argv[1]); N=int(sys.argv[2])
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
n=0;bad=0
for k in range(1,q+1):
    for S in itertools.combinations(range(1,N+1),k):
        A=red(S)
        for z in S:
            n+=1
            A2=red(tuple(y for y in S if y!=z))
            if red(tuple(sorted(set(A2)|{z}|set(A))))!=A:
                bad+=1
                if bad<10: print("EP2 FAIL S",S,"z",z,"A'",A2,"A",A)
print("q",q,"N",N,"cases",n,"EP2 failures",bad)
