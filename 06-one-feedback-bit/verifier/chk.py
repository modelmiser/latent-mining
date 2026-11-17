#!/usr/bin/env python3
# Referee witness checker, from the problem definition only.
import sys
def D(s): return {s[:i]+s[i+1:] for i in range(len(s))}
def O(u,a,b): return {u+a} | {u+x for x in D(a)} | {x+b for x in D(u)}
def load(path):
    out=[]
    for ln in open(path):
        ln=ln.strip()
        if not ln or ln.startswith('#'): continue
        p=[('' if x=='-' else x) for x in ln.split()]
        out.append(tuple(p))
    return out
def check(n,t,strats,need=None):
    if need is not None and len(strats)<need: return "FAIL: only %d strategies, need %d"%(len(strats),need)
    seen={}
    for i,s in enumerate(strats):
        if len(s)!=3: return "FAIL: line %d not a triple"%i
        u,a,b=s
        if any(c not in '01' for c in u+a+b): return "FAIL: non-binary in %r"%(s,)
        if len(u)!=t or len(a)!=n-t or len(b)!=n-t: return "FAIL: wrong lengths in %r"%(s,)
        o=O(u,a,b)
        if any(len(w) not in (n,n-1) for w in o): return "FAIL: internal"
        for w in o:
            if w in seen: return "FAIL: outcome %r shared by %r and %r"%(w,strats[seen[w]],s)
        for w in o: seen[w]=i
    return "OK %d"%len(strats)
if __name__=="__main__":
    n,t=int(sys.argv[1]),int(sys.argv[2]); need=int(sys.argv[4]) if len(sys.argv)>4 else None
    print(sys.argv[3],check(n,t,load(sys.argv[3]),need))
