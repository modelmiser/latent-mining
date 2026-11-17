#!/usr/bin/env python3
"""Plain checker, straight from the definition.
usage: check.py FILE      (FILE lines: 'n: i1 i2 ... im', letters are comparator indices)
For each word reports: size, sorts?, slip-proof?, deletion-proof?  on ALL n! inputs if n<=7,
else on all 2^n 0-1 inputs (0-1 principle)."""
import sys, itertools
def run(word, v):
    v = list(v)
    for i in word:
        if v[i-1] > v[i]: v[i-1], v[i] = v[i], v[i-1]
    return v
def inputs(n):
    return itertools.permutations(range(1, n+1)) if n <= 7 else itertools.product((0, 1), repeat=n)
def sorts(word, n):
    return all(run(word, v) == sorted(v) for v in inputs(n))
def slips(word, n):
    for p, i in enumerate(word):
        for j in (i-1, i+1):
            if 1 <= j <= n-1: yield word[:p] + [j] + word[p+1:]
def deletions(word):
    for p in range(len(word)): yield word[:p] + word[p+1:]
def bubble2(n):   # W_n = D_1 D_2 ... D_{n-1} D_{n-1},  D_k = c_k c_{k-1} ... c_1
    w = []
    for k in list(range(1, n)) + [n-1]: w += list(range(k, 0, -1))
    return w
def oddeven(n, rounds, first=1):   # round r uses c_i with i = first, first+2, ... alternating parity
    w = []
    for r in range(rounds):
        w += list(range(1 + (r + first + 1) % 2, n, 2))
    return w
if __name__ == '__main__':
    for line in open(sys.argv[1]):
        line = line.split('#')[0].strip()
        if not line: continue
        n, rest = line.split(':'); n = int(n); w = [int(x) for x in rest.split()]
        assert all(1 <= i <= n-1 for i in w)
        s = sorts(w, n); sp = s and all(sorts(x, n) for x in slips(w, n)); dp = s and all(sorts(x, n) for x in deletions(w))
        print(f"n={n} size={len(w)} target={(n-1)*(n+2)//2} sorts={s} slip_proof={sp} deletion_proof={dp}  word={' '.join(map(str,w))}")
