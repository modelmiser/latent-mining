#!/usr/bin/env python3
"""Print the adversary's winning tree for the history game at (d,B) (naive model, value windows->
compressed to (signs, x, rest) for display). usage: losetree.py d B maxdepth"""
import sys
from verify import step, obs, init, sg
d=int(sys.argv[1]); B=int(sys.argv[2]); MD=int(sys.argv[3])
memo={}
def lose_depth(bel, k):
    """min number of ticks within which adversary forces exit (<=k), else None"""
    if k==0: return None
    key=(bel,k)
    if key in memo: return memo[key]
    worst=0
    for u in (-1,0,1):
        parts={}; ok=True
        for s in bel:
            n=step(s,u,B)
            if n is None: ok=False;break
            for q in n: parts.setdefault(obs(q),set()).add(q)
        if not ok: r=1
        else:
            r=None
            for p in parts.values():
                x=lose_depth(frozenset(p),k-1)
                if x is not None and (r is None or x+1<r): r=x+1
        if r is None: worst=None;break
        worst=max(worst,r)
    memo[key]=worst
    return worst
def show(bel):
    return ' '.join(sorted({''.join('-0+'[sg(v)+1] for v in s[0][:-1])+':%+d%s'%(s[0][-1],'r' if s[1] else 'f') for s in bel}))
def pr(bel,k,ind,t):
    L=lose_depth(bel,k)
    print('  '*ind+'t=%d lose_in=%s  {%s}'%(t,L,show(bel)))
    if ind>MD: return
    for u in (-1,0,1):
        parts={}; ok=True
        for s in bel:
            n=step(s,u,B)
            if n is None: ok=False;break
            for q in n: parts.setdefault(obs(q),set()).add(q)
        if not ok: print('  '*ind+' u=%+d: exits now'%u); continue
        best=None
        for o,p in parts.items():
            x=lose_depth(frozenset(p),k-1)
            if x is not None and (best is None or x<best[0]): best=(x,o,p)
        print('  '*ind+' u=%+d: adversary shows next obs %s'%(u,'-0+'[best[1]+1]))
        pr(frozenset(best[2]),k-1,ind+1,t+1)
b0=frozenset([init(d)])
print('d',d,'B',B,'forced exit within',lose_depth(b0,60))
pr(b0,60,0,0)
