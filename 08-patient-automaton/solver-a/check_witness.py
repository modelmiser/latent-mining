#!/usr/bin/env python3
# check_witness.py : stand-alone checker (no shared code with wd.cpp) for a claimed patient automaton.
# usage: check_witness.py A B first wait strike ra rb [DMAX]
#   A,B: images of states 1..n under a,b (digit strings).  Strategy sigma_d:
#   x_1 = first; x_t = wait for t != 1, tau; x_tau = strike, where tau = least t >= max(d+2,3)
#   with t = ra (mod 3) if y_1 = a, t = rb (mod 3) if y_1 = b.
# Part 1: sigma_d wins for d = 0..DMAX (all Spoiler plays, tracked as the set of possible images; plus literal
#         enumeration of all 2^(T-1) Spoiler plays for d <= 8).
# Part 2: not blind-winnable: exhaustive closure of the reachable "sets of possible images" (BFS), no winning letter.
# Part 3: (only for the n=4 witness) the 4-family pair certificate used in proofs.txt.
import sys, itertools
A=[int(c)-1 for c in sys.argv[1]]; B=[int(c)-1 for c in sys.argv[2]]
first,wait,strike=sys.argv[3:6]; ra,rb=int(sys.argv[6]),int(sys.argv[7]); DMAX=int(sys.argv[8]) if len(sys.argv)>8 else 300
n=len(A); L={'a':A,'b':B}; Q=frozenset(range(n))
def im(S,x): return frozenset(L[x][i] for i in S)
def tau(d,y1):
    t=max(d+2,3); r=ra if y1=='a' else rb
    while t%3!=r: t+=1
    return t
def xword(d,y1):
    T=tau(d,y1); return [None]+[first if t==1 else (strike if t==T else wait) for t in range(1,T+1)]
ok=True; Ts=[]
for d in range(DMAX+1):
    wa,wb=xword(d,'a'),xword(d,'b')
    # legality: letters x_1..x_{d+1} may not depend on y_1
    if wa[1:d+2]!=wb[1:d+2]: ok=False; print("ILLEGAL strategy at d=",d)
    for y1,w in (('a',wa),('b',wb)):
        T=len(w)-1; fam={im(im(Q,w[1]),y1)}
        for t in range(2,T+1):
            fam={im(S,w[t]) for S in fam}
            if t<T: fam={im(S,y) for S in fam for y in 'ab'}
        if any(len(S)!=1 for S in fam): ok=False; print("FAIL d=%d y1=%s T=%d"%(d,y1,T))
    Ts.append(max(tau(d,'a'),tau(d,'b')))
    if d<=8:  # literal enumeration of every Spoiler play of length T-1
        T=Ts[-1]
        for ys in itertools.product('ab',repeat=T-1):
            w=xword(d,ys[0]); S=Q
            for t in range(1,T+1):
                S=im(S, w[t] if t<len(w) else wait)
                if t<T: S=im(S,ys[t-1])
            if len(S)!=1: ok=False; print("FAIL(enum) d=%d ys=%s"%(d,''.join(ys)))
print("PART1 strategy wins for d=0..%d: %s ; T(d) for d=0..11: %s"%(DMAX,ok,Ts[:12]))
# Part 2
def norm(F): return frozenset(S for S in F if not any(S<T for T in F))
start=norm({Q}); seen={start}; todo=[start]; blindwin=False
while todo:
    F=todo.pop()
    for x in 'ab':
        if all(len(im(S,x))==1 for S in F): blindwin=True
        G=norm({im(im(S,x),y) for S in F for y in 'ab'})
        if G not in seen: seen.add(G); todo.append(G)
print("PART2 reachable families: %d ; blind-winnable: %s"%(len(seen),blindwin))
with open("blind_cert_%s_%s.txt"%(sys.argv[1],sys.argv[2]),"w") as f:
    f.write("# every family (maximal possible images before a Synchronizer letter) reachable in the blind game; closed under both letters; none has a letter making all members singletons\n")
    for F in sorted(seen,key=lambda F:sorted(map(sorted,F))): f.write(" ".join("".join(str(i+1) for i in sorted(S)) for S in sorted(F,key=sorted))+"\n")
# Part 3
if n==4 and sys.argv[1]=="2213":
    P=lambda s: frozenset(int(c)-1 for c in s)
    fams=[{P('13'),P('14')},{P('14'),P('23')},{P('12'),P('13'),P('34')},{P('12'),P('14'),P('34')}]
    good=True
    for F in fams:
        for x in 'ab':
            succ={im(im(S,x),y) for S in F for y in 'ab'}; succ={S for S in succ if len(S)==2}
            if not any(G<=succ for G in fams): good=False
    for x in 'ab':
        opts={frozenset(p) for y in 'ab' for p in itertools.combinations(sorted(im(im(Q,x),y)),2)}
        if not any(G<=opts for G in fams): good=False
        if len(im(Q,x))==1: good=False
    print("PART3 pair-family certificate valid:",good)
print("VERDICT:", "PATIENT (to d=%d)"%DMAX if ok and not blindwin else "REJECTED")
