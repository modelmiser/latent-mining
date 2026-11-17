# solver-a report: charged pebbles, clean delivery

Files: `solver-a/`.

## 1. Tables

C(p,k), every entry COMPUTED by exhaustive breadth-first search over all reachable states (`bfs2.cpp`). Every entry equals (pk+2)·2^(p-2).

| p | k values run | C(p,k) | largest state count |
|---|---|---|---|
| 2 | 0–5, 8, 15 | 2, 4, 6, 8, 10, 12, 18, 32 | 8,960 |
| 3 | 0–6, 8, 15 | 4, 10, 16, 22, 28, 34, 40, 52, 94 | 11,440,760 |
| 4 | 0–6 | 8, 24, 40, 56, 72, 88, 104 | 112,202,419 |
| 5 | 0, 1, 2 | 16, 56, 96 | 117,951,562 |
| 6 | 0 | 32 | 1,626,196 |

- The search caps node positions at N=240. Each run reported the highest node ever occupied below the cap, so the cap never suppressed a move (argument in `proofs.txt` P4).
- In every run the highest node ever occupied was C(p,k) + 2^(p-1) − 1.
- Cross-checks, all agreeing on C, highest node and (where comparable) state count:
  - `slow.py`, a direct Python implementation with pebble identities: p=2 (k=0,1,2,5), p=3 (k=0–3), p=4 (k=0,1), plus eight unequal-charge cases.
  - `cert.py`, a third implementation with no cap: p=2 (k=0–3), p=3 (k=0–3), p=4 (k=0,1); state counts identical to the C++ search.
- Not reached: p=6, k=1 and p=5, k=3.

Side result (COMPUTED, outside the problem as posed): with unequal starting charges the value is not a function of total charge c, though always at most (c+2)·2^(p-2). Examples: p=2 charges 3,0 give 3 and 2,1 give 5; p=3 charges 3,0,0 give 6, 2,1,0 give 10, 3,3,0 give 16; p=4 charges 3,0,0,0 give 12 and 2,2,0,0 give 24.

## 2. Witnesses

Move lists are plain text (`P node charge`, `R node`, `F node`), checked by `check.py p k n file`, which shares no code with the search.

- **Reaching the value, shortest sequences from the search:** `witness/bfs_p{2,3,4}_k{1,2,3}.txt` and `witness/bfs_p5_k1.txt`. All accepted.
- **Reaching the value, general strategy:** `strategy.py p k` writes the lower-bound strategy for any (p,k). Accepted instances are `witness/strat_p*_k*.txt` for (3,1), (4,4), (4,6), (5,2), (5,3), (6,1), (6,2), (7,1), (3,15), (2,15).
- **Nothing better is possible, small cases:** `cert/closed_p{2,3}_k{0..3}.txt` and `cert/closed_p4_k{0,1}.txt` list the full reachable set. `cert.py check p k file` verifies the start state is present, the set is closed under every legal move, and reports the largest clean n. All ten pass. `closed_p4_k1.txt` is 508 kB, over the 300 kB guideline, kept because it is a certificate.
- **Nothing better is possible, larger cases:** the only evidence is the exhaustive search itself (`run1.log`, `run2.log`, `run3.log`); the reachable sets are too large to ship.
- **Checks watched failing on purpose:**
  - `check.py` rejects a wrong target n, a wrong k, an F relabelled as R (`witness/BAD_p3_k1_F_as_R.txt`), and an F on a charge-0 pebble (`witness/BAD_p2_k1_extra.txt`).
  - `cert.py` rejects a closed set with one line deleted (`cert/BAD_closed_p3_k1.txt`).
  - A deliberately binding cap (p=3, k=3, N=20) is flagged CAP-BINDING and returns 20 instead of 22.

## 3. Headline

**YES, as far as I can tell: C(p,k) = (pk+2)·2^(p-2). No counterexample exists in the range computed.**

- Proved for all (p,k): C(p,k) ≥ (pk+2)·2^(p-2).
- Proved: equality for p=2 and all k.
- Computed only: equality for p=3 (k ≤ 6, 8, 15), p=4 (k ≤ 6), p=5 (k ≤ 2), p=6 (k=0).
- Guess: equality for all p ≥ 3 and all k, confidence about 90%. The feared end-game trick does not show up in 28 exhaustive (p,k) cases.

## 4. Proofs (full text in `proofs.txt`)

- **P1, lower bound for all p ≥ 2, k ≥ 0: COMPLETE.** Reversible doubling lemma, then pk rounds of "advance 2^(p-2) and forget the base", rotating pebbles cyclically so each is forgotten exactly k times.
- **P2, upper bound for p=2: COMPLETE.** With s the number of F-moves so far, the invariant is: a lone pebble sits at x ≤ s+2; a pair has its top at ≤ s+3, and at ≤ s+2 if the pair is not adjacent. Since s ≤ 2k, n ≤ 2k+2.
- **P3, upper bound for p ≥ 3: NOT PROVED.** I have no sketch I trust. `proofs.txt` notes one idea (induction on the number of F-moves via reversible-equivalence classes) and why the unequal-charge data constrains any induction.
- **P4, cap safety: COMPLETE.**

## 5. Interpretation

- Pebbles of equal charge in hand are interchangeable; a placement chooses which charge to place. Pebbles on the board keep their own charge.
- F is illegal on node 1, since node 0 always counts as pebbled; removing node 1 is always R.
- "Clean at n" needs n ≥ 1; the empty board is not clean. I found no ambiguity that changes a value.
- The problem's hand values all hold (C(2,k) and C(3,1) with equality); k=0 gives 2^(p-1) for p ≤ 6.
- p=1 is outside the stated range and degenerate: C(1,k)=1, while the formula gives (k+2)/2.

## 6. Code

- `bfs2.cpp`: every number in the main table and the unequal-charge side table; optional 4th argument writes a witness, 5th gives explicit charges. `run1.log` came from `bfs.cpp`, the same search before the p=6 packing was added.
- `slow.py` (reference), `cert.py` (closed-set certificates), `check.py` (witness checker), `strategy.py` (strategy generator).
- Logs: `run1.log`, `run2.log`, `run3.log`, `slow.log`, `witness.log`, `cert.log`. The seven smallest cases (p=2, k ≤ 3; p=3, k ≤ 2) were run before logging started; `cert.log` re-derives them.
