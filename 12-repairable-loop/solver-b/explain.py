#!/usr/bin/env python3
"""explain.py n w : for each distinct adversary move list all mechanic replies that give a universal loop (independent python checker).
exit status 0 iff w repairable. Writes a strategy table to stdout."""
import sys, itertools
def words(v,n):
    m=len(v); return {tuple(v[(i+j)%m] for j in range(n)) for i in range(m)}
def universal(v,n): return len(words(v,n))==2**n
def edits(v):
    m=len(v); out=[("pass",-1,tuple(v))]
    if m>=2:
        for k in range(m): out.append(("loss",k,tuple(v[:k]+v[k+1:])))
    for k in range(m): out.append(("dbl",k,tuple(v[:k+1]+v[k:])))
    return out
def canon(v): return min(v[i:]+v[:i] for i in range(len(v)))
def s(v): return "".join(map(str,v))
def main():
    n=int(sys.argv[1]); w=tuple(int(c) for c in sys.argv[2]); ok=True; seen=set()
    print("w=",s(w),"len",len(w),"universal" if universal(w,n) else "NOT universal")
    for (a,k,w1) in edits(w):
        if w1 in seen: continue
        seen.add(w1)
        reps=[]; s2=set()
        for (b,j,w2) in edits(w1):
            if w2 in s2: continue
            s2.add(w2)
            if universal(w2,n): reps.append((b,j))
        if not reps: ok=False
        print("%-4s %3d -> %s : %s"%(a,k,s(w1), " ".join("%s@%d"%r for r in reps) if reps else "NO REPLY"))
    print("REPAIRABLE" if ok else "NOT REPAIRABLE"); sys.exit(0 if ok else 1)
main()
