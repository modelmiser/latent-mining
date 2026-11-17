"""Exact best-response adversary against a GIVEN clock: BFS over (x, last d signs, rest) per time layer.
breach(clock,d,B,T) -> (t, w-sequence, x-sequence) of an earliest play with |x_t|>B, or None if none within T ticks.
clock: function t -> (f(-),f(0),f(+))."""
import sys
def sgn(x): return (x>0)-(x<0)
def breach(clock,d,B,T):
    s0=(0,(0,)*d,0); layer={s0:None}; hist=[layer]
    for t in range(T):
        f=clock(t); nxt={}
        for s in layer:
            x,sg,r=s; u=f[sg[0]+1]; ns=sg[1:]+(sgn(x),)
            for w in ((0,) if r else (0,1,-1)):
                nx=x+u+w
                n=(nx,ns,1 if w else 0)
                if abs(nx)>B:
                    ws=[w]; xs=[nx,x]; cur=s
                    for tt in range(t,0,-1):
                        p,pw=hist[tt][cur]; ws.append(pw); xs.append(p[0]); cur=p
                    return t+1, ws[::-1], xs[::-1]
                if n not in nxt: nxt[n]=(s,w)
        layer=nxt; hist.append(layer)
    return None
def halfduty(t): return (1,0,-1) if t%2==0 else (0,0,0)
if __name__=="__main__":
    d=int(sys.argv[1]); B=int(sys.argv[2]); T=int(sys.argv[3]) if len(sys.argv)>3 else 12*d+20
    r=breach(halfduty,d,B,T)
    if r is None: print(f"half-duty d={d}: no breach of band {B} within {T} ticks")
    else:
        t,ws,xs=r; print(f"half-duty d={d}: band {B} breached at t={t}\n w:",''.join('-0+'[w+1] for w in ws),"\n x:",xs)
