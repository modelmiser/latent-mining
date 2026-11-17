# Literal implementation of the definition (no state abstraction): searches Synchronizer strategies
# x_t = f(x_1..x_{t-1}, y_1..y_{t-1-d}) as an AND-OR tree, win = image singleton after x_T for ALL spoiler plays.
# Compares with game.cpp 'dump' output (exact minimal T from the fast rank solver).
import sys, itertools, random, subprocess
def run(n, dmax, Tmax, codes):
    out=subprocess.run(['./game','dump',str(n),str(dmax)],capture_output=True,text=True).stdout.split('\n')
    fast={}
    for l in out:
        if l.strip():
            p=list(map(int,l.split())); fast[p[0]]=p[1:]
    bad=0; checked=0
    for code in codes:
        c=code; t=[[0]*n,[0]*n]
        for x in range(2):
            for q in range(n): t[x][q]=c%n; c//=n
        def image(xs,ys):  # word x1 y1 ... x_{T-1} y_{T-1} x_T, len(ys)=len(xs)-1
            S=set(range(n))
            for i,x in enumerate(xs):
                S={t[x][q] for q in S}
                if i<len(ys): S={t[ys[i]][q] for q in S}
            return S
        def win(xs,known,d,T):
            tt=len(xs)+1
            for x in (0,1):
                xs2=xs+(x,)
                if tt==T:
                    if all(len(image(xs2,known+rest))==1 for rest in itertools.product((0,1),repeat=T-1-len(known))): return True
                    continue
                if tt-d>=1:   # y_{tt-d} is revealed before x_{tt+1}
                    if all(win(xs2,known+(y,),d,T) for y in (0,1)): return True
                else:
                    if win(xs2,known,d,T): return True
            return False
        def blind(T):
            return any(all(len(image(xs,ys))==1 for ys in itertools.product((0,1),repeat=T-1)) for xs in itertools.product((0,1),repeat=T))
        f=fast[code]
        for T in range(1,Tmax+1):
            checked+=1
            if blind(T)!=(0<f[0]<=T): bad+=1; print('BLIND MISMATCH',n,code,T,f)
        for d in range(dmax+1):
            for T in range(d+1,Tmax+1):
                checked+=1
                if win((),(),d,T)!=(0<f[1+d]<=T): bad+=1; print('MISMATCH',n,code,d,T,f)
    print('n=%d automata=%d comparisons=%d mismatches=%d'%(n,len(codes),checked,bad))
run(2,2,6,range(16))
run(3,2,6,range(729))
random.seed(1)
run(4,2,6,random.sample(range(65536),150)+[ (0+0*4+1*16+2*64)+ (3+0*4+2*16+3*64)*256 ])
