#!/usr/bin/env python3
"""(1) First fit vs EVERY adversary (BFS over reachable layouts, unbounded heap): max extent, games O and N.
   (2) formulas vs the computed table.  (3) hexad manager (solver A's P3) under random/greedy stress."""
import sys, math, random
def ff_max_extent(M, G):
    start = ()            # layout = tuple of (start,size) sorted
    seen = {start}; todo = [start]; best = 0
    while todo:
        L = todo.pop()
        live = sum(s for _, s in L)
        occ = set()
        for p, s in L: occ.update(range(p, p + s))
        nxt = []
        for s in (1, 2, 3):
            if live + s <= M:
                p = 0
                while any((p + j) in occ for j in range(s)): p += 1
                best = max(best, p + s)
                nxt.append(tuple(sorted(L + ((p, s),))))
        for i, (p, s) in enumerate(L):
            if G == 'O' and s != 1: continue
            nxt.append(L[:i] + L[i + 1:])
        for n in nxt:
            if n not in seen: seen.add(n); todo.append(n)
    return best, len(seen)
hN = dict(zip(range(3, 14), [4, 6, 7, 9, 11, 13, 14, 16, 18, 20, 22]))
hO = dict(zip(range(3, 14), [4, 6, 7, 9, 11, 13, 14, 16, 18, 20, 21]))
if sys.argv[1] == 'ff':
    for G in 'ON':
        for M in range(3, int(sys.argv[2]) + 1):
            e, n = ff_max_extent(M, G)
            ub = (5 * M + 2) // 3
            print(f"FF game {G} M={M} reachable={n} max extent={e}  ceil(5M/3)={ub}" + (f"  {'OK' if e <= ub else 'VIOLATION'}" if G == 'O' else ""), flush=True)
if sys.argv[1] == 'formulas':
    print(" M hO hN | L1=3(K+t)-2 (5M-14)/3 | U1=ceil(5M/3) | L2=ceil((7M-17)/4) | 3M-6 | 43M/18+12")
    for M in range(3, 14):
        K = -(-M // 3); t = (M - K) // 3; L1 = 3 * (K + t) - 2; U1 = (5 * M + 2) // 3
        L2 = -((-(7 * M - 17)) // 4); U2b = 3 * M - 6 if M >= 4 else 4; U2a = 43 * M / 18 + 12
        ok = L1 <= hO[M] <= U1 and L2 <= hN[M] <= min(U2b, U2a) and (5*M-14)/3 <= L1
        print(f"{M:2d} {hO[M]:2d} {hN[M]:2d} | {L1:2d} {(5*M-14)/3:6.2f} | {U1:2d} | {L2:2d} | {U2b:2d} | {U2a:6.2f}  {'OK' if ok else 'CONTRADICTION'}")
if sys.argv[1] == 'hexad':
    rng = random.Random(3)
    for M in (6, 13, 24, 60, 120):
        worst = 0
        for trial in range(60):
            hx = {}   # index -> [type, set of used aligned slots]
            blocks = []; live = 0; mode = rng.random()
            for step in range(6000):
                sizes = [s for s in (1, 2, 3) if live + s <= M]
                if sizes and (not blocks or rng.random() < 0.5 + 0.4 * math.sin(step / (20 + 200 * mode))):
                    s = rng.choice(sizes) if rng.random() < 0.7 else (sizes[0] if (step // 150) % 2 else sizes[-1])
                    cand = [j for j, (ty, used) in hx.items() if ty == s and len(used) < 6 // s]
                    if cand: j = cand[0]
                    else:
                        j = 0
                        while j in hx: j += 1
                        hx[j] = [s, set()]
                    slot = min(set(range(6 // s)) - hx[j][1]); hx[j][1].add(slot)
                    blocks.append((j, slot, s)); live += s
                    worst = max(worst, 6 * (max(hx) + 1))
                else:
                    # bias: free blocks from the fullest hexads' neighbours -> leave one block per hexad
                    if rng.random() < 0.7:
                        multi = [b for b in blocks if len(hx[b[0]][1]) > 1]
                        b = rng.choice(multi) if multi else rng.choice(blocks)
                    else: b = rng.choice(blocks)
                    blocks.remove(b); j, slot, s = b; hx[j][1].discard(slot); live -= s
                    if not hx[j][1]: del hx[j]
        print(f"hexad M={M}: worst extent seen={worst}  bound 43M/18+12={43*M/18+12:.1f}  {'OK' if worst <= 43*M/18+12 else 'VIOLATION'}")
