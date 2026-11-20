#!/usr/bin/env python3
"""Writes the problem statement's pair scheme (claimed to survive floor(n/2)) as a witness file."""
import sys, itertools
n = int(sys.argv[1]); t = int(sys.argv[2]); k = n // 2
def Q(c):
    for j in reversed(range(k)):
        p = c[2*j:2*j+2]
        if p == '01': return '0'
        if p == '10': return '1'
    return 'EMPTY'
def E(c, op):
    c = list(c)
    if op < 2:
        for j in range(k):
            if c[2*j:2*j+2] == ['0', '0']:
                c[2*j:2*j+2] = list('01' if op == 0 else '10'); break
    else:
        for j in reversed(range(k)):
            if c[2*j:2*j+2] in (['0', '1'], ['1', '0']):
                c[2*j:2*j+2] = ['1', '1']; break
    return ''.join(c)
print("n %d t %d" % (n, t))
for bits in itertools.product('01', repeat=n):
    c = ''.join(bits)
    print(c, Q(c), E(c, 0), E(c, 1), E(c, 2))
