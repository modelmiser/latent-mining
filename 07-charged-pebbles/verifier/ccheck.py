#!/usr/bin/env python3
# Referee closed-set checker. usage: ccheck.py p k file  -> prints max clean n if start present and set closed.
import sys,re
def parse(l):
    b,h=l.split('|'); bd=tuple(sorted(tuple(map(int,x.split(':'))) for x in re.split(r'[ ,]+',b.strip()) if x))
    hd=tuple(sorted(int(x) for x in re.split(r'[ ,]+',h.strip()) if x)); return (bd,hd)
def succ(st):
    bd,hd=st; d=dict(bd); out=[]
    for c in set(hd):
        h=list(hd); h.remove(c)
        for t in [1]+[i+1 for i in d]:
            if t not in d: out.append((tuple(sorted(bd+((t,c),))),tuple(h)))
    for i,c in bd:
        rest=tuple(x for x in bd if x[0]!=i)
        if i==1 or i-1 in d: out.append((rest,tuple(sorted(hd+(c,)))))
        elif c>=1: out.append((rest,tuple(sorted(hd+(c-1,)))))
    return out
def check(p,k,lines):
    S=set(parse(l) for l in lines if l.strip() and not l.startswith('#'))
    if ((),tuple([k]*p)) not in S: return "REJECT: start missing"
    for st in S:
        if len(st[0])+len(st[1])!=p: return "REJECT: wrong pebble count"
        for n in succ(st):
            if n not in S: return "REJECT: not closed at %s -> %s"%(st,n)
    return "OK states=%d maxclean=%d"%(len(S),max([s[0][0][0] for s in S if len(s[0])==1]+[0]))
if __name__=="__main__":
    import glob,os
    B=''
    rd=lambda f: open(f).read().split("\n")
    for f in sorted(glob.glob(B+'solver-a/cert/closed*')+glob.glob(B+'solver-b/cert_*')):
        p,k=map(int,re.findall(r'(\d+)_k?(\d+)\.txt',f)[0]); print(os.path.relpath(f,B),p,k,check(p,k,rd(f)),"formula",(p*k+2)*2**(p-2))
    print("A BAD:",check(3,1,rd(B+'solver-a/cert/BAD_closed_p3_k1.txt'))[:60])
    print("B neg:",check(3,1,rd(B+'solver-b/neg/cert_3_1_state_deleted.txt'))[:60])
    L=[l for l in rd(B+'solver-b/cert_3_1.txt') if l.strip()]
    print("mine wrong k:",check(3,2,L)[:60]); print("mine drop last:",check(3,1,L[:-1])[:60]); print("mine drop mid:",check(3,1,L[:200]+L[201:])[:60])
    print("mine only clean<=9 removed:",check(3,1,[l for l in L if not l.startswith('10:')])[:60])
