#!/usr/bin/env python3
"""Explicit families, checked with an independent (array-based) computation of C(q).
 split(w): 'split-end counter' with L=w-1 levels (w>=3), states = w+1:
     levels 1..L-2 interior; bottom split into B0 (stayed) / B1 (arrived from level 1);
     top split into T0 (stayed) / T1 (arrived).
 plain(w,m): saturating up/down counter with m states.
 Writes witness files witness_w<w>.txt for the claimed s = (w-1)//2 (w != 2)."""
import sys
def split(w):
    L = w - 1
    # state ids: 0=B0(stay),1=B1(arrive), 2..L-1 = levels 1..L-2, L = T1(arrive), L+1 = T0(stay)
    n = L + 2
    lev = lambda j: 1 if j == 0 else (j + 1 if j <= L - 2 else L)   # entering level j from adjacent level
    d = [None] * n
    up_from_bottom = 2 if L >= 3 else L          # level 1 (or top-arrive if L==2)
    d[0] = (0, up_from_bottom); d[1] = (0, up_from_bottom)
    for j in range(1, L - 1):                    # interior level j has id j+1
        dn = 1 if j == 1 else j                  # level j-1 id (or B1)
        up = L if j == L - 2 else j + 2          # level j+1 id (or T1)
        d[j + 1] = (dn, up)
    down_from_top = L - 1 if L >= 3 else 1
    d[L] = (down_from_top, L + 1); d[L + 1] = (down_from_top, L + 1)
    return d
def plain(m):
    return [(max(i - 1, 0), min(i + 1, m - 1)) for i in range(m)]
def spreads(w, d):
    n = len(d); full = (1 << w) - 1
    seen = [bytearray(1 << w) for _ in range(n)]
    cur = [(0, 0)]
    for _ in range(w):
        cur = [(d[q][b], (x << 1) | b) for q, x in cur for b in (0, 1)]
    st = []
    for q, x in cur:
        if not seen[q][x]: seen[q][x] = 1; st.append((q, x))
    while st:
        q, x = st.pop()
        for b in (0, 1):
            t = d[q][b]; y = ((x << 1) | b) & full
            if not seen[t][y]: seen[t][y] = 1; st.append((t, y))
    out = []
    for q in range(n):
        cs = {bin(x).count('1') for x in range(1 << w) if seen[q][x]}
        out.append((min(cs), max(cs)) if cs else None)
    return out
def gval(d, sp):
    n = len(d)
    for s in range(64, -1, -1):
        S = {q for q in range(n) if sp[q] and sp[q][1] - sp[q][0] >= s}
        ch = True
        while ch:
            ch = False
            for q in list(S):
                if d[q][0] not in S and d[q][1] not in S: S.discard(q); ch = True
        if S: return s
if __name__ == '__main__':
    W = int(sys.argv[1])
    for w in range(3, W + 1):
        d = split(w); sp = spreads(w, d); g = gval(d, sp)
        k = (w - 1) // 2
        m = w // 2 + 2 - (1 - w % 2)  # plain counter size ceil(w/2)+1
        dp = plain((w + 1) // 2 + 1); gp = gval(dp, spreads(w, dp))
        print(f"w={w} split: states={len(d)} g={g} maxspread={max(b-a for a,b in sp)} target={k} {'OK' if g==k else 'FAIL'} | plain({(w+1)//2+1}) g={gp}  ranges={sp if w<=8 else ''}")
        if g == k:
            lit = [q for q in range(len(d)) if sp[q][1] - sp[q][0] <= k]
            with open(f"witness_w{w}.txt", "w") as f:
                f.write(f"{w}  # w\n{k}  # s\n{len(d)}  # number of states, start state 0\n")
                for a, b in d: f.write(f"{a} {b}\n")
                f.write(" ".join(map(str, lit)) + "  # lit states\n")
