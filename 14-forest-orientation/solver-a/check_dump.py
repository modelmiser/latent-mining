#!/usr/bin/env python3
"""Independent checker (shares no code with fast.cpp) for a value table dumped by
   ./fast fast N 2 0 DUMPFILE
Each line:  value | code code ...   (a state = multiset of oriented trees, >= 3 components)
code grammar: tree := '(' { ('a'|'b') tree } ')' ; 'a' = arc parent->child, 'b' = arc child->parent;
canonical = root at a centroid, children sorted as strings, lexicographically least over centroids.

For every listed state S with value v it verifies
  UB: for EVERY Adversary move (two components, any vertex in each) some reply (flip set F of old
      edges of the merged tree, direction of the new edge, all outdegrees <= 2) has |F| + val(succ) <= v
      with val(succ) known (listed, or <= 2 components and computed here by brute force);
  LB: SOME Adversary move has, for ALL replies with |F| < v, val(succ) known and |F| + val(succ) >= v.
(Replies that also change other components are not needed: laziness lemma, proofs.txt.)
States with 2 components are evaluated here from the definition (all 2^E orientations), not by a formula.
Finally it prints the value of the start state (N singletons), which must be listed."""
import sys
from itertools import combinations

CAP = 2

def parse(code):
    arcs = []; cnt = [0]; pos = [0]
    def rec():
        node = cnt[0]; cnt[0] += 1
        pos[0] += 1
        while code[pos[0]] != ')':
            d = code[pos[0]]; pos[0] += 1
            ch = rec()
            arcs.append((node, ch) if d == 'a' else (ch, node))
        pos[0] += 1
        return node
    rec()
    return cnt[0], arcs

def canon(m, arcs):
    adj = [[] for _ in range(m)]
    for a, b in arcs:
        adj[a].append((b, 'a')); adj[b].append((a, 'b'))
    # centroids by brute force definition
    def compsize(v, p):
        s = 1
        for w, _ in adj[v]:
            if w != p: s += compsize(w, v)
        return s
    cents = [v for v in range(m) if all(2 * compsize(w, v) <= m for w, _ in adj[v])]
    def code(v, p):
        return '(' + ''.join(sorted(d + code(w, v) for w, d in adj[v] if w != p)) + ')'
    return min(code(c, -1) for c in cents)

def valid(m, arcs):
    od = [0] * m
    for a, _ in arcs:
        od[a] += 1
        if od[a] > CAP: return False
    return True

def replies(tA, tB, u, v, c):
    """all (canonical code) results of merging at (u,v) with exactly c old-edge flips"""
    mA, aA = tA; mB, aB = tB
    old = list(aA) + [(a + mA, b + mA) for a, b in aB]
    m = mA + mB; out = set()
    for F in combinations(range(len(old)), c):
        arcs = [(b, a) if i in F else (a, b) for i, (a, b) in enumerate(old)]
        for new in ((u, v + mA), (v + mA, u)):
            al = arcs + [new]
            if valid(m, al): out.add(canon(m, al))
    return out

two_cache = {}
def val2(c1, c2):
    key = (c1, c2) if c1 <= c2 else (c2, c1)
    if key in two_cache: return two_cache[key]
    tA = parse(key[0]); tB = parse(key[1])
    mA, aA = tA; mB, aB = tB; m = mA + mB
    old = list(aA) + [(a + mA, b + mA) for a, b in aB]; E = len(old)
    best = 0
    for u in range(mA):
        for v in range(mB):
            r = None
            for c in range(E + 1):
                if r is not None: break
                for F in combinations(range(E), c):
                    arcs = [(b, a) if i in F else (a, b) for i, (a, b) in enumerate(old)]
                    if valid(m, arcs + [(u, v + mA)]) or valid(m, arcs + [(v + mA, u)]):
                        r = c; break
            best = max(best, r)
    two_cache[key] = best
    return best

def main():
    table = {}
    for line in open(sys.argv[1]):
        if line.startswith('#') or not line.strip(): continue
        v, rest = line.split('|')
        table[tuple(sorted(rest.split()))] = int(v)
    def val(state):
        if len(state) == 1: return 0
        if len(state) == 2: return val2(state[0], state[1])
        return table.get(state)
    bad = 0; n = None
    for S, v in table.items():
        trees = [parse(c) for c in S]
        n = sum(t[0] for t in trees)
        ub_ok = True; lb_ok = (v == 0)
        seen = set()
        for i in range(len(S)):
            for j in range(i + 1, len(S)):
                if (S[i], S[j]) in seen: continue
                seen.add((S[i], S[j]))
                rest = [S[x] for x in range(len(S)) if x != i and x != j]
                for u in range(trees[i][0]):
                    for w in range(trees[j][0]):
                        found_ub = False; all_lb = True
                        for c in range(0, v + 1):
                            for code in replies(trees[i], trees[j], u, w, c):
                                sv = val(tuple(sorted(rest + [code])))
                                if sv is not None and c + sv <= v: found_ub = True
                                if c < v and (sv is None or c + sv < v): all_lb = False
                        if not found_ub: ub_ok = False
                        if all_lb: lb_ok = True
        if not (ub_ok and lb_ok):
            bad += 1
            if bad <= 5: print("INCONSISTENT:", v, S, "UB ok" if ub_ok else "UB FAILS", "LB ok" if lb_ok else "LB FAILS")
    start = tuple(['()'] * n)
    print(f"states checked: {len(table)}  inconsistent: {bad}  n={n}  value of start state: {table.get(start)}")
    print("CERTIFICATE OK" if bad == 0 and start in table else "CERTIFICATE REJECTED")

if __name__ == "__main__":
    main()
