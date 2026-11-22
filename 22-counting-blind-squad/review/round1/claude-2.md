<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of a Claude seat run on the plan login (one of the two on Claude Opus). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Claude seat 2: experiment 22 (counting-blind firing squad)

All paths are relative to `22-counting-blind-squad/`. I did not run any repository program. I ran a 25-line simulator of my own, written from `chain/3-question.txt`, outside the repository.

## Verdicts

**Q1 (C4): CONFIRMED.** By hand, using k over existing neighbours only:
- n=2: GQ → GU1 → GU2 → GG → U1U1 → FF. Fires at t=5.
- n=3: GQQ → G U1 Q → G G U1 → U1 G U2 → U2 Q G → G U1 G → GGG → U1U1U1 → FFF. Fires at t=8.
- n=4: GQQQ → G U1 Q Q → G G U1 Q → U1 G G U1 → U2 G G U2 → GGGG → U1×4 → F×4. Fires at t=7.

My simulator agrees and also gives n=5 at t=10. At n=6 cells 1 and 2 enter F at t=12 while the other cells are in U2, G, U1, U1, so the rule fails. The witness line `0 3 3 3 1 0 2 4 1 2 1 0` (solver-a/witnesses.txt:6), read under the numbering in witnesses.txt:1-2, is exactly the README table (README.md:54-59).

**Q2 (C1): CONFIRMED** for `solver-b/brute.cpp`:
- k counts existing neighbours only (brute.cpp:21-22).
- The update is simultaneous, through a separate buffer b (brute.cpp:23, 28).
- A full fire returns the time and a partial fire returns failure (brute.cpp:26-27).
- "Never fires" is decided exactly by Brent cycle detection on F-free configurations, with no step cap (brute.cpp:29-31).
- d(Q,0)=0 is fixed (brute.cpp:43).
- There are ne = 3(s−1)−1 free entries, giving s^ne rules (brute.cpp:36, 39).
- N is the longest prefix 2..N that synchronises, and the program takes the maximum over all rules (brute.cpp:45-55). This is the stated L.

`fsq.cpp` brute (fsq.cpp:94-97) is the same with no pruning. Output: L=5, 1,244 rules, no "HIT NMAX" (solver-b/brute_out.txt).

**Q3 (C2): CONFIRMED.**
- Lazy assignment is sound. A run reads only the entries it needs, and a branch is cut only on a partial fire or on a configuration repeated under entries that are already fixed (lazy.cpp:136-155, fsq.cpp:64-78).
- The "lowest unused label" rule is sound. An unnamed row can only be reached after that state has appeared as a value, so the search is equivariant under renaming, and every rule has a canonical relabelling that the search reaches.
- The weight sum shows that the leaves of the unpruned search cover every rule exactly once. Tree leaves are disjoint cylinders, so a sum equal to 7^17 means none were lost. I summed the 8 `l7_8_*` and 24 `o7_24_*` files: each set totals 232,630,513,987,207 = 7^17 and holds 96 rules with prefix 11.
- The weight sum does not check how each leaf was classified.
- Search or pruning errors can only make L too small. A simulator error could make it too large, but the witnesses are rechecked by independent checkers (and by me at s=5).
- The README's description of what the three instances' searches share (README.md:70-76) matches referee.md:20-26.

**Q4 (C3): CONFIRMED. Every step follows.**
- Fact 0 (solver-a/proofs.txt:1-4) is a correct induction: cell n is still Q at time n−2.
- d(Q,1)=F on n=3: cell 3 is an end cell whose only neighbour is Q, so k=0 and it stays Q, which is a partial fire. d(Q,1)=Q: cells 2..n stay Q for ever (solver-b/proofs.txt:25-30).
- Lemma 2: the light-cone agreement holds, and the end step uses Fact 0 for both cell n and cell n+1 (solver-a/proofs.txt:13-18). In the case t(n) ≤ 2n−2, both t(m) < t(n) and t(m) > t(n) lead to a contradiction.
- P3: I re-derived each case (solver-b/proofs.txt:39-44), including b=Q giving (Q,G,G) → (G,G,Q).
- One paraphrase is inaccurate (Finding 2).

