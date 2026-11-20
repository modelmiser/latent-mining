#!/usr/bin/env python3
# Writes the explicit pair scheme (lower bound R(n) >= floor(n/2)) as a witness file for check.py. usage: pairscheme.py n out
import sys
n=int(sys.argv[1]); t=n//2
def E(c,op):
    c=list(c)
    if op=='POP':
        for i in range(t-1,-1,-1):
            if c[2*i]!=c[2*i+1]: c[2*i]=c[2*i+1]='1'; break
    else:
        for i in range(t):
            if c[2*i]=='0' and c[2*i+1]=='0':
                c[2*i+(0 if op=='PUSH0' else 1)]='1'; break   # pair "10" = bit 0, pair "01" = bit 1
    return ''.join(c)
def Q(c):
    for i in range(t-1,-1,-1):
        if c[2*i]!=c[2*i+1]: return '0' if c[2*i]=='1' else '1'
    return 'EMPTY'
import itertools
with open(sys.argv[2],'w') as f:
    f.write("# pair scheme n=%d t=%d\nn %d\nt %d\n"%(n,t,n,t))
    for bits in itertools.product('01',repeat=n):
        c=''.join(bits)
        f.write("Q %s %s\n"%(c,Q(c)))
        for op in ('PUSH0','PUSH1','POP'):
            f.write("E %s %s %s\n"%(c,op,E(c,op)))
