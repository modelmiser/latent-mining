#!/usr/bin/env python3
# Referee: check the solvers' strategy tables with ref.py only.
import sys, re
from ref import covers_all, edits
def apply(v, typ, k):
    if typ == 'pass': return v
    if typ == 'loss':
        assert len(v) >= 2 and 0 <= k < len(v); return v[:k] + v[k+1:]
    if typ in ('dbl', 'double'):
        assert 0 <= k < len(v); return v[:k+1] + v[k:]
    raise ValueError(typ)
def canon(v):
    s = ''.join(map(str, v)); return min(s[i:] + s[:i] for i in range(len(s)))
def tup(s): return tuple(int(c) for c in s)
def check_a(path):
    lines = open(path).read().split('\n')
    m = re.match(r'# n=(\d+) w=([01]+)', lines[0]); n = int(m.group(1)); w = tup(m.group(2))
    seen = set(); bad = 0
    for ln in lines[2:]:
        if not ln.strip(): continue
        a, b, c = [x.strip() for x in ln.split('->')]
        at, ak = a.split(); bt, bk = b.split(); ak = int(ak); bk = int(bk)
        w1 = apply(w, at, ak); w2 = apply(w1, bt, bk)
        if ''.join(map(str, w2)) != c or not covers_all(w2, n): bad += 1
        seen.add(('dbl' if at == 'double' else at, ak))
    full = {('pass', -1)} | {('loss', k) for k in range(len(w))} | {('dbl', k) for k in range(len(w))}
    print(f"A-format {path}: n={n} len={len(w)} lines={len(seen)} bad_lines={bad} missing_adversary_moves={len(full - seen)} extra={len(seen - full)} -> {'OK' if bad == 0 and full == seen else 'FAIL'}")
def check_b(path):
    lines = open(path).read().split('\n')
    hdr = lines[0].split(); w = tup(hdr[1]); n = None
    # n is not in the header: infer as the largest n for which w is universal
    n = int(re.search(r'strategy_n(\d+)', path).group(1))
    table = {}; bad = 0; nrep = 0
    for ln in lines[1:]:
        m = re.match(r'(pass|loss|dbl)\s+(-?\d+) -> ([01]+) :(.*)', ln)
        if not m: continue
        at, ak, w1s, reps = m.group(1), int(m.group(2)), m.group(3), m.group(4).split()
        w1 = apply(w, at, ak)
        if ''.join(map(str, w1)) != w1s: bad += 1
        mine = set()
        for name, w2 in edits(w1):
            if covers_all(w2, n): mine.add(name)
        # their list is one cell per run; every listed reply must work, and the list must be empty iff mine is
        for r in reps:
            t, k = r.split('@'); k = int(k)
            if not covers_all(apply(w1, t, k), n): bad += 1
            nrep += 1
        if (len(reps) == 0) != (len(mine) == 0): bad += 1
        if len(reps) == 0: bad += 1
        table[canon(w1)] = len(reps)
    miss = 0
    for name, w1 in edits(w):
        if canon(w1) not in table: miss += 1
    print(f"B-format {path}: n={n} len={len(w)} rows={len(table)} replies_checked={nrep} bad={bad} adversary_moves_not_covered={miss} -> {'OK' if bad == 0 and miss == 0 else 'FAIL'}")
if __name__ == '__main__':
    for p in sys.argv[2:]:
        (check_a if sys.argv[1] == 'a' else check_b)(p)
