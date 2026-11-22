<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — Each card flips sign when its owner's bit changes, so a_i = W on the positions where player i's two words differ, and a_i = 0 on positions where they agree and on house cards. The supports are disjoint, so row(x) is linear in x (solver-b/proofs.txt:42-43). With R = Z_m, M(x) is the multiset of rotations of row(x), so equal M means "rotation of" (README:80-82). If row(1,1,1) is a rotation, then M(1ⁿ) = M(0,0,0,1,…) = M(0ⁿ), which breaks correctness. Nothing else is needed: the argument works for any real vector and any m. Only disjoint supports, n ≥ 3, and the other players fixed to **1** matter. README:74 leaves out "to 1" — replacement: "Fix all players but three, holding 1".

Q2: CONFIRMED — I re-derived every step. (∗) and row(111) follow by substitution (README:89-90). The Fourier convention is consistent (proofs.txt:47). Two unit vectors with a given non-zero sum are determined as a set (they are mirror images across the sum). Zero sums need m even, and for odd m the case is empty; README:103 does not say so, proofs.txt:51 does. r(s_ik−s_i−s_k) = m/2 − θ_i − θ_k = −2θ_k holds, and 2θ_i ≡ 2θ_k. Any two of the three pairs share an index, so the (1,2),(1,3) check covers every choice. Case A: rt = 2Σθ − Σθ. Case B: θ_i = 0 is forced by the two non-exceptional pairs, and rt = m/2, so ω^{rt} = −1. t does not depend on r, and both sides vanish where Ŵ(r) = 0, so inverse DFT gives row(111) = z^t W. Any fixed choice of the s's works when W has a period.

Q3: CONFIRMED — I ran a brute force over all W ∈ {0,1}^m and all labellings (none/1/2/3) for m = 3..7, with any corner as the odd one. It found 0 cases where seven rows are rotations of one word and the eighth is not. With every player non-empty and balanced, no configuration exists at m = 6, 7, 8 where even all eight rows are rotations, which matches solver-b/fullR.out. Degenerate cases (a_i = 0, constant W) are covered by the proof.

Q4: CONFIRMED — Swapping x_i for 1−x_i gives W′ = W−2a_i and a_i′ = −a_i. The supports stay disjoint, and the proof never used a_i = W on its support. For equality with n ≥ 4, fix x4…xn = 0: only 0…0 outputs 1, so seven rows with output 0 must be rotations of one word, which forces the eighth. Three-bit equality has two output-1 inputs (000 and 111), so only six output-0 rows: not covered. The README never says this explicitly. README:135 fixes only x₄ — replacement: "fix x₄ = … = xₙ = 0".

Q5: CONFIRMED — vcheck.py:23-41 builds M from the definition (t_q = w_{q−j}). It tests all seven AND-0 inputs against M(0) and checks set-disjointness against M(1). I ran it: "valid". Player sizes are 12/10/6 positions plus 8 house positions; house cards satisfy u0 = u1. Every strand s+6k contains all three players, and R = 6ℤ reads one strand per shift (L5, solver-a/proofs.txt:35-41).

Q6: CONFIRMED — T1 (proofs.txt:22-34) is a proof; I re-derived the formula for B_v. T4 (88-109) is a proof; I re-derived both sign systems (a+2T > 0 together with 3a+4T < 0, and s(A+B1) > 0 together with < 0). C3 and T5 also hold. On the searches, the README matches referee.md:17 and :64.

Q7: CONFIRMED with exceptions — The 2816/2200 figures check out (9·256+512; 2·9·100+4·100). The weak-reading 12 matches solver-a.md:66 and referee.md:18. The "three programs" for m ≤ 4 holds, and so does the 55%. The novelty claims match prior-art-deep.md:113-125. Exceptions are listed below.

Q8: CONFIRMED — Top-level README.md:47 matches the folder. The README protocol section omits the shift direction, but M(x) does not depend on it because j is uniform. M(x), both validity conditions and the non-emptiness rules match chain/3-question.txt.

F: MODERATE — README:181-183 — The text suggests that m ≤ 13 is protected by an independent recomputation. referee.md:64 says "All three exhaustive programs use the same normalisation", so the referee's red.cpp shares it too. — confidence: medium — Fix: "The referee's recomputation to 13 cards uses the same normalisation as the solvers."

F: MODERATE — README:67 — The claim cell states "36 is the least size" as a fact, but cases (7,5), (11..12, partial R) and (13..17, 2) were not searched (solver-a.md:27, referee.md:74). — confidence: high — Fix: "Within the strand-family cases searched, none is below 36; some cases were not searched."

F: LOW — README:74 — "Fix all players but three" does not say the others hold 1. If they hold 0, all eight inputs are AND-0 and there is no contradiction. — confidence: high — Fix: add "holding 1".

F: LOW — README:103 — The zero-sum case does not say that it needs m even and is empty for m odd. — confidence: high — Fix: add "(possible only for m even)".

F: LOW — README:107-108 — Only the pairs (1,2),(1,3) are checked, with no symmetry remark. — confidence: high — Fix: "any two of the three pairs share an index; by symmetry take (1,2),(1,3)".

F: LOW — README:135-138 — For n > 4 the extra bits must also be fixed. The README never says that three-bit equality is not covered. — confidence: high — Fix: "fix x₄…xₙ = 0; three-bit equality (two inputs with output 1) is not covered."

F: LOW — README:11-14, 49-51, 142-145 — Statements about the outside papers are asserted as fact here. Hedging and attribution appear only at lines 47 and 126/175. — confidence: medium — Fix: prefix with "according to the deep-read instance".

F: LOW — README:61 — The Where column for m = 14 omits solver-a/fast_3_14_p*.txt, solver-b/fast2.out and m14w.part*.out. — confidence: high — Fix: list them.

F: LOW — README:184-185 — The validation list omits solver B's leaf-count cross-check (leafcount.py, solver-b.md:94). — confidence: low — Fix: mention it.

Not asked:
- The claim that the organiser read the proof and checked the paper wording (README:57, 177) cannot be checked from the files.
- The README calls the m ≤ 13 result "exhaustive", but all three programs are pruned searches. Their normalisation rests on lemmas L0/L1 and the symmetries, all of which I found sound, so the residual risk is low.
