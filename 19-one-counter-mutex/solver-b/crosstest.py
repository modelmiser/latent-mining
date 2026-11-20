#!/usr/bin/env python3
"""Compare ./check (C++) with slowcheck.py on (a) every single-field mutation of the witness files,
(b) random legal systems.  Also shows deliberately wrong objects being rejected."""
import subprocess, random, sys, os, collections
import slowcheck
def dump(n,b,procs,fn):
    with open(fn,'w') as f:
        f.write("%d %d\n"%(n,b))
        for st in procs:
            f.write("%d\n"%len(st))
            for a in st:
                f.write(("R "+" ".join(map(str,a[1])) if a[0]=='R' else "A %d %d"%(a[1],a[2]))+"\n")
def fast(fn):
    out=subprocess.run(["./check",fn],capture_output=True,text=True).stdout.split()
    return out[0] if out else "?"
tally=collections.Counter(); bad=0
def compare(n,b,procs):
    global bad
    dump(n,b,procs,"_tmp_sys.txt"); a=fast("_tmp_sys.txt"); s=slowcheck.check(n,b,procs)
    tally[(a,s)]+=1
    if a!=s: bad+=1; print("MISMATCH",a,s,procs)
for fn in sys.argv[1:]:
    n,b,procs=slowcheck.load(fn)
    for p in range(n):
        for s in range(len(procs[p])):
            a=procs[p][s]; alts=[]
            if a[0]=='R':
                for v in range(b):
                    for q in range(len(procs[p])):
                        if q!=a[1][v]:
                            l=list(a[1]); l[v]=q; alts.append(('R',l))
            else:
                for c in range(b):
                    for q in range(len(procs[p])):
                        if (c,q)!=(a[1],a[2]): alts.append(('A',c,q))
            alts.append(('A',1,a[1][0]) if a[0]=='R' else ('R',[a[2]]*b))
            for alt in alts:
                pr=[list(x) for x in procs]; pr[p][s]=alt; compare(n,b,pr)
random.seed(7)
for _ in range(1500):
    n=random.choice([2,2,3]); b=random.choice([2,3,4]); procs=[]
    for p in range(n):
        k=random.randint(3,5); st=[]
        for s in range(k):
            succ=lambda: 0 if s==1 else random.choice([1]+list(range(2,k))*2)
            if random.random()<0.6: st.append(('R',[succ() for _ in range(b)]))
            else: st.append(('A',random.randrange(b),succ()))
        procs.append(st)
    compare(n,b,procs)
for k,v in sorted(tally.items()): print(k,v)
print("mismatches:",bad)
