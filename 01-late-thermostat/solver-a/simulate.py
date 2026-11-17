"""Independent simulation (true window of x kept explicitly; observation read from the true x_{t-d}).
Controllers: relay (1,0,-1); half-duty clock (relay on even ticks, 0 on odd); history 'mid1' (belief midpoint).
Adversaries: random, greedy-away-from-zero, greedy-with-random-idling, alternating-phase. Reports max|x| seen."""
import random, sys
from game import sgn
def simulate(d, ctrl, adv, T, rng):
    xs={}; X=lambda s: xs.get(s,0) if s>0 else 0
    x=0; rest=0; mx=0; bel=frozenset([(0,(0,)*(d-1),0)]); pending=None
    for t in range(T):
        o=sgn(X(t-d))
        if ctrl=='mid1':
            if t>0:
                bel=frozenset(s for (oo,s) in pending if oo==o)
                true=(x,tuple(sgn(X(t-d+1+i)) for i in range(d-1)),rest)
                assert true in bel, "true state not in belief"
            lo=min(s[0]-(0 if s[2] else 1) for s in bel); hi=max(s[0]+(0 if s[2] else 1) for s in bel); u=-sgn(lo+hi)
            pending=set()
            for (bx,sg,br) in bel:
                full=sg+(sgn(bx),)
                for w in ((0,) if br else (0,1,-1)): pending.add((full[0],(bx+u+w,full[1:],1 if w else 0)))
        elif ctrl=='relay': u=-o
        else: u=-o if t%2==0 else 0
        if rest: w=0
        else:
            y=x+u
            if adv=='random': w=rng.choice((-1,0,1))
            elif adv=='greedy': w=1 if y>0 else (-1 if y<0 else rng.choice((-1,1)))
            elif adv=='lazy': w=0 if rng.random()<0.3 else (1 if y>0 else (-1 if y<0 else rng.choice((-1,1))))
            elif adv=='runs':   # push one direction for a random stretch, then flip
                if t==0 or rng.random()<1.0/(2*d+2): simulate.dir=rng.choice((-1,1))
                w=simulate.dir
            else: w=rng.choice((0,1))*(1 if (t//(3*d+1))%2==0 else -1)
        x=x+u+w; rest=1 if w else 0; xs[t+1]=x; mx=max(mx,abs(x))
        xs.pop(t-d-2,None)
    return mx
rng=random.Random(12345); T=int(sys.argv[2]) if len(sys.argv)>2 else 20000
for d in range(1,int(sys.argv[1])+1):
    row={}
    for ctrl,bound in (('relay',(3*d)//2+1),('clock',d+1),('mid1',2*(d//2)+1)):
        m=0
        for adv in ('random','greedy','lazy','runs','phase'):
            for rep in range(3): m=max(m,simulate(d,ctrl,adv,T,rng))
        row[ctrl]=(m,bound); assert m<=bound,(d,ctrl,m,bound)
    print(d,row,flush=True)
