# Independent checks of the witnesses (does not use game.cpp).
#  (1) explicit delay-d strategy, simulated literally against ALL spoiler plays, d=0..DMAX
#  (2) not blind-winnable: (i) BFS over families of image sets (complete), (ii) brute force all words up to length L,
#      (iii) the explicit Spoiler counter-strategy from proofs.txt run against all words of length <= L (C2 only)
#  (3) deliberately wrong objects must be rejected.
import itertools, sys
AUT={
 'C2':({1:1,2:1,3:2,4:3},{1:4,2:1,3:3,4:4}),
 'C1':({1:1,2:1,3:2,4:3},{1:3,2:4,3:2,4:2}),
 'C2pad6':({1:1,2:1,3:2,4:3,5:1,6:1},{1:4,2:1,3:3,4:4,5:1,6:1}),
}
def strategy(d, t, y1, shift=0):
    # x_1 = b ; afterwards a, except one b at round r+1, r = least integer >= d+1 with r = 2 (mod 3) if y_1=a, r = 0 (mod 3) if y_1=b.
    # (y1 is only consulted when t >= d+2, i.e. when it is legally known.)
    if t==1: return 'b'
    if t<d+2: return 'a'
    r=d+1
    while r%3 != ((2 if y1=='a' else 0)+shift)%3: r+=1
    return 'b' if t==r+1 else 'a'
def check_strategy(name,d,shift=0):
    A={'a':AUT[name][0],'b':AUT[name][1]}; T=d+4
    for ys in itertools.product('ab',repeat=T-1):
        S=set(A['a'])
        for t in range(1,T+1):
            x=strategy(d,t,ys[0] if t>=d+2 else None,shift)
            S={A[x][q] for q in S}
            if t<T: S={A[ys[t-1]][q] for q in S}
        if len(S)!=1: return False
    return True
def blind_bfs(name):
    A={'a':AUT[name][0],'b':AUT[name][1]}
    im=lambda S,c: frozenset(A[c][q] for q in S)
    F0=frozenset([frozenset(A['a'])]); seen={F0}; cur=[F0]; T=1
    while cur:
        nxt=[]
        for F in cur:
            for x in 'ab':
                G=frozenset(im(S,x) for S in F)
                if all(len(S)==1 for S in G): return T
                H=frozenset(im(S,y) for S in G for y in 'ab')
                if H not in seen: seen.add(H); nxt.append(H)
        cur=nxt; T+=1
    return 0, len(seen)
def blind_brute(name,L):
    A={'a':AUT[name][0],'b':AUT[name][1]}
    # family of possible images after x1 y1..x_k y_k, DFS over words
    def rec(F,k):
        for x in 'ab':
            G={frozenset(A[x][q] for q in S) for S in F}
            if all(len(S)==1 for S in G): return True
            if k<L and rec({frozenset(A[y][q] for q in S) for S in G for y in 'ab'},k+1): return True
        return False
    return rec({frozenset(A['a'])},1)
def spoiler_C2(word):
    # counter-strategy of proofs.txt (Lemma B). word is extended by b's; returns True if image after last x is non-singleton
    A={'a':AUT['C2'][0],'b':AUT['C2'][1]}
    w=word+'bbbb'; S=frozenset([1,2,3,4]); n=len(w)
    for i,x in enumerate(w[:len(word)]):
        S1=frozenset(A[x][q] for q in S)
        if i==len(word)-1: return len(S1)>1
        if x=='a': y='b'
        else:
            if w[i+1]=='b': y='b'
            else:
                m=0
                while i+1+m<n and w[i+1+m]=='a': m+=1
                y='a' if m%3==2 else 'b'
        S=frozenset(A[y][q] for q in S1)
ok=True
for name in AUT:
    r=[check_strategy(name,d) for d in range(0,13)]
    print(name,'strategy wins with T=d+4 for d=0..12:',all(r)); ok&=all(r)
    print(name,'blind BFS (0,=not winnable; second = families visited):',blind_bfs(name)); ok&=(blind_bfs(name)[0]==0 if isinstance(blind_bfs(name),tuple) else False)
    b=blind_brute(name,14); print(name,'some blind word of length <=14 wins:',b); ok&=not b
s=all(spoiler_C2(''.join(w)) for L in range(1,17) for w in itertools.product('ab',repeat=L))
print('C2 explicit Spoiler counter-strategy beats all words of length <=16:',s); ok&=s
# negative controls
neg1=[check_strategy('C2',d,shift=1) for d in range(0,7)]
print('NEGATIVE: wrong residue strategy accepted for d=0..6 ->',neg1,'(must be all False)'); ok&=not any(neg1)
AUT['SYNC']=({1:1,2:1,3:1,4:1},{1:2,2:3,3:4,4:1})
print('NEGATIVE: constant-letter automaton blind BFS ->',blind_bfs('SYNC'),'(must be 1)'); ok&=(blind_bfs('SYNC')==1)
AUT['C2bad']=({1:1,2:1,3:2,4:3},{1:4,2:1,3:3,4:1})
print('NEGATIVE: perturbed C2 with same strategy, d=0..3 ->',[check_strategy('C2bad',d) for d in range(4)])
print('ALL OK' if ok else 'FAILURE')
