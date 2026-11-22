<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. `solver-b/proofs.txt:12-18`: if the unlit states form a closed walk, that walk contains a simple cycle on which every spread is > s, and the converse is immediate. Self-loops count as cycles of length 1. States with empty C lie on no cycle (Lemma 0, line 8), so leaving them out of "unlit" changes nothing. The game reading in `chain/3-question.txt` and `README.md:24-28` is equivalent: take F = all reachable states with spread ≤ s. — none

Q2: CONFIRMED. `solver-b/proofs.txt:42-49`. Case (i) gives a ≥ k because each 1 raises the counter by at most one. Case (ii) wastes no 0 and has h ≤ K. For odd w, L(k) = k for every k (I checked k ≤ t and k = t+1), so a lies in [k, k+t]. The symmetry argument only uses h ≤ K, so it does not depend on the start state. The machine has ⌊w/2⌋+2 ≤ w+1 states, and every state has spread ≤ ⌊w/2⌋. — none

Q3: CONFIRMED. `solver-b/proofs.txt:95-125`:
- The accounting identity holds: steps from 0 add +1 to the plain counter's move and steps from K add −1, so k = h + 2a − w + e0 − eK.
- The parity fact holds because 0 and K = w are both even.
- The symmetry transfer is legitimate because h is arbitrary in every bound.
- All four cases for state 0 hold, including the K−1 zeros when eK ≥ 1.
- Both cases for odd k hold; a′ ≥ ⌈(k−2)/2⌉ = (k−1)/2 because a′ is an integer.
- Even inner states move only to k±1, which are odd.

I checked w=4 and w=6 in `witnesses.txt` against the B_w rule. I also recomputed the per-state count ranges independently for w = 4, 6, 8, 10 (script in /tmp). Odd k gives [(k+1)/2, t+(k−1)/2], and the ends give [0, t−1] and [t+1, 2t]. — none

Q4: CONFIRMED. `solver-b/proofs.txt:60-66`: a lies on the cycle of the run on 0^∞, so q0·0^n = a for infinitely many n, and 0 ∈ C(a). p_0..p_k are distinct because k < J, which gives at least k+1 ≥ w−s+1 states. Theorem C (`proofs.txt:69-84`) is valid; "k ≥ w" is in fact k = w (`referee.md:42`). — none

Q5: CONFIRMED. In `verifier/ref.cpp:63-95,122-127`, the C sets of a partial machine are closures along defined edges only, so they are subsets of the final C sets. The `cyc` function correctly finds a cycle by repeatedly deleting states with no successor in the set. So pruning can only lose machines that are really bad, and an unsafe cut could only make f too large; each f is also met by a witness (`witnesses.txt`, B_10). The provenance in `README.md:37-40` matches `referee.md:9-11,21` and `final_lb.log`, with two exceptions: the "same tree" remark (F3) and the top-level row (F2). — none

Q6: CONFIRMED. State counts, the jump rule and the reliable levels all match `proofs.txt:37-39,86-92` and `solver-a/proofs.txt:4-7`. I checked by hand that S_w ≅ B_w (B0→0, B1→1, level j→j+1, T1→w−1, T0→w). The match does not depend on parity, so it holds for every w ≥ 3. That is stronger than the README states, but not a contradiction. — optional: "same machine as B_w (for every w; the referee tested w = 4 to 12)"

Q7: REFUTED (minor). Most cells match their sources: 262,279,982; 85/65; 6,000; 24m44; the small errors; prior art. Two do not:
- `README.md:38` says the solvers' searches "visit the same tree". That is true only for B's ORDER=0 (A 18,357,815 = B 18,357,815; 258,801,970 = 258,801,970). B's ORDER=1 run is a different tree (`final_lb.log:12,14`).
- `README.md:43` says "proved by hand by both solvers". A's P3 (`solver-a/proofs.txt:41-46`) is a lemma about paths, and A says it was not turned into a general bound. — see F3, F4

Q8: REFUTED. Top-level `README.md:49` says "values to w = 6 by exhaustive search three times". Solver B's unpruned search stops at w = 5 (`reports/solver-b.md:157`), so only two unpruned searches reach w = 6. Also, `20-short-memory-count/README.md:11-12` says the chain's guess is "one too many at every even w from 4 on". That presents the conjecture as settled for even w ≥ 12. — see F1, F2

F: MODERATE — `20-short-memory-count/README.md:11-12` — "one too many at every even w from 4 on" assumes f(w) = w/2 − 1 exactly for all even w. That has been computed only for w = 4, 6, 8, 10; Theorem D proves only "at least one too many". — high — "which is too large at every even w from 4 on (by Theorem D), and exactly one too many at w = 4, 6, 8, 10"

F: MODERATE — `README.md:49` (top level) — "values to w = 6 by exhaustive search three times". Solver B's w = 6 run is pruned (`reports/solver-b.md:157-158`). — high — "values to w = 5 by unpruned search three times, w = 6 twice unpruned and once pruned, beyond that by pruned searches"

F: LOW — `20-short-memory-count/README.md:38` — "The two solvers' searches visit the same tree (identical node counts)". This holds only for B's breadth-first run; B's depth-first run has different node counts (3,883,022 and 16,682,611), though it uses the same code base. — high — "A's search and B's breadth-first run visit the same tree (identical node counts) and count as one check; B's depth-first run uses the same program; the referee's program is independent"

F: LOW — `20-short-memory-count/README.md:43` — "proved by hand by both solvers". A's P3 states the path lemma and stops short of the state-count bound; deriving the bound from it is immediate, but A did not write that step. — medium — "proved by solver B (Theorem B); solver A's P3 is the same path lemma"

F: LOW — `20-short-memory-count/README.md:57-58` — "The even inner levels may be worse". By A's P1 and my recomputation, only state 2 at w = 4 is worse; for even w ≥ 6 every state is within the bound. — medium — "Only at w = 4 is an even inner level worse (state 2, spread 2)…"

F: LOW — `20-short-memory-count/README.md:63-66` — "for even w it is the same machine". The isomorphism holds for every w ≥ 3, which matters for crediting the proofs as independent. For even w, A's P1 and B's Theorem D are two proofs about one machine. — medium — add "so for even w the two proofs are of the same machine"

F: LOW — `20-short-memory-count/README.md:41` — "tested the stated ranges by program" leaves out that `lemmas.py` printed 8 FAIL lines at w = 3, which the referee attributes to its own test's over-strict bound (`verifier/runs.log:42`). — low — mention it in "Small errors"

Things the questions did not ask about:
1. The pruned searches at w ≥ 7 are cross-checked only by the referee, so they still rest on that program.
2. The referee read only B's `fast3.cpp` (`referee.md:62`). A's `fast.cpp` and B's `fast.cpp`/`fast2.cpp` have not been reviewed line by line.
