<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — Every step of Lemma 1 holds (NOTE.md:74-90; matches solver-b/proofs.txt:24-34). The Corollary's shadow construction works against an adaptive Adversary, because L is built round by round (NOTE:95-101, the same as reports/open-limit-referee.md:16). One gap: the note never checks that L's path flip is a legal position (solver-b:32 does). — Insert after NOTE:88 "(legal: inner vertices keep out-degree 2, w_d gains one, x swaps x→w₁ for x→y)".

Q2: CONFIRMED — Facts (a), (b), (c), the free reply at out-degree 0 or 1 (P = −1 = p, the other slot keeps Q), and the path cases all check (NOTE:128-152). The note drops the clause "in both cases y still has type (p′,q′)" (open-limit:59). Section 5 relies on that clause. — Add to Lemma 2 "and y keeps type (p′,q′)", with (c) widened to "the slot values of y do not change".

Q3: CONFIRMED — The nesting holds: A_k ⊇ R_k ⊇ A_{k+1}. F(2k+1) = i/2 + 1/6 ≤ i/2 + 1/3, and the steps between consecutive F values are 1/3 and 2/3. The four case computations in Lemma 3 are right (NOTE:156-174). — none

Q4: CONFIRMED — I recomputed it all. Lowering Y_(r+1)'s index from b_(r+1) to i_r gives line 206. The telescoping uses Σ_{r≤m}F(b_r) − Σ_{r<m}F(i_r) = F(i_m) + Σ_{r<m}(F(b_r) − F(i_r)). Then 2^(⌊i_m/2⌋+1) − 1 ≤ n gives i_m ≤ 2log₂(n+1) − 1, and −(7/6)(2log − 1) − 1/3 = −(7/3)log + 5/6 (NOTE:199-218). — none

Q5: REFUTED (partly) — The proof of Theorem 2 is right. Three problems:
- **Superadditivity (NOTE:242-244):** it leaves out why the Algorithm's flips in the first tree during the second phase do not help it (solver-b:52-54 gives the reason).
- **"Every Adversary strategy that tracks one witness…" (NOTE:262-263):** this is wider than N3 at open-limit:7-9, which limits the result to the pair game as defined.
- **"does not give an Algorithm" (NOTE:268-271):** open-limit Section 8 (lines 155-164) shows only that one natural potential inequality fails. NOTE:271 also says "the inequality" without ever defining G or the inequality.
— Replace with: "no Adversary strategy of this pair game forces more than (n−1)/3"; and "the natural Algorithm that minimises cost + G, with G(C) the maximum of g over C, cannot be analysed this way: for a 9-vertex tree … every normal reply gives cost + G(new) = 10/3 > 8/3".

Q6: CONFIRMED (one LOW) — The statement matches FO/Main.lean:9. Spec.lean is 89 lines; the toolchain is v4.36.0 with no dependencies; AXIOMS.txt agrees. The first n with n + 3 − 7⌊log₂(n+1)⌋ > 0 is 26. At n = 1000: ⌈940/3⌉ = 314, and ⌊1000 − log₂1001⌋ = 990. The .lean files total 1,973 lines, so "about 1,900" undercounts slightly. — "about 2,000 lines"; and add "(but not at n = 31, 32)" after NOTE:298.

Q7: REFUTED — Open question 2 (NOTE:390-392) says that by Section 5 a proof of L = 1/3 "would need an Algorithm whose analysis sees more than one vertex per tree". Nothing proves that: the source shows only that one potential fails (open-limit:154-164). The other numbers check: positive from n = 34 (recomputed), the maximum of g(n)/n is 0.5579 (I ran gdp.py), and 1/3 ≤ L ≤ 1 holds. — Replace with "the one-witness potential of Section 5 does not transfer directly to real forests; a disproof would need an Adversary outside the pair game."

Q8: REFUTED — The note's citations do not agree with the prior-art files. Details in the F lines below.

F: MODERATE — NOTE:317-319 — Gupta's survey slides [8] appear nowhere in prior-art.md or prior-art-deep.md, so the claim "we read this in slides" cannot be traced. — high — Either add the source to prior-art-deep.md or cite only the derivation from Brodal–Fagerberg Lemma 1, with its Δ = 2δ caveat (prior-art.md:21).
F: MODERATE — NOTE:327-328 — "Bender et al. obtain O(1) amortized … out-degree 3" contradicts prior-art.md:9, which records O(log log n) per insertion w.h.p. — high — Use the prior-art wording.
F: MODERATE — NOTE:349-351 — "we know those results only through [6]" is false for Chaudhuri et al., which was read directly (prior-art-deep.md:14, 27). — high — "Chaudhuri et al. read (introduction); Grove et al. only through [6] and Chaudhuri et al."
F: MODERATE — NOTE:390-392 — Overclaim, see Q7. — high — Use the Q7 wording.
F: MODERATE — NOTE:262-271 — The wider claim and the undefined inequality, see Q5. — medium — Use the Q5 wording.
F: MODERATE — NOTE:119-126 — Lemma 2 leaves out "y keeps its type", but the pair game in Section 5 depends on it. — high — Add the clause.
F: LOW — NOTE:344 — "(α = 1)" sits inside the quotation marks but is not in the quote as recorded (prior-art-deep.md:26). — high — Move it outside the quote.
F: LOW — NOTE:338-339 — "[3] … with insertions and deletions" is not supported by the prior-art files. — medium — Remove it or source it.
F: LOW — NOTE:333, 352-353 — [9] is listed under "statements we read", but it was only keyword-scanned (deep:15), and its bound is Ω(1/Δ), not a constant. — high — "scanned only: an Ω(1/Δ) amortized lower bound…"
F: LOW — NOTE:310-311 — The note promises to say, for each statement, whether the source was read or relayed, but most items ([2], [3], [4], [5]) don't say. — high — Tag each item "(read: §…)" or "(scanned)".
F: LOW — NOTE:309 — "the same AI instances" is wrong; the searches were done by separate instances of the same model. — medium — "instances of the same model".
F: LOW — NOTE:242-244 — The superadditivity argument is missing the restriction step. — high — Add "payments restricted to B are at most real payments".
F: LOW — NOTE:81-88 — Legality of the path flip is never argued. — high — See Q1.
F: LOW — NOTE:232-233 — f is "defined" with ≤. — high — "f(n) = max …".
F: LOW — NOTE:276 — Line count; see Q6. — high — "about 2,000".
F: LOW — NOTE:298 — Not stated that the bound forces nothing at n = 31, 32. — high — Add it.
F: LOW — NOTE:251 — The certificates rely on Lemma 1 (normal-form replay, reports/solver-b.md:32), but the note doesn't say so. — medium — Add "(against normal replies; valid by Lemma 1)".

**Not covered by the questions:** the Brodal–Fagerberg 3(n−1) derivation drops the Δ = 2δ boundary caveat (prior-art.md:21; deep:33). The note also never says that its framing of Lemma 1 for a fixed sequence fixes the overstatement recorded as N1 in solver-b. That change is correct, but a reader comparing the note with solver-b:22 should be told it was deliberate.
