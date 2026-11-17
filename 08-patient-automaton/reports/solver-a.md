# solver-a report

**Headline: YES.** A patient automaton exists; the least n is 4. So s(n) = 0 for n ≤ 3 and s(n) = ∞ for every n ≥ 4.
Confidence: existence at n = 4 is proved by hand and machine-checked; minimality (nothing at n ≤ 3) is an exact exhaustive computation.

## 1. Tables

All entries COMPUTED, exhaustive over all n^(2n) labelled automata. Method: the delay game is solved exactly as a reachability game on positions (image, queue of ≤ d pending Synchronizer letters); the blind game by BFS over antichains of possible images. No horizon and no pruning. n = 5 uses orbit representatives under state relabelling and a↔b swap, weighted by orbit size; this was checked against the unreduced run at n = 4, and the weights sum to n^(2n).

| d | n=2 (16) | n=3 (729) | n=4 (65536) | n=5 (9765625) |
|---|---|---|---|---|
| 0 | 12 | 405 | 28960 | 3645945 |
| 1 | 12 | 405 | 27952 | 3318105 |
| 2 | 12 | 405 | 27808 | 3253305 |
| 3 | 12 | 405 | 27808 | 3243465 |
| 4 | 12 | 405 | 27808 | 3241545 |
| 5 | 12 | 405 | 27808 | 3240825 |
| 6 | 12 | 405 | 27808 | 3240105 |
| 7–11 (n=4), 7–12 (n=5) | | | 27808 | 3240105 |
| W_inf | 12 | 405 | 27712 | 3199305 |

- n = 2, 3: W_d = W_inf already at d = 0.
- n = 4: no d in 0..6 (or up to 11) gives agreement. The gap from d = 2 on is exactly 96 automata, and by Theorems 1–2 it never closes for the 48 in Witness 1's orbit.
- n = 5: no agreement up to d = 12. |W_d| is constant from d = 6, with a gap of 40800 automata (170 classes). Those are patient only as far as d = 12, except the padded Witness 1, which is proved.

Cross-check against a separate brute-force program written from the definition (explicit histories, horizon Tmax): 0 mismatches of minimal T on all automata for n = 2, 3 (d ≤ 4, Tmax 8) and n = 4 (d ≤ 4, Tmax 9), and a 9796-automaton sample for n = 5 (d ≤ 3, Tmax 8). It confirms losses only up to its horizon.

## 2. Witnesses (`witnesses.txt`)

**Witness 1**, n = 4: a = (1→2, 2→2, 3→1, 4→3), b = (1→4, 2→3, 3→1, 4→1).

Strategy σ_d: x_1 = b. Let τ be the least t ≥ max(d+2, 3) with t ≡ 0 (mod 3) if y_1 = a, or t ≡ 1 (mod 3) if y_1 = b. Play x_τ = b and x_t = a at every other t ≥ 2. The image is a singleton after x_τ.

T(d) for d = 0..11 is 4, 4, 6, 7, 7, 9, 10, 10, 12, 13, 13, 15, which equals the solver's minimal T for those d.

Why: after "b ? a" the image is a pair. Under a-rounds the Spoiler's only surviving path is the 3-cycle 34 → 14 → 13 → 34, with phase fixed by y_1. The Synchronizer waits until y_1 is visible, then plays b exactly when the pair is 34. A blind word fixes the phase in advance and the Spoiler picks y_1 off it.

**Witness 2**, the only other class at n = 4: a = (1,4,2,4), b = (3,2,2,1). Strategy: x_1 = a, wait with b, strike with a at t ≡ 1 (mod 3) if y_1 = a, t ≡ 0 if y_1 = b. This is machine-checked for d ≤ 300 only; no written proof.

**n ≥ 5**: add states that copy state 1 (a→2, b→4).

`check_witness.py` is a stand-alone Python checker sharing no code with the solver. It verifies σ_d against all Spoiler plays for d = 0..300 (literal enumeration for d ≤ 8), recomputes the closed set of 9 blind-game families (`blind_cert_2213_4311.txt`), and checks the 4-family certificate from the proof. Four deliberately wrong inputs (two wrong residues, two altered transitions) were all REJECTED.

## 3. Conjectures and the problem's beliefs

- True: s(2) = 0 (computed); uniform T (König); W_0 ≠ W_inf first at n = 4. "At least 3 states" is true but weak (least is 4). The exponential bound on finite s(n) is vacuous for n ≥ 4.
- Guess, not proved: the 40800 n = 5 automata in W_12 \ W_inf are all patient. |W_d(n)| stops changing at d = 0, 0, 2, 6 for n = 2, 3, 4, 5.

## 4. Proofs (`proofs.txt`)

- Theorem 1, COMPLETE: Witness 1 is d-winnable for all d, by a mod-3 invariant on the pair round-tables.
- Theorem 2, COMPLETE: Witness 1 is not blind-winnable. The pair families {13,14}, {14,23}, {12,13,34}, {12,14,34} are closed under either Synchronizer letter, and after any first letter the Spoiler's options contain one of them.
- Corollary, COMPLETE: s(n) = ∞ for n ≥ 4, by padding.
- Reduction of the delay game to the (image, queue) game, COMPLETE.
- Minimality (n ≤ 3): computation only, but exact: |W_0(n)| = |W_inf(n)|.

## 5. Interpretation

- "Singleton after x_T for one uniform T" is equivalent to "every play reaches a singleton", since singletons persist (and König).
- A Spoiler who knows the deterministic strategy effectively has d letters of lookahead; the first d+1 Synchronizer letters are a fixed word.
- I found no ambiguity that changes the answers.

## 6. Files (in `solver-a/`)

- `wd.cpp`: all table numbers (`./wd n dmax sym [dump]`).
- `dump4sym.txt`, `dump5sym12.txt`: class representatives where the W_d profile differs from blind.
- `wdfull.cpp` piped into `slow.cpp`: the brute-force cross-check.
- `strat.py`: independent Python game solver for one automaton (minimal T, opening word).
- `check_witness.py`, `witnesses.txt`, `proofs.txt`, `blind_cert_2213_4311.txt`, `blind_cert_1424_3221.txt`.
- `blind_cert_2211_4311.txt`, `blind_cert_2213_4312.txt`: mutation-test by-products; ignore.
