# solver-a report

Notation: W_0 = 1, W_r = W_{r-1}(W_{r-1}+1) = 2, 6, 42, 1806, 3263442, … (= s_{r+1} − 1, Sylvester's sequence).

## 1. Tables

| r | d | D(r) | status |
|---|---|------|--------|
| 1 | 1 | 2 | COMPUTED, exhaustive (`lazy.cpp`) |
| 2 | 3 | **6** (not 7) | COMPUTED, exhaustive (`lazy.cpp`, 482 search nodes) |
| 3 | 7 | **42** | COMPUTED, exhaustive twice: `lazy.cpp` (6.8e6 nodes) and independently `naive3.cpp` (every tree up to a stated WLOG, 9.5e7 trees, full functional graph of all 128 states) |
| 4 | 15 | ≥ 1806 | lower bound PROVED + checked; **not** exhaustive. Annealing from random trees stops at exactly 1806 in 8 of 8 runs (`sa.cpp`) |
| 5 | 31 | ≥ 3263442 | PROVED + witness simulated |

Restricted version B(r,m) = longest cycle with depth r and at most m distinct indices:

- r=2, m=1..3: 2, 4, 6 — COMPUTED exhaustive.
- r=3, m=3..7: 8, 12, 20, 30, 42 — COMPUTED exhaustive.
- r=4, m=4, 5: 16, 24 — COMPUTED exhaustive (m=5: 5.9e9 nodes). m=6: 40 found, but the exhaustive run TIMED OUT (1500 s), so not exhaustive.
- r=4, m=6..11: 40, 72, 108, 180, 270, 420 — annealing only (3 seeds each, identical), NOT exhaustive.

Every entry equals the composition rule B(r,m) = max over m1+m2 = m−1 of B(r−1,m2)·(B(r−1,m1)+1), B(r,0) = B(0,m) = 1.

`lazy.cpp`: depth-first search along a trajectory; tree nodes and initial bit values are decided only when first needed. Prunings: (a) indices named in order of first use (safe: renaming invariance); (b) a trajectory revisiting a non-start state is dropped (safe: that start is on no cycle; every cycle is found from each of its states). An unsafe pruning could only lower the value. `naive3.cpp` shares no code or method (WLOG: root index 1, 0-child index 2, 1-child index 2 or 3).

Extra COMPUTED facts at r=3 (`naive3.out`, `naive3b.cpp`): the trees reaching 42 are exactly the 24 relabelings of the witness below (unique optimum); in every cycle that mixes root=0 and root=1 states there are at most 6 = D(2) root=1 states and at most 6 consecutive root=0 states.

## 2. Witnesses (plain text, checked by `verify.py`, a direct simulator of the definition)

- `sylvester_r1.txt … sylvester_r5.txt`: cycles of length 2, 6, 42, 1806, 3263442 (from `gen.py`), start all-zero.
- `witness_r2_m3_N6.txt`, `witness_r3_m7_N42.txt`: found by the exhaustive search (same tree).
- Failure seen on purpose: `bad_N43.txt`, `bad_N41.txt`, `bad_leaf.txt`, `bad_repeat.txt`, `bad_shortcycle.txt` are all REJECTED.
- "Next value impossible" for r ≤ 3 is the exhaustive search; cheapest re-check: `naive3.cpp` (13 s), a histogram of trees by longest cycle, empty above 42.

## 3. Headline

- **log2 D(r) is NOT polynomially bounded: PROVED.** 2^(r−1) ≤ log2 D(r) ≤ 2^r − 1. The problem's GUESS is wrong.
- **D(r) ≥ W_r for all r: PROVED** (D(2) ≥ 6, D(3) ≥ 42, D(4) ≥ 1806).
- **D(2) = 6, D(3) = 42: COMPUTED exhaustively.**
- **Conjecture: D(r) = W_r = D(r−1)² + D(r−1) for all r**, i.e. D(r) = s_{r+1} − 1 ≈ 1.59791^(2^r) − 1/2, log2 D(r) ≈ 0.676·2^r. NOT proved. Confidence: r=4 about 90% (annealing; composition rule matches every restricted-m value); all r about 70% — W_r uses only ~0.68 of the bits and nothing excludes subtrees sharing memory at large r.

Exact function settled only for r ≤ 3; the polynomial question is settled.

## 4. Proofs (`proofs.txt`)

- Theorem 1, D(r) ≥ W_r — COMPLETE. Induction: M_r has a cycle of length W_r on which exactly one state uses a leaf with R = ∅ ("free leaf"). M_r = new root a; 0-subtree = M_{r−1} on X with the free leaf writing a; 1-subtree = M_{r−1} on disjoint Y. The X-copy runs a full cycle, its free leaf sets a, then one Y-step erases a: W² + W distinct states, exactly 2^r − 1 indices.
- Corollary, 2^(r−1) ≤ log2 D(r) ≤ 2^r − 1 — COMPLETE.
- Lemma 2 — COMPLETE: on a cycle containing a weight-raising step, every state has at most one 1 among the indices of the all-zero path; a cycle without one has length ≤ 2^r.
- Remark 3 — COMPLETE: D(r) ≤ 2^d − 1 for r ≥ 2 (true, but see section 5).
- Upper bound D(r) ≤ W_r: no proof and no sketch I trust.

## 5. Problem statement

- "D(1) = 2": true. "6 ≤ D(2) ≤ 7": true; exactly 6.
- "For r ≥ 2 the all-ones state has no preimage": **false as stated** — if the root's 0-child is a leaf writing the root, 011…1 → 11…1. The conclusion D(r) ≤ 2^d − 1 survives by a different argument (Remark 3).
- The weight claim (k ones read lowers weight by ≥ k − 1; only the all-zero path raises it): true (Lemma 2a).
- The GUESS fails because a carry (erase many, set one) needs only one write per step.
- No real ambiguity found; reading used: paths of length 0..r, a leaf may write any index it read.

## 6. Code → numbers

- `lazy.cpp`: all exhaustive D and B values; outputs `witness_*.txt`, `lazy_r4_m6.out`.
- `naive3.cpp` → `naive3.out`: D(3) = 42 independently; `naive3b.cpp`: the root=0 / root=1 statistics.
- `gen.py`: Sylvester witnesses; `verify.py`: checker.
- `sa.cpp`: heuristic values (r=4); `results.txt`: all run results in one place.

Folder: `solver-a/`.
