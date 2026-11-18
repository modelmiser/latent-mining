#!/usr/bin/env python3
"""Independent checker for LOWER-BOUND certificates written by fast3 (env DUMPLO).
usage: check_lb.py J FILE [EXPECTED_N EXPECTED_LB]
J = 0: plain game (terminal payoff 0);  J >= 1: W_J game, terminal payoff
       psi_J(X) = min over valid re-orientations X' of dist(X,X') + min(J, maxd(X')).
Each line 'v | components' claims value(state) >= v.  The claim is accepted iff SOME Adversary move
(two components, a vertex in each) has: for EVERY reply with c < v flips (any flip set of old edges of
the merged tree, either direction of the new edge, outdegrees <= 2), lb(successor) >= v - c, where
lb(successor) is the listed bound, or psi_J for a single tree, or (J = 0 only) the brute-force value of
a 2-component state.  Unlisted successors count as 0.  No restriction on the Algorithm is assumed
beyond the laziness lemma (proofs.txt P1); the Adversary may be restricted (it only needs one move)."""
import sys
from itertools import combinations
from check_dump import parse, canon, replies, valid, val2

def maxd(m, arcs):
    out = [[] for _ in range(m)]
    for a, b in arcs: out[a].append(b)
    best = 0
    for u in range(m):
        if len(out[u]) < 2: continue
        dist = {u: 0}; q = [u]; found = None
        while q and found is None:
            x = q.pop(0)
            for y in out[x]:
                if y not in dist:
                    dist[y] = dist[x] + 1
                    if len(out[y]) < 2: found = dist[y]; break
                    q.append(y)
        assert found is not None
        best = max(best, found)
    return best

psi_cache = {}
def psi(code, J):
    if code in psi_cache: return psi_cache[code]
    m, arcs = parse(code)
    best = min(J, maxd(m, arcs))
    for c in range(1, J):
        if c >= best: break
        for F in combinations(range(len(arcs)), c):
            al = [(b, a) if i in F else (a, b) for i, (a, b) in enumerate(arcs)]
            if valid(m, al): best = min(best, c + min(J, maxd(m, al)))
    psi_cache[code] = best
    return best

def main():
    J = int(sys.argv[1]); table = {}; n = None
    for line in open(sys.argv[2]):
        if line.startswith('#') or not line.strip(): continue
        v, rest = line.split('|')
        table[tuple(sorted(rest.split()))] = int(v)
    def lb(state):
        if len(state) == 1: return psi(state[0], J) if J else 0
        if len(state) == 2 and J == 0: return val2(state[0], state[1])
        return table.get(state, 0)
    bad = 0
    for S, v in table.items():
        trees = [parse(c) for c in S]; n = sum(t[0] for t in trees)
        ok = False; seen = set()
        for i in range(len(S)):
            for j in range(i + 1, len(S)):
                if ok or (S[i], S[j]) in seen: continue
                seen.add((S[i], S[j]))
                rest = [S[x] for x in range(len(S)) if x != i and x != j]
                for u in range(trees[i][0]):
                    for w in range(trees[j][0]):
                        if ok: break
                        good = True
                        for c in range(0, v):
                            for code in replies(trees[i], trees[j], u, w, c):
                                if c + lb(tuple(sorted(rest + [code]))) < v: good = False; break
                            if not good: break
                        if good: ok = True
        if not ok:
            bad += 1
            if bad <= 5: print("NOT JUSTIFIED:", v, S)
    start = tuple(['()'] * n)
    print(f"J={J} claims checked: {len(table)}  unjustified: {bad}  n={n}  proved lower bound for the start state: {table.get(start, 0)}")
    exp_ok = True
    if len(sys.argv) > 4: exp_ok = (n == int(sys.argv[3]) and table.get(start, 0) >= int(sys.argv[4]))
    print("CERTIFICATE OK" if bad == 0 and exp_ok else "CERTIFICATE REJECTED")

if __name__ == "__main__":
    main()
