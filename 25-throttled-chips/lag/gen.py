#!/usr/bin/env python3
"""gen.py -- builds the list of Theorem L (PROOFS.md) against the lag rule with a given tie rule,
by playing the TRUE game (chips, readiness, largest-lag rule) and asserting every claim of the
proof as it goes.  The assertions are the mechanical check: they test rules R1/R2 and the stage
invariant S(W, m) on the real game, they are not assumed.

usage: gen.py N TIE OUT [--greedy] [--no-reservoir] [--overdraw]
   N    number of gadgets (N = 34^j guarantees j stages)
   TIE  L (leftmost), R (rightmost), H (fixed pseudo-random order of the vertices)
   OUT  file for the list, one vertex per line
   --no-reservoir : deliberately wrong input (skip the one-chip-everywhere phase); must FAIL
   --overdraw     : deliberately wrong accounting (rebuild m//2 gadgets instead of
                    ((m//2)-1)//16); must FAIL
Levels are 5*lag (integers).  level[x] = 3k[x] + k[x-1] + k[x+1] - 5F[x].
"""
import sys, heapq
from array import array

N = int(sys.argv[1]); TIE = sys.argv[2]; OUT = sys.argv[3]
GREEDY = '--greedy' in sys.argv; NORES = '--no-reservoir' in sys.argv; OVER = '--overdraw' in sys.argv
P = 2 * N + 8                      # pads
G0 = 20                            # first gadget centre
PAD0 = G0 + 8 * N + 8              # first pad centre
SIZE = PAD0 + 4 * P + 20
lev = array('i', [0]) * SIZE       # 5*lag
chips = array('i', [0]) * SIZE
cnt = {0: SIZE}                    # level -> number of vertices
hi = 0                             # current largest level
heap = []
lst = []
rec_round = 0                      # largest level seen after a round
rec_add = 0                        # largest level seen right after an add
passes = 0

def key(x):
    if TIE == 'L': return x
    if TIE == 'R': return -x
    return (x * 2654435761 + 12345) % 4294967291

def setlev(x, v):
    global hi
    o = lev[x]; cnt[o] -= 1; cnt[v] = cnt.get(v, 0) + 1; lev[x] = v
    if v > hi: hi = v
    while cnt.get(hi, 0) == 0: hi -= 1

def push(x):
    if chips[x] >= 2 and lev[x] >= 5: heapq.heappush(heap, (-lev[x], key(x), x))

def add(x):
    """one round: add a chip at x, then the firer's move.  Returns the fired vertex or None."""
    global rec_round, rec_add, passes
    lst.append(x)
    chips[x] += 1
    setlev(x - 1, lev[x - 1] + 1); setlev(x, lev[x] + 3); setlev(x + 1, lev[x + 1] + 1)
    push(x - 1); push(x); push(x + 1)
    if hi > rec_add: rec_add = hi
    f = None
    while heap:
        l, kk, y = heap[0]
        if chips[y] >= 2 and lev[y] == -l: f = y; break
        heapq.heappop(heap)
    if f is None:
        passes += 1
    else:
        heapq.heappop(heap)
        setlev(f, lev[f] - 5); chips[f] -= 2; chips[f - 1] += 1; chips[f + 1] += 1
        push(f - 1); push(f); push(f + 1)
    if hi > rec_round: rec_round = hi
    return f

# ---------------------------------------------------------------- reservoir
if not NORES:
    for x in range(2, SIZE - 2):
        assert add(x) is None, "reservoir: a vertex fired"
    assert all(lev[x] == 5 for x in range(3, SIZE - 3))
# ---------------------------------------------------------------- pads (6,3,6)
pads = [PAD0 + 4 * i for i in range(P)]
for p in pads:
    assert add(p) == p, "pad: centre not fired"
    assert (lev[p - 1], lev[p], lev[p + 1]) == (6, 3, 6)
padi = 0
def waste():
    global padi
    p = pads[padi]; padi += 1
    assert (lev[p - 1], lev[p], lev[p + 1]) == (6, 3, 6)
    return add(p)
# ---------------------------------------------------------------- base: S(8, N)
G = [G0 + 8 * i for i in range(N)]
for x in G:
    assert add(x - 2) == x - 2; assert add(x + 2) == x + 2
    assert [lev[x + d] for d in range(-3, 4)] == [6, 3, 6, 5, 6, 3, 6]
for x in G:
    assert add(x) == x
    assert (lev[x - 1], lev[x], lev[x + 1]) == (7, 3, 7)
