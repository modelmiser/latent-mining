import sys, itertools
sys.path.insert(0,'solver-a')
sys.dont_write_bytecode=True
from game import init, succ, clock_step, sgn
d=int(sys.argv[1]); B=int(sys.argv[2])
def expand(S):
    obs = sorted({s[1][0] for s in S}); out={}
    for vals in itertools.product((0,1,-1), repeat=len(obs)):
        f=[None,None,None]
        for o,v in zip(obs,vals): f[o+1]=v
        T=clock_step(S,[0 if v is None else v for v in f],B)
        if T is not None: out[tuple(f)]=T
    return out
# depth-first: longest survival for each prefix; print all maximal-depth clocks
memo={}
def depth(S):
    if S in memo: return memo[S]
    memo[S]=10**9  # cycle => infinite
    ch=expand(S)
    r=0 if not ch else 1+max(depth(T) for T in ch.values())
    memo[S]=r; return r
S0=frozenset([init(d)])
D=depth(S0); print("max survival depth",D, "sets",len(memo))
def show(f): return ''.join('.' if v is None else '-0+'[v+1] for v in f)
def walk(S,pre,need):
    if need==0:
        print(' '.join(pre)); return
    for f,T in expand(S).items():
        if depth(T)>=need-1 and (need>1 or True):
            if depth(T)==need-1: walk(T,pre+[show(f)],need-1)
lim=int(sys.argv[3]) if len(sys.argv)>3 else D
walk(S0,[],D)
