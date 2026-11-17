#!/usr/bin/env python3
"""Independent checker for ngclock certificates (written separately, tuple-based, no shared code).
Certificate: line 'd D B BAND', then nogoods in learned order: 'k s1 ... sk' (state ids).
State id = ((x+B)*3^d + sc)*2 + r ; sc = base-3 digits of (sgn x_{t-d},...,sgn x_{t-1})+1, oldest most significant.
Checks, for EVERY nogood K_i and EVERY one of the 27 maps f=(f(-),f(0),f(+)):
    the closed one-step image of K_i under f leaves the band, or contains some nogood K_j with j<i.
and finally that the initial state {(0, 0^d, free)} (closed) contains a nogood.
If all pass: by induction every K_i is a dead set, the initial set is dead => band B infeasible for clocks.
usage: ngcheck.py certfile [override_B]"""
import sys, itertools
def sgn(x): return (x>0)-(x<0)
def main():
    path=sys.argv[1]
    with open(path) as fh:
        hd=fh.readline().split(); d=int(hd[1]); B=int(hd[3])
        ngs=[tuple(map(int,l.split()[1:])) for l in fh if l.strip()]
    Bchk=int(sys.argv[2]) if len(sys.argv)>2 else B      # band used for the violation test
    P3=3**d
    def dec(s):
        r=s&1; q=s>>1; sc=q%P3; x=q//P3-B
        win=[]; 
        for _ in range(d): win.append(sc%3-1); sc//=3
        return x,tuple(win[::-1]),r
    def enc(x,win,r):
        sc=0
        for v in win: sc=sc*3+v+1
        return ((x+B)*P3+sc)*2+r
    maps=list(itertools.product((-1,0,1),repeat=3))
    bucket={}   # key element -> list of earlier nogoods (as frozensets) keyed at their first element
    def has_ng(T):
        for e in T:
            for K in bucket.get(e,()):
                if K<=T: return True
        return False
    cache={}
    def succ(s,u):
        key=(s,u)
        if key in cache: return cache[key]
        x,win,r=dec(s); nw=win[1:]+(sgn(x),); out=[]
        for w in ((0,) if r else (-1,0,1)):
            nx=x+u+w
            if abs(nx)>Bchk: out=None; break
            out.append(enc(nx,nw,1 if w else 0))
            if w==0: out.append(enc(nx,nw,1))          # closure: free => also rest twin
        cache[key]=out; return out
    for i,K in enumerate(ngs):
        if len(K)==0: print("FAIL: empty nogood",i); return 1
        decs=[(s,dec(s)[1][0]) for s in K]
        for f in maps:
            T=set(); bad=False
            for s,o in decs:
                r=succ(s,f[o+1])
                if r is None: bad=True; break
                T.update(r)
            if bad: continue
            if not has_ng(T):
                print(f"FAIL: nogood #{i} (size {len(K)}) not refuted for f={f}"); return 1
        bucket.setdefault(K[0],[]).append(frozenset(K))
        if i%20000==0 and i: print("  checked",i,file=sys.stderr,flush=True)
    s0=enc(0,(0,)*d,0); root={s0,s0|1}
    if not has_ng(root): print("FAIL: initial set contains no nogood"); return 1
    print(f"OK: d={d} band={Bchk}: {len(ngs)} nogoods verified (27 maps each); initial set is dead => no clock holds |x|<={Bchk}")
    return 0
sys.exit(main())
