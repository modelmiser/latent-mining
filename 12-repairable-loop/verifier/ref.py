#!/usr/bin/env python3
# Referee's reference implementation, literal from the problem statement. No pruning, every cell.
import sys, itertools
def covers_all(v, n):
    m = len(v)
    if m < 1: return False
    seen = set()
    for i in range(m):
        seen.add(tuple(v[(i + j) % m] for j in range(n)))
    return len(seen) == 2 ** n
def edits(v):
    """all (name, result) for one legal edit or pass"""
    m = len(v); out = [(('pass', -1), v)]
    for k in range(m):
        if m >= 2: out.append((('loss', k), v[:k] + v[k+1:]))
        out.append((('dbl', k), v[:k+1] + v[k:]))   # copy of v_k immediately after v_k
    return out
def replies(w1, n):
    return [name for name, w2 in edits(w1) if covers_all(w2, n)]
def repairable(w, n, strategy=None):
    ok = True
    for name, w1 in edits(w):
        r = replies(w1, n)
        if strategy is not None: strategy.append((name, w1, r))
        if not r:
            if strategy is None: return False
            ok = False
    return ok
def table(n, Lmax):
    for L in range(1, Lmax + 1):
        cnt = 0; nonu = 0; ex = None
        for t in itertools.product((0, 1), repeat=L):
            if repairable(t, n):
                cnt += 1
                if not covers_all(t, n): nonu += 1
                if ex is None: ex = ''.join(map(str, t))
        print(f"n={n} L={L} repairable_strings={cnt} nonuniversal={nonu} first={ex}", flush=True)
if __name__ == '__main__':
    if sys.argv[1] == 'table':
        table(int(sys.argv[2]), int(sys.argv[3]))
    elif sys.argv[1] == 'check':
        n = int(sys.argv[2]); w = tuple(int(c) for c in sys.argv[3])
        st = []
        ok = repairable(w, n, st)
        bad = [(a, ''.join(map(str, w1))) for a, w1, r in st if not r]
        print(f"n={n} len={len(w)} universal={covers_all(w, n)} adversary_moves={len(st)} unanswered={len(bad)} -> {'REPAIRABLE' if ok else 'NOT repairable'}")
        for b in bad[:3]: print("   unanswered:", b)
