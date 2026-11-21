#!/usr/bin/env python3
"""Mechanical check of the construction in the proof of Theorem E (PROOFS.md, section 1).

For random trees (heavy index sharing), for every cycle that has a state with all-zero-path bits 0:
build the excursion tree F = U(j0, {}) exactly as in the proof and check
  (1) F is a legal tree of the model (no index twice on a path, leaf writes nothing or an index of its path,
      no index of the all-zero path of T occurs in F);
  (2) f_F maps the data part of each home state to the data part of the next home state;
  (3) the F-step is empty iff the excursion ends with an empty leaf of T;
  (4) no dummy leaf is reached from a home state of the cycle;
  (5) the cycle has at most one empty step (Theorem E itself).
usage: check_F.py NTREES SEED [mutate]
  mutate=1: do not resolve already-read data indices (wrong on purpose)
  mutate=2: resolve z-nodes to the 1-child (wrong on purpose)
"""
import random, sys

NT = int(sys.argv[1]); SEED = int(sys.argv[2]); MUT = int(sys.argv[3]) if len(sys.argv) > 3 else 0
rng = random.Random(SEED)


def rand_tree(depth, path, nidx, pleaf):
    if depth == 0 or len(path) == nidx or rng.random() < pleaf:
        w = rng.choice([None] + list(path)) if path else None
        return ('l', w)
    u = rng.choice([i for i in range(nidx) if i not in path])
    return ('n', u, rand_tree(depth - 1, path + [u], nidx, pleaf), rand_tree(depth - 1, path + [u], nidx, pleaf))


def step(tree, s):
    """s: tuple of bits. returns (new state, leaf_is_empty, leaf_is_dummy)"""
    read = []
    t = tree
    while t[0] == 'n':
        read.append(t[1]); t = t[3] if s[t[1]] else t[2]
    o = list(s)
    for u in read: o[u] = 0
    if t[1] is not None: o[t[1]] = 1
    return tuple(o), t[1] is None, len(t) > 2


def legal(tree, path, forbidden):
    if tree[0] == 'l':
        return tree[1] is None or tree[1] in path
    if tree[1] in path or tree[1] in forbidden: return False
    return legal(tree[2], path + [tree[1]], forbidden) and legal(tree[3], path + [tree[1]], forbidden)


def build_F(T):
    Z = []; S = []; t = T
    while t[0] == 'n':
        Z.append(t[1]); S.append(t[3]); t = t[2]
    lam0 = t[1]
    if lam0 is None: return None, Z
    j0 = Z.index(lam0)
    zset = set(Z)

    def U(i, K, visited):
        return proc(S[i], K, False, visited)

    def proc(node, K, grew, visited):
        if node[0] == 'n':
            u = node[1]
            if u in zset:
                return proc(node[3] if MUT == 2 else node[2], K, grew, visited)
            if u in K and MUT != 1:
                return proc(node[2], K, grew, visited)
            K2 = K | {u}
            return ('n', u, proc(node[2], K2, True, visited), proc(node[3], K2, True, visited))
        w = node[1]
        if w is None or w not in zset: return ('l', w)
        j = Z.index(w)
        if grew: return U(j, K, frozenset([j]))
        if j in visited: return ('l', None, 'dummy')
        return U(j, K, visited | {j})

    return U(j0, frozenset(), frozenset([j0])), Z


def sylv(r, base):
    # the lower-bound tree M_r on indices base .. base+2^r-2 ; returns (tree, next free index)
    if r == 0: return ('l', None), base
    X, n1 = sylv(r - 1, base + 1); Y, n2 = sylv(r - 1, n1)
    def freeleaf(t):
        if t[0] == 'l': return ('l', base)
        return ('n', t[1], t[2], freeleaf(t[3]))
    return ('n', base, freeleaf(X), Y), n2


def edit(t, path, nidx):
    # one random local change: relabel a node, or change what a leaf writes
    if t[0] == 'l':
        return ('l', rng.choice([None] + list(path)) if path else None)
    c = rng.random()
    if c < 0.25:
        below = set()
        def col(q):
            if q[0] == 'n': below.add(q[1]); col(q[2]); col(q[3])
        col(t)
        cand = [i for i in range(nidx) if i not in path and (i not in below or i == t[1])]
        u = rng.choice(cand)
        def fix(q, old, new):  # leaves below that wrote the old index now write the new one
            if q[0] == 'l': return ('l', new if q[1] == old else q[1])
            return ('n', q[1], fix(q[2], old, new), fix(q[3], old, new))
        return ('n', u, fix(t[2], t[1], u), fix(t[3], t[1], u))
    if c < 0.62: return ('n', t[1], edit(t[2], path + [t[1]], nidx), t[3])
    return ('n', t[1], t[2], edit(t[3], path + [t[1]], nidx))


def main():
    ntree = ncyc = nhome = 0; maxe = 0; multi = 0; maxH = 0
    for it in range(NT):
        if it % 2 == 0:
            nidx = rng.randint(1, 7); depth = rng.randint(1, 5)
            T = rand_tree(depth, [], nidx, rng.choice([0.05, 0.15, 0.3]))
        else:
            r = rng.choice([2, 3]); T, nidx = sylv(r, 0)
            for _ in range(rng.randint(0, 4)): T = edit(T, [], nidx)
            assert legal(T, [], set())
        try:
            F, Z = build_F(T)
        except RecursionError:
            print("FAIL (1): construction of F does not terminate MUT=%d" % MUT); sys.exit(1)
        ntree += 1
        states = [tuple((x >> i) & 1 for i in range(nidx)) for x in range(1 << nidx)]
        nxt = {s: step(T, s) for s in states}
        seen = set()
        for s in states:
            if s in seen: continue
            orb = []; pos = {}; x = s
            while x not in pos and x not in seen:
                pos[x] = len(orb); orb.append(x); x = nxt[x][0]
            cyc = orb[pos[x]:] if x in pos else []
            seen.update(orb)
            if len(cyc) < 2: continue
            ncyc += 1
            e = sum(1 for c in cyc if nxt[c][1])
            maxe = max(maxe, e)
            if e > 1:
                print("FAIL (5): cycle with", e, "empty steps", T, cyc[0]); sys.exit(1)
            homes = [q for q, c in enumerate(cyc) if all(c[z] == 0 for z in Z)]
            if not homes: continue
            if F is None:
                print("FAIL: home state on a cycle of length>=2 but all-zero leaf empty", T); sys.exit(1)
            if not legal(F, [], set(Z)):
                print("FAIL (1): F is not a legal tree", T, F); sys.exit(1)
            L = len(cyc)
            if len(homes) >= 2: multi += 1
            maxH = max(maxH, len(homes))
            for a, q in enumerate(homes):
                q2 = homes[(a + 1) % len(homes)]
                last = cyc[(q2 - 1) % L]            # last state of the excursion
                h, h2 = cyc[q], cyc[q2]
                o, emp, dummy = step(F, h)          # F never touches Z bits, which are 0 in h
                nhome += 1
                if dummy: print("FAIL (4): dummy reached", T, h); sys.exit(1)
                if o != h2: print("FAIL (2): f_F(home) != next home", T, F, h, o, h2); sys.exit(1)
                if emp != nxt[last][1]: print("FAIL (3): emptiness mismatch", T, F, h); sys.exit(1)
    print("OK trees=%d cycles(len>=2)=%d of which with >=2 home states=%d (max %d) home states checked=%d max empty steps on a cycle=%d MUT=%d" % (ntree, ncyc, multi, maxH, nhome, maxe, MUT))


main()
