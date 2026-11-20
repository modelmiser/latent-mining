import sys, itertools
# group witnesses into isomorphism classes ignoring start state and 0/1 complement
def canon(d):
    n=len(d); best=None
    for comp in (0,1):
        dd=[(a,b) if not comp else (b,a) for a,b in d]
        for s in range(n):
            # BFS labelling from s
            lab={s:0}; order=[s]; i=0
            while i<len(order):
                q=order[i]; i+=1
                for b in (0,1):
                    t=dd[q][b]
                    if t not in lab: lab[t]=len(order); order.append(t)
            if len(order)<n: continue
            key=tuple((lab[dd[q][0]],lab[dd[q][1]]) for q in order)
            if best is None or key<best: best=key
    return best
cl={}
for l in open(sys.argv[1]):
    if not l.startswith("WITNESS"): continue
    part=l.split(':')[1].split('spreads')[0].split()
    d=[tuple(map(int,p.split(','))) for p in part]
    cl.setdefault(canon(d),[]).append(l.strip())
for k,v in cl.items(): print(len(v), k, '|', v[0].split('spreads:')[1])
