# Referee report

Code and outputs: `verifier/`. P(n) is the Pelc expression from the problem.

## Verdicts

**Headline (Q_2(n) = U1(n) for all n): UNSUPPORTED.** Both solvers say so themselves. No counterexample: my own program (`ms.cpp`) gives Q_2(n) = P(n) for 2 <= n <= 400. The solvers' larger ranges were not re-run; their files contain no exception and use the correct P(n).

**State abstraction (A's P1, B's Lemma 1): CONFIRMED.** Proof read with no gap, and checked against the literal definition (`lit.py`: every L ⊆ {1..t} enumerated, ell_s from C_(s-1)):
- Exhaustive over all question and answer sequences: n=2 (5 questions), n=3 (4), n=4 (3), each for k=1,2,3. That is 393,216 histories, including 664,512 steps with ell = 0 and 249,856 with dead elements in S.
- 4,000 random histories (n <= 8, k <= 4, up to 11 questions).
- Alive sets and obligations agree at every step. Two deliberately wrong transition rules were both caught.
- Game level: literal search, no memo, all 2^n subsets (`litgame.cpp`), gives Q_2 = Q_3 = 3, 5, 5, 6 for n = 2..5, and no k=1 win within 7 questions (n=2,3,4) or 6 (n=5).

**Q_1(n) = infinity for n in {2,...,7,9}: CONFIRMED, not by depth cutoff.**
- My own backward induction to a fixed point on the uncapped state space; the never-winning set is re-checked as closed and contains the start.
- A labelled-state fixed point over all 2^n subsets, without multiset symmetry, for n <= 7 (`lab.py`).
- B's seven trap sets pass my checker, and 300 random literal plays of 60 questions against each trap responder keep |C| >= 2.
- All of this rests on Lemma 1.

**Tables: CONFIRMED; no entry differs from either solver.**
- Fixed point: k=1 n <= 13, k=2 n <= 11, k=3 n <= 8.
- Q_k(n) = P(n) for k = 2, 3, 4, 5, 8 and n <= 400.
- Q_1(n) = P(n) for n = 8 and 10 <= n <= 700, beyond both solvers (314 and 317).
- U1(n) = P(n) for n <= 120 by my own DP.

**Witnesses: CONFIRMED.**
- My literal checker (`chk.py`) accepts all 57 of A's trees and all 76 of B's.
- It accepts strategies from my own solver: k=2 n=300, 399; k=3 n=300; k=8 n=400; k=1 n=314, 315, 317, 400, 585.
- It rejects 18 broken witnesses: mutated root question, q reduced by one, wrong k, wrong n, a trap state removed, a trap set under the wrong k, both solvers' negative files.

**Proofs marked COMPLETE: no gap found in any.**
- A P2 / B Lemma 2 (Q_k >= U1). A's version uses determinacy of the finite one-lie game without saying so.
- A P3 / B Lemma 3 (U1 >= P, with the odd-n refinement).
- A P3' and B Lemma 4 (pruning bounds). Pruned and unpruned searches agree for n <= 13, q <= 8; unpruned search confirms no win at P(n)-1 for eight n up to 29.
- A P4 (Q_1(2), Q_1(3) infinite by hand) and B Proposition 5 (n=2).

## Disagreements between the solvers

None on any value, formula or reading. Coverage differs:
- A proves Q_1(3) = infinity by hand; B only by machine.
- B computed the other reading (ell = 0 questions forbidden); A did not. I confirm the values are unchanged for k=1 n <= 11 and k=2 n <= 9.

## Errors and weak points

- A: "fixed point agrees for n <= 13" (k=2) has a file only for n = 10..13. My run covers 2..11 and agrees.
- A: the three deliberate failures are not in `check_log.txt`. I re-ran A's checker; all three fail as claimed.
- Both: the "no win at P(n)-1" column of every sweep is decided by the proved weight bound at the root or first question. It restates the proof.

## Problem as written

Both answered the stated problem, reading it as I do: ell counts alive candidates only, ell = 0 questions are allowed, and n is in E_k only if Q_k(n) is finite. The lower bound Q_k(n) >= U1(n) >= P(n) is proved; the upper bound is computed only.

## New observation

For k = 2 and k = 3, take a state with a unobligated candidates, b permanently blocked ones and j questions left. For all 74,556 such states with a+b <= 110, j <= 12, the Q_k game is winnable exactly when the one-lie game is. The same test for k = 1 finds 112 differences at a+b <= 40, j <= 9, so it can fail.

## Not checked

- Any n > 400 for k >= 2, or n > 700 for k = 1.
- The solvers' C++ line by line.
- Pelc's upper bound beyond n = 120, other than through Q_k = P.

## Follow-up tasks

1. Prove or refute: for k >= 2 and all a, b, j, the questioner wins the Q_k game within j questions from (a candidates with obligation 0, b that can never lie again) if and only if she wins the one-lie game from (a, b) in j. With Pelc's theorem this gives E_k = {n >= 2}.
2. Prove E_1 = {8} ∪ {n >= 10}: a hand trap invariant for n in {4, 5, 6, 7, 9}, and an explicit strategy family for n >= 10.
