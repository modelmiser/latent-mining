#!/usr/bin/env python3
"""Referee witness checker (FIFO game). usage: wcheck.py adv|rule n L file fmt   (fmt a = solver-a, b = solver-b)
Internal state: tuple of (w,o) oldest first, bins relabelled by first appearance (oldest first)."""
import sys
kind, n, L, fn, fmt = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), sys.argv[4], sys.argv[5]
def canon(st):
    m = {}
    out = []
    for w, o in st:
        if w not in m: m[w] = len(m)
        if o not in m: m[o] = len(m)
        out.append((m[w], m[o]))
    return tuple(out)
def parse_state(s):
    s = s.strip()
    if fmt == 'a':
        st = tuple(tuple(int(x) for x in b.split(':')) for b in s.split()) if s else ()
    else:
        st = () if s in ('-', '') else tuple((int(b[0], 16) - 1, int(b[1], 16) - 1) for b in s.split(','))
    for w, o in st:
        if w == o: raise SystemExit('REJECT: loop ball')
    if canon(st) != st: raise SystemExit(f'REJECT: non-canonical state {s!r}')
    return st
def info(st):
    used = 0; ld = {}; cp = {}
    def f(x):
        cp.setdefault(x, x)
        while cp[x] != x: x = cp[x]
        return x
    for w, o in st:
        used = max(used, w + 1, o + 1); ld[w] = ld.get(w, 0) + 1
        a, b = f(w), f(o)
        if a == b: raise SystemExit('REJECT: state is not a forest')
        cp[a] = b
    if used > n: raise SystemExit('REJECT: state uses more than n bins')
    return used, ld, f
def offers(st):
    """all legal offers up to renaming of untouched bins: pairs (u,v), labels >= used are fresh"""
    used, ld, f = info(st)
    res = []
    for u in range(used):
        for v in range(u + 1, used):
            if f(u) != f(v): res.append((u, v))
        if used < n: res.append((u, used))
    if used + 2 <= n: res.append((used, used + 1))
    return res
def maxload(st):
    ld = {}
    for w, o in st: ld[w] = ld.get(w, 0) + 1
    return max(ld.values()) if ld else 0
lines = [l.rstrip('\n') for l in open(fn) if not l.startswith('#')]
if kind == 'rule':
    S = set(parse_state(l) for l in lines)
    if () not in S: raise SystemExit('REJECT: empty state missing')
    for st in S:
        if maxload(st) >= L: raise SystemExit('REJECT: load >= L in set')
        if st and canon(st[1:]) not in S: raise SystemExit(f'REJECT: not closed under Expire at {st}')
        for u, v in offers(st):
            ok = False
            for w, o in ((u, v), (v, u)):
                t = canon(st + ((w, o),))
                if t in S: ok = True
            if not ok: raise SystemExit(f'REJECT: offer {u},{v} unanswerable at {st}')
    print(f'ACCEPT rule n={n} L={L}: {len(S)} states')
else:
    T = {}
    for l in lines:
        if fmt == 'a':
            r, mv, s = l.split('|'); st = parse_state(s); r = int(r)
            mv = ('E',) if mv == 'E' else ('T',) if mv == 'T' else ('O', int(mv.split()[1]), int(mv.split()[2]))
        else:
            s, r, mv = l.split(); st = parse_state(s); r = int(r)
            if mv == 'T': mv = ('T',)
            elif mv[0] == 'E':
                if mv != 'E0': raise SystemExit('REJECT: non-FIFO removal')
                mv = ('E',)
            else: mv = ('O', int(mv[1], 16) - 1, int(mv[2], 16) - 1)
        T[st] = (r, mv)
    seen = set(); stack = [()]; maxdepth = 0
    while stack:
        st = stack.pop()
        if st in seen: continue
        seen.add(st)
        if maxload(st) >= L: continue           # Adversary has won
        if st not in T: raise SystemExit(f'REJECT: reachable state {st} has no move')
        r, mv = T[st]
        if mv[0] == 'T': raise SystemExit(f'REJECT: T claimed at non-terminal {st}')
        if mv[0] == 'E':
            if not st: raise SystemExit('REJECT: Expire on empty')
            nxt = [canon(st[1:])]
        else:
            u, v = sorted(mv[1:])
            if (u, v) not in offers(st): raise SystemExit(f'REJECT: illegal offer {mv} at {st}')
            nxt = [canon(st + ((u, v),)), canon(st + ((v, u),))]
        for t in nxt:
            if maxload(t) >= L: continue
            if t not in T: raise SystemExit(f'REJECT: successor {t} of {st} has no move')
            if T[t][0] >= r: raise SystemExit(f'REJECT: rank does not decrease {st} -> {t}')
            stack.append(t)
    print(f'ACCEPT adv n={n} L={L}: {len(seen)} states visited, start rank {T[()][0]}')
