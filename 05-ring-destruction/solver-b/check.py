#!/usr/bin/env python3
"""Independent checker (sets, straight from the definition).
usage: check.py n k file [expect_rounds]
File lines: 't  src>dst src>dst ...' (lines starting with # ignored; a round with no destruction is 't').
Verifies: links exist, |D_t|<=k, D_t disjoint from D_{t-1}, rounds numbered 1..T consecutively,
and reports the first round at which some process is complete (or 'none through T').
Exit status 0 iff legal and no process is complete through the last listed round."""
import sys
def check(n,k,lines):
    K=[{i} for i in range(n)]; prev=set(); T=0
    for ln in lines:
        ln=ln.split('#')[0].split()
        if not ln: continue
        T+=1
        if int(ln[0])!=T: return ("ILLEGAL: round numbering",T)
        D=set()
        for tok in ln[1:]:
            s,d=map(int,tok.split('>'))
            if not(0<=s<n and 0<=d<n) or (d-s)%n not in (1,n-1): return ("ILLEGAL: not a link "+tok,T)
            D.add((s,d))
        if len(D)>k: return ("ILLEGAL: budget",T)
        if D&prev: return ("ILLEGAL: link destroyed twice in a row",T)
        new=[set(x) for x in K]
        for i in range(n):
            for j in ((i-1)%n,(i+1)%n):
                if (j,i) not in D: new[i]|=K[j]
        K=new; prev=D
        if any(len(x)==n for x in K): return ("COMPLETE",T)
    return ("OK",T)
if __name__=="__main__":
    n=int(sys.argv[1]);k=int(sys.argv[2])
    r=check(n,k,open(sys.argv[3]).read().splitlines())
    print(sys.argv[3],r)
    ok = r[0]=="OK" and (len(sys.argv)<5 or r[1]==int(sys.argv[4]))
    sys.exit(0 if ok else 1)
