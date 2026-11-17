#!/usr/bin/env python3
"""Numerical check of LEMMA G (two-play lemma, general band B, odd d) on real safe clocks.
Lemma G_B: if a clock keeps |x|<=B, play A is negative on [tau-d,tau-1] with x_tau=alpha, play B' is positive on
[tau-d,tau-1] with x_tau=beta, and sgn(alpha)=sgn(beta), then
      N(tau,tau+d) - P(tau,tau+d) <= 2B - d - 1 + beta - alpha .
We enumerate ALL reachable states of eventually periodic clocks (exact closure), collect per phase the alpha's and
beta's, and test the inequality.  slack=0 is the lemma; slack=1 asks for one less on the right-hand side and MUST
fail somewhere (shows the check can fail and the lemma is tight)."""
import sys
sys.dont_write_bytecode=True
from verify_clock import parse
def sgn(x): return (x>0)-(x<0)
def run(prefix,cycle,d,B,slack):
    P=len(prefix); L=len(cycle)
    f=lambda i: prefix[i] if i<P else cycle[(i-P)%L]
    nxt=lambda i: i+1 if i+1<P+L else P
    s0=(0,(0,(0,)*d,0)); seen={s0}; st=[s0]
    while st:
        i,(x,sg,r)=st.pop(); u=f(i)[sg[0]+1]; ns=sg[1:]+(sgn(x),)
        for w in ((0,) if r else (0,1,-1)):
            n=(nxt(i),(x+u+w,ns,1 if w else 0))
            assert abs(x+u+w)<=B
            if n not in seen: seen.add(n); st.append(n)
    al={}; be={}
    for i,(x,sg,r) in seen:
        if all(v<0 for v in sg): al.setdefault(i,set()).add(x)
        if all(v>0 for v in sg): be.setdefault(i,set()).add(x)
    tested=0; worst=None
    for i in set(al)&set(be):
        j=i; D=0
        for _ in range(d): D+=f(j)[0]-f(j)[2]; j=nxt(j)
        for a in al[i]:
            for b in be[i]:
                if sgn(a)==sgn(b):
                    tested+=1; m=(2*B-d-1+b-a-slack)-D
                    if worst is None or m<worst: worst=m
    return tested,worst
if __name__=="__main__":
    hd=lambda: ([],[(1,0,-1),(0,0,0)])
    zoo=[(d,d+1,*hd(),'half-duty') for d in (1,3,5,7)]
    import glob,re
    for fn in sorted(glob.glob('feasible_*.txt')):
        Ls=open(fn).read().split('\n'); m=re.search(r'd=(\d+) B=(\d+) FEASIBLE',Ls[0]); a,b=Ls[1].split('|')
        zoo.append((int(m.group(1)),int(m.group(2)),parse(a),parse(b),fn))
    for slack in (0,1):
        print(f"== slack={slack} ({'the lemma: every margin must be >= 0' if slack==0 else 'NEGATIVE CONTROL: some margin must be < 0'}) ==")
        for d,B,pre,cyc,name in zoo:
            if d>7: continue
            t,w=run(pre,cyc,d,B,slack)
            print(f"  d={d} B={B} {name:22s}: same-sign (alpha,beta) configurations tested={t}, min margin={w}  {'OK' if (w is None or w>=0) else 'VIOLATED'}")
