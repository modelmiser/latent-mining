# Mechanical tests of the lemmas used in proofs.txt.   usage: lemmas.py q [mut]
#  T2  Shape lemma: A in {empty,{a}}, S in comp(A), ANY z in S, A'=red(S-z)  =>  A' empty or H_A(A')
#  T3  Closure: every Q with H_A(Q) has red(Q) empty or H_A(red(Q))
#  T4  EP*: red_q( red_{q-1}(S-z) + z ) == red_q(S)       (S in comp(A))
#  T7  Lemma T: elements of S-z below min A' are <= 2^(q-1)-1
#  mut=1 : shrink the allowed shift in H by one (the tests must then FAIL)
import sys
q=int(sys.argv[1]); MUT=int(sys.argv[2]) if len(sys.argv)>2 else 0
def red(S,q=q):
    S=list(S)
    ch=True
    while ch:
        ch=False
        r=q-len(S)
        if r<0: raise SystemExit("too many pebbles")
        for i,x in enumerate(S):
            prev=S[i-1] if i else 0
            if x-prev<=2**r:
                del S[i]; ch=True; break
    return tuple(S)
def comp(A):
    seen={A}; st=[A]
    while st:
        S=st.pop(); s=set(S); nb=[]
        if len(S)<q:
            if 1 not in s: nb.append(tuple(sorted(S+(1,))))
            for x in S:
                if x+1 not in s: nb.append(tuple(sorted(S+(x+1,))))
        for x in S:
            if x==1 or x-1 in s: nb.append(tuple(y for y in S if y!=x))
        for T in nb:
            if T not in seen: seen.add(T); st.append(T)
    return seen
def shiftbound(k):
    b=2**(q-k-1)
    return b-1 if MUT==1 else b
def H(A,C,Q):
    # exists V in C (=comp(A)) with |V|=|Q| and Q==V or Q = V-u+u' order preserving, |u-u'|<=2^(q-k-1)
    if Q in C: return True
    k=len(Q)
    if k>q-1: return False
    B=shiftbound(k)
    for i,x in enumerate(Q):
        lo=Q[i-1]+1 if i else 1
        hi=Q[i+1]-1 if i+1<k else x+B
        for y in range(max(lo,x-B),min(hi,x+B)+1):
            if y==x: continue
            V=Q[:i]+(y,)+Q[i+1:]
            if V in C: return True
    return False
def run(A):
    C=comp(A)
    st=dict(A=A,comp=len(C),t2=0,t2bad=0,t3=0,t3bad=0,t4bad=0,t7bad=0)
    for S in C:
        for z in S:
            S1=tuple(y for y in S if y!=z)
            A1=red(S1)
            st['t2']+=1
            if A1 and not H(A,C,A1):
                st['t2bad']+=1
                if st['t2bad']<4: print("  T2 FAIL",A,S,z,A1)
            A2=red(S1,q-1)
            if red(tuple(sorted(A2+(z,))))!=A: st['t4bad']+=1
            if A1 and any(y<A1[0] and y>2**(q-1)-1 for y in S1): st['t7bad']+=1
    for V in C:
        k=len(V)
        if k>q-1 or k==0: continue
        B=shiftbound(k) if MUT!=1 else 2**(q-k-1)   # T3 hypotheses always use the true bound
        for i,u in enumerate(V):
            lo=V[i-1]+1 if i else 1
            hi=V[i+1]-1 if i+1<k else u+B
            for y in range(max(lo,u-B),min(hi,u+B)+1):
                if y==u: continue
                Q=V[:i]+(y,)+V[i+1:]
                R=red(Q)
                st['t3']+=1
                if R and not H(A,C,R):
                    st['t3bad']+=1
                    if st['t3bad']<4: print("  T3 FAIL",A,V,u,y,R)
    print(st)
    return st['t2bad']+st['t3bad']+st['t4bad']+st['t7bad']
bad=run(())
for a in range(2**(q-1)+1, 2**(q-1)+2**(q-1)+4):
    bad+=run((a,))
print("q",q,"mut",MUT,"TOTAL FAILURES",bad)
