<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review of a short mathematical note before it is published

Repository: latent-mining (public). File under review: `14-forest-orientation/NOTE.md`.
It is a self-contained short note restating results whose original proofs are in
`14-forest-orientation/solver-b/proofs.txt` (Lemmas 1-5) and
`14-forest-orientation/open-limit/proofs.txt` (Lemmas A, B, Type Lemma, Theorems 1-3),
with a Lean 4 formalisation in `14-forest-orientation/lean/` and an overview in
`14-forest-orientation/README.md`. Literature notes: `prior-art.md`, `prior-art-deep.md`
in the same folder.

The note was rewritten from those files by one model. Assume it introduced errors in the
rewriting. Read NOTE.md as a referee would read a submitted note: every proof step must
follow from what the NOTE itself says, not from the source files. Then compare with the
source files for anything dropped, strengthened or changed.

Q1. Section 2, Lemma 1 (simulation) and its proof: is every step valid as written in the
    note? Is the Corollary (shadow play) correctly stated and proved, in particular for an
    adaptive Adversary?
Q2. Section 3, Lemma 2 (one reply): check facts (a), (b), (c) and the three reply cases as
    written, including free replies at a vertex with out-degree 0 or 1, and that the slot
    values after the reply are what the note says.
Q3. Section 3, the definitions of A_k, R_k, index, F, the nesting claim, the bounds
    F(i) <= i/2 + 1/3 and F(i+1) - F(i) <= 2/3, and Lemma 3 with inequality (*).
Q4. Section 4, proof of Theorem 1: main phase, cleanup (the inequality for Phi(Y_r) and the
    telescoping to n/3 - F(i_m) - (2/3)(m-1)), the counting step, and the final constant.
    Recompute everything. Does the proof as written in the note prove the stated bound?
Q5. Section 5: the proof of Theorem 2; the superadditivity and limit claim; the statements
    about small cases; the paragraph "A limit of the method" (is any sentence stronger than
    Theorem 3 of open-limit/proofs.txt and its Section 8 support?).
Q6. Section 6: is every statement about the Lean development accurate against `lean/`
    (the theorem statement, what it implies, the claim that it first forces T(n) >= 1 at
    n = 26, the figures at n = 1000, the listed limits)? Recompute the numbers.
Q7. Summary and Section 8: does any sentence claim more than is proved or computed? Are the
    numerical statements (positive from n = 34; 0.558; 1/3 <= L <= 1) right and correctly
    qualified?
Q8. Section 7 (related work) and the provenance paragraph: you cannot check the literature
    from inside the repository, so judge only this: is each statement worded so that a reader
    can tell what was read and what was relayed, does it agree with prior-art.md and
    prior-art-deep.md, and does any sentence overclaim novelty? Would a specialist reading
    this note find anything in tone or framing that wastes their time or misleads them?

For each finding give NOTE.md line numbers and the replacement wording you would use.
