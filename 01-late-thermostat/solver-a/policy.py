"""Exhaustive evaluation of explicit belief-based HISTORY policies (all adversaries, via belief reachability).
Policy 'mid'  : u = -sgn(min x + max x) over the belief (states consistent with o_0..o_t).
Policy 'mid1' : u = -sgn(lo+hi), lo/hi = min/max of x_t + w_t over belief incl. the adversary's legal next move."""
import sys
from collections import deque
from game import sgn, init
def run(d, pol, cap):
    x0, sg0, r0 = init(d)
    B0 = frozenset([(x0, sg0[1:], r0)]); seen={B0}; q=deque([B0]); m=0
    while q:
        Bel=q.popleft()
        if pol=='mid':
            xs=[s[0] for s in Bel]; u=-sgn(min(xs)+max(xs))
        else:
            lo=min(s[0]-(0 if s[2] else 1) for s in Bel); hi=max(s[0]+(0 if s[2] else 1) for s in Bel); u=-sgn(lo+hi)
        parts={}
        for (x,sg,rest) in Bel:
            full=sg+(sgn(x),); o=full[0]; ns=full[1:]
            for w in ((0,) if rest else (0,1,-1)):
                nx=x+u+w
                if abs(nx)>cap: return None,len(seen)
                m=max(m,abs(nx)); parts.setdefault(o,set()).add((nx,ns,1 if w else 0))
        for p in parts.values():
            p=frozenset(p)
            if p not in seen: seen.add(p); q.append(p)
    return m,len(seen)
for d in range(int(sys.argv[2]) if len(sys.argv)>2 else 1,int(sys.argv[1])+1):
    print(d, {pol: run(d,pol,d+3) for pol in (sys.argv[3:] or ('mid','mid1'))}, flush=True)
