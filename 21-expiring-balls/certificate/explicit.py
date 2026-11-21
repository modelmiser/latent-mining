#!/usr/bin/env python3
"""explicit.py -- for small n: expand the compressed certificate into the explicit set S and check properties
(1)-(4) literally, with no reduction.  Independent of check.cpp: states are built by adding NEWEST balls
(the direction of play), kept oldest first as in the referee's wcheck.py, and membership in A is tested by
relabelling every age-suffix.
usage: explicit.py n L certfile [S_out [reference_rule_file]]
S_out is written in solver-a's format (balls oldest first 'w:o', bins by first appearance oldest first), so
that verifier/wcheck.py can be run on it.  reference_rule_file (solver-a format) is compared with S."""
import sys
n, L, cf = int(sys.argv[1]), int(sys.argv[2]), sys.argv[3]
def canon(st):                       # labels by first appearance in the order given
    m = {}; out = []
    for w, o in st:
        if w not in m: m[w] = len(m)
        if o not in m: m[o] = len(m)
        out.append((m[w], m[o]))
    return tuple(out)
C = set()
for l in open(cf):
    l = l.strip()
    if l == '': continue
    C.add(() if l == '-' else canon(tuple((int(t[0], 16), int(t[1], 16)) for t in l.split())))   # newest first
def inA(st):                         # st oldest first; some age-suffix (newest j balls, j >= 0), read newest first, is in C
    return any(canon(st[j:][::-1]) in C for j in range(len(st) + 1))
def info(st):
    used = 0; ld = {}; cp = {}
    for w, o in st:
        used = max(used, w + 1, o + 1); ld[w] = ld.get(w, 0) + 1
        a, b = cp.setdefault(w, w), cp.setdefault(o, o)
        assert a != b
        for x in cp:
            if cp[x] == b: cp[x] = a
    return used, ld, cp
def offers(st):
    used, ld, cp = info(st); res = []
    for u in range(used):
        for v in range(u + 1, used):
            if cp[u] != cp[v]: res.append((u, v))
        if used < n: res.append((u, used))
    if used + 2 <= n: res.append((used, used + 1))
    return res, ld
# all non-final states, by adding newest balls from the empty state
allst = {()}; frontier = [()]
while frontier:
    nxt = []
    for st in frontier:
        offs, ld = offers(st)
        for u, v in offs:
            for w, o in ((u, v), (v, u)):
                if ld.get(w, 0) + 1 >= L: continue
                t = canon(st + ((w, o),))
                if t not in allst: allst.add(t); nxt.append(t)
    frontier = nxt
S = set(st for st in allst if not inA(st))
print(f'n={n} L={L}: non-final states {len(allst)}, in A {len(allst) - len(S)}, S {len(S)}')
bad = None
if () not in S: bad = '(1) empty state not in S'
for st in S:
    if bad: break
    if st and canon(st[1:]) not in S: bad = f'(3) Expire leaves S at {st}'; break
    offs, ld = offers(st)
    for u, v in offs:
        if not any(ld.get(w, 0) + 1 < L and canon(st + ((w, o),)) in S for w, o in ((u, v), (v, u))):
            bad = f'(4) offer {u},{v} has no answer in S at {st}'; break
print('EXPLICIT REJECT: ' + bad if bad else f'EXPLICIT ACCEPT: S has properties (1)-(4), {len(S)} states')
if len(sys.argv) > 4:
    with open(sys.argv[4], 'w') as f:
        f.write('# S expanded from ' + cf + '\n')
        for st in sorted(S, key=lambda s: (len(s), s)): f.write(' '.join(f'{w}:{o}' for w, o in st) + '\n')
if len(sys.argv) > 5:
    R = set()
    for l in open(sys.argv[5]):
        if l.startswith('#'): continue
        l = l.strip(); R.add(tuple(tuple(int(x) for x in b.split(':')) for b in l.split()) if l else ())
    print(f'reference list: {len(R)} states; inside S: {len(R & S)}; outside S: {len(R - S)}; S states not in reference: {len(S - R)}')
sys.exit(1 if bad else 0)
