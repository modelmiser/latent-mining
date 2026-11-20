#!/usr/bin/env python3
# Independent checker of upper-bound certificates. usage: check_paths.py pathsfile Dfile
# Each line "m r moves": run moves (A: a=2a, B: b=2b, C: a=a+b, D: b=a+b) over the INTEGERS from (1,1);
# require final a or b ≡ r (mod m), n>=1, and len(moves) <= D(m) claimed in Dfile; require every residue of every m present.
import sys
D={int(l.split()[0]):int(l.split()[1]) for l in open(sys.argv[2])}
seen={}; bad=0; mx={}
for l in open(sys.argv[1]):
    p=l.split(); m,r=int(p[0]),int(p[1]); mv=p[2] if len(p)>2 else ""
    a=b=1
    for c in mv:
        if c=='A':a*=2
        elif c=='B':b*=2
        elif c=='C':a+=b
        elif c=='D':b+=a
        else: bad+=1
    if not (a%m==r or b%m==r) or len(mv)>D[m]: bad+=1; print("REJECT",l.strip())
    seen.setdefault(m,set()).add(r); mx[m]=max(mx.get(m,0),len(mv))
for m in seen:
    if len(seen[m])!=m: bad+=1; print("MISSING residues for m",m)
    if mx[m]!=D[m]: print("note: max path length",mx[m],"!= D",D[m],"for m",m)
print("checked m:",len(seen),"lines ok" if bad==0 else "FAILURES %d"%bad)
