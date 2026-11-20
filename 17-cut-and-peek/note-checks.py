#!/usr/bin/env python3
"""Three small checks cited in NOTE.md (Section 3, Section 4, Section 6). Run: python3 note-checks.py
1. The 36-card protocol for three-bit AND, from the three strings in the note.
2. The frequency identity behind Lemma 1: for every row length m <= 9, every choice of the six shifts and every
   frequency r that satisfies the three equations (*), the conclusion w^(th1)+w^(th2)+w^(th3)-2 = w^(r t) holds.
3. The four-variable function with a published single-cut full-open protocol (truth table copied from Table 2 of
   Shinagawa and Nuida, arXiv:2507.03323): none of its eight restrictions to three variables has a 7-1 split,
   and the published rows are cyclically equal exactly when the outputs are equal."""
import cmath, itertools
from collections import Counter
o = "111111012301331022102230222123003012"; u0 = "000011000011011001000000110111111011"; u1 = "111100011110100010101110001000110000"
m = 36; R = range(0, 36, 6)
def M(x):
    w = [u0[p] if o[p] == '0' or x[int(o[p]) - 1] == 0 else u1[p] for p in range(m)]
    return Counter(''.join(w[(q - j) % m] for q in R) for j in range(m))
Ms = {x: M(x) for x in itertools.product((0, 1), repeat=3)}; z = Ms[(0, 0, 0)]
print('1. privacy:', all(Ms[x] == z for x in Ms if x != (1, 1, 1)), '| distinct words, AND 0:', len(z),
      '| distinct words, AND 1:', len(Ms[(1, 1, 1)]), '| disjoint:', not (set(z) & set(Ms[(1, 1, 1)])))
bad = hits = 0
for m in range(1, 10):
    w = [cmath.exp(2j * cmath.pi * k / m) for k in range(m)]
    for s1, s2, s3, s12, s13, s23 in itertools.product(range(m), repeat=6):
        t = (s12 + s13 + s23 - s1 - s2 - s3) % m
        for r in range(m):
            a, b, c = w[r * s1 % m], w[r * s2 % m], w[r * s3 % m]
            if abs(a + b - 1 - w[r * s12 % m]) < 1e-9 and abs(a + c - 1 - w[r * s13 % m]) < 1e-9 and abs(b + c - 1 - w[r * s23 % m]) < 1e-9:
                hits += 1; bad += abs(a + b + c - 2 - w[r * t % m]) > 1e-9
print('2. cases satisfying (*):', hits, '| violations of the conclusion:', bad)
keys = [format(i, '04b') for i in range(16)]            # x y z w
f = dict(zip(keys, [0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0]))
rows = dict(zip(keys, "CCCCHHHH CCCHHHHC CCHCHHCH CCHHHHCC CHCCHCHH CHCHHCHC CHHCHCCH CHHHHCCC HCCCCHHH HCCHCHHC HCHCCHCH HCHHCHCC HHCCCCHH HHCHCCHC HHHCCCCH HHHHCCCC".split()))
splits = sorted({sum(f[k] for k in f if k[v] == b) for v in range(4) for b in '01'})
rot = lambda s: {s[i:] + s[:i] for i in range(len(s))}
print('3. ones among the 8 inputs of each three-variable restriction:', splits,
      '| published rows cyclically equal iff outputs equal:', all((rows[a] in rot(rows[b])) == (f[a] == f[b]) for a in rows for b in rows))
