#!/usr/bin/env python3
"""Reduced (delay-free) game explorer.  d odd, r=(d-1)/2.
y_c = x_c + u_c+..+u_{c+d-1}.  Controller at tick c+d sees sgn(y_c - S_c), S_c = sum of last d controls,
picks u; z = y+u must lie in [-R,R] (R=r for band d; R = r+k for band d+2k... we use general zband);
then y' = z + w (w=0 if rest).  Belief = (F,R) sets of y for free/rest.
usage: red.py policy dmin dmax [zband_offset]"""
import sys
from collections import deque
sg=lambda v:(v>0)-(v<0)
def run(d,pol,off=0,collect=None):
    r=(d-1)//2+off
    start=(frozenset([0]),frozenset(),(0,)*d)
    seen={start};q=deque([start]);nb=set()
    while q:
        F,Rs,U=q.popleft()
        Y=F|Rs; lo=min(Y);hi=max(Y)
        if pol=='mid': u=-sg(lo+hi)
        elif pol=='mid1':
            l=min([min(F)-1] if F else [])if F else 99; 
            l=min(([min(F)-1] if F else [])+([min(Rs)] if Rs else []))
            h=max(([max(F)+1] if F else [])+([max(Rs)] if Rs else []))
            u=-sg(l+h)
        if lo+u<-r or hi+u>r: return None,len(seen)
        if collect is not None: collect.append((F,Rs,U,u))
        U2=U[1:]+(u,);S=sum(U2)
        F2={y+u for y in Y}; R2={y+u+1 for y in F}|{y+u-1 for y in F}
        for o in (-1,0,1):
            Fo=frozenset(y for y in F2 if sg(y-S)==o); Ro=frozenset(y for y in R2 if sg(y-S)==o)
            if Fo or Ro:
                k=(Fo,Ro,U2)
                if k not in seen: seen.add(k);q.append(k)
    return True,len(seen)
if __name__=='__main__':
    pol=sys.argv[1]
    off=int(sys.argv[4]) if len(sys.argv)>4 else 0
    for d in range(int(sys.argv[2]),int(sys.argv[3])+1,2):
        print(d,pol,off,run(d,pol,off),flush=True)
