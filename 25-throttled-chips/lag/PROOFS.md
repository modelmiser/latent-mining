# Experiment 25 follow-up: the lag of the firing rule is Theta(log n)

Every part is marked PROVED (complete proof here), COMPUTED or NOT DONE.

## Setting

As in `25-throttled-chips/solver-a/proofs.txt` and `constant/proofs.txt`: k_x adds at x, F_x firings at
x, eta_x chips on x, T_x = (3k_x + k_{x-1} + k_{x+1})/5, lag l_x = T_x - F_x.
RULE: in step (2) the firer fires a ready vertex (eta >= 2) whose lag is the largest among ready
vertices, provided that lag is >= 1; otherwise it passes. Ties are broken in any way.
Facts used (from those files): (a) l_x >= 0. (b) eta_x = eta*_x + 2l_x - l_{x-1} - l_{x+1} with
eta*_x = (k_{x-2}+...+k_{x+2})/5. (c) l_x <= 3 eta*_x.
An add at x raises the lags of x-1, x, x+1 by 1/5, 3/5, 1/5; a firing at z lowers l_z by 1 and
changes no other lag. Lags are multiples of 1/5. "Level" means 5*lag (an integer).

(d') If the largest lag M after an add is > 3, every vertex of lag M is ready, so the vertex fired
has lag exactly M. (For such x, 2l_x - l_{x-1} - l_{x+1} >= 0, so eta_x >= eta*_x >= M/3 > 1 by (b),
(c), and eta_x is an integer. This is fact (d) of the earlier files with 3 in place of 6.)

## Theorem U (upper bound; PROVED)

Fix a list. Let V be the set of vertices within distance 1 of some entry and n = |V| (n <= 3m for a
list of length m; lags are 0 outside V). Under the RULE with any tie-breaking (even one that looks
at the rest of the list), after every round t and for every k = 1..n,

    S_k(t) := sum of the k largest lags  <=  B_k := k (3 + H_n - H_k),      H_j = 1 + 1/2 + ... + 1/j.

In particular every lag is at most 2 + H_n after a round, and at most 2.6 + H_n after an add.

Proof. Induction on t; t = 0 is trivial. Let l, l', l'' be the lags after round t, after the add of
round t+1 and after round t+1; M = max l'. The add raises the total lag by exactly 1.
Case 0, M <= 3: firing only lowers lags, so every l'' <= 3 and S_k(t+1) <= 3k <= B_k.
Case 1, M > 3: by (d') a vertex j with l'_j = M is fired, l''_j = M - 1, nothing else changes.
Fix k and let A be a set of k vertices of V carrying the k largest values of l''.
 1a, j in A: sum_A l'' = sum_A l' - 1 <= sum_A l + 1 - 1 <= S_k(t) <= B_k.
 1b, j not in A (so k < n): sum_{A+j} l'' = sum_{A+j} l' - 1 <= S_{k+1}(t) <= B_{k+1}. Also
   l''_j = M - 1 >= max_A l'' - 1 >= (sum_A l'')/k - 1. Hence
   sum_A l'' <= B_{k+1} + 1 - (sum_A l'')/k, i.e.
   sum_A l'' <= (k/(k+1)) (B_{k+1} + 1) = k (3 + H_n - H_{k+1} + 1/(k+1)) = B_k.   QED
(This is the standard argument for the greedy emptier in the "cup game"; I did not look up a
reference, no network.)

Corollary U1 (PROVED). Largest lag <= 2 + H_{3t} <= 3 + ln(3t) after round t. This replaces the
6 + sqrt(t) of Theorem 2(f).

Corollary U2 (PROVED; a different lower bound on G, not a better constant). At every step of round
t <= m, eta_x <= eta*_x + 2 l_x <= t/5 + 5.2 + 2 H_{3m}. So every list of length m with
m/5 + 5.2 + 2 H_{3m} < h loses, and G(h) >= 5h - 26 - 10 H_{15h}. `corollary.py` evaluates the
exact condition: G(59) >= 200, G(100) >= 398, G(1000) >= 4873 (against 119, 324, 4824 from 5h - 176
and 117, 199, 1999 from 2h - 1). It beats 2h - 1 from h = 28 on and is weaker than 5h - 176 for very
large h (by the estimate H_n ~ ln n + 0.577, from h of the order of 10^5; that crossover was not
computed). The constant 176 itself is NOT improved.

## Theorem L (lower bound: the lag is unbounded; PROVED)

Let the tie-breaking be any rule that picks among the ready vertices of largest lag as a function
of the play so far (leftmost, rightmost, any fixed order, any function of the history). For every
j >= 0 there is a list of length at most 24*34^j + 84 after which some vertex has lag (8 + j)/5.
So over lists of length t the largest lag is between 1.6 + 0.2*floor(log_34((t-84)/24)) and
2 + H_{3t}: it grows like log t, between 0.056 ln t and ln t. No bound independent of the list
(or of e) exists.

The list is built round by round against the rule (each entry depends on what the firer did
before; for a fixed rule this is one explicit list, and `gen.py` prints it). NOT covered: a firer
that breaks ties with knowledge of the future of the list.

### Two rules of play

Start with the RESERVOIR: one chip on each vertex of a long interval I, in any order. No vertex ever
holds 2 chips, so the firer passes every time; afterwards every vertex whose five-point
neighbourhood lies in I ("interior") has level 5, and eta* >= 1 there for ever. All later adds are
at interior vertices at distance >= 4 from the ends, so non-interior vertices keep level <= 5.

R1. If after an add the largest level is >= 5 and is attained at a single interior vertex z, then
    z is fired.
R2. If after an add the largest level is >= 5 and some interior vertex attaining it has a neighbour
    of smaller level, then the vertex fired has the largest level.
Proof. For an interior vertex x of largest level with a strictly smaller neighbour,
2l_x - l_{x-1} - l_{x+1} >= 1/5, so eta_x >= 1 + 1/5 by (b), so eta_x >= 2: x is ready, with lag
>= 1. Hence the largest lag among ready vertices is the largest lag overall and the RULE fires a
vertex of that lag. In R1 there is only one such vertex. QED

### Objects

A PAD is an interior triple (p-1, p, p+1) at levels (6, 3, 6). Made from reservoir ground by one
add at p: levels (6, 8, 6), p is the single largest (everything else <= 7 at that time, see the
base), R1 fires it. A WASTE add is an add at the centre of an unused pad; it gives (7, 6, 7), so it
creates no level above 7.
A GADGET is seven consecutive interior vertices x-3..x+3: outers x-3, x+3; buffers x-2, x+2; flanks
x-1, x+1; centre x. Gadgets and pads are pairwise disjoint. Adds are made only at centres (C-add:
flanks +1, centre +3) and at buffers (B-add at x+2: flank x+1 +1, buffer +3, outer x+3 +1), so
nothing outside the seven vertices changes.

S(W, m), for W >= 8: (i) the largest level is W, and the vertices at level W are exactly m flanks
("alive") of the gadgets below; (ii) there are m gadgets with centre at level W - 2 and each flank
at level W or W - 5.

### Base: S(8, N) for every N (all moves forced except the last N)

On the reservoir make 2N + 8 pads (R1 each time: the new 8 is the only level above 6).
For each of N gadgets: B-add left, B-add right. Each time the buffer goes to 8, everything else is
<= 6, R1 fires it: the gadget is (6,3,6,5,6,3,6). Then a C-add at each gadget: centre 8, its flanks
7, everything else <= 7: R1 fires the centre; flanks at 7, centre 3. Then a second C-add at each
gadget: centre 6, flanks 8. The largest level is 8, attained only at such flanks, whose centre
neighbour is lower; by R2 a vertex of level 8 is fired, which is one of these flanks, and it drops
to 3. After N such rounds 2N flanks were raised to 8 and N fired: S(8, N).

### Stage: S(W, m) gives S(W+1, m') with m' = floor((floor(m/2) - 1)/16)   (needs m' >= 1)

Phase 1 (burn). Make ceil(m/2) waste adds. Each creates nothing at level >= W, the largest level
W is attained only at alive flanks, whose centre (W - 2) is lower: by R2 an alive flank is fired
and drops to W - 5. Now floor(m/2) flanks are alive; they lie in at most floor(m/2) gadgets, so at
least ceil(m/2) gadgets have both flanks at W - 5. Select m' of them.

Phase 2 (rebuild). For each selected gadget and each of its two flanks, four times: one B-add at
the adjacent buffer; then, if some vertex is above W, one waste add ("flush"). Claims, by induction
over these rounds:
 (1) Before each B-add no vertex is above W and at least one flank is alive.
 (2) The B-add puts the flank at <= W - 1, the buffer at b + 3, the outer at o + 1 (b, o <= W the
     old values). If both are <= W, the largest level is W, an alive flank has a lower neighbour,
     and R2 fires a vertex of level W. If exactly one of them is the single largest (> W), R1 fires
     it and it falls to <= W - 2. If b + 3 = o + 1 > W, these two adjacent vertices are the only
     largest, the buffer's other neighbour (the flank) is lower, and R2 fires one of them.
     In every case: the fired vertex has the largest level, so it is not a flank being rebuilt
     (<= W - 1) and not a centre (W - 2); at most one alive flank is lost; and afterwards at most
     one vertex is above W (the buffer or the outer, the one not fired), at level <= W + 3.
 (3) If one vertex s is above W, the waste add creates nothing above 7 < W + 1, so s is the single
     largest and R1 fires it; it falls to <= W - 2. No vertex is above W again.
 (4) There are at most 16 m' <= floor(m/2) - 1 rounds, each losing at most one alive flank, so
     one stays alive throughout.
 Afterwards every selected gadget is (W - 1, W - 2, W - 1) on flank, centre, flank, and nothing is
 above W.

Phase 3 (trigger). C-add at each selected gadget in turn: centre W + 1, its flanks W, everything
else <= W. R1 fires the centre (to W - 4). Still nothing above W.

Phase 4 (resolve). C-add at each selected gadget in turn: centre W - 1, flanks W + 1. The largest
level W + 1 is attained only at flanks of selected gadgets already resolved, each next to a lower
centre; by R2 one of them is fired and drops to W - 4. After m' rounds 2m' flanks were raised to
W + 1 and m' fired. The selected gadgets have centre W - 1 = (W+1) - 2 and flanks at W + 1 or
(W+1) - 5, and every other vertex is <= W: S(W+1, m'). QED

### Counting

If m >= 34q with q >= 1 then floor(m/2) >= 17q and m' >= floor((17q - 1)/16) >= q. So N = 34^j
gives S(8 + j, m) with m >= 1: a vertex of lag (8 + j)/5 after a round.
Pads used: at most ceil(m/2) + 8m' <= 0.75m + 1 per stage, under 2N + 8 in total.
Length: reservoir 16N + 76 (the interval used by `gen.py`), pads 2N + 8, base 4N, stages at most
m + 2m' each, under 1.1N in total: at most 24N + 84. (`gen.py`: 859 for N = 34, 26482 for N = 1156,
897727 for N = 39304.)

Remark (greedy variant, COMPUTED lags, same proof). The claims of phase 2 only need an alive flank
to remain. `gen.py --greedy` starts a rebuild whenever a gadget with two dead flanks exists and 17
flanks are alive, and burns one flank otherwise; every claim above is asserted in the run. It
reaches much larger lags for the same N (table in REPORT.md), about a factor 2 to 4 in m per level
instead of 34.

## What is NOT done

- The true growth per doubling of the list: at least 0.2/log2(34) = 0.04 (proved); 0.14 to 0.2 in
  the greedy runs (N = 1156, 39304, 300000 give 2.6, 3.6, 4.0); 0.2 to 0.8 in the earlier search
  (`constant/lagadv2_out.txt`); at most ln 2 = 0.69 (proved).
- The constant 176 in G(h) >= 5h - 176 is untouched; no better upper bound than 5h - 6; no new
  exact value of G.
