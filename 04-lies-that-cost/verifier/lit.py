#!/usr/bin/env python3
"""Referee: literal implementation of the definition (pairs (x,L), all L subsets of {1..t} enumerated,
ell_s from C_{s-1}) compared step by step with the obligation abstraction (solvers' Lemma 1 / P1).
Usage: lit.py exhaustive | random SEED TRIALS"""
import sys, random, itertools

def literal(n, k, Ss, As):
    """Return (list of C_0..C_t, list of V_0..V_t).  Pure definition: enumerate every L."""
    Cs = [frozenset(range(1, n + 1))]
    Vs = [frozenset((x, frozenset()) for x in range(1, n + 1))]
    ells = []
    for t in range(1, len(Ss) + 1):
        S = Ss[t - 1]; Cp = Cs[-1]
        ells.append(min(len(S & Cp), len(Cp - S)))          # ell_t from C_{t-1}
        V = set()
        for x in range(1, n + 1):
            for mask in range(1 << t):
                L = frozenset(s for s in range(1, t + 1) if mask >> (s - 1) & 1)
                ok = True
                for s in range(1, t + 1):                    # (i)
                    truthful = (As[s - 1] == (x in Ss[s - 1]))
                    if truthful != (s not in L): ok = False; break
                if not ok: continue
                for s in L:                                   # (ii)
                    for u in range(s + 1, s + k * ells[s - 1] + 1):
                        if u in L: ok = False; break
                    if not ok: break
                if ok: V.add((x, L))
        Vs.append(frozenset(V))
        Cs.append(frozenset(x for x, _ in V))
    return Cs, Vs, ells

def abstract_step(state, S, ans, k):
    """state: dict alive x -> remaining obligation r.  Claimed transition."""
    alive = set(state)
    ell = min(len(S & alive), len(alive - S))
    new = {}
    for x, r in state.items():
        if (x in S) == ans: new[x] = max(r - 1, 0)
        elif r == 0: new[x] = k * ell
    return new

def check(n, k, Ss, As, stats):
    Cs, Vs, ells = literal(n, k, Ss, As)
    st = {x: 0 for x in range(1, n + 1)}
    for t in range(1, len(Ss) + 1):
        st = abstract_step(st, Ss[t - 1], As[t - 1], k)
        if frozenset(st) != Cs[t]:
            print("MISMATCH alive", n, k, Ss, As, t, sorted(st), sorted(Cs[t])); sys.exit(1)
        # each alive x has exactly one L; r_x equals max(s+k*ell_s)-t clipped
        byx = {}
        for x, L in Vs[t]: byx.setdefault(x, []).append(L)
        for x, Ls in byx.items():
            if len(Ls) != 1: print("MISMATCH: x with several L", x, Ls); sys.exit(1)
            D = max([s + k * ells[s - 1] for s in Ls[0]] + [0])
            if max(0, D - t) != st[x]:
                print("MISMATCH obligation", n, k, Ss, As, t, x, st[x], D - t); sys.exit(1)
        # monotonicity of V: restriction of a valid pair is valid earlier
        for x, L in Vs[t]:
            if (x, frozenset(s for s in L if s < t)) not in Vs[t - 1]:
                print("MISMATCH restriction"); sys.exit(1)
        if ells[t - 1] == 0: stats['ell0'] += 1
        if len(Cs[t]) == 0: stats['empty'] += 1
        if Ss[t - 1] - Cs[t - 1]: stats['outside'] += 1
    stats['hist'] += 1

def subsets(n):
    return [frozenset(x for x in range(1, n + 1) if m >> (x - 1) & 1) for m in range(1 << n)]

def main():
    stats = dict(hist=0, ell0=0, empty=0, outside=0)
    if sys.argv[1] == 'exhaustive':
        for n, T in ((2, 5), (3, 4), (4, 3)):
            subs = subsets(n)
            for k in (1, 2, 3):
                for Ss in itertools.product(subs, repeat=T):
                    for am in range(1 << T):
                        As = [bool(am >> i & 1) for i in range(T)]
                        check(n, k, list(Ss), As, stats)
                print("exhaustive n=%d T=%d k=%d ok" % (n, T, k), stats, flush=True)
    else:
        rng = random.Random(int(sys.argv[2]))
        for _ in range(int(sys.argv[3])):
            n = rng.randint(2, 8); k = rng.randint(1, 4); T = rng.randint(1, 11)
            Ss = []; As = []
            st = {x: 0 for x in range(1, n + 1)}
            for t in range(T):
                mode = rng.random()
                alive = sorted(st)
                if mode < 0.15 and alive:      # ell = 0 question, possibly with dead elements
                    S = set(alive) if rng.random() < 0.5 else set()
                    S |= {x for x in range(1, n + 1) if x not in st and rng.random() < 0.5}
                elif mode < 0.45 and alive:    # lopsided
                    S = set(rng.sample(alive, min(len(alive), 1)))
                    S |= {x for x in range(1, n + 1) if x not in st and rng.random() < 0.5}
                else:
                    S = {x for x in range(1, n + 1) if rng.random() < 0.5}
                S = frozenset(S)
                ans = rng.random() < 0.5
                # bias toward legal answers so that games run long
                if not abstract_step(st, S, ans, k) and rng.random() < 0.9: ans = not ans
                Ss.append(S); As.append(ans)
                st = abstract_step(st, S, ans, k)
            check(n, k, Ss, As, stats)
        print("random ok", stats)

main()
