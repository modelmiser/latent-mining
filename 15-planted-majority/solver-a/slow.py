#!/usr/bin/env python3
"""slow.py -- the definition, implemented directly and obviously (agents are a sorted tuple of states).
A protocol is a dict: s, qA, qB, w (list of 'A'/'B'), d (dict (p,q)->(p2,q2), missing = identity)."""
import sys, itertools, random

def succs(P, cfg):
    """all legal moves from cfg (sorted tuple of states): list of successor cfgs (with repeats)"""
    out = []
    n = len(cfg)
    for i in range(n):
        for j in range(n):
            if i == j: continue
            p, q = cfg[i], cfg[j]
            p2, q2 = P['d'].get((p, q), (p, q))
            if (p2, q2) == (p, q): continue
            l = list(cfg); l[i] = p2; l[j] = q2
            out.append(tuple(sorted(l)))
    return out

def wins_instance(P, a, b, x):
    maj = 'A' if a > b else 'B'
    c0 = tuple(sorted([P['qA']] * a + [P['qB']] * b + [x]))
    R = {c0: None}; stack = [c0]
    while stack:
        c = stack.pop(); R[c] = set(succs(P, c))
        for n in R[c]:
            if n not in R: R[n] = None; stack.append(n)
    for c in R:
        good = all(P['w'][q] == maj for q in c)
        if good: continue
        if not R[c]: return False, ('terminal', c)
        # is c on a directed cycle?  search from successors of c back to c
        seen = set(); st = list(R[c])
        while st:
            u = st.pop()
            if u == c: return False, ('cycle', c)
            if u in seen: continue
            seen.add(u); st.extend(R[u])
    return True, None

def instances(N, s):
    for t in range(2, N + 1):
        for a in range(t + 1):
            b = t - a
            if abs(a - b) >= 2:
                for x in range(s): yield (a, b, x)

def wins_upto(P, N):
    for (a, b, x) in instances(N, P['s']):
        ok, why = wins_instance(P, a, b, x)
        if not ok: return False, (a, b, x, why)
    return True, None

def parse(line):
    """parse the text format written by the C++ tools: s=3 qA=0 qB=1 w=ABA d: 0,1->2,2 ..."""
    tok = line.split()
    P = {'d': {}}
    for t in tok:
        if t.startswith('s='): P['s'] = int(t[2:])
        elif t.startswith('qA='): P['qA'] = int(t[3:])
        elif t.startswith('qB='): P['qB'] = int(t[3:])
        elif t.startswith('w='): P['w'] = list(t[2:])
        elif '->' in t:
            l, r = t.split('->')
            if r == '?': continue
            P['d'][tuple(map(int, l.split(',')))] = tuple(map(int, r.split(',')))
    return P

if __name__ == '__main__':
    # usage: slow.py N < protocol-lines ; prints max-N verdict for each
    N = int(sys.argv[1])
    for line in sys.stdin:
        if 's=' not in line or line.startswith('#'): continue
        line = line[line.index('s='):]
        P = parse(line)
        print(wins_upto(P, N), line.strip()[:100])
