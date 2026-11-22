<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. The definition of an encoding is at NOTE.md:48-51 and the SCFO condition at :53-54. The model of [2] at :56-60 is the special case with two complementary cards per bit; a known permutation keeps each card tied to one bit (`prior-art-deep.md`:10). The two conditions are equivalent. If f is not constant, take z_b = s(x) for any x with f(x) = b. If f is constant, the unused z can be any word not cyclically equal to the rows. — Replacement: none.

Q2: CONFIRMED. I re-derived every step at :67-136 and none fails. (∗) and (∗∗) follow by substitution. A unit pair with a given non-zero sum is determined, since uv = c/c̄. (X) is impossible for odd m. The exceptional pair satisfies m/2 − θᵢ − θₖ ≡ −2θₖ, and the formula is symmetric because 2·(m/2) ≡ 0. The formula for r·t at :115 is exact. In case B, r·t ≡ m/2. The frequencies where Ŵ(r) = 0 are handled. Both W′ and the row s(c) lie in the cube after the substitution at :132-136. The parenthetical at :72-74 holds: the proof only uses W − 2aᵢ = z^(sᵢ)W and W − 2aᵢ − 2aₖ = z^(sᵢₖ)W. — Replacement: none.

Q3: CONFIRMED. The proof at :140-145 is valid, because fixing variables turns their cards into additional cards. Checked by hand:
- AND with the others fixed to 1 and OR with the others fixed to 0 each give 7:1 restrictions.
- Equality with x₄ = … = xₙ = 0 is 1 only at 000.
- Majority of five with two bits fixed to 1 becomes OR of three.

Both "does not settle" items (:168-175) are stated correctly. The reformulation at :147-148 is correct, but "the negation of one" can be misread as "the negation of one literal". — Replacement: "…is, up to negating inputs, x∧y∧z or ¬(x∧y∧z)".

Q4: CONFIRMED. I re-derived the inversion formula at :198 (Σ_S Π(1+t_q u_q) = 2^{|R|}[t=u]). I also re-derived B_v = c[v=0] + 2^{−n}δ(−1)^{|v|}, and the count holds. The further facts at :216-220 match T5, T4 (|R| ≥ n+1, ≥5 for n=3), C3 and L1 in `proofs.txt`:16-20, :66-68, :88-109 and :119-123. The three strings at :233-235 match `w_n3_m36.txt`:4-6 character by character. The format matches `check.py` (1 = heart, owner 0 = additional card, u0 = u1 there). My own recomputation gives: privacy true, 33 distinct words, 16 for input 111, none shared. Each player owns an even number of cards, half hearts (12/6, 10/5, 6/3). — Replacement: none.

Q5: REFUTED, in part. :263-264 says [3] shows that "for three-variable functions no new protocols exist" with one helper colour. The repository supports this only for AND (`prior-art-deep.md`:17). :272-273 needs the condition n ≥ 3. The claims at :151 and :154 ("This answers…") are not conditioned on the proof being correct, and neither is the title. — Replacement: "…show by integer programming that three-bit AND (and, they report, other classes) has no such protocol…"; and "If Lemma 1 is correct, this answers…".

Q6: UNSUPPORTED, in part. Neither the five reviewers from four vendors (:287, :300, :309) nor the 1,011,693-case test (:297-298) is backed by any report or log in the folder; grep finds them only in NOTE/README. The same goes for the [4] truth-table check (:161-164). The rest matches: `referee.md`:16-19 and :34, :41 and :64; `proofs.txt`:64; and `prior-art-deep.md`:35. "Adds the remarks on m even" (:288) overstates, since T2 already has "(m even)" at `proofs.txt`:51. The provenance is clear but sits at :4-5 and :32-33 rather than in the title or the opening sentence. — Replacement: add the logs, or write "(reviews not archived here)".

Q7: REFUTED, in part. Several terms are used before they are defined or never defined:
- "the same experiment" (:35) and "organising instance" (:285)
- "cyclically equalizable" (:170)
- "layouts" and "normalisation" (:223-225)
- "solver B's T1" (:194)

"Exhaustive search" (:38) hides that the search is pruned. — Replacement: define each term on first use, and write "a pruned exhaustive search" at :38.

F: MODERATE — NOTE.md:263-264 — The note attributes to [3] a result for all three-variable functions; the repository supports it only for AND — medium — restrict it to AND, or quote [3].
F: MODERATE — NOTE.md:286-287,300,309 — "Five reviewing models from four vendors confirmed": no reports are in the folder — high — archive the reviews or drop the claim.
F: MODERATE — NOTE.md:296-298 — There is no code or log for the 1,011,693-case test — high — add the script and its output.
F: LOW — NOTE.md:1,151,154 — The title and "This answers" are unconditional for an unrefereed AI proof — medium — add "if Lemma 1 holds" or "we claim".
F: LOW — NOTE.md:161-164 — The [4] consistency check has no artifact — medium — add it.
F: LOW — NOTE.md:272-273 — "Cannot be a random cut" lacks n ≥ 3 and "uniform" — low — add both.
F: LOW — NOTE.md:288 — The m-even remark is already in T2 (`proofs.txt`:51) — high — write "expands the remark on m even".
F: LOW — NOTE.md:147-148 — "the negation of one" is ambiguous — medium — use the wording from Q3.
F: LOW — NOTE.md:35,170,194,223-225,285 — Terms are used before they are defined — high — define them on first use.
F: LOW — NOTE.md:38 — "Exhaustive" without "pruned" — medium — write "pruned exhaustive".
F: LOW — NOTE.md:108-109 — The without-loss-of-generality relabelling is implicit — low — add "relabel so the shared index is 1".
F: LOW — NOTE.md:3-5 — The provenance warning is not in the opening sentence — low — put "AI-generated, unrefereed" in the subtitle.

Points the questions did not ask about:
1. `proofs.txt`:64 says no 7-of-8 cube of rotations exists at all for m ≤ 18 (also stated at :291-293). A specialist will want the open question raised plainly: if no such configuration exists for any m, Lemma 1 holds vacuously and the sub-question of [2] at :168-172 would have the answer "no". The note should say outright that this is open.
2. Proposition 3, T1 and T2 also hold when correctness is weakened to M(1) ≠ M(0) (`proofs.txt`:117). The note does not mention this, and it strengthens Section 4.
3. Lemma 1 extends to cards of more than two kinds by writing each card as a vector, since the proof uses only the affine form. The note could say so. This is optional.
