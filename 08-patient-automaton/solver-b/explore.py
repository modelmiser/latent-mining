import sys, itertools
from functools import lru_cache
A={'a':{1:1,2:1,3:2,4:3},'b':{1:4,2:1,3:3,4:4}}
if len(sys.argv)>1 and sys.argv[1]=='c1': A={'a':{1:1,2:1,3:2,4:3},'b':{1:3,2:4,3:2,4:2}}
def im(S,w):
    for c in w: S=frozenset(A[c][q] for q in S)
    return S
Q=frozenset([1,2,3,4])
def fam_step(F,x):
    return frozenset(im(S,x+y) for S in F for y in 'ab')
def maxl(F):
    return sorted([''.join(map(str,sorted(S))) for S in F if not any(S<T for T in F) ])
# blind-winnable from family F: BFS
def blind_from(F,limit=200000):
    seen={F}; cur=[(F,'')]
    while cur:
        nxt=[]
        for G,w in cur:
            for x in 'ab':
                if all(len(im(S,x))==1 for S in G): return w+x
                H=fam_step(G,x)
                H=frozenset(S for S in H if not any(S<T for T in H))
                if H not in seen: seen.add(H); nxt.append((H,w+x))
        cur=nxt
    return None
subs=[frozenset(c) for r in range(2,5) for c in itertools.combinations([1,2,3,4],r)]
for S in subs: print(''.join(map(str,sorted(S))), blind_from(frozenset([S])))
print('pairs of sets jointly:')
for S,T in itertools.combinations(subs,2):
    if not (S<T or T<S): 
        r=blind_from(frozenset([S,T])); 
        if blind_from(frozenset([S])) and blind_from(frozenset([T])): print(''.join(map(str,sorted(S))),''.join(map(str,sorted(T))),r)
# family after blind words
for L in range(1,5):
    for w in itertools.product('ab',repeat=L):
        F=frozenset([Q])
        for x in w[:-1]: F=fam_step(F,x)
        # family before last synch letter's spoiler reply: show after full rounds
        F2=fam_step(F,w[-1])
        print(''.join(w), maxl(F2))
