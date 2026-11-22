<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — The disagreement path, invariant, and adaptive shadow construction are valid (`14-forest-orientation/NOTE.md:64-101`; `solver-b/proofs.txt:18-35`) — none

Q2: CONFIRMED — Locality covers both empty-slot cases, preserves the untouched slot and h(y), and yields exactly X1/X2 (`14-forest-orientation/NOTE.md:118-153`; `open-limit/proofs.txt:40-65`) — none

Q3: CONFIRMED — Nesting, both F bounds, and all A_k/R_k alternatives recompute correctly (`14-forest-orientation/NOTE.md:155-175`; `open-limit/proofs.txt:67-78`) — none

Q4: CONFIRMED — The cleanup recurrence telescopes as claimed; m≤i_m+1 gives the coefficient 7/6 and final constant 5/6 (`14-forest-orientation/NOTE.md:180-221`; `open-limit/proofs.txt:82-97`) — none

Q5: REFUTED — The upper bound and limit are valid, but “needs a different invariant” and “same potential does not give an Algorithm” exceed Theorem 3 and the single tested extension G(C)=max_v g(slots(v)) (`14-forest-orientation/NOTE.md:266-271`; `open-limit/proofs.txt:124-143,154-164`) — “This pair game cannot prove L>1/3; the natural max-over-vertices extension of g also fails.”

Q6: CONFIRMED — The theorem and specification match Lean (`lean/FO/Main.lean:9-10`; `lean/FO/Spec.lean:11-87`); the bound first forces 1 at n=26 and gives 314≤T(1000)≤990, while the axioms match `lean/AXIOMS.txt:1-15` (`14-forest-orientation/NOTE.md:274-306`) — none

Q7: REFUTED — The numerical claims 34, 0.558, and 1/3≤L≤1 check out, but Section 8 unjustifiably says both directions require tracking more than one vertex (`14-forest-orientation/NOTE.md:386-392`; `open-limit/proofs.txt:124-128,154-164`) — “The specified pair game cannot disprove L=1/3, and one tested one-vertex upper potential fails.”

Q8: REFUTED — The section is generally careful, but its promise to label every source as read or relayed (`14-forest-orientation/NOTE.md:309-312`) is not met by the unqualified Bender claim at `NOTE.md:327-329`, which is not documented in the repository’s summary (`prior-art.md:8`) — “We read [2] as claiming …” or explicitly “This was relayed by …”

F: MODERATE — `14-forest-orientation/NOTE.md:258-268` — the purported self-contained method barrier omits the four-option inequality proving the potential invariant — confidence: high — include the proof or label it as an external supporting result

F: MODERATE — `14-forest-orientation/NOTE.md:266-268` — Theorem 3 rules out only the defined pair game, not every proof using the same invariant — confidence: high — narrow the conclusion to that game

F: MODERATE — `14-forest-orientation/NOTE.md:268-271` — “the inequality” and the real-forest potential G are undefined, and one failed extension does not rule out all Algorithms — confidence: high — define G and state only that this extension fails

F: MODERATE — `14-forest-orientation/NOTE.md:390-392` — the asserted necessity of seeing multiple vertices in both directions is unproved — confidence: high — replace with the narrower pair-game limitation

F: LOW — `14-forest-orientation/NOTE.md:250-253` — the T(64) certificate is omitted from the repository (`README.md:247-249`), so its reported verification is not reproducible here — confidence: high — disclose that omission

F: LOW — `14-forest-orientation/NOTE.md:309-329` — read-versus-relayed provenance is not consistently stated — confidence: high — mark each paragraph explicitly

F: LOW — `14-forest-orientation/NOTE.md:1` — the title literally says n/3 although the finite bound is n/3−O(log n) — confidence: medium — use “An asymptotic n/3 lower bound”

F: LOW — `14-forest-orientation/NOTE.md:28-29` — “contributes the constant” sounds like a novelty claim despite the acknowledged unread likely source (`NOTE.md:355-360`) — confidence: high — say “Our search did not find this constant.”
