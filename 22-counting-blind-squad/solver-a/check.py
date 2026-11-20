#!/usr/bin/env python3
# Independent slow implementation, straight from the definition.
# States are strings; rule is a dict (state,k)->state. Cycle detection by visited set.
import sys, itertools
def names(s): return ['Q','G','F']+['U%d'%i for i in range(1,s-2)]
def run(d,n):
    """firing time (>=1) if d synchronises n; 0 if it fails; raises KeyError((x,k)) if entry missing"""
    c=tuple(['G']+['Q']*(n-1)); seen={c}; t=0
    while True:
        new=[]
        for i,x in enumerate(c):
            k=0
            if i-1>=0 and c[i-1]!=x: k+=1
            if i+1<n and c[i+1]!=x: k+=1
            new.append(d[(x,k)])
        t+=1; c=tuple(new)
        nf=sum(1 for x in c if x=='F')
        if nf==n: return t
        if nf>0: return 0
        if c in seen: return 0
        seen.add(c)
def Lrule(d,cap=300):
    n=2; times=[]
    while n<=cap:
        t=run(d,n)
        if t==0: break
        times.append(t); n+=1
    return n-1,times
def brute(s):
    S=names(s); keys=[(x,k) for x in S if x!='F' for k in range(3) if (x,k)!=('Q',0)]
    hist={}; best=[]
    for vals in itertools.product(S,repeat=len(keys)):
        d=dict(zip(keys,vals)); d[('Q',0)]='Q'
        L,times=Lrule(d); hist[L]=hist.get(L,0)+1
        best.append((L,times,vals))
    return hist,best,keys
def lazy(s):
    """lazy DFS, NO symmetry breaking; counts full rules by weight s^(#undefined)"""
    S=names(s); nfree=3*(s-1)-1; hist={}
    d={('Q',0):'Q'}
    def rec(n):
        while True:
            try: t=run(d,n)
            except KeyError as e:
                key=e.args[0]
                for v in S:
                    d[key]=v; rec(n)
                del d[key]; return
            if t==0:
                w=s**(nfree-(len(d)-1)); hist[n-1]=hist.get(n-1,0)+w; return
            n+=1
    rec(2); return hist
def parse(line,s):
    S=names(s); num=['Q','G','F']+S[3:]
    v=[int(x) for x in line.split()]
    keys=[(x,k) for x in S if x!='F' for k in range(3)]
    assert len(v)==len(keys)
    d={kk:num[x] for kk,x in zip(keys,v)}
    assert d[('Q',0)]=='Q'
    return d
if __name__=='__main__':
    m=sys.argv[1]
    if m=='brute':
        s=int(sys.argv[2]); hist,best,keys=brute(s); print('brute s=%d'%s,sorted(hist.items()))
        # time lemma check: n<m both synchronised => t(n)>=2n-1 or t(n)==t(m)
        bad=0
        for L,times,vals in best:
            for a in range(len(times)):
                for b in range(a+1,len(times)):
                    n=a+2
                    if not (times[a]>=2*n-1 or times[a]==times[b]): bad+=1
        print('time-lemma violations:',bad)
        if s==3:
            S=names(3)
            ok=[vals for vals in itertools.product(S,repeat=5) if run(dict(list(zip(keys,vals))+[(('Q',0),'Q')]),3)>0]
            print('s=3 rules synchronising n=3:',len(ok),'with (dQ1,dG1) in',sorted(set((dict(zip(keys,v))[('Q',1)],dict(zip(keys,v))[('G',1)]) for v in ok)))
        if s==4:
            S=names(4)
            bad=[vals for L,t,vals in best if L>=3 and dict(zip(keys,vals))[('Q',1)] in ('Q','F')]
            print('s=4 rules with L>=3 and d(Q,1) in {Q,F}:',len(bad))
    elif m=='lazy':
        s=int(sys.argv[2]); h=lazy(s); print('lazy s=%d'%s,sorted(h.items()),'total',sum(h.values()),'expected',s**(3*(s-1)-1))
    elif m=='witness':
        s=int(sys.argv[2]); L=int(sys.argv[3]); d=parse(sys.argv[4],s)
        got,times=Lrule(d)
        print('rule synchronises n=2..%d, fails n=%d; times'%(got,got+1),times, 'ACCEPT' if got==L else 'REJECT')
