#!/usr/bin/env python3
"""Deliberately naive second implementation.  States are frozensets of explicit blocks
(start, size); no symmetry, no bit tricks.  States are discovered by forward search from the
empty heap over EVERY manager choice, then the greatest fixpoint is taken on that set.
usage: naive.py G M H [sizes e.g. 123]"""
import sys

def run(G, M, H, sizes=(1, 2, 3)):
    def cells(S):
        occ = set()
        for (p, s) in S:
            occ.update(range(p, p + s))
        return occ
    def placements(S, s):
        occ = cells(S)
        return [S | {(p, s)} for p in range(H - s + 1) if not any(c in occ for c in range(p, p + s))]
    def moves(S, sz):
        out = []
        for b in S:
            if b[1] == sz:
                R = S - {b}
                occ = cells(R)
                for p in range(H - sz + 1):
                    if p != b[0] and not any(c in occ for c in range(p, p + sz)):
                        out.append(R | {(p, sz)})
        return out
    def after_free(S, b):
        R = S - {b}
        res = [R]
        if G == 'F':
            if b[1] == 2:
                res += moves(R, 1)
            if b[1] == 3:
                one = moves(R, 1)
                res += one + moves(R, 2)
                for T in one:
                    res += moves(T, 1)
        return res
    def live(S):
        return sum(s for (_, s) in S)
    # forward closure
    start = frozenset()
    seen = {start}
    todo = [start]
    succ = {}
    while todo:
        S = todo.pop()
        req = []
        L = live(S)
        for s in sizes:
            if L + s <= M:
                req.append([frozenset(T) for T in placements(S, s)])
        for b in S:
            if G == 'O' and b[1] != 1:
                continue
            req.append([frozenset(T) for T in after_free(S, b)])
        succ[S] = req
        for opts in req:
            for T in opts:
                if T not in seen:
                    seen.add(T)
                    todo.append(T)
    safe = set(seen)
    changed = True
    while changed:
        changed = False
        for S in list(safe):
            if any(not any(T in safe for T in opts) for opts in succ[S]):
                safe.discard(S)
                changed = True
    return start in safe, len(seen)

if __name__ == '__main__':
    G, M, H = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
    sizes = tuple(int(c) for c in sys.argv[4]) if len(sys.argv) > 4 else (1, 2, 3)
    w, n = run(G, M, H, sizes)
    print(f"G={G} M={M} H={H} sizes={sizes} reachable={n} {'WIN' if w else 'LOSE'}")
