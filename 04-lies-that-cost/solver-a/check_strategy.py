#!/usr/bin/env python3
"""Independent checker (shares no code with fast.cpp).  Usage: check_strategy.py FILE n k q
FILE lines:  '<path> ? e1 e2 ...'  question S asked after answer path (Y/N string, '-' = empty)
             '<path> = x'          claims C = {x} after that path
             '<path> X'            claims that answer is illegal (C empty)
Verifies from the DEFINITION (pairs (x,L), conditions (i),(ii)) that along every legal answer sequence
the questioner reaches |C_t|=1 at some t<=q."""
import sys
def C_of(n, k, hist):
    Cs=[set(range(1,n+1))]; ells=[]
    for t in range(1,len(hist)+1):
        S=hist[t-1][0]; Cp=Cs[-1]; ells.append(min(len(S&Cp),len(Cp-S)))
        C=set()
        for x in range(1,n+1):
            L=[s for s in range(1,t+1) if (x in hist[s-1][0])!=hist[s-1][1]]
            if all(not any(s+1<=u<=s+k*ells[s-1] for u in L) for s in L): C.add(x)
        Cs.append(C)
    return Cs[-1]
def main():
    f,n,k,q=sys.argv[1],int(sys.argv[2]),int(sys.argv[3]),int(sys.argv[4])
    Qs={}; leaf={}; dead=set()
    for line in open(f):
        if line.startswith('#') or not line.strip(): continue
        p=line.split(); path='' if p[0]=='-' else p[0]
        if p[1]=='?': Qs[path]=frozenset(int(v) for v in p[2:])
        elif p[1]=='=': leaf[path]=int(p[2])
        elif p[1]=='X': dead.add(path)
    paths=0; maxd=0
    stack=[('',())]
    while stack:
        path,hist=stack.pop()
        C=C_of(n,k,list(hist))
        if len(C)==0:
            continue  # responder may not give this answer
        if len(C)==1:
            if len(path)>q: sys.exit(f"FAIL: path {path} longer than q")
            if path in leaf and leaf[path] not in C: sys.exit(f"FAIL: wrong leaf at {path}")
            paths+=1; maxd=max(maxd,len(path)); continue
        if len(path)>=q: sys.exit(f"FAIL: at path '{path}' still |C|={len(C)} after {len(path)} questions")
        if path not in Qs: sys.exit(f"FAIL: no question for path '{path}' with |C|={len(C)}")
        S=Qs[path]
        stack.append((path+'Y',hist+((S,True),))); stack.append((path+'N',hist+((S,False),)))
    print(f"OK: n={n} k={k}: every legal answer sequence ends with |C|=1 within {maxd} <= {q} questions ({paths} leaves)")
main()
