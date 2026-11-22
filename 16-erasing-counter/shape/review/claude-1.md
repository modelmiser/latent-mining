<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: experiment 16, shape-bound counterexample and Theorem Z

Method: traced every proof by hand; wrote my own simulator and random tester from the definition (outside the repository; none of its programs were run).

## Verdicts

**Q1. CONFIRMED.** T* is legal: paths 1-2-3, 1-2-3-4-5, 1-2, 1-3-6-7, 1-3; every leaf writes nothing or an index on its own path; depth 5 (`chain/3-question.txt:4-7`). By hand L = 5·(1·(4·2 + 1) + 1) = 50 and the orbit of zero is 4 rounds of 13 states = 52 (`shape/PROOFS.md:18-32`). My program: all three trees legal, depth 5, L = 50 / 98 / 602, orbit of the all-zero state a pure cycle of 52 / 114 / 762, and it is the only cycle of each tree. No contradiction with the depth ≤ 4 computation (the trees have depth 5; within the family depth 4 forces c_3 ≤ 2, giving N − L = −4 or less, `PROOFS.md:42`). D(5) ≥ W_5 is a construction and is untouched.

**Q2. CONFIRMED.** ℓ_1 = c_1 + 1, ℓ_2 = 1, ℓ_3 = c_3 in (L-spine) give L = (c_1 + 1)(2c_3 + 2); each x-round is 3c_1 + 1 states, c_3 rounds; N − L = c_1c_3 − 2c_1 − c_3 − 2 (`PROOFS.md:38-39`). Values 2, 16, 160 match (c_1, c_3) = (4,4), (6,6), (42,6).

**Q3. CONFIRMED.** Traced `PROOFS.md:53-66`: one 1 read and one spine bit written per step, data frozen, the maximum of B never leaves, peel c − 1 times. My test: 49,762 home-free cycles on arbitrary random trees and 27,859 on (H) trees, no violation of N ≤ k − c + 1; with the bound k − c it fails (702 hits), so the bound is tight and the check can fail.

**Q4. CONFIRMED.** S_i° is legal (a path of S_i° is a subsequence of a path of S_i; data nodes are never deleted, so a surviving write stays on its path). In mode i every spine node inside S_i is some z_j with j > i and reads 0, so the walk is that of S_i° (`:93-95`). (H) enters at `:98` only, and suffices because idx(T) \ Z is the union of the D_i. Recurrence: the mode sequence m_t is the same in every excursion because each mode has at most one jump state and one target (Theorem E, `:107-109`); a stage in a repeated mode is assigned to the first occurrence τ of its mode and (5) still applies, so the count misses nothing. Program, 485,059 home cycles of 147,992 (H) trees (up to 11 indices): the T-step equals g_i on x_i and fixes the rest, x_i stays on one g_i-cycle, at most one empty state per C_i, one jump state and target per mode, visited modes = chain, N ≤ 2C_1 + … + C_l with c_u the true length of the g-cycle: zero violations, 457,539 tight. With (H) dropped, each check fires; with 2C_1 replaced by C_1 it fails 83,822 times.

**Q5. CONFIRMED.** C_t ≤ w_1…w_(l−t+1) (distinct modes, w decreasing, all ≥ 1); the right side grows with l; W_r = Σ_(q≤k) w_1…w_q + w_1…w_k·W_(r−k) checked by induction from w_(j−1) = w_j(w_j + 1). The "more generally" clause rests only on M(S_i°) ≤ W_depth, which for depth ≤ 4 is the computation of [U]; stated so at `:156-158`. Membership: read-once ⊂ 𝒞 by `:149-151`; (a), (b), T* and both larger counterexamples are in 𝒞 and not read-once (hand and program).

**Q6. CONFIRMED with defects** (findings 1-5). Struck row values, plain-language paragraph ("three states": home, mode 2, mode 1; relay leaf has L = 1), and open case all follow from the files. All three cycles are checked by both programs (`out_cex.txt`, `out_checks.txt`) and `organiser_check_out.txt`.

## Findings

1. **MODERATE (minor), confidence high. `README.md:67`: "tested on 6,003 random trees of the class".** `shape/test_Z.py:38-61` builds trees satisfying (H) only; the S_i reuse data positions across their own branches, and membership in 𝒞 is never tested (in my similar generator about 5% fall outside). Three of the 6,003 are the fixed examples (`test_Z.py:191-193`), not random. What it tests is Lemma A, Theorem Z1 and the W_depth bound. Replacement: "tested (Lemma A, the counting bound of Theorem Z1, and the bound W_depth) on 6,000 random trees satisfying the top-level condition, with up to 11 positions, and on the three examples; this checks the bookkeeping and is not strong evidence".

2. **LOW, confidence high. `README.md:67`: "the two small examples below".** Only example (a) is on the page (`README.md:134`); (b) is only in `upper/PROOFS.md:159-164`. Replacement: "the two small examples of `upper/PROOFS.md` §5 (the first is shown below)".

3. **LOW, confidence medium. `README.md:67` and `:148-149`: "the same holds recursively inside them".** The recursion in `PROOFS.md:130-131` is on S_i°, the subtree after deleting the nodes that read path positions and emptying the leaves that write them, not on S_i itself; the spine of S_i° can differ from that of S_i. Replacement: "…and the same holds recursively inside each of them once the nodes reading path positions are removed".

4. **LOW, confidence medium. `README.md:65`: "for a few hours".** The conjecture row was committed at 15:52 (70f0c0d, on origin/main); the refutation files are dated 16:53. Replacement: "for about an hour" or the actual interval at publication.

5. **LOW, confidence high. `README.md:107-108`: stale section head.** "This section follows `upper/PROOFS.md`, the work of one follow-up instance" now also covers the paragraph from `shape/PROOFS.md`. Replacement: add "; its last paragraph follows `shape/PROOFS.md`". Nothing else on the page is stale: `:137` and `:245` (a reviewer found no cycle longer than L on 8,000 random trees) remain true as history.

No CRITICAL finding. I found no gap in Lemma A, Theorem Z1 or Theorem Z.

Limits: my random trees are small (longest cycle 34), so the program checks the proof's bookkeeping, not the W_r bound at scale. One procedural note: another process overwrote a file in the scratch folder I first used; every figure above is from a rerun in a private folder.
