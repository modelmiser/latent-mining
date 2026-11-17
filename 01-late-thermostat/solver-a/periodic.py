"""Max |x| reachable under a periodic clock (pure periodic from t=0). Upper bounds on C(d)."""
import sys, itertools
from collections import deque
from game import MAPS, init, succ
def per_max(d, seq, cap):
    p=len(seq); s0=(init(d),0); seen={s0}; q=deque([s0]); m=0
    while q:
        s,ph=q.popleft(); u=seq[ph][s[1][0]+1]
        for n in succ(s,u):
            if abs(n[0])>cap: return None
            k=(n,(ph+1)%p)
            if k not in seen: seen.add(k); q.append(k); m=max(m,abs(n[0]))
    return m
if __name__=="__main__":
    P=int(sys.argv[1])
    for d in range(int(sys.argv[2]),int(sys.argv[3])+1):
        best=None; arg=[]
        for seq in itertools.product(MAPS,repeat=P):
            v=per_max(d,seq,(best if best is not None else 3*d+3))
            if v is None: continue
            if best is None or v<best: best=v; arg=[seq]
            elif v==best: arg.append(seq)
        print(f"d={d} period={P} best={best} n={len(arg)} e.g. {arg[:6]}",flush=True)
