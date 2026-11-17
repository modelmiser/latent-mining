#!/usr/bin/env python3
"""Direct, slow implementation of the definition. usage: slow.py n L  -> lists all repairable loops of length L (as strings)"""
import sys, itertools
def covers(v, c):
    m=len(v); n=len(c)
    return any(all(v[(i+j)%m]==c[j] for j in range(n)) for i in range(m))
def universal(v, n):
    return all(covers(v,c) for c in itertools.product((0,1),repeat=n))
def edits(v):
    m=len(v); out=[tuple(v)]            # pass
    if m>=2:
        for k in range(m): out.append(tuple(v[:k]+v[k+1:]))        # loss
    for k in range(m): out.append(tuple(v[:k+1]+v[k:]))            # doubling: copy of v_k right after v_k
    return out
def repairable(w, n):
    w=tuple(w)
    for w1 in edits(w):
        if not any(universal(w2,n) for w2 in edits(w1)): return False
    return True
if __name__=="__main__":
    n=int(sys.argv[1]); L=int(sys.argv[2]); cnt=0
    for w in itertools.product((0,1),repeat=L):
        if repairable(w,n):
            cnt+=1; print("".join(map(str,w)))
    print("COUNT",n,L,cnt)
