# solver-b report

All files are in `solver-b/`. Machine line format: `w n d(0,0) d(0,1) … d(n-1,0) d(n-1,1) g` (start state 0; pad with unreachable states to w+1).

## 1. Tables

**f(w)** — entries for w ≤ 9 COMPUTED (exhaustive over all machines with ≤ w+1 reachable states, numbered in discovery order; witness checked by the literal checker `slow.py`):

| w | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|
| f(w) | 0 | 1 | 1 | 1 | 2 | 2 | 3 | 3 | 4 | ≤ 4 (proved), not searched |

Methods for "nothing better exists":
- w ≤ 3: literal definition over all raw machines (`slow.py`: 729 and 65536 machines).
- w ≤ 5: unpruned enumeration of all numbered machines (`fast … hist`: 166152 and 6097692 machines), every machine for w ≤ 3 and a 1-in-23 sample for w = 4 re-checked by `slow.py`.
- w ≤ 8: pruned search, twice, with two different state numberings (breadth-first and depth-first), `final_lb.log`.
- w = 9: pruned depth-first search only (one program, `fast3`), split into 5525 prefixes, 1.70·10⁹ nodes, all NONE (`split_w9s3.*`). This is the least cross-checked entry.

Pruning: a partial machine is dropped when its states of spread > s already contain a cycle; safe by Lemma 2 in `proofs.txt` (an unsafe pruning could only bias f upwards). Pruned counts of machines equal the unpruned histogram counts where both exist (w=4: 5; w=5: 3573), and BFS and DFS numbering agree on w=6: 91.

**e(w)** (all states lit): e(1..9) = 0,1,1,2,2,2,3,3,4. COMPUTED for w ≤ 5 (unpruned histograms); for 6–9 squeezed between f(w) and an explicit machine with every spread ≤ f(w). Only gap: e(4)=2 > f(4)=1.

**N\*(w,s)** = least number of states with g ≤ s (COMPUTED, `nstar_2_8.txt`, `results.txt`): by rows s=0,1,2,…: w=3: 7,3,2; w=4: 10,5,3,2; w=5: ≥12,8,4,3,2; w=6: –,≥10,7,4,3,2; w=7: –,≥11,9,5,4,3,2; w=8: –,–,≥10,8,5,4,3,2; w=9: N\*(9,3)=11, N\*(9,4)=6. So f(7)=3 and f(9)=4 fail to be one smaller by exactly one state (9 and 11 states suffice: `w7n9.txt`, `w9n11_s3.txt`).

## 2. Witnesses (`witnesses.txt`, all 12 lines verified by `slow.py check`)

- odd w: saturating counter with ⌊w/2⌋+2 states, g = ⌊w/2⌋.
- even w ≥ 4: "bouncy counter" B_w on states 0..w: the saturating counter except 0 —1→ 2 and w —0→ w−2; g = w/2 − 1 (`bouncy.py`, checked for even w ≤ 16).
- w=1: state = last bit; w=2: any 2-state machine with g=1.
- Impossibility certificates are the exhaustive search logs (`final_lb.log`, `split_w9s2.*`, `split_w9s3.*`); they can only be checked by re-running a search.

## 3. Headline

Not settled. What I have:
- PROVED for all w ≥ 3: f(w) ≤ ⌊(w−1)/2⌋. PROVED: f(w) ≥ 1 for w ≥ 2.
- COMPUTED: f(w) = ⌊(w−1)/2⌋ for 3 ≤ w ≤ 9; f(1)=0, f(2)=1.
- CONJECTURE (a guess): f(w) = ⌊(w−1)/2⌋ for all w ≥ 3. Confidence about 65%. Against it: the lower bound is fragile in the state count (w+2 states already beat it at w = 7 and 9), and I have no proof idea that uses "w+1" sharply. A 23-minute non-exhaustive probe for w=11, s=4 found no counterexample (inconclusive).
- The author's guess f(w)=⌊w/2⌋ is FALSE for even w ≥ 4 (f(4)=1, f(6)=2, f(8)=3).

## 4. Proofs (`proofs.txt`; all marked COMPLETE there, no sketches)

- Lemma 1: g(M) ≤ s iff the states of spread > s induce an acyclic graph (also gives the game reading).
- Lemma 2/3: safety of pruning and of enumerating by discovery numbering.
- Theorem A: f(w) ≤ ⌊w/2⌋ via the counter with ⌊w/2⌋+2 states; the author's 0..w counter claim is also true.
- Theorem B: g(M) ≤ s forces at least w−s+1 reachable states.
- Theorem C: f(w) ≥ 1 for w ≥ 2.
- Theorem D: g(B_w) ≤ w/2 − 1 for even w ≥ 4 (accounting identity a = w/2 + (k − h − e₀ + e_K)/2 plus a parity argument).
- No general lower bound beyond Theorem C is proved.

## 5. Interpretation

- "|Q| = w+1": unreachable states change nothing, so I minimise over machines with at most w+1 reachable states.
- C(q) uses only strings of length ≥ w, as written (no zero padding of short strings). States reached only by shorter strings have empty C and cannot lie on a cycle.
- The game reading is equivalent to the definition (Lemma 1). No degeneracy found.
- Author's beliefs: f(1)=0, f(2)=1, f(3)≤1 (in fact =1), f ≤ ⌊w/2⌋, f ≤ w−1, f ≥ 1 for w ≥ 2, e(2)=1 are all correct.

## 6. Code

- `slow.py` — literal definition (raw enumeration; `check FILE` re-computes g of listed machines). Deliberate failure test: a line with g altered was rejected (`bad.txt`).
- `fast.cpp` — w ≤ 7, unpruned histograms (`hist`) and pruned search.
- `fast2.cpp` — same pruned search, w ≤ 12, BFS numbering.
- `fast3.cpp` — incremental version, BFS or DFS numbering (`ORDER=0/1`), prefix splitting; `split.sh`, `nstar.sh` drive it.
- `counter.py`, `bouncy.py`, `show.py` — spreads of the two families and of any listed machine.

Process note: for a few minutes three of my processes ran at once (a timed-out run I believed finished), exceeding the 2-core limit.
