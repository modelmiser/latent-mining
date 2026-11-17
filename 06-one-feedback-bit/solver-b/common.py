# Direct implementation of the definitions. Strings are python str over '0','1'.
from itertools import product
def D(s):
    return {s[:i]+s[i+1:] for i in range(len(s))}
def strs(m):
    return [''.join(p) for p in product('01', repeat=m)]
def outcome(u,a,b):
    return {u+a} | {u+x for x in D(a)} | {x+b for x in D(u)}
def strategies(n,t):
    for u in strs(t):
        for a in strs(n-t):
            for b in strs(n-t):
                yield (u,a,b)
