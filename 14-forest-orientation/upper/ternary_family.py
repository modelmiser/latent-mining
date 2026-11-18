#!/usr/bin/env python3
"""Theorem 4 of NOTE.md: the ternary family A_j against rule R.   usage: ternary_family.py JMAX   (NOTE.md quotes 9)
  A_1 = root with two leaves (cost 0).
A_j: take three copies X, Y, Z of A_{j-1} with roots x, y, z.  Insert edge {x, y}, then edge {x, z}.
Each time both endpoints have h = j-1; the rule (tie broken towards x) flips a shortest path from x and adds x -> y, x -> z.
Checks: every insertion is a legal greedy step, each of the two pays j-1, h(root of A_j) = j, total cost = (3^j - 1)/2 - j."""
import sys
sys.setrecursionlimit(100000)
def hs(out):
    h = [None] * len(out)
    for x in range(len(out)):                      # iterative post-order
        st = [x]
        while st:
            v = st[-1]
            if h[v] is not None: st.pop(); continue
            if len(out[v]) < 2: h[v] = 0; st.pop(); continue
            todo = [c for c in out[v] if h[c] is None]
            if todo: st.extend(todo)
            else: h[v] = 1 + min(h[out[v][0]], h[out[v][1]]); st.pop()
    return h
def insert(out, p, q):
    """greedy step for edge {p,q}, flipping from p; returns cost.  Asserts p is a legal choice."""
    h = hs(out)
    if h[p] > h[q]: sys.exit('FAILED: rule R would flip from the other endpoint')
    path = [p]
    while h[path[-1]] > 0:
        v = path[-1]; path.append(next(c for c in out[v] if h[c] == h[v] - 1))
    for a, b in zip(path, path[1:]): out[a].remove(b); out[b].append(a)
    out[p].append(q)
    if any(len(o) > 2 for o in out): sys.exit('FAILED: out-degree above 2')
    return len(path) - 1
def build(j):
    if j == 1:                                     # A_1 is played too: two rounds of cost 0 out of vertex 0
        out = [[], [], []]; c = insert(out, 0, 1) + insert(out, 0, 2)
        if c != 0: sys.exit('FAILED: A_1 cost')
        return out, 0, 0                           # out-lists, root, cost
    A, r, c = build(j - 1); n = len(A)
    out = [list(o) for o in A] + [[w + n for w in o] for o in A] + [[w + 2 * n for w in o] for o in A]
    x, y, z = r, r + n, r + 2 * n
    k1 = insert(out, x, y); k2 = insert(out, x, z)
    if not (k1 == j - 1 and k2 == j - 1): sys.exit('FAILED: round costs %d %d at level %d' % (k1, k2, j))
    return out, x, 3 * c + k1 + k2
for j in range(1, int(sys.argv[1]) + 1):
    out, r, c = build(j); n = len(out); h = hs(out)
    if not (h[r] == j == max(h) and c == (3 ** j - 1) // 2 - j): sys.exit('FAILED at level %d' % j)
    print('j', j, 'n', n, 'cost', c, '(n-1)/2 - j =', (n - 1) // 2 - j, ' cost/n %.4f' % (c / n), ' H', max(h), flush=True)
