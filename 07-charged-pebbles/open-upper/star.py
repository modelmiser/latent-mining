# examine F-moves out of comp(empty): S in comp_q(empty), z in S (any z), A'=red(S-z).
# slack = (2^q - 2^(q-m'-1)) - max A'  ; record min slack by (m', rank_S(b)-m')
import sys, itertools
q=int(sys.argv[1])
def red(S,q=q):
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
# enumerate comp(empty) by BFS
seen={()}; st=[()]
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
print("q",q,"|comp(empty)|",len(seen))
best={}
for S in seen:
    for z in S:
        A2=red(tuple(y for y in S if y!=z))
        if not A2: continue
        b=A2[-1]; m2=len(A2)
        rk=sum(1 for y in S if y<=b)
        slack=(2**q-2**(q-m2-1))-b
        key=(m2,rk-m2)
        if key not in best or slack<best[key][0]: best[key]=(slack,S,z,A2)
for k in sorted(best): print(k,best[k])
