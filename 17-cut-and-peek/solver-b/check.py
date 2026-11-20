#!/usr/bin/env python3
# Independent checker, straight from the problem text. Reads witness lines
#   n=.. m=.. owner=<m chars: digit = player, H = helper> c0=<card if owner's bit is 0> c1=<card if bit is 1> R=<0/1 mask>
# (cards: 1 = heart, 0 = club). Prints VALID / INVALID(reason) per line.
import sys, itertools
from collections import Counter
def check(line):
    kv=dict(tok.split('=') for tok in line.split() if '=' in tok)
    n=int(kv['n']); m=int(kv['m']); ow=kv['owner']; c0=kv['c0']; c1=kv['c1']; R=[q for q in range(m) if kv['R'][q]=='1']
    assert len(ow)==len(c0)==len(c1)==len(kv['R'])==m
    if not R: return 'INVALID(empty R)'
    for i in range(n):
        if str(i) not in ow: return 'INVALID(empty P_%d)'%i
    for p in range(m):
        if ow[p]=='H' and c0[p]!=c1[p]: return 'INVALID(H card depends on input)'
    def row(x): return [ (c1[p] if (ow[p]!='H' and x[int(ow[p])]) else c0[p]) for p in range(m)]
    def M(x):
        w=row(x); c=Counter()
        for j in range(m):
            after=[None]*m
            for p in range(m): after[(p+j)%m]=w[p]      # card at p moves to p+j
            c[''.join(after[q] for q in R)]+=1
        return c
    xs=list(itertools.product([0,1],repeat=n)); one=tuple([1]*n); zero=tuple([0]*n)
    M0=M(zero)
    for x in xs:
        if x!=one and M(x)!=M0: return 'INVALID(privacy fails at x=%s)'%(x,)
    if set(M(one))&set(M0): return 'INVALID(correctness: shared word)' + (' but WEAK-VALID: private and M(1^n)!=M(0^n); M0=%s M1=%s'%(dict(M0),dict(M(one))) if M(one)!=M0 else ' and M(1^n)=M(0^n)')
    return 'VALID'
for line in sys.stdin:
    line=line.strip()
    if line and not line.startswith('#'): print(check(line[line.index('n='):]), '|', line)
