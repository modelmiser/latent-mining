#!/usr/bin/env python3
# Referee: mechanical test of the belief-set reduction.  Random cop histories on T_h (moves, glimpse decisions, and
# an arbitrary consistent answer each glimpse); the belief S computed by the update rule is compared with the set of
# end vertices of ALL explicit robber trajectories (start != c0, never captured, giving exactly those answers).
import random, sys
h = int(sys.argv[1]); trials = int(sys.argv[2]); broken = len(sys.argv) > 3
n = 2 ** (h + 1) - 1
nb = {v: [u for u in (v // 2, 2 * v, 2 * v + 1) if 1 <= u <= n] for v in range(1, n + 1)}
def toward(c, r):
    # neighbour of c on the path to r
    path = [r]; anc = set()
    x = c
    while x: anc.add(x); x //= 2
    y = r
    while y not in anc: y //= 2
    if y == c:   # r below c
        z = r
        while z // 2 != c: z //= 2
        return z
    return c // 2
rng = random.Random(7); mism = 0; nonempty = 0
for G in 'ED':
    for _ in range(trials):
        c = rng.randint(1, n); S = set(range(1, n + 1)) - {c}
        trajs = {(r,) for r in S}            # explicit trajectories (tuples)
        for rnd in range(rng.randint(1, 7)):
            if not S: break
            if rng.random() < 0.4:           # glimpse: adversary picks any consistent answer
                if G == 'E':
                    a = rng.choice(sorted(S)); S = {a}; trajs = {t for t in trajs if t[-1] == a}
                else:
                    a = rng.choice(sorted({toward(c, r) for r in S}))
                    S = {r for r in S if toward(c, r) == a}; trajs = {t for t in trajs if toward(c, t[-1]) == a}
            c2 = rng.choice(nb[c] + [c])
            S1 = S - {c2}; trajs = {t for t in trajs if t[-1] != c2}
            S = set() if broken else set(S1)
            for v in S1: S.update(nb[v])
            S.discard(c2)
            trajs = {t + (r2,) for t in trajs for r2 in nb[t[-1]] + [t[-1]] if r2 != c2}
            c = c2
            ends = {t[-1] for t in trajs}
            if ends != S: mism += 1
            if S: nonempty += 1
print("T_%d: %d random histories per game, belief != explicit trajectory endpoints in %d round-checks (non-empty checks %d)%s" % (h, trials, mism, nonempty, "  [broken update: must be >0]" if broken else ""))
