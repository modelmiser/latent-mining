#!/usr/bin/env python3
# Referee: all-states least-fixpoint solver (no reachability, no memo tricks) for h<=3, compared state-by-state
# with solve.cpp ("query").  usage: brute.py G h kmax
import sys, subprocess
G, h, kmax = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
n = 2 ** (h + 1) - 1
nb = {v: [u for u in (v // 2, 2 * v, 2 * v + 1) if 1 <= u <= n] for v in range(1, n + 1)}
nbm = {v: sum(1 << u for u in nb[v]) | (1 << v) for v in nb}
def expand(S1, c2):
    R = 0; v = 1
    while S1 >> v:
        if S1 >> v & 1: R |= nbm[v]
        v += 1
    return R & ~(1 << c2)
def comp(c, u):
    seen = {u}; st = [u]
    while st:
        x = st.pop()
        for y in nb[x]:
            if y != c and y not in seen: seen.add(y); st.append(y)
    return sum(1 << v for v in seen)
br = {c: [comp(c, u) for u in nb[c]] for c in nb}
ALL = sum(1 << v for v in range(1, n + 1))
states = [(c, S) for c in range(1, n + 1) for S in range(2, ALL + 1, 2) if not S >> c & 1]
succ = {}
for (c, S) in states:
    l = []
    for c2 in nb[c] + [c]:
        S1 = S & ~(1 << c2)
        l.append(None if S1 == 0 else (c2, expand(S1, c2)))
    succ[(c, S)] = l
def answers(c, S):
    if G == 'E': return [1 << v for v in range(1, n + 1) if S >> v & 1]
    return [S & b for b in br[c] if S & b]
W = []
for j in range(kmax + 1):
    win = set()
    for st in states:
        c, S = st
        if None in succ[st]: win.add(st); continue
        if j > 0:
            ok = True
            for A in answers(c, S):
                good = False
                for c2 in nb[c] + [c]:
                    A1 = A & ~(1 << c2)
                    if A1 == 0 or (c2, expand(A1, c2)) in W[j - 1]: good = True; break
                if not good: ok = False; break
            if ok: win.add(st)
    changed = True; sweeps = 0
    while changed:
        changed = False; sweeps += 1
        for st in states:
            if st not in win and any(t in win for t in succ[st]): win.add(st); changed = True
    W.append(win)
    full = sum(1 for c in range(1, n + 1) if (c, ALL & ~(1 << c)) in win)
    print("brute %s h=%d k=%d: winning states %d of %d, winning full starts %d/%d (sweeps %d)" % (G, h, j, len(win), len(states), full, n, sweeps))
# compare every state with the C++ solver
p = subprocess.Popen(["./solve", "query", G, str(h)], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
q = [(j, c, S) for j in range(kmax + 1) for (c, S) in states]
out, _ = p.communicate("".join("%d %d %x\n" % t for t in q))
res = out.split()
assert len(res) == len(q)
mism = sum(1 for (j, c, S), r in zip(q, res) if (r == '1') != ((c, S) in W[j]))
print("compared %d states with solve.cpp: mismatches %d" % (len(q), mism))
