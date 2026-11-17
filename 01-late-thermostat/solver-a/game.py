#!/usr/bin/env python3
"""Delayed-sign control game: exact R(d), C(d), H(d).

Physical state BEFORE observation at tick t:
    (x_t, (sgn x_{t-d}, ..., sgn x_{t-1}), rest)      rest=1 iff w_t must be 0
Observation o_t = first element of the sign tuple.
Only signs of past levels matter for future observations, so this is an exact
(lossless for the game) compression of the window (x_{t-d..t}).
Transition with control u, disturbance w (w=0 if rest):
    x' = x+u+w ; signs' = signs[1:] + (sgn x,) ; rest' = (w != 0)
Band: |x_t| <= B for all t.
"""
import sys, itertools
from collections import deque

def sgn(x): return (x > 0) - (x < 0)

MAPS = list(itertools.product((-1, 0, 1), repeat=3))  # f = (f(-), f(0), f(+)), index o+1

def init(d): return (0, (0,) * d, 0)

def succ(state, u):
    x, sg, rest = state
    ns = sg[1:] + (sgn(x),)
    if rest:
        return [(x + u, ns, 0)]
    return [(x + u, ns, 0), (x + u + 1, ns, 1), (x + u - 1, ns, 1)]

# ---------------- RELAY ----------------
def relay_max(d, f, cap):
    """max |x| reachable under relay f, or None if it exceeds cap."""
    s0 = init(d); seen = {s0}; q = deque([s0]); m = 0
    while q:
        s = q.popleft()
        u = f[s[1][0] + 1]
        for n in succ(s, u):
            if abs(n[0]) > cap: return None
            if n not in seen:
                seen.add(n); q.append(n); m = max(m, abs(n[0]))
    return m

def relay(d, cap=60):
    res = {}
    for f in MAPS:
        res[f] = relay_max(d, f, cap)
    best = min(v for v in res.values() if v is not None)
    return best, [f for f, v in res.items() if v == best], res

# ---------------- CLOCK ----------------
def clock_step(S, f, B):
    out = set()
    for s in S:
        u = f[s[1][0] + 1]
        for n in succ(s, u):
            if abs(n[0]) > B: return None
            out.add(n)
    return frozenset(out)

def clock_safe(d, B, node_limit=3_000_000):
    """Does a clock sequence keep |x|<=B forever?  Iterative DFS on the graph of
    reachable-state-sets; success = a cycle (closed walk) found; returns (bool, witness)."""
    S0 = frozenset([init(d)])
    dead = set()
    onstack = {}
    # stack entries: [S, list of (f, T) successors, idx]
    def expand(S):
        obs = sorted({s[1][0] for s in S})
        seen = {}
        # only f-values on present observations matter
        for vals in itertools.product((-1, 0, 1), repeat=len(obs)):
            f = [0, 0, 0]
            for o, v in zip(obs, vals): f[o + 1] = v
            f = tuple(f)
            T = clock_step(S, f, B)
            if T is not None and T not in dead and T not in seen:
                seen[T] = f
        # heuristic: smaller sets first
        return sorted(((f, T) for T, f in seen.items()), key=lambda p: len(p[1]))
    stack = [[S0, expand(S0), 0, None]]
    onstack[S0] = 0
    count = 1
    while stack:
        top = stack[-1]
        S, ch, i, _ = top
        if i >= len(ch):
            dead.add(S); del onstack[S]; stack.pop(); continue
        top[2] += 1
        f, T = ch[i]
        if T in dead: continue
        if T in onstack:
            k = onstack[T]
            seq = [stack[j + 1][3] for j in range(len(stack) - 1)] + [f]
            return True, (seq[:k], seq[k:])
        count += 1
        if count > node_limit: raise RuntimeError("node limit")
        onstack[T] = len(stack)
        stack.append([T, expand(T), 0, f])
    return False, len(dead)

def clock(d, lo=0, hi=40):
    for B in range(lo, hi):
        ok, w = clock_safe(d, B)
        if ok: return B, w
        print(f"   clock d={d} B={B} impossible ({w} dead sets explored)", flush=True)
    return None, None

# ---------------- HISTORY ----------------
def hist_safe(d, B, node_limit=3_000_000):
    """Safety game on belief sets.  A node is the belief AFTER observing o_t:
    frozenset of (x_t, (sgn x_{t-d+1..t-1}), rest).  Controller picks u; then the
    belief is partitioned by the next observation sgn(x_{t+1-d}); adversary picks the part."""
    def moves(Bel):
        res = []
        for u in (-1, 0, 1):
            parts = {}; bad = False
            for (x, sg, rest) in Bel:
                full = sg + (sgn(x),)          # signs of x_{t-d+1..t}
                o = full[0]; ns = full[1:]
                for w in ((0,) if rest else (0, 1, -1)):
                    nx = x + u + w
                    if abs(nx) > B: bad = True; break
                    parts.setdefault(o, set()).add((nx, ns, 1 if w else 0))
                if bad: break
            res.append(None if bad else tuple(frozenset(p) for p in parts.values()))
        return res
    x0, sg0, r0 = init(d)
    B0 = frozenset([(x0, sg0[1:], r0)])
    graph = {}; q = deque([B0]); graph[B0] = None
    while q:
        b = q.popleft()
        mv = moves(b); graph[b] = mv
        for m in mv:
            if m:
                for nb in m:
                    if nb not in graph:
                        graph[nb] = None; q.append(nb)
                        if len(graph) > node_limit: raise RuntimeError("node limit")
    # greatest fixed point
    lose = set(); pred = {}
    alive = {b: [m is not None for m in mv] for b, mv in graph.items()}
    for b, mv in graph.items():
        for i, m in enumerate(mv):
            if m:
                for nb in m: pred.setdefault(nb, []).append((b, i))
    work = [b for b, a in alive.items() if not any(a)]
    lose.update(work)
    while work:
        b = work.pop()
        for (p, i) in pred.get(b, ()):
            if p in lose: continue
            if alive[p][i]:
                alive[p][i] = False
                if not any(alive[p]):
                    lose.add(p); work.append(p)
    win = B0 not in lose
    strat = None
    if win:
        strat = {b: [(-1, 0, 1)[i] for i, a in enumerate(alive[b]) if a] for b in graph if b not in lose}
    return win, strat, len(graph)

def history(d, lo=0, hi=40):
    for B in range(lo, hi):
        ok, st, n = hist_safe(d, B)
        if ok: return B, st, n
        print(f"   history d={d} B={B} impossible ({n} beliefs)", flush=True)
    return None, None, None

if __name__ == "__main__":
    ds = [int(a) for a in sys.argv[1:]] or [1, 2, 3]
    for d in ds:
        R, fs, _ = relay(d)
        print(f"d={d} R={R} maps(f(-),f(0),f(+))={fs}", flush=True)
        H, st, n = history(d)
        print(f"d={d} H={H} (beliefs explored {n}, winning {len(st)})", flush=True)
        C, w = clock(d, lo=H)
        print(f"d={d} C={C} clock prefix={w[0]} cycle={w[1]}", flush=True)
