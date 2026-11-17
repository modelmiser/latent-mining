#!/usr/bin/env python3
# Theorem 3 (B) and the referee's run-count lemma on the n=5, L=34 and n=4, L=18 solution lists (lists are the solvers'; test is mine)
import sys
from ref import covers_all, repairable
from lemmas import words, runs, S
from collections import Counter
for n, path in ((5, '../solver-b/n5L34.txt'), (4, '../solver-b/n4L18.txt'), (5, '../solver-a/n5_L34_all.txt'), (4, '../solver-a/n4_L18_all.txt')):
    ok = 0; tot = 0; rset = Counter(); sset = Counter(); rep = 0
    for ln in open(path):
        tok = [t for t in ln.split() if set(t) <= {'0', '1'} and len(t) == 2 ** n + 2]
        if not tok: continue
        v = tuple(int(c) for c in tok[0]); tot += 1
        if repairable(v, n): rep += 1
        c = Counter(words(v, n)); sur = [w for w, k in c.items() if k > 1]
        if all(all(w[i] != w[i + 1] for i in range(n - 1)) for w in sur) and covers_all(v, n): ok += 1
        rset[len(runs(v))] += 1; sset[S(v)] += 1
    print(f"{path}: loops={tot} repairable(ref.py)={rep} universal with only alternating surplus words={ok} run counts={dict(rset)} S={dict(sset)}")
