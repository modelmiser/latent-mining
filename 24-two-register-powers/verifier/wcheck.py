#!/usr/bin/env python3
# referee witness checker. Input normalised to tuples (m, r, word). Uses MY tables (vals.txt, my.pkl).
# accept iff: every m in [2,mmax] present, each m has every residue exactly once, word over ABCD,
# replay over the integers from (1,1) leaves a or b ≡ r (mod m), len(word) <= my D(m).
# also reports how many words are longer than my exact g_m(r).
import sys,pickle
S=""
my,levN=pickle.load(open(S+"verifier/my.pkl","rb"))
byn=[tuple(map(int,l.split())) for l in open(S+"verifier/vals.txt")]
def gtab(m):
    g=[99]*m
    for n,gg in byn:
        if gg<g[n%m]: g[n%m]=gg
    return g
def replay(w):
    a=b=1
    for ch in w:
        if ch=='A': a=a+a
        elif ch=='B': b=b+b
        elif ch=='C': a=a+b
        elif ch=='D': b=a+b
        else: return None
    return a,b
def check(items,mmax,exact=False,claimed_g=None):
    errs=[]; per={}; nonmin=0
    for (m,r,w) in items:
        if m in per and r in per[m]: errs.append(("dup",m,r)); continue
        per.setdefault(m,{})[r]=w
    for m in range(2,mmax+1):
        if m not in per: errs.append(("missing m",m)); continue
        g=gtab(m)
        if sorted(per[m])!=list(range(m)): errs.append(("residue set",m))
        for r,w in per[m].items():
            if not (0<=r<m): errs.append(("r range",m,r)); continue
            st=replay(w)
            if st is None: errs.append(("letter",m,r)); continue
            if st[0]%m!=r and st[1]%m!=r: errs.append(("not reached",m,r))
            if len(w)>my[m][0]: errs.append(("too long",m,r,len(w)))
            if len(w)<g[r]: errs.append(("IMPOSSIBLE shorter than my g",m,r))
            if len(w)>g[r]:
                nonmin+=1
                if exact: errs.append(("not minimal",m,r))
    for m in per:
        if not 2<=m<=mmax: errs.append(("extra m",m))
    return errs,nonmin
def loadA(fn):
    out=[]
    for l in open(fn):
        p=l.split()
        if not p: continue
        out.append((int(p[0]),int(p[1]),p[2] if len(p)>2 else ""))
    return out
def loadB(fn):
    out=[];D={};bad=[]
    for l in open(fn):
        p=l.split()
        if p[0]=="D": D[int(p[1])]=int(p[2])
        elif p[0]=="W":
            w="" if p[4]=="-" else p[4]
            if len(w)!=int(p[3]): bad.append(("g field",p[1],p[2]))
            out.append((int(p[1]),int(p[2]),w))
    return out,D,bad
def verdict(name,errs,nonmin=None):
    print(name,"->","ACCEPT" if not errs else "REJECT (%d errors, first %s)"%(len(errs),errs[0]),"" if nonmin is None else "non-minimal words: %d"%nonmin)
if __name__=="__main__":
    A=loadA(S+"solver-a/paths_m400.txt")
    e,nm=check(A,400); verdict("A paths_m400.txt (%d lines)"%len(A),e,nm)
    B,BD,bad=loadB(S+"solver-b/dm128w.out")
    e,nm=check(B,128,exact=True); e+=bad; e+=[("D line",m) for m in BD if BD[m]!=my[m][0]]; e+=[("D lines missing",)] if sorted(BD)!=list(range(2,129)) else []
    verdict("B dm128w.out (%d W lines, exact-length mode)"%len(B),e,nm)
    # the solvers' own deliberately broken files
    e,nm=check(loadA(S+"solver-a/bad_paths_test.txt"),400); verdict("A bad_paths_test.txt",e)
    for fn in ("bad_w.txt","bad_w2.txt"):
        X,XD,bad=loadB(S+"solver-b/"+fn); e,nm=check(X,128,exact=True); e+=bad; e+=[("D line",m) for m in XD if XD[m]!=my[m][0]]
        verdict("B "+fn,e)
    # my own broken witnesses, built from A's file
    import copy
    def mut(f):
        X=copy.copy(A); f(X); return X
    def m1(X):  # flip one letter in the word for m=389 r=worst
        i=next(i for i,(m,r,w) in enumerate(X) if m==389 and len(w)==11); m_,r,w=X[i]; X[i]=(m_,r,w[:5]+("C" if w[5]!="C" else "D")+w[6:])
    def m2(X):  # drop a residue
        i=next(i for i,(m,r,w) in enumerate(X) if m==200 and r==77); del X[i]
    def m3(X):  # valid but over-long word: append two A-steps then fix? -> use binary method word for a residue, longer than D
        i=next(i for i,(m,r,w) in enumerate(X) if m==255 and r==254)
        n=254; bits=bin(n)[3:]; w="".join("B"+("D" if c=="1" else "") for c in bits); assert replay(w)[1]==254
        X[i]=(255,254,w)
    def m4(X):  # relabel residue: word for r given to r+1 and vice versa is dup-free but wrong
        i=next(i for i,(m,r,w) in enumerate(X) if m==300 and r==10); j=next(i for i,(m,r,w) in enumerate(X) if m==300 and r==11)
        X[i],X[j]=(300,10,X[j][2]),(300,11,X[i][2])
    def m5(X):  # illegal letter (subtraction-like 'S')
        i=next(i for i,(m,r,w) in enumerate(X) if m==17 and r==5); X[i]=(17,5,"S"+X[i][2])
    def m6(X):  # duplicate a line in place of another residue
        i=next(i for i,(m,r,w) in enumerate(X) if m==64 and r==3); X[i]=X[i+1]
    for nm_,f in (("flip letter",m1),("drop residue",m2),("valid but longer than D(m)",m3),("swap two residues' words",m4),("illegal letter",m5),("duplicate line",m6)):
        e,_=check(mut(f),400); verdict("MY broken witness: "+nm_,e)
