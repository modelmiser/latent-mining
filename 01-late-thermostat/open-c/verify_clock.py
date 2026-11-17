#!/usr/bin/env python3
"""Exhaustive verification of an eventually periodic clock (prefix + cycle) against ALL adversaries.
Reachability closure over (phase index, x, last d signs, rest); returns the exact sup |x| (or '>cap').
Also: best-response attack tests (attack.breach) against a zoo of clocks.
usage: verify_clock.py            -> runs the whole battery, prints a table"""
import sys, random, re
sys.dont_write_bytecode=True
from attack import breach, halfduty
def sgn(x): return (x>0)-(x<0)
def parse(s): return [tuple('-0+'.index(c)-1 for c in tok) for tok in s.split()]
def sup_abs(prefix,cycle,d,cap=200):
    """exact sup|x| over all adversary plays, for clock = prefix then cycle repeated."""
    P=len(prefix); L=len(cycle)
    def f(i): return prefix[i] if i<P else cycle[(i-P)%L]
    def nxt(i): return i+1 if i+1<P+L else P
    s0=(0,(0,(0,)*d,0)); seen={s0}; stack=[s0]; m=0
    while stack:
        i,(x,sg,r)=stack.pop(); u=f(i)[sg[0]+1]; ns=sg[1:]+(sgn(x),)
        for w in ((0,) if r else (0,1,-1)):
            nx=x+u+w; m=max(m,abs(nx))
            if abs(nx)>cap: return None
            n=(nxt(i),(nx,ns,1 if w else 0))
            if n not in seen: seen.add(n); stack.append(n)
    return m
def clock_fn(prefix,cycle):
    P=len(prefix); L=len(cycle)
    return lambda t: prefix[t] if t<P else cycle[(t-P)%L]
if __name__=="__main__":
    random.seed(20261116)
    print("== A. half-duty relay: exact sup|x| over all adversaries (closure), expected d+1 ==")
    for d in (1,3,5,7,9):
        m=sup_abs([],[(1,0,-1),(0,0,0)],d); print(f"  d={d}: sup|x| = {m}   {'OK' if m==d+1 else 'UNEXPECTED'}")
    print("== B. NEGATIVE CONTROL: ask the best-response adversary to breach band d+1 (i.e. force d+2) against half-duty: must NOT succeed ==")
    for d in (3,5,7):
        r=breach(halfduty,d,d+1,40*d); print(f"  d={d}: breach of band {d+1} within {40*d} ticks: {'NONE (control behaves)' if r is None else 'FOUND?! '+str(r)}")
    print("== C. best-response adversary breaches band d against half-duty (forces |x|>=d+1) ==")
    for d in (3,5,7,9):
        r=breach(halfduty,d,d,12*d); print(f"  d={d}: band {d} breached at t={r[0]}  w={''.join('-0+'[w+1] for w in r[1])}")
    print("== D. optimal clocks found by the solvers (band d+1 witnesses): exact sup must be d+1, and band d must be breached ==")
    zoo=[]
    txt=open('solver-b/clock_witnesses.log').read().split('\n')
    for i,l in enumerate(txt):
        m=re.match(r'd=(\d+) B=(\d+).*CLOCK FEASIBLE',l)
        if m:
            pre=cyc=None
            for l2 in txt[i+1:i+5]:
                if l2.strip().startswith('prefix:'): pre=parse(l2.split(':',1)[1])
                if l2.strip().startswith('cycle:'): cyc=parse(l2.split(':',1)[1])
            zoo.append((int(m.group(1)),int(m.group(2)),pre,cyc,'solver-b log'))
    import glob
    for fn in sorted(glob.glob('feasible_*.txt')):
        L=open(fn).read().split('\n'); m=re.search(r'd=(\d+) B=(\d+) FEASIBLE',L[0])
        if not m: continue
        a,b=L[1].split('|'); zoo.append((int(m.group(1)),int(m.group(2)),parse(a),parse(b),fn))
    for d,B,pre,cyc,src in zoo:
        m=sup_abs(pre,cyc,d); r=breach(clock_fn(pre,cyc),d,B-1,len(pre)+20*len(cyc)+12*d)
        print(f"  d={d} claimed band {B} [{src}]: exact sup|x|={m} {'OK' if m==B else 'MISMATCH'}; band {B-1} breached at t={r[0] if r else None}")
    print("== E. random clocks: best-response adversary must breach band d (odd d) ==")
    for d in (3,5,7):
        T=10*d; n=0; latest=0; fails=0
        gens={'uniform':lambda t:tuple(random.choice((-1,0,1)) for _ in range(3)),
              'relay-ish':lambda t:(random.choice((0,1,1)),random.choice((0,0,1,-1)),random.choice((0,-1,-1))),
              'halfduty+noise':lambda t:(halfduty(t) if random.random()<0.85 else (random.choice((0,1)),random.choice((-1,0,1)),random.choice((0,-1))))}
        for name,g in gens.items():
            N=300 if d<7 else 120; late=0
            for _ in range(N):
                seq=[g(t) for t in range(T)]; r=breach(lambda t:seq[t],d,d,T)
                if r is None: fails+=1
                else: late=max(late,r[0])
            print(f"  d={d} {name:15s}: {N} clocks, unbreached within {T} ticks: {fails}; latest breach t={late}")
