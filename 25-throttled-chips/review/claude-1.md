<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of a Claude seat run on the plan login (one of the two on Claude Opus). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Claude seat 1: experiment 25 (throttled chips)

**The mathematics holds: I found no gap in C1, C2, C3 or the Lemma Z reduction. The defects are in the README: two sentences claim more than is proved, one link is dead, and two attributions have no source.**

Paths are relative to `25-throttled-chips/`. I did the algebra by hand and simulated the Theorem 2 strategy with my own code. I did not read the search programs, so C4's lower bounds rest on the logs.

## Verdicts

- **Q1 CONFIRMED.** `solver-a/proofs.txt:6-12`: an add at 0 leaves Q unchanged; a firing adds (x-1)²+(x+1)²-2x² = 2; at most m-1 firings precede step (1) of round m. x² ≥ 9-w_x holds at 0, ±1, ±2 with equality, so Q ≥ 9m-35H, and 7m ≤ 35H-2 gives m ≤ 5H-1. `solver-b/proofs.txt:36-43`: with m = 5h-6, Q ≤ 2(m-2) = 10h-16 and the filling gives 4m-10(h-1) = 10h-14. Both bounds concern heights after step (1), which the win condition allows.
- **Q2 CONFIRMED.** `solver-a/proofs.txt:2,25-29`: (*) is chip bookkeeping; the expansion of k-2T+T+T gives coefficient 1/5 on each of x-2..x+2; (**) follows by substituting F = T-l; (a) and (c) are right.
- **Q3 CONFIRMED.** `solver-a/proofs.txt:30-41`.
  - (d): at any vertex of maximal lag, height ≥ η* ≥ M/3 ≥ 2 when M ≥ 6. Every max-lag vertex is then a candidate, so the fired vertex has lag exactly M whatever the tie-break.
  - (e): g(u+d)-g(u) ≤ d·g'(u+d) ≤ 2d·max(M-6,0) with M the post-add maximum. The decrease is 2(M-6)-1 for M ≥ 7 and (M-6)² for 6 ≤ M < 7; the net is ≤ 1 in both. The firer passes only when M < 6, where Θ = 0 after the add.
  - (f): after the add, height ≤ t/5 + 2(6.6+√(t-1)); after the firing, ≤ t/5 + 2(6+√t). Both are ≤ t/5 + 2√t + 13.2.
- **Q4 CONFIRMED.** The rule uses only past adds, fires only ready vertices, at most one per round. If m ≤ 5h-10√(5h)-72 then m/5 ≤ h-2√(5h)-14.4 and 2√m ≤ 2√(5h), so the sum is ≤ h-1.2. The bound holds for every h (vacuous below h ≈ 45); the limit 5 follows with C1.
- **Q5: could not break it.** My simulation asserted (**), lag ≥ 0, and readiness of max-lag vertices at every step. Lists tried: zeros; alternating sites 1, 2, 3 and 100 apart; cycles 0,2,4 and -1,1,0,0; blocks; spread-then-burst; far singles then a burst; 300 random lists of length 400; small beam searches for lag and for height on widths 3, 5, 7. Largest lag 2.4, largest height minus t/5 3.2, a max-lag vertex unready only at lag ≤ 1.0. This matches `reports/referee.md:39`. Lags ≥ 6 never occurred, so this does not exercise (d) or (e); those rest on the algebra.
- **Q6 CONFIRMED, conditional on the searches.**
  - P2 (`solver-b/proofs.txt:20-33`) is a proof: neighbours of v never fire, k ≤ 1+⌊(c(v)-h+1)/2⌋, neighbour height ≤ h-1/2.
  - Lemma Z (`solver-b/proofs.txt:83-112`): (i) a Z-line is a line; (ii) the superposition works because Z-lines keep each sublist's chips inside disjoint final zones, and the composite is a Z-line; (iii) the induction on a shortest Z-winning list is sound. The Γ prefix test is a valid necessary condition.
  - G(4) = 10 and G(5) = 15 follow from `reports/referee.md:48` and `verifier/out_Z_h5_m14.txt`, with the hash caveat.
  - README:45 on G(6) matches `reports/referee.md:14-15` and `reports/solver-b.md:14`.
- **Q7 REFUTED in part.** The two explanatory sections match the proofs, including "cheapest arrangement" (10H + 9(m-5H) = 9m-35H), "a third of its lag" (true for a max-lag vertex, as the sentence says) and the square-root remark. The Short answer overclaims (F1, F2).
- **Q8 REFUTED in part.** Numbers and counts match their sources. Exceptions: F3, F4, F5. "The game" matches the formal text, which is in `chain/3-link.md:7-14`. The top-level row inherits F2.

## Findings

**F1. MODERATE, confidence high. README:15-16, "Mixing helps only at h = 3."** Proved only for h ≤ 5. G(6) lies between 15 (16 by B) and 20 with C(6) = 20, and nothing is known beyond. README:45 and README:47 contradict it.
Replacement: "Mixing helps at h = 3 and not at h = 4 or 5; whether it helps for any larger h is open."

**F2. MODERATE, confidence high. README:9-10 and top-level `README.md:55`, "for large h no list does better than dropping every chip on the same point" / "as good as anything".** The proofs give G(h) ≥ 5h - O(√h) against C(h) ≤ 5h-6: a gap of order √h remains.
Replacement: "for large h no list beats dropping every chip on the same point by more than about 10√(5h) rounds, so the ratio to 5h tends to 1."

**F3. LOW, confidence high. README:32 links `chain/3-question.txt`, which does not exist** (the folder has `1-question.txt` and `2-question.txt` only).
Replacement: "formal text in the FINAL QUESTION section of chain/3-link.md", or add the file.

**F4. LOW, confidence high. README:146-147, "Solver B and the referee each reported a third process overlapping briefly."** Only `reports/solver-b.md:77` says this; `reports/referee.md` has no such statement. README:144-146 (solver A's wrong-directory files) is also in no file in the folder.
Replacement: "Solver B reported …", and for the others either cite a source or mark "(from the session logs, not in the reports)".

**F5. LOW, confidence high. README:142-143, "solver A reports 5h − 8 for zeros alone."** `reports/solver-a.md:47` and `solver-a/out_ALT.txt` give computed values 1, 4, 7, 12, …, 32 for h = 1..8, fitting 5h-8 from h = 3; it is not a general statement.
Replacement: "solver A computes 5h − 8 for zeros alone for 3 ≤ h ≤ 8".

**F6. LOW, confidence medium. README:40 "every short list", README:108-109 "minimal winning lists", README:119 "never occur in play".** The sources say: all lists inside windows of 4 to 7 vertices (`reports/referee.md:37`); minimal Z-winning lists inside a window (`:47`); never occurred in the tests (`:40`).
Replacement: "every short list on 4 to 7 neighbouring points"; "minimal lists winning against the restricted firer"; "never occurred in any test".
