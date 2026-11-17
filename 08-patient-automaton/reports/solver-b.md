# solver-b report

**Headline: YES.** A patient automaton exists; the least number of states is **n = 4**. Hence s(n) = 0 for n ≤ 3 and s(n) = ∞ for every n ≥ 4. Confidence: very high (hand proof plus three independent machine checks).

## 1. Tables

All entries COMPUTED, exhaustive over all n^(2n) labelled automata.

| d | n=2 | n=3 | n=4 | n=5 | n=6 |
|---|---|---|---|---|---|
| 0 | 12 | 405 | 28960 | 3645945 | 708201936 |
| 1 | 12 | 405 | 27952 | 3318105 | 601411536 |
| 2 | 12 | 405 | 27808 | 3253305 | 578288016 |
| 3 | 12 | 405 | 27808 | 3243465 | 572993136 |
| 4 | 12 | 405 | 27808 | 3241545 | 571486896 |
| 5 | 12 | 405 | 27808 | 3240825 | 570912336 |
| 6 | 12 | 405 | 27808 | 3240105 | 570537936 |
| W_inf | 12 | 405 | 27712 | 3199305 | 556677936 |
| total | 16 | 729 | 65536 | 9765625 | 2176782336 |

- **n=3:** |W_d| = |W_inf| already at d = 0.
- **n=4:** the two numbers never agree, in range or beyond. |W_d(4)| = 27808 for every d ≥ 2 (proved, given the d = 2 count); the gap is the 96 patient automata.
- **n=5:** no agreement for d ≤ 14. |W_d| stays at 3240105 for d = 6..14; the 40800 survivors (170 classes) are not individually proved patient.
- **n=6:** computed to d = 8 only: |W_7| = 570477456, |W_8| = 570451536, still falling.
- **Finite drops exist:** at n=5, 720 automata are 5-winnable but not 6-winnable; at n=6, 25920 are 7-winnable but not 8-winnable.

Methods:
- n = 2..5: C++ with no shortcuts (`game full`), which also asserts W_inf ⊆ W_(d+1) ⊆ W_d per automaton (0 violations).
- n = 4, 5 again, and n = 6 only this way: canonical representatives under state renaming × letter swap, orbit-weighted, with monotonicity shortcuts (`game sym`). Identical where both were run.
- n = 3, 4 a third time in Python with a different blind-game algorithm (`table_py.py`). Identical.
- Literal definition (AND-OR search over strategies, no state abstraction, T ≤ 6, d ≤ 2) against the fast solver's exact minimal T: all n=2 and n=3 automata and 151 n=4 automata, 0 mismatches (`slow.py`).
- Negative control `slow_negctl.py` (compared against the wrong column) reports 21 mismatches at n=4, so the comparison can fail.

## 2. Witnesses (`witness.txt`)

**C2**, states 1..4, rows are images of 1,2,3,4:

- a: 1 1 2 3
- b: 4 1 3 4

**Strategy σ_d, winning with T = d+4:**
- x_1 = b.
- Let r be the least integer ≥ d+1 with r ≡ 2 (mod 3) if y_1 = a, or r ≡ 0 (mod 3) if y_1 = b.
- Play a in rounds 2..r, b in round r+1, then a.
- y_1 is first needed at time d+2, when it is legally known.

Why it works: after x_1 = b the image is 123 or 34. While both players' letters are a/b, the image runs round the 3-cycle 14 → 34 → 13 → 14; any Spoiler a collapses it. Only 14 is killed by b, and y_1 fixes the phase.

Exact minimal T for d = 0..10 is 4 4 6 7 7 9 10 10 12 13 13, so d+4 is not always optimal.

**Not blind-winnable:** the Spoiler replies b to every a, b to a b followed by b, and to a b followed by a^m b replies a iff m ≡ 2 (mod 3).

**Second class C1:** a: 1 1 2 3, b: 3 4 2 2, same σ_d. The patient automata on 4 states are exactly the 96 images of C1 and C2 (48 + 48).

**n ≥ 5:** pad C2 with states whose both letters go to 1.

`verify.py` (independent of the C++) checks:
- σ_d against all 2^(d+3) Spoiler plays for d = 0..12, for C1, C2 and the padded 6-state automaton.
- Blind unwinnability by exact family BFS (closes after 26–27 families) and by brute force over all words of length ≤ 14.
- The Spoiler rule for C2 against all words of length ≤ 16.
- Three negative controls, all rejected as they should be: a wrong-residue strategy, a perturbed automaton, and a trivially synchronizing automaton.

## 3. Conjectures and guesses

- s(n) = ∞ for n ≥ 4 is proved, so there is no formula to conjecture.
- GUESS: the n=5 survivors at d = 14 are all patient, i.e. W_d(5) is stable from d = 6.
- OPEN: where the n=6 chain stabilises.
- The problem's belief "at most exponential" is moot for n ≥ 4.

## 4. Proofs (`proofs.txt`)

- Lemma 0, inclusions: COMPLETE.
- Lemma 1, uniform T costs nothing (König): COMPLETE.
- Lemma 2, the reduction to a finite game on (revealed image, pending d letters) and the pair-set blind game: COMPLETE.
- Theorem 1, C2 is patient: COMPLETE. Lemma A is the σ_d induction; Lemma B is the Spoiler invariant over runs of a and b.
- Theorem 2, no patient automaton with n ≤ 3: rests on the exhaustive computation W_0 = W_inf for n = 2, 3.
- Theorem 3, padding to every n ≥ 4: COMPLETE.
- Theorem 4, the n=4 census: C1 is d-winnable for all d: COMPLETE (same argument). C1 not blind-winnable: COMPUTED only. The count of 96 rests on the computation.

## 5. Interpretation

- Winning is read literally: the image after x_T (before y_T) is a singleton, with one T for all plays. Singletons persist, so "at T" equals "by T".
- At delay d the first d+1 letters are blind.
- The Spoiler's adaptivity is irrelevant against a deterministic strategy; only the set of Spoiler sequences matters.
- The problem's beliefs: s(2) = 0 is true; "uniform T costs nothing" is true (Lemma 1); "a patient automaton has at least 3 states" is true but weak (it needs 4); W_0 ≠ W_inf first happens at n = 4.
- I saw no ambiguity that changes an answer.

## 6. Files (all in `solver-b/`)

- `game.cpp`: `full` and `sym` produce the tables, `one` the minimal T, `dump` feeds `slow.py`.
- Outputs: `n5_full.txt`, `n5_sym.txt` (d ≤ 8, survivor list), `n5_sym14.txt` (d ≤ 14), `n6_sym.txt`, `table_py_n4.txt`. The n = 2..4 tables were printed to the terminal and regenerate in under a second.
- `table_py.py`, `slow.py`, `slow_negctl.py`, `verify.py`: the independent checks.
- `explore.py`, `strat.py`: exploration only.
- `proofs.txt`, `witness.txt`.
