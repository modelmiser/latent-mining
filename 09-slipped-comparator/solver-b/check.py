#!/usr/bin/env python3
# Independent checker. Usage: check.py n mode "1 2 1 ..."   (mode slip|del)
# Tests the word and every slip/deletion on ALL n! permutations. Exit 0 iff proof.
import sys, itertools
from slow import proof, sorts, slips, dels, ups
n = int(sys.argv[1]); mode = sys.argv[2]; w = tuple(int(x) for x in sys.argv[3].split())
assert all(1 <= x <= n-1 for x in w)
perms = list(itertools.permutations(range(1, n+1)))
ok = proof(w, n, perms, mode)
print(f"n={n} mode={mode} size={len(w)} {'PROOF' if ok else 'NOT-PROOF'}")
if not ok:
    if not sorts(w, n, perms): print(" word itself fails")
    else:
        for s in {'slip': slips, 'del': dels, 'up': ups}[mode](w, n):
            if not sorts(s, n, perms): print(" failing variant:", ' '.join(map(str, s))); break
sys.exit(0 if ok else 1)
