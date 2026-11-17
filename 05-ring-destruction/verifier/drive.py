#!/usr/bin/env python3
import sys, glob, os, random, collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from ref import *
BASE='.'
mode=sys.argv[1]
stats=collections.Counter()
def run(n,k,rounds,label):
    try:
        T=lemma_tests(n,k,rounds,stats); stats['plays']+=1; return T
    except Fail as e:
        stats['FAIL']+=1; print('FAIL',label,'n=%d k=%d'%(n,k),e); return None
if mode=='files':
    # every witness file of both solvers: legality, survival length, lemmas
    exp={}
    for path in sorted(glob.glob(BASE+'/solver-a/wit/*.txt')+glob.glob(BASE+'/solver-a/witgen/*.txt')+glob.glob(BASE+'/solver-b/surv_*.txt')+glob.glob(BASE+'/solver-b/witness_*.txt')+glob.glob(BASE+'/solver-b/sawtooth_*.txt')):
        n,k,R=parse(path)
        st,first,_=check(n,k,R,extra=2*n)
        nm=os.path.relpath(path,BASE)
        if st!='LEGAL': print('ILLEGAL',nm,st); stats['illegal']+=1; continue
        surv=first-1                      # rounds survived (with no destruction after the file ends)
        claim=None
        b=os.path.basename(path)
        if b.startswith('w_'): claim=int(b.split('_T')[1].split('.')[0])
        elif b.startswith('surv_') or b.startswith('sawtooth_') : claim=len(R)
        elif b.startswith('g_'): m=min(k,n); claim=n-cdiv(n,m)-1
        elif b.startswith('witness_'): claim=len(R)-1 if False else None
        listed_ok = (first>len(R))        # nobody complete within the listed rounds
        if claim is not None and surv<claim: print('CLAIM FAILS',nm,'claims survive',claim,'actually',surv); stats['claimfail']+=1
        stats['files']+=1
        key=(n,k); exp[key]=max(exp.get(key,0),first)
        if b.startswith('witness_'): print(nm,'len',len(R),'first completion',first)
        run(n,k,R,nm)
    print('best first-completion shown by any witness file, per (n,k):')
    for n in sorted(set(a for a,b in exp)):
        print(' n=%d:'%n,' '.join('k%d=%d'%(k,exp[(n,k)]) for (nn,k) in sorted(exp) if nn==n))
elif mode=='strat':
    N=int(sys.argv[2]); KK=int(sys.argv[3]); bad=0
    for n in range(3,N+1):
        for k in range(1,KK+1):
            for nm,S in (('A',strat_A),('B',strat_B)):
                T,R=S(n,k)
                if T<0: continue
                st,first,_=check(n,k,R,extra=2*n)
                want=n-cdiv(n,min(k,n))
                if st!='LEGAL' or first<want:
                    stats['strat_'+nm+'_below']+=1
                    if stats['strat_'+nm+'_below']<=25: print('strategy',nm,'n=%d k=%d'%(n,k),st,'first completion',first,'claimed >=',want)
                elif first>want: stats['strat_'+nm+'_above']+=1; print('strategy',nm,'n=%d k=%d EXCEEDS formula: '%(n,k),first,'>',want)
                else: stats['strat_'+nm+'_ok']+=1
                if st=='LEGAL' and n<=40: run(n,k,R,'strat'+nm)
elif mode=='dump':
    for path in sys.argv[2:]:
        for ln in open(path):
            parts=ln.split('|'); n,k=map(int,parts[0].split())
            R=[[tuple(map(int,x.split('>'))) for x in p.split()] for p in parts[1:]]
            run(n,k,R,path)
elif mode=='random':
    rng=random.Random(int(sys.argv[2])); cnt=int(sys.argv[3])
    for it in range(cnt):
        n=rng.randint(3,12); k=rng.randint(1,2*n if rng.random()<.3 else min(n-1,6))
        links=[(i,(i+1)%n) for i in range(n)]+[(i,(i-1)%n) for i in range(n)]
        R=[];prev=set()
        style=rng.random()
        for t in range(1,2*n):
            av=[l for l in links if l not in prev]
            if style<.5:
                # ramp-like: bias to continue chains (hit link one step against the flow of something hit 2 rounds ago)
                pref=[]
                if t>=3:
                    for (s,d) in R[t-3]:
                        dd=(d-(s-d)) % n if False else None
                    for (s,d) in R[t-3]:
                        step=(d-s)%n   # 1 for B-link, n-1 for A-link
                        if step==1: c=((s+1)%n,(d+1)%n)
                        else: c=((s-1)%n,(d-1)%n)
                        if c not in prev: pref.append(c)
                D=set(pref[:k])
                extra=rng.sample(av,min(len(av),max(0,k-len(D))))
                for l in extra:
                    if len(D)<k and rng.random()<.8: D.add(l)
            else:
                D=set(rng.sample(av,rng.randint(0,min(k,len(av)))))
            R.append(sorted(D)); prev=D
        run(n,k,R,'random')
print(mode,dict(stats))
