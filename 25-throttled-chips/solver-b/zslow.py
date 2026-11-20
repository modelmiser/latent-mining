#!/usr/bin/env python3
"""Slow reference for the ZONE-restricted game (Lemma Z): Firer may never put a chip outside
Z_t = union over add positions v of [v-rad, v+rad], rad = 0 if count<T1, 1 if T1<=count<T2, 2 if count>=T2."""
import sys, itertools
def zwins(lst, h, T1=2, T2=4):
    S = {frozenset()}; cnt = {}
    for a in lst:
        cnt[a] = cnt.get(a, 0) + 1
        zone = set()
        for v, c in cnt.items():
            rd = 2 if c >= T2 else (1 if c >= T1 else 0)
            zone.update(range(v - rd, v + rd + 1))
        out = set()
        for c in S:
            d = dict(c); d[a] = d.get(a, 0) + 1
            if d[a] >= h: continue
            out.add(frozenset(d.items()))
            for x, v in list(d.items()):
                if v >= 2 and x - 1 in zone and x + 1 in zone:
                    e = dict(d); e[x] = v - 2
                    if e[x] == 0: del e[x]
                    e[x-1] = e.get(x-1, 0) + 1; e[x+1] = e.get(x+1, 0) + 1
                    if e[x-1] >= h or e[x+1] >= h: continue
                    out.add(frozenset(e.items()))
        S = out
        if not S: return True
    return False
def connected(lst, T1=2, T2=4):
    cnt = {}
    for a in lst: cnt[a] = cnt.get(a, 0) + 1
    iv = sorted((v - (2 if c >= T2 else (1 if c >= T1 else 0)), v + (2 if c >= T2 else (1 if c >= T1 else 0))) for v, c in cnt.items())
    R = iv[0][1]
    for l, r in iv[1:]:
        if l > R: return False
        R = max(R, r)
    return True
if __name__ == '__main__':
    from slow import wins
    h, m, R = map(int, sys.argv[1:4])
    nz = nw = 0; firstz = None; bad = 0
    for rest in itertools.product(range(-R, R + 1), repeat=m - 1):
        lst = (0,) + rest
        z = zwins(lst, h); w = wins(lst, h)
        if w and not z: bad += 1          # impossible: winning implies Z-winning
        if z:
            nz += 1
            if firstz is None and rest[0] >= 0: firstz = lst
        if w: nw += 1
    print('h=%d m=%d R=%d: Z-winning lists %d, winning lists %d, (winning but not Z-winning: %d), first Z-winning with a2>=0: %s' % (h, m, R, nz, nw, bad, firstz))
