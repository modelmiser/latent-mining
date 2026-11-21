#!/usr/bin/env python3
"""aggregate.py S CD THRESH file... : check chunk coverage and sum histograms. Exit 1 on any failure."""
import sys
S, CD, TH = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
ND = 3 * (S - 1) - 1
seen = {}
hist = [0] * 21
rules = {}
ok = True
for fn in sys.argv[4:]:
    for line in open(fn):
        p = line.split()
        if not p: continue
        if p[0] == 'CHUNK':
            assert p[1] == 'S=%d' % S and p[2] == 'cd=%d' % CD, line
            cid = int(p[3][3:]); vis = int(p[4][8:]); h = [int(x) for x in p[6:]]
            if cid in seen: print('FAIL duplicate chunk', cid); ok = False
            seen[cid] = vis
            if vis != S ** (ND - CD): print('FAIL chunk', cid, 'visited', vis); ok = False
            if sum(h) != vis: print('FAIL chunk', cid, 'histogram sum'); ok = False
            for i, x in enumerate(h): hist[i + 1] += x
        elif p[0] == 'RULE':
            N = int(p[2][2:]); dg = p[3][2:]
            if dg in rules: print('FAIL duplicate rule', dg); ok = False
            if len(dg) != ND: print('FAIL truncated rule line', line); ok = False
            rules[dg] = N
        else:
            print('FAIL unparsed line', line); ok = False
missing = [c for c in range(S ** CD) if c not in seen]
extra = [c for c in seen if not 0 <= c < S ** CD]
if missing: print('FAIL missing chunks', missing[:20], len(missing)); ok = False
if extra: print('FAIL out-of-range chunks', extra); ok = False
total = sum(seen.values())
print('chunks', len(seen), 'of', S ** CD, ' rules visited', total, ' expected S^ND =', S ** ND,
      'MATCH' if total == S ** ND else 'FAIL')
if total != S ** ND: ok = False
print('histogram: N=0 (fails length 2):', hist[1])
for n in range(2, 21):
    if hist[n]: print('histogram: N=%d: %d' % (n, hist[n]))
mx = max(n for n in range(1, 21) if hist[n])
print('maximum N =', mx)
for n in range(TH, 21):
    cnt = sum(1 for v in rules.values() if v == n)
    if cnt != hist[n]: print('FAIL rule lines for N=%d: %d vs histogram %d' % (n, cnt, hist[n])); ok = False
if mx >= TH:
    with open('max_rules_s%d.txt' % S, 'w') as f:
        for dg in sorted(d for d, v in rules.items() if v == mx):
            f.write('RULE S=%d N=%d d=%s\n' % (S, mx, dg))
    print('wrote max_rules_s%d.txt with %d rules' % (S, hist[mx]))
print('ALL CHECKS PASSED' if ok else 'CHECKS FAILED')
sys.exit(0 if ok else 1)