**Q5: CONFIRMED, with one misapplication.**
- README.md:20-32 matches chain/3-question.txt:5-13. "Stays asleep when all its neighbours are asleep" is d(Q,0)=Q. "No cell was F before" covers both the partial-fire rule and the never-fires rule.
- "Restricted ordinary rule" is correct: k is a function of (left, self, right) with a border symbol.
- The README applies that fact to "part of the result here" (README.md:87-90), but the only result it could support (no 3-state rule synchronises a single length ≥ 5, prior-art.md:16) is not stated in the README, and L(3)=2 does not follow from it (Finding 1).

**Q6: CONFIRMED with exceptions.**
- These match their sources:
  - 36, 232 and 1,244 rules
  - 336 rules
  - 96 = 4×24 rules (4! relabellings)
  - minimum times 1,3,4,6,7,9,10, with A's only to n=7 (solver-a.md:33)
  - 2,487,460 counterexamples (referee.md:15)
  - Giam N₄=8 and N₅≥14
  - four sources read in part
  - the bug caught by the weight sum
  - ⌊3(n−1)/2⌋, which I checked on all 7 values
- Exceptions are Findings 2-5.

**Q7: CONFIRMED.**
- Line 12 bolds the values but lines 14-16 say they are computed and that the general question is open.
- The top-level row (README.md:53 at the repository root) matches the folder.
- Nothing overstates settledness beyond Finding 1.

## Findings

1. **MODERATE** (confidence 0.8), README.md:87-90. "part of the result here follows from Yunès's 2008 result" is wrong. L(3)=2 concerns lengths 2 and 3, while Yunès's result covers n ≥ 5. The fact it does imply, that no 3-state rule synchronises any single length ≥ 5 (solver-b/brute_out.txt, s=3), is never stated in the README.
   Replace with: "For three states, the computed fact that no three-state rule synchronises any single line of 5 or more cells (solver B's single-length counts) also follows from Yunès's 2008 result as quoted in that survey, since a counting rule is a restricted ordinary rule; L(3) = 2 itself does not follow from it."

2. **LOW** (0.85), README.md:46. "proved by hand by both solvers" is too broad. Solver A's Lemma 1 covers only n=3 (solver-a/proofs.txt:6); the "3 or more" form is B's P1.
   Replace with: "…proved by hand by both solvers (the second for n = 3 by A, for every n ≥ 3 by B)…" and cite `solver-b/proofs.txt` P1, P2.

3. **LOW** (0.8), README.md:99-100. "could not fail (no rule there reaches the relevant case)" is true at s=3. At s=4, however, 511 pairs were tested; only the equal-time branch was never reached (referee.md:41).
   Replace with: "Solver A's test of its Lemma 2 is vacuous at three states and never reaches the equal-time case at four; the referee's five-state test (549,534 pairs, 320 equal-time) is a real one."

4. **LOW** (0.6), README.md:44. "three search orders" comes from referee.md:10, but referee.md:24 documents only two s=6 runs (ascending, and descending from n=9), and `verifier/o6_chunks.txt` is the descending run.
   Replace with "in two search orders (ascending, and descending from n = 9)", unless a third run is in the files.

5. **LOW** (0.9), README.md:87. "the surveys of small-state solutions (Correa, …)" should be singular, because prior-art.md:8 lists one survey.
   Replace with: "the survey of small-state solutions (Correa, Gustavo, Lemos, Settle, 2017)".

6. **LOW** (0.6), file provenance for C2. `verifier/run_ord.sh` runs `./ref3 ord …`, but there is no `ref3.cpp`. The `ord` mode is in `ref2.cpp` (ref2.cpp:143, usage "ref2 ord"), whose header comment (ref2.cpp:1-7) still describes ref.cpp's modes.
   Add to README Files: "`run_ord.sh` calls the binary built from `ref2.cpp` (named ref3 at run time)."
