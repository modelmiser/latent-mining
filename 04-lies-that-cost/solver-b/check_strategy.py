# check_strategy.py FILE [--u1]
# Independent checker for a questioner strategy tree. Implements the problem definition literally
# (explicit candidates, lie sets, ell_s from C_{s-1}); does NOT use the obligation-multiset abstraction.
# File format: header line "k K n N q Q"; then lines "PATH: e1 e2 ..." where PATH is the string of
# answers so far (1=yes, 0=no, "-" for the root) and e_i are the elements of the question set S.
# Accepts iff against every legal responder the alive set has size 1 after at most Q questions.
import sys
def load(fn):
    tree={}; hdr=None
    for l in open(fn):
        l=l.strip()
        if not l or l.startswith('#'): continue
        if hdr is None:
            p=l.split(); hdr=(int(p[1]),int(p[3]),int(p[5])); continue
        path,_,els=l.partition(':')
        path='' if path.strip()=='-' else path.strip()
        if path in tree: raise SystemExit("REJECT duplicate node "+path)
        tree[path]=frozenset(int(e) for e in els.split())
    return hdr,tree
def alive(n,k,Ss,As,u1):
    # recompute C_0..C_t from scratch by the definition; return C_t
    Cprev=set(range(1,n+1)); ell=[]
    C=Cprev
    for u in range(1,len(Ss)+1):
        S=Ss[u-1]
        ell.append(min(len(S&Cprev),len(Cprev-S)))
        C=set()
        for x in range(1,n+1):
            L=[s for s in range(1,u+1) if (x in Ss[s-1])!=(As[s-1]==1)]
            if u1: ok=len(L)<=1
            else:
                Ls=set(L)
                ok=all(not any((w in Ls) for w in range(s+1,s+k*ell[s-1]+1)) for s in L)
            if ok: C.add(x)
        Cprev=C
    return C
def main():
    u1='--u1' in sys.argv
    (k,n,q),tree=load(sys.argv[1])
    worst=[0]; leaves=[0]
    def rec(path,Ss,As):
        C=alive(n,k,Ss,As,u1)
        if len(C)==1:
            worst[0]=max(worst[0],len(path)); leaves[0]+=1; return
        if len(path)>=q: raise SystemExit("REJECT: |C|=%d after %d questions on path %s"%(len(C),len(path),path))
        if path not in tree: raise SystemExit("REJECT: no question for path '%s'"%path)
        S=tree[path]; any_=False
        for a in (1,0):
            if len(alive(n,k,Ss+[S],As+[a],u1))==0: continue
            any_=True; rec(path+str(a),Ss+[S],As+[a])
        assert any_
    rec('',[],[])
    print("ACCEPT %s: %s n=%d wins within %d questions (worst case %d, %d terminal paths)"%(sys.argv[1],"one-lie game" if u1 else "k=%d"%k,n,q,worst[0],leaves[0]))
main()
