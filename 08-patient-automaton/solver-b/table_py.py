# Independent (Python, bitmask) recomputation of the small tables. Different code from game.cpp:
# blind game by BFS over FAMILIES OF IMAGE SETS (not pair sets); d-game by attractor on (S, pending word).
import sys, itertools
n=int(sys.argv[1]); dmax=int(sys.argv[2])
FULL=(1<<n)-1
def tables(ta,tb):
    im=[[0]*(1<<n),[0]*(1<<n)]
    for x,t in ((0,ta),(1,tb)):
        for S in range(1,1<<n):
            r=0
            for q in range(n):
                if S>>q&1: r|=1<<t[q]
            im[x][S]=r
    return im
def blind(im):
    F0=frozenset([FULL]); seen={F0}; cur=[F0]
    while cur:
        nxt=[]
        for F in cur:
            for x in (0,1):
                G=[im[x][S] for S in F]
                if all(S&(S-1)==0 for S in G): return True
                H=frozenset(im[y][S] for S in G for y in (0,1) if S&(S-1))
                if H not in seen: seen.add(H); nxt.append(H)
        cur=nxt
    return False
def dwin(im,d):
    NW=1<<d; win=[[ (S!=0 and S&(S-1)==0) for S in range(1<<n)] for w in range(NW)]
    ch=True
    while ch:
        ch=False
        for w in range(NW):
            for S in range(1,1<<n):
                if win[w][S]: continue
                for x in (0,1):
                    if d==0: ok=win[0][im[0][im[x][S]]] and win[0][im[1][im[x][S]]]
                    else:
                        S1=im[w&1][S]; w2=(w>>1)|(x<<(d-1)); ok=win[w2][im[0][S1]] and win[w2][im[1][S1]]
                    if ok: win[w][S]=True; ch=True; break
    return any(win[w][FULL] for w in range(NW))
cnt=[0]*(dmax+1); cb=0
for ta in itertools.product(range(n),repeat=n):
    for tb in itertools.product(range(n),repeat=n):
        im=tables(ta,tb)
        if not dwin(im,0): continue
        cnt[0]+=1
        if blind(im):
            cb+=1
            for d in range(1,dmax+1): cnt[d]+=1
            continue
        for d in range(1,dmax+1):   # monotone in d (proofs.txt, Lemma 0): stop at first failure
            if dwin(im,d): cnt[d]+=1
            else: break
print('n=%d'%n,' '.join('|W_%d|=%d'%(d,cnt[d]) for d in range(dmax+1)),'|W_inf|=%d'%cb)
