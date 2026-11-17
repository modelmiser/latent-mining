#!/usr/bin/env python3
"""P6 adversary vs an OPTIMAL game-N manager (random choice among placements that stay in the
maximal safe set dumped by v3 at H = h^N(M)).  Also a lopsided-manager negative control."""
import sys, random, math
import p6
def load(fn): return set(l.strip() for l in open(fn))
def key(h, H):
    out = []
    for i in range(H):
        b = h.cell[i]
        if b < 0: out.append('0')
        else:
            p, s = h.blocks[b]
            out.append(str(s) if p == i else '4')
    return ''.join(out)
def mk_opt(S, H, rng):
    def m(h, s):
        assert key(h, H) in S, "left the safe set (should be impossible in game N)"
        k = key(h, H); good = []
        for p in range(H - s + 1):
            if h.fits(p, s):
                k2 = k[:p] + str(s) + '4' * (s - 1) + k[p + s:]
                if k2 in S: good.append(p)
        assert good, "safe layout with no safe placement?!"
        return rng.choice(good)
    return m
if sys.argv[1] == 'opt':
    for M, H in [(int(a), int(b)) for a, b in zip(sys.argv[2::2], sys.argv[3::2])]:
        S = load(f"safe_N_{M}_{H}.txt"); rng = random.Random(7)
        ext = [p6.p6_adversary(M, mk_opt(S, H, rng), H)[0] for _ in range(500)]
        print(f"M={M} H=h^N={H} safe layouts={len(S)} 500 runs: extent min={min(ext)} max={max(ext)} "
              f"P6 bound={p6.bound(M)}  {'OK' if min(ext) >= p6.bound(M) else 'VIOLATION'}")
else:  # lopsided manager: all 2s start on the emptied class, so one kept class gets no 2s
    def mk_lop():
        def m(h, s):
            if s != 2: return p6.first_fit(h, s)
            cnt = [0, 0, 0]
            for b, (p, sz) in h.blocks.items():
                if sz == 1: cnt[p % 3] += 1
            drop = cnt.index(min(cnt))
            for p in range(h.cap):
                if p % 3 == drop and h.fits(p, 2): return p
        return m
    print("  M bound | P6 vs lopsided | worse_r vs lopsided")
    for M in [int(x) for x in sys.argv[2:]]:
        a = p6.p6_adversary(M, mk_lop(), 8 * M)[0]; b = p6.p6_adversary(M, mk_lop(), 8 * M, weak='worse_r')[0]
        print(f"{M:4d} {p6.bound(M):4d} | {a:5d} {'>=bound' if a >= p6.bound(M) else 'VIOLATION'} | {b:5d} {'short' if b < p6.bound(M) else 'NOT short'}")
