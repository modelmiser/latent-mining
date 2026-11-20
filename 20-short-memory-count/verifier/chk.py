#!/usr/bin/env python3
"""Referee's definitional evaluator/checker. No code shared with ref.cpp or the solvers.
A machine is (w, trans) with trans[q] = (d(q,0), d(q,1)), start state 0 (or `start`)."""
import sys, itertools

def Csets(w, tr, start=0):
    """C(q) by the definition: all strings of length exactly w explicitly, then closure of (state, window)."""
    n = len(tr)
    seen = set()
    for x in itertools.product((0, 1), repeat=w):
        q = start
        for b in x:
            q = tr[q][b]
        seen.add((q, x))
    stack = list(seen)
    while stack:
        q, x = stack.pop()
        for b in (0, 1):
            y = (tr[q][b], x[1:] + (b,))
            if y not in seen:
                seen.add(y); stack.append(y)
    C = [set() for _ in range(n)]
    for q, x in seen:
        C[q].add(sum(x))
    return C

def reach(tr, start=0):
    R = {start}; st = [start]
    while st:
        q = st.pop()
        for b in (0, 1):
            if tr[q][b] not in R:
                R.add(tr[q][b]); st.append(tr[q][b])
    return R

def simple_cycles(tr, R):
    """all simple cycles (as state tuples, smallest state first) among reachable states"""
    out = []
    def dfs(first, path, on):
        q = path[-1]
        for t in set(tr[q]):
            if t == first:
                out.append(tuple(path))
            elif t > first and t not in on and t in R:
                on.add(t); path.append(t); dfs(first, path, on); path.pop(); on.discard(t)
    for f in sorted(R):
        dfs(f, [f], {f})
    return out

def g_of(w, tr, start=0):
    C = Csets(w, tr, start)
    R = reach(tr, start)
    sp = [max(c) - min(c) if c else None for c in C]
    best = None
    for Z in simple_cycles(tr, R):
        assert all(sp[q] is not None for q in Z)
        m = min(sp[q] for q in Z)
        best = m if best is None else max(best, m)
    return best, sp, C

def legal(w, tr, lit, s, start=0):
    """game reading: (1) no cycle of reachable unlit states, (2) lit states with nonempty C have spread <= s"""
    C = Csets(w, tr, start); R = reach(tr, start)
    for q in lit:
        if not (0 <= q < len(tr)): return False, "lit state out of range"
        if C[q] and max(C[q]) - min(C[q]) > s:
            return False, "lit state %d has spread %d > %d" % (q, max(C[q]) - min(C[q]), s)
    for Z in simple_cycles(tr, R):
        if not any(q in lit for q in Z):
            return False, "cycle %s has no lit state" % (Z,)
    return True, "ok"

def valid_machine(w, tr, maxn=None):
    n = len(tr)
    if maxn is not None and len(reach(tr)) > maxn: return False
    return all(len(t) == 2 and all(0 <= x < n for x in t) for t in tr)

def parse_a(path):
    toks = []
    for line in open(path):
        line = line.split('#')[0].split()
        if line: toks.append([int(x) for x in line])
    w = toks[0][0]; s = toks[1][0]; n = toks[2][0]
    tr = [tuple(t) for t in toks[3:3 + n]]
    lit = toks[3 + n] if len(toks) > 3 + n else []
    return w, s, tr, lit

def parse_b_line(line):
    v = [int(x) for x in line.split()]
    w, n = v[0], v[1]
    tr = [(v[2 + 2 * i], v[3 + 2 * i]) for i in range(n)]
    g = v[2 + 2 * n] if len(v) > 2 + 2 * n else None
    assert len(v) in (2 + 2 * n, 3 + 2 * n)
    return w, tr, g

if __name__ == "__main__":
    mode = sys.argv[1]
    if mode == "a":   # solver-a witness files: legal (M,F,s) with <= w+1 states?
        for p in sys.argv[2:]:
            w, s, tr, lit = parse_a(p)
            if not valid_machine(w, tr) or len(reach(tr)) > w + 1:
                print(p, "REJECT bad machine / too many states"); continue
            ok, why = legal(w, tr, set(lit), s)
            g, sp, C = g_of(w, tr)
            print(p, "ACCEPT" if ok else "REJECT", why, "w=%d s=%d n=%d g=%s spreads=%s" % (w, s, len(tr), g, sp))
    elif mode == "b":  # solver-b lines: recompute g, compare with the claimed g
        for p in sys.argv[2:]:
            for line in open(p):
                if not line.strip(): continue
                w, tr, gc = parse_b_line(line)
                if not valid_machine(w, tr): print(p, "REJECT bad machine", line.strip()); continue
                g, sp, C = g_of(w, tr)
                print(p, "ACCEPT" if g == gc else "REJECT", "w=%d n=%d reach=%d claimed g=%s mine g=%s spreads=%s" % (w, len(tr), len(reach(tr)), gc, g, sp))
