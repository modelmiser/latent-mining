# Type Lemma against EVERY valid re-orientation of the merged tree (not only normal replies).
# S: strict lemma at the tail endpoint of the new edge.  W: cost + F(best index of ANY vertex) >= 2F(i).
# L1: Lemma-1 walk gives a normal reply with cost(L) + dist(L-result, D') <= cost(D').
import sys, itertools
from lib import *
MAXTOT = int(sys.argv[1]); variant = sys.argv[2]
L = gen_trees(MAXTOT - 1)
def info(T):
    m, e = T; o = outs(m, e); h = hvals(m, o)
    return (m, e, o, h, [slots(v, o, h) for v in range(m)])
S = {m: [info(T) for T in L[m]] for m in L}
st = dict(quads=0, orient=0, nonnormal=0, S_cases=0, S_fail=0, W_fail=0, L1_fail=0, mincost_mismatch=0, S_fail_normal=0)
exS = []; exW = []
for a in range(1, MAXTOT):
  for b in range(a, MAXTOT - a + 1):
    for X in S[a]:
      for Y in S[b]:
        mx, ex, ox, hx, sx = X; my, ey, oy, hy, sy = Y; m = mx + my
        base = list(ex) + [(u + mx, v + mx) for (u, v) in ey]
        ne = len(base)
        for x in range(mx):
          for y0 in range(my):
            y = y0 + mx; st['quads'] += 1
            imax = min(max_index(sx[x]), max_index(sy[y0]))
            o0 = outs(m, base)
            normal = set()
            for (e, other) in ((x, y), (y, x)):
                reps = [[e]] if len(o0[e]) < 2 else normal_paths(e, o0)
                for path in reps:
                    fl = set(zip(path, path[1:]))
                    normal.add(tuple((bb, aa) if (aa, bb) in fl else (aa, bb) for (aa, bb) in base) + ((e, other),))
            minn = min(sum(1 for j in range(ne) if D[j] != base[j]) for D in normal)
            minall = 99
            for mask in range(1 << (ne + 1)):
                D = tuple((base[j][1], base[j][0]) if mask >> j & 1 else base[j] for j in range(ne)) + \
                    (((x, y),) if mask >> ne & 1 else ((y, x),))
                o2 = outs(m, D)
                if any(len(z) > 2 for z in o2): continue
                st['orient'] += 1
                cost = bin(mask & ((1 << ne) - 1)).count('1')
                minall = min(minall, cost)
                isn = D in normal
                if not isn: st['nonnormal'] += 1
                h2 = hvals(m, o2); e = D[-1][0]; other = D[-1][1]
                s_e = slots(e, o2, h2)
                best = max(max_index(slots(v, o2, h2)) for v in range(m))
                # Lemma 1 walk from the old orientation towards D
                Dset = set(D)
                if len(o0[e]) < 2: lpath = [e]
                else:
                    lpath = [e]
                    while True:
                        v = lpath[-1]
                        nxt = [w for w in o0[v] if (w, v) in Dset]
                        if not nxt: lpath = None; break
                        lpath.append(nxt[0])
                        if len(o0[nxt[0]]) < 2: break
                if lpath is None: st['L1_fail'] += 1
                else:
                    fl = set(zip(lpath, lpath[1:]))
                    DL = tuple((bb, aa) if (aa, bb) in fl else (aa, bb) for (aa, bb) in base) + ((e, other),)
                    dist = sum(1 for j in range(ne + 1) if DL[j] != D[j])
                    slack = 1 if variant == 'fL1' else 0      # false variant: demand one unit more
                    if (len(lpath) - 1) + dist + slack > cost: st['L1_fail'] += 1
                for i in range(imax + 1):
                    k = i // 2; st['S_cases'] += 1
                    if i % 2 == 0: ok = cost >= k and has_index(s_e, i + 1)
                    else: ok = (cost >= k and has_index(s_e, i + 1)) or (cost >= k + 1 and has_index(s_e, i))
                    if not ok:
                        st['S_fail'] += 1
                        if isn: st['S_fail_normal'] += 1
                        if len(exS) < 1 or (m < exS[0][0]): exS[:] = [(m, base, x, y, D, i, cost)]
                    wneed = 2 * F(i) + (Fr(1, 3) if variant == 'fW' else 0)
                    if cost + F(best) < wneed:
                        st['W_fail'] += 1
                        if len(exW) < 1: exW.append((m, base, x, y, D, i, cost, best))
            if minall != minn: st['mincost_mismatch'] += 1
print('MAXTOT', MAXTOT, variant, st)
print(' example strict failure', exS); print(' example weak failure', exW)