assert hi == 7
for x in G:
    f = add(x)
    assert f is not None and lev[f] == 3 and (f - G0) % 8 in (1, 7), "base resolve"
assert hi == 8

def check_S(W, gad):
    """stage invariant S(W, m)"""
    m = len(gad)
    assert hi == W, ("max level", hi, W)
    alive = 0
    for x in gad:
        assert lev[x] == W - 2
        for y in (x - 1, x + 1):
            assert lev[y] in (W, W - 5)
            alive += lev[y] == W
    assert alive == m, (alive, m)
    assert cnt[W] == m, "a vertex other than an alive flank is at level W"

W = 8; gad = G
print(f"tie {TIE}  N {N}  board {SIZE}")
while True:
    check_S(W, gad); m = len(gad)
    print(f"S({W},{m}) holds after round {len(lst)}: {m} vertices with lag {W/5}", flush=True)
    m2 = m // 2 if OVER else (m // 2 - 1) // 16
    if m2 < 1 and not GREEDY: break
    aliveset = set()
    for x in gad:
        for y in (x - 1, x + 1):
            if lev[y] == W: aliveset.add(y)
    def burn():
        f = waste()
        assert f is not None and lev[f] == W - 5, "burn: fired vertex was not at level W"
        aliveset.discard(f)
    def rebuild(x):
        assert lev[x - 1] == W - 5 and lev[x + 1] == W - 5
        for s in (-1, 1):
            for i in range(4):
                assert hi == W, "rebuild: max level is not W before a B-add"
                assert aliveset, "rebuild: no alive flank left"
                before = lev[x + s]
                f = add(x + 2 * s)
                assert f is not None and f not in (x - 1, x, x + 1)
                assert lev[x + s] == before + 1 and lev[x] == W - 2
                if f in aliveset:
                    aliveset.discard(f)
                    if GREEDY: note(f)
                if hi > W:
                    assert cnt[hi] == 1 and sum(cnt.get(v, 0) for v in range(W + 1, hi)) == 0
                    f = waste()
                    assert f is not None and hi <= W, "flush failed"
            assert lev[x + s] == W - 1
    sel = []
    if GREEDY:
        # same moves, but decided as the game goes: rebuild while 17 alive flanks remain
        done = set(); ptr = 0; gl = list(gad)
        def nextdead():
            # a gadget of this stage, not yet rebuilt, with both flanks at W-5 (scan resumes; a
            # gadget passed over while it still had an alive flank is revisited from deadq)
            global ptr
            while deadq:
                y = deadq.pop()
                if y not in done and lev[y - 1] == W - 5 and lev[y + 1] == W - 5: return y
            return None
        deadq = [y for y in gl if lev[y - 1] == W - 5 and lev[y + 1] == W - 5]
        gset = set(gl)
        def note(f):   # f was just fired: if its gadget is now fully dead, queue it
            for y in (f - 1, f + 1):
                if y in gset and y not in done and lev[y - 1] == W - 5 and lev[y + 1] == W - 5: deadq.append(y)
        while len(aliveset) >= 17:
            x = nextdead()
            if x is not None:
                done.add(x); n0 = len(lst); rebuild(x); sel.append(x)
            elif len(aliveset) >= 18:
                before = set(aliveset); burn()
                for f in before - aliveset: note(f)
            else: break
    else:
        while len(aliveset) > m // 2: burn()          # phase 1
        dead = [x for x in gad if lev[x - 1] == W - 5 and lev[x + 1] == W - 5]
        assert len(dead) >= m - m // 2
        r0 = len(lst)
        for x in dead[:m2]: rebuild(x); sel.append(x)  # phase 2
        assert OVER or len(lst) - r0 <= m // 2 - 1
    assert hi == W and aliveset
    if not sel: break
    # phase 3: trigger
    for x in sel:
        assert add(x) == x, "trigger: centre not fired"
        assert (lev[x - 1], lev[x], lev[x + 1]) == (W, W - 4, W) and hi == W
    # phase 4: resolve
    selfl = set()
    for x in sel: selfl.add(x - 1); selfl.add(x + 1)
    for x in sel:
        f = add(x)
        assert f in selfl and lev[f] == W - 4 and hi == W + 1, "resolve"
    W += 1; gad = sel

with open(OUT, 'w') as fh:
    fh.write('\n'.join(map(str, lst))); fh.write('\n')
print(f"length {len(lst)}  largest lag after a round {rec_round/5}  after an add {rec_add/5}  passes {passes}  pads used {padi}")
