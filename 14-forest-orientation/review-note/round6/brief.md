<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Round 6: Theorems 3 and 4 as now written in the note, and the text around them

Since round 5 the working tree of the repository has changed (uncommitted; read the files as they are on disk):
`14-forest-orientation/NOTE.md` now contains Theorem 3 (T(n) <= max(0, floor((n-3)/2))) with Lemmas 4 and 5,
and a NEW Theorem 4 with a construction (the "ternary family") that no reviewer has seen. New programs are in
`14-forest-orientation/upper/`. `14-forest-orientation/README.md` and the top-level `README.md` were updated.
`dispositions.md` (shown to you) records what round 5 found.

Assume the author is competent and wrong until you have traced each step yourself. Read NOTE.md Section 1 for
the definitions and Section 5 from "A potential for the rule of [7]" to "The limit exists".

Q1. Theorem 4 (NOTE.md, "Proof of Theorem 4"). Is the construction a legal play against rule R as defined in
    the note, and is the proof correct? Check: (a) the invariant I(j) and its base case; (b) in the first round
    {x, y}: that every shortest path from x has length j-1, the out-neighbours of x afterwards, and h(x)
    afterwards; (c) in the second round: that h(x) = h(z), that every shortest path goes through c, the cost,
    and that I(j) then holds; (d) that the flipped vertices of X cannot disturb the out-tree of the new root;
    (e) the recurrence c_j = 3 c_(j-1) + 2(j-1) and its closed form; (f) the count of vertices 3^j and H = j.

Q2. "The worst case of rule R" paragraph. Are these correct as stated: G(n) <= max(0, floor((n-3)/2));
    G(a+b) >= G(a) + G(b); G(n) >= n/2 - (J+1)^2 with J = floor(log_3 n); the description of where ties are
    used; and the sentence about exhaustive search for n <= 14 (compare `upper/greedy_exact.cpp`: does that
    program compute the worst case of rule R with ties to the Adversary)?

Q3. Lemma 4 and Lemma 5 as now written in NOTE.md (round 5 saw a draft). Re-check every case, in particular
    the k = 0 case, the rewritten "No h decreases" induction, the w_0 cases, and Lemma 5's two-neighbour case
    with the quantity E. Is the counterexample given for the smaller-tree rule correct (compute it)?

Q4. Scope and consistency of the surrounding text. (a) Summary: are Theorems 3 and 4 stated correctly, and is
    "an upper bound on L below 1/2 needs a different algorithm" justified given that Theorem 4 uses adversarial
    tie-breaking? Give the wording you would use. (b) Does any sentence in NOTE.md, 14-forest-orientation/README.md
    or the top-level README.md still attribute the (n-3)/2 bound to the smaller-tree rule, or still give
    1/3 <= L <= 9/16 as the current state? (c) "The two bounds compared": does `upper/compare_bounds.py`
    establish exactly what the paragraph says?

Q5. The new programs: `upper/lemma4_check.py`, `upper/lemma5_check.py`, `upper/ternary_family.py`,
    `upper/pool_search.cpp`, `upper/compare_bounds.py`. Does each do what NOTE.md and README.md say it does?
    In particular: does ternary_family.py really play rule R (legal endpoint, shortest path) at every round;
    does pool_search.cpp test Lemma 4 on real rule-R rounds; can any of them report success after a failure?

Q6. Is Theorem 3 or Theorem 4 contradicted by anything else in the repository (exact values, Theorem 1, the
    table of f, the README's results table)? Try to break Theorem 4 by replaying j = 2 (9 vertices, cost 2)
    and j = 3 (27 vertices, cost 10) by hand or by your own reasoning.

For each question give CONFIRMED / REFUTED / UNSUPPORTED with file:line evidence and the replacement wording
you would use where something is wrong or unclear.
