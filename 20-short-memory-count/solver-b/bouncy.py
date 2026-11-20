#!/usr/bin/env python3
# g of the bouncy counter B_w (Theorem D) for even w, computed literally; writes machine lines to bouncy_wit.txt
from collections import deque
from counter import Csets
def g_from(n, d, C):
    sp = [max(c) - min(c) if c else -1 for c in C]
    for s in range(0, 100):
        un = [q for q in range(n) if sp[q] > s]
        # acyclic test on induced subgraph (Kahn)
        indeg = {q: 0 for q in un}
        for q in un:
            for b in (0, 1):
                if d[q][b] in indeg: indeg[d[q][b]] += 1
        st = [q for q in un if indeg[q] == 0]; rem = 0
        while st:
            q = st.pop(); rem += 1
            for b in (0, 1):
                t = d[q][b]
                if t in indeg:
                    indeg[t] -= 1
                    if indeg[t] == 0: st.append(t)
        if rem == len(un): return s, sp
out = open("bouncy_wit.txt", "w")
for w in range(4, 17, 2):
    K = w
    d = [None] * (K + 1)
    for k in range(1, K): d[k] = (k - 1, k + 1)
    d[0] = (0, 2); d[K] = (K - 2, K)
    C = Csets(w, K + 1, d)
    g, sp = g_from(K + 1, d, C)
    print("w=%d g(B_w)=%d  w/2-1=%d  spreads=%s" % (w, g, w // 2 - 1, sp))
    out.write("%d %d %s %d\n" % (w, K + 1, " ".join("%d %d" % x for x in d), g))
