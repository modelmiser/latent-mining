# strat.py : independent (python) delay-game solver for one automaton; prints min T and an optimal strategy tree.
import sys, itertools, functools
a=[int(c)-1 for c in sys.argv[1]]; b=[int(c)-1 for c in sys.argv[2]]; d=int(sys.argv[3]); show=len(sys.argv)>4
n=len(a); L=[a,b]
def im(S,x): return frozenset(L[x][i] for i in S)
Q=frozenset(range(n))
@functools.lru_cache(None)
def imm(S,q,x):
    if not q: return len(im(S,x))==1
    S1=im(S,q[0]); return all(imm(im(S1,y),q[1:],x) for y in (0,1))
# value iteration over full states
subs=[frozenset(c) for k in range(1,n+1) for c in itertools.combinations(range(n),k)]
qs=list(itertools.product((0,1),repeat=d))
rk={}
for S in subs:
    for q in qs:
        if imm(S,q,0) or imm(S,q,1): rk[(S,q)]=1
it=2
def nxt(S,q,x,y):
    if d==0: return (im(im(S,x),y),())
    return (im(im(S,q[0]),y), q[1:]+(x,))
while True:
    new=[]
    for S in subs:
        for q in qs:
            if (S,q) in rk: continue
            if any(all(nxt(S,q,x,y) in rk for y in (0,1)) for x in (0,1)): new.append((S,q))
    if not new: break
    for k in new: rk[k]=it
    it+=1
@functools.lru_cache(None)
def pre(q):
    if len(q)==d: return rk.get((Q,q),0)
    if imm(Q,q,0) or imm(Q,q,1): return 1
    c=[pre(q+(x,)) for x in (0,1)]; c=[v+1 for v in c if v]
    return min(c) if c else 0
print("d=%d minT=%d"%(d,pre(())))
if show and pre(()):
    # opening
    q=(); w=""
    while len(q)<d and not (imm(Q,q,0) or imm(Q,q,1)):
        x=min((0,1),key=lambda x: pre(q+(x,)) or 10**9); alt=[ "ab"[x] for x in (0,1) if pre(q+(x,))==pre(q+(min((0,1),key=lambda x: pre(q+(x,)) or 10**9),))]
        w+="ab"[x] if len(alt)==1 else "("+"|".join(alt)+")"; q+=(x,)
    print("opening:",w)
    def name(S): return "".join(str(i+1) for i in sorted(S))
    def rec(S,q,hist,depth):
        if imm(S,q,0) or imm(S,q,1):
            x=0 if imm(S,q,0) else 1; print("  "*depth+"ys=%s S=%s q=%s -> play %s WIN"%(hist,name(S),"".join("ab"[c] for c in q),"ab"[x])); return
        best=None
        for x in (0,1):
            if all(nxt(S,q,x,y) in rk for y in (0,1)):
                v=max(rk[nxt(S,q,x,y)] for y in (0,1))
                if best is None or v<best[0]: best=(v,x)
        x=best[1]; print("  "*depth+"ys=%s S=%s q=%s -> play %s"%(hist,name(S),"".join("ab"[c] for c in q),"ab"[x]))
        for y in (0,1):
            rec(*nxt(S,q,x,y),hist+"ab"[y],depth+1)
    if len(q)==d and int(sys.argv[4])>0: rec(Q,q,"",0)
