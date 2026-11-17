#!/usr/bin/env python3
"""Referee's witness checker (raw rules, own code).
 wcheck.py A k n policyfile [lostfile]   solver-a policy table (normal form: probe U tops first)
 wcheck.py B k n stratfile  [lostfile]   solver-b strategy table (explicit OUT moves)
 wcheck.py C k n certfile                solver-b Bellman certificate
Prints beaten count; exit 1 on any mismatch with claimed header / lost file / Bellman equation."""
import sys, itertools, math
mode, k, n, path = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), sys.argv[4]
lostf = sys.argv[5] if len(sys.argv) > 5 else None

class Game:
    """raw rules; items are [value, flag]"""
    def __init__(self, order):
        self.order = list(order); self.pos = 0; self.m = 1
        self.S = [[] for _ in range(k)]
    def unarrived(self): return n - self.pos
    def attempt(self, mv):
        t, i = mv
        if t == 'IN':
            if len(self.S[0]) < 2 and self.pos < n:
                self.S[0].append([self.order[self.pos], 0]); self.pos += 1; return True
            return False
        if t == 'SHIFT':
            if 1 <= i < k and self.S[i-1] and len(self.S[i]) < 2:
                self.S[i].append(self.S[i-1].pop()); return True
            return False
        if t == 'OUT':
            if 1 <= i <= k and self.S[i-1] and self.S[i-1][-1][0] == self.m:
                self.S[i-1].pop(); self.m += 1
                for s in self.S:
                    for it in s: it[1] = 0
                return True
            if 1 <= i <= k and self.S[i-1]: self.S[i-1][-1][1] = 1   # Sorter's own bookkeeping
            return False
        raise ValueError(mv)
    def done(self): return self.m == n + 1

def parse_move(w):
    if w == 'IN': return ('IN', 0)
    if w.startswith('SHIFT'): return ('SHIFT', int(w[5:]))
    if w.startswith('OUT'): return ('OUT', int(w[3:]))
    if w.startswith('S'): return ('SHIFT', int(w[1:]))
    raise ValueError(w)

def play_A(tab, order):
    g = Game(order)
    for _ in range(100000):
        ch = True
        while ch:
            ch = False
            for i in range(k):
                if g.S[i] and g.S[i][-1][1] == 0:
                    g.attempt(('OUT', i+1)); ch = True; break
        if g.done(): return True
        key = ("|".join("".join("UK"[it[1]] for it in s) or "-" for s in g.S), g.unarrived())
        if key not in tab: return False
        if not g.attempt(tab[key]): return False      # refused forever
    return False

def play_B(tab, order):
    g = Game(order)
    for _ in range(100000):
        if g.done(): return True
        key = (g.unarrived(), tuple("".join(str(it[1]) for it in s) for s in g.S))
        if key not in tab: return False
        mv = tab[key]; ok = g.attempt(mv)
        if not ok and mv[0] != 'OUT': return False
        if not ok and mv[0] == 'OUT' and key == (g.unarrived(), tuple("".join(str(it[1]) for it in s) for s in g.S)):
            return False                               # refused OUT that taught nothing: loops forever
    return False

if mode in 'AB':
    tab = {}; claimed = None
    for line in open(path):
        line = line.strip()
        if not line: continue
        if line.startswith('#'):
            if 'claimed_beaten=' in line: claimed = int(line.split('claimed_beaten=')[1])
            continue
        if mode == 'A':
            st, u, mv = line.split(); key = (st, int(u))
        else:
            r, st, mv = [x.strip() for x in line.split('|')]
            key = (int(r), tuple('' if s.strip() == '-' else s.strip() for s in st.split(';')))
        if key in tab: print("REJECT duplicate key", key); sys.exit(1)
        tab[key] = parse_move(mv)
    play = play_A if mode == 'A' else play_B
    lost = [p for p in itertools.permutations(range(1, n+1)) if not play(tab, p)]
    beaten = math.factorial(n) - len(lost)
    msg = "%s k=%d n=%d beaten=%d of %d" % (path.split('/')[-1], k, n, beaten, math.factorial(n))
    rc = 0
    if claimed is not None and claimed != beaten: msg += "  REJECT (claimed %d)" % claimed; rc = 1
    if lostf:
        theirs = sorted(tuple(map(int, l.split())) for l in open(lostf) if l.strip() and l[0].isdigit())
        if theirs != sorted(lost): msg += "  REJECT lost-file mismatch (%d vs %d)" % (len(theirs), len(lost)); rc = 1
        else: msg += "  lost-file matches (%d, all with 1 last: %s)" % (len(lost), all(p[-1] == 1 for p in lost))
    print(msg); sys.exit(rc)

# mode C: Bellman certificate on flag states (presupposes the flag reduction)
val = {}
for line in open(path):
    r, st, v = [x.strip() for x in line.split('|')]
    val[(int(r), tuple('' if s.strip() == '-' else s.strip() for s in st.split(';')))] = int(v)
root = (n, ('',) * k)
if root not in val: print("REJECT no root"); sys.exit(1)
bad = 0; todo = [root]; seen = {root}
while todo:
    r, st = todo.pop()
    succs = []
    if r > 0 and len(st[0]) < 2: succs.append([(r-1, (st[0] + '0',) + st[1:])])
    for i in range(k-1):
        if st[i] and len(st[i+1]) < 2:
            t = list(st); t[i+1] = t[i+1] + t[i][-1]; t[i] = t[i][:-1]; succs.append([(r, tuple(t))])
    for i in range(k):
        if st[i] and st[i][-1] == '0':
            t = list(st); t[i] = t[i][:-1]; t = tuple('0' * len(s) for s in t)
            f = list(st); f[i] = f[i][:-1] + '1'
            succs.append([(r, t), (r, tuple(f))])
    for o in succs:
        for s in o:
            if s not in val: print("REJECT missing", s); sys.exit(1)
            if s not in seen: seen.add(s); todo.append(s)
    want = 1 if (r == 0 and not any(st)) else max([sum(val[s] for s in o) for o in succs], default=0)
    if want != val[(r, st)]: bad += 1
if bad: print("%s REJECT: %d states violate Bellman" % (path.split('/')[-1], bad)); sys.exit(1)
print("%s cert OK root=%d reachable=%d listed=%d" % (path.split('/')[-1], val[root], len(seen), len(val)))
