# EP2 failures classified.  Also EP2x: for each x in A'\A : x not in red(A | A' | {z})  (x is a FREE pebble of that config's component?)
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
n=0;cls={};shown={}
for k in range(1,q+1):
    for S in itertools.combinations(range(1,N+1),k):
        A=red(S)
        for z in S:
            n+=1
            A2=red(tuple(y for y in S if y!=z))
            R=red(tuple(sorted(set(A2)|{z}|set(A))))
            if R!=A:
                new=set(A2)-set(A)
                key=(len(A),len(new), 'z in R' if z in R else 'z not in R', 'new in R' if new&set(R) else 'new not in R', 'z anchor' if z in A else 'z free')
                cls[key]=cls.get(key,0)+1
                if cls[key]<=2: print(key,"S",S,"z",z,"A'",A2,"A",A,"R",R)
print("q",q,"N",N,"cases",n)
for k,v in sorted(cls.items()): print(k,v)
