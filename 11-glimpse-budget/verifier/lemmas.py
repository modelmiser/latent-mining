#!/usr/bin/env python3
# Referee: mechanical tests of solver A's blind lemmas W_full and W_seed (all three clauses F, W, K),
# and of the answer choice in the induction step of Lam(j).  Blind play only (no glimpses), exhaustive
# over every cop walk (as a set of belief states per time step).
import sys
HMAX_FULL, HMAX_SEED = int(sys.argv[1]), int(sys.argv[2])
def setup(h):
    n = 2 ** (h + 1) - 1
    nb = {v: [u for u in (v // 2, 2 * v, 2 * v + 1) if 1 <= u <= n] for v in range(1, n + 1)}
    nbm = {v: sum(1 << u for u in nb[v]) | (1 << v) for v in nb}
    dep = {v: v.bit_length() - 1 for v in nb}
    return n, nb, nbm, dep
def expand(S1, c2, nbm):
    R = 0; x = S1
    while x:
        low = x & -x; R |= nbm[low.bit_length() - 1]; x ^= low
    return R & ~(1 << c2)
def sub(v, n):
    out = []; st = [v]
    while st:
        x = st.pop(); out.append(x)
        if 2 * x <= n: st += [2 * x, 2 * x + 1]
    return out
def step(level, nb, nbm):
    nxt = set()
    for c, S in level:
        for c2 in nb[c] + [c]:
            S1 = S & ~(1 << c2)
            assert S1, ("belief emptied", c, S, c2)
            nxt.add((c2, expand(S1, c2, nbm)))
    return nxt
def has(S, v): return S >> v & 1

# ---- W_full: every blind-reachable state from a full start ----
def wfull(h, minheight=2, fail_ok=False):
    n, nb, nbm, dep = setup(h); ALL = sum(1 << v for v in nb)
    seen = set((c, ALL & ~(1 << c)) for c in nb); st = list(seen); viol = 0
    while st:
        c, S = st.pop()
        for z in nb:
            if z != c and h - dep[z] >= minheight and not has(S, z): viol += 1
        if h - dep[c] == 2 and not (has(S, 2 * c) or has(S, 2 * c + 1)): viol += 1
        for c2 in nb[c] + [c]:
            S1 = S & ~(1 << c2)
            if not S1: viol += 1; continue
            t = (c2, expand(S1, c2, nbm))
            if t not in seen: seen.add(t); st.append(t)
    return len(seen), viol
for h in range(3, HMAX_FULL + 1):
    cnt, viol = wfull(h)
    print("W_full (W)+(K), T_%d: %d blind states from all full starts, violations %d" % (h, cnt, viol))
print("  [check that can fail] W_full with 'height>=1' on T_3: violations %d (must be >0)" % wfull(3, 1)[1])
print("  [check that can fail] W_full on T_2 (m>=3 violated): violations %d (must be >0)" % wfull(2)[1])

# ---- W_seed and the Lam step ----
def wseed(h, v, mutate=None):
    n, nb, nbm, dep = setup(h); m = h - dep[v]; X = set(sub(v, n)); Xm = sum(1 << x for x in X)
    seed = (1 << v) | (1 << 2 * v) | (1 << 2 * v + 1)
    level = set((c, seed) for c in nb if c not in X)
    viol = {'F': 0, 'W': 0, 'K': 0, 'U': 0, 'two': 0}; examined = 0; allseen = set()
    for t in range(0, 4 * h + 12):
        new = level - allseen
        if not new and t > m + 3: break      # (time matters only through depth<=t+1, vacuous once t+1>=m)
        allseen |= level
        for c, S in level:
            examined += 1
            for z in X:
                dz = dep[z] - dep[v]; hz = h - dep[z]
                if dz == t + 1 and not has(S, z): viol['F'] += 1
                if z != c and hz >= 2 and dz <= t + 1 and not has(S, z): viol['W'] += 1
            if c in X and h - dep[c] == 2 and not (has(S, 2 * c) or has(S, 2 * c + 1)): viol['K'] += 1
            if sum(1 for z in (v, 2 * v, 2 * v + 1) if z != c and has(S, z)) < 2: viol['two'] += 1
            # Lam step: the prescribed exact answer u (needs m>=4)
            if m >= 4:
                if c not in X: u = 2 * v
                elif c == v: u = 4 * v if mutate != 'child' else 2 * v
                else:
                    a = c
                    while a // 2 != v: a //= 2
                    u = a ^ 1
                Xu = set(sub(u, n))
                # distance >= 2  <=>  c not in N[u]
                if not has(S, u) or c in Xu or c == u or c in nb[u]: viol['U'] += 1
        level = step(level, nb, nbm)
    return m, examined, viol
for h in range(4, HMAX_SEED + 1):
    tot = {}; ex = 0
    for v in range(2, 2 ** (h - 2)):          # every v != root with m = h-depth(v) >= 3
        m, e, viol = wseed(h, v); ex += e
        for k in viol: tot[k] = tot.get(k, 0) + viol[k]
    print("W_seed in T_%d, every v with m>=3, every cop start outside X: %d timed states; violations F=%d W=%d K=%d ; 'two of v,v1,v2 remain' %d ; Lam-step answer u (in S, dist>=2, cop outside X_u) %d" % (h, ex, tot['F'], tot['W'], tot['K'], tot['two'], tot['U']))
# checks that can fail
m, e, viol = wseed(4, 4)      # m = 2: lemma not claimed
print("  [check that can fail] W_seed with m=2 (v=4 in T_4): F=%d W=%d K=%d two=%d (some must be >0)" % (viol['F'], viol['W'], viol['K'], viol['two']))
m, e, viol = wseed(5, 2, mutate='child')
print("  [check that can fail] Lam step with u=child of v when cop is at v: U violations %d (must be >0)" % viol['U'])
