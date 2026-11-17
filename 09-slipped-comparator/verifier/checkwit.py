#!/usr/bin/env python3
# referee witness checker, from the definition. lines: "n: letters" ; n<=7 all n! inputs, else all 0-1 inputs.
import sys, itertools, re
def run(w, v):
    v = list(v)
    for i in w:
        if v[i-1] > v[i]: v[i-1], v[i] = v[i], v[i-1]
    return v
def sorts(w, n):
    ins = itertools.permutations(range(1, n+1)) if n <= 7 else itertools.product((0, 1), repeat=n)
    return all((lambda r: all(r[k] <= r[k+1] for k in range(n-1)))(run(w, v)) for v in ins)
def verdict(w, n):
    if n < 3 or any((not isinstance(i, int)) or i < 1 or i > n-1 for i in w): return ('INVALID',)*3
    s = sorts(w, n)
    sl = s and all(sorts(w[:p]+[j]+w[p+1:], n) for p, i in enumerate(w) for j in (i-1, i+1) if 1 <= j <= n-1)
    de = s and all(sorts(w[:p]+w[p+1:], n) for p in range(len(w)))
    return s, sl, de
if __name__ == '__main__':
    for line in open(sys.argv[1]):
        line = line.split('#')[0].strip()
        if not line: continue
        m = re.match(r'n=(\d+).*?:\s*(.*)', line) or re.match(r'(\d+)\s*:\s*(.*)', line)
        n = int(m.group(1)); w = [int(x) for x in m.group(2).split()]
        s, sl, de = verdict(w, n)
        print(f"n={n} size={len(w)} target={(n-1)*(n+2)//2} sorts={s} slip_proof={sl} deletion_proof={de}", flush=True)
