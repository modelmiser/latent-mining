#!/usr/bin/env python3
"""Recursive witness W_r (Sylvester-type construction). W_1=(1 [1] []).
W_r = (1  W_{r-1} on positions 2..2^(r-1) with its unique empty leaf replaced by [1]   W_{r-1} on positions 2^(r-1)+1..2^r-1)."""
import sys
def W(r,off,div=None):   # tree text using positions off+1.. ; the unique empty leaf is printed as [div] if div given
    root=off+1
    if r==1: return '(%d [%d] %s)'%(root,root,'[]' if div is None else '[%d]'%div)
    m=2**(r-1)-1
    return '(%d %s %s)'%(root,W(r-1,off+1,root),W(r-1,off+1+m,div))
a=2
for r in range(1,6):
    d=2**r-1
    open('witness_r%d.txt'%r,'w').write(W(r,0)+'\n'+'0'*d+'\n%d\n'%a)
    a=a*(a+1)
