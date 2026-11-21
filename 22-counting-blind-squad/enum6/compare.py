#!/usr/bin/env python3
"""Compare my lists with solver-b's files (opened only after the s=6 run completed)."""
import sys, itertools
B = sys.argv[1]
L5file = sys.argv[2] if len(sys.argv) > 2 else B + '/all_L5_rules_s5.txt'
ok = True
mine5 = {l.split()[3][2:] for l in open('max_rules_s5.txt')}
theirs5 = {''.join(str('QGFAB'.index(ch)) for ch in l.strip()) for l in open(L5file) if l.strip()}
print('s=5: mine', len(mine5), 'theirs', len(theirs5), 'identical sets:', mine5 == theirs5)
ok &= mine5 == theirs5
mine6 = {l.split()[3][2:] for l in open('max_rules_s6.txt')}
w = {}
for l in open(B + '/witness_s6.txt'):
    p = l.split()
    if len(p) == 3 and p[0] in 'QGABC' and p[1] in '012': w[(p[0], int(p[1]))] = 'QGFABC'.index(p[2])
dg = ''.join(str(w[(x, k)]) for x in 'QGABC' for k in range(3) if (x, k) != ('Q', 0))
print('s=6 witness as digits', dg, 'in my 336:', dg in mine6)
ok &= dg in mine6
# closure of my list under renaming of the unnamed states 3,4,5
def perm(d, pi):
    full = [0] + [int(c) for c in d]              # entries Q0..C2
    rows = [full[i:i+3] for i in range(0, 15, 3)]  # Q,G,3,4,5
    m = {0: 0, 1: 1, 2: 2, 3: pi[0], 4: pi[1], 5: pi[2]}
    new = {0: rows[0], 1: rows[1]}
    for i, x in enumerate((3, 4, 5)): new[m[x]] = rows[2 + i]
    out = [m[v] for x in (0, 1, 3, 4, 5) for v in new[x]]
    return ''.join(map(str, out[1:]))
closed = all(perm(d, pi) in mine6 for d in mine6 for pi in itertools.permutations((3, 4, 5)))
orbits = {min(perm(d, pi) for pi in itertools.permutations((3, 4, 5))) for d in mine6}
print('s=6: my list size', len(mine6), 'closed under renaming:', closed, 'classes:', len(orbits))
ok &= closed and len(mine6) == 336
print('COMPARE OK' if ok else 'COMPARE FAILED'); sys.exit(0 if ok else 1)
