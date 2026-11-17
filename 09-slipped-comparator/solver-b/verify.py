#!/usr/bin/env python3
# Sanity checks, all on ALL n! inputs (definition-level, via slow.py).
import itertools, sys
from slow import proof, sorts, apply, slips, dels
def W(n):   # B_n I_n = I_2 I_3 ... I_n I_n,  I_k = c_{k-1} ... c_1
    w = []
    for k in range(2, n+1): w += list(range(k-1, 0, -1))
    return tuple(w + list(range(n-1, 0, -1)))
def OET(n, rounds):
    w = []
    for r in range(rounds): w += list(range(1 + r % 2, n, 2))
    return tuple(w)
out = open('witnesses.txt', 'w')
for n in range(3, 8):
    perms = list(itertools.permutations(range(1, n+1)))
    w = W(n); N = n*(n-1)//2
    assert len(w) == N + n - 1
    d = proof(w, n, perms, 'del'); s = proof(w, n, perms, 'slip')
    print(f"n={n} W(n) size={len(w)} deletion-proof={d} slip-proof={s}")
    out.write(f"n={n} size={len(w)} slip-proof and deletion-proof: {' '.join(map(str, w))}\n")
    for extra in (0, 1, 2):
        o = OET(n, n+extra)
        print(f"   OET rounds={n+extra} size={len(o)} sorts={sorts(o,n,perms)} deletion-proof={proof(o,n,perms,'del')} slip-proof={proof(o,n,perms,'slip')}")
    # deliberately wrong object: W(n) with its last letter removed must be rejected
    bad = w[:-1]
    print(f"   deliberately wrong (last letter dropped): slip-proof={proof(bad,n,perms,'slip')}  (must be False)")
    assert not proof(bad, n, perms, 'slip')
out.close()
# belief (a): sorts reverse <=> sorts all, exhaustive over all words up to a length
for n, maxL in ((3, 9), (4, 9), (5, 8)):
    perms = list(itertools.permutations(range(1, n+1))); rev = tuple(range(n, 0, -1)); bad = 0; tot = 0; ns = 0
    for L in range(maxL+1):
        for w in itertools.product(range(1, n), repeat=L):
            a = apply(w, rev) == list(range(1, n+1))
            if a:
                ns += 1
                if not sorts(w, n, perms): bad += 1
            tot += 1
    print(f"belief(a) n={n} words up to length {maxL}: {tot} words, {ns} sort the reverse input, counterexamples={bad}")
# belief (d): insertion never breaks sorting, exhaustive n=4, sorting words up to length 8
n = 4; perms = list(itertools.permutations(range(1, n+1))); bad = 0; cnt = 0
for L in range(6, 9):
    for w in itertools.product(range(1, n), repeat=L):
        if sorts(w, n, perms):
            for p in range(L+1):
                for c in range(1, n):
                    cnt += 1
                    if not sorts(w[:p] + (c,) + w[p:], n, perms): bad += 1
print(f"belief(d) n=4: {cnt} insertions into sorting words of length 6..8, counterexamples={bad}")
