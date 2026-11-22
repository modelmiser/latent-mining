<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Round 7: a claimed proof that T(n) <= (n-1)/3 (which would settle L = 1/3)

The repository's note (`14-forest-orientation/NOTE.md`) proves 1/3 <= L <= 1/2. Attachment A is a NEW draft, not in
the repository, claiming T(n) <= floor((n-1)/3) for a tie-broken version ("rule L") of the Gupta-Kumar-Stein rule,
by a potential with three lemmas. Attachment B is the program the draft cites for its finite cases.

This would close the main open question of the note, so treat it with suspicion. The author is the only reader so
far, and two earlier candidates for this proof were wrong (the second failed on a 43-vertex example with a vertex
of in-degree 4 that no test had contained). Assume the draft is wrong until you have traced every step yourself.
Do not rely on the program's verdict: recompute cases by hand or with your own reasoning. Read NOTE.md Sections 1
and 5 (rule R, Lemma 4 and its Step 1, Theorem 4) for the definitions used.

Q1. Lemma 6 (the bound Phi <= n - max gamma on every tree). Is the induction correct? Check: the formula with the
    maximum really bounds w(r) as used (including g(r) >= gamma(r)); deleting a vertex of in-degree 0 changes no
    other weight; both cases; the base case; and that the weight table equals the formula with the maximum
    (verify at least: one out-neighbour of height 0 and of height >= 1; two with q = p, q = p+1, q >= p+2).

Q2. Lemma 7 (monotone). Are (S1) to (S4) all the ways the pair (h, eps) of an out-neighbour can change when one of
    its slots rises by one? Is every line of the case analysis right? Does "monotone in single-slot increases"
    give what Theorem 5 needs, given that in a round a vertex's slots may rise by more than one and both slots of
    an out-neighbour may rise?

Q3. Lemma 8 (the path pays). Check: (i) the description of the round (new out-neighbours, new heights, o_i,
    when new and old slots are equal); (ii) the old-weight and new-weight formulas in (a) and (b); (iii) the
    carry pi and that (a) + (b) + (c) sum to 3k, including k = 1 (no interior vertex) and the role of f in the
    state when the next vertex is the end w_k; (iv) the reductions to finitely many cases; (v) where rule L is used
    and whether the stated consequence ("beta = 0 and eps_y = 0 imply alpha = 0") is all that the rule gives and
    is correctly derived; (vi) cases (c) and (d). Recompute by hand at least ten cases of (a), (b), (c) of your
    choice, including some with pi = -1 on either side, and report them.

Q4. Theorem 5. Does the proof account for every vertex whose weight can change in a round: path vertices,
    vertices off the path in the same tree (any number of them, any in-degree), vertices of the other tree?
    Is "no h decreases" really available for rule L from Lemma 4's Step 1? Are slots of path vertices
    non-decreasing as claimed? Is rule L always executable (is h(x) <= h(y) guaranteed; is the path flip legal)?
    Does the conclusion T(n) <= floor((n-1)/3) follow, and "T(n) = n/3 - Theta(log n)" with Theorem 1?

Q5. Attachment B. Does the program model Lemma 8 faithfully (heights, flags, states, the rule-L constraint, the
    end vertex, k = 0)? Name any case of a real round that the program's enumeration does not cover, and any way
    it could print ALL OK after a failure.

Q6. Try to break the theorem. (i) Is it consistent with Theorem 1, Theorem 4, the exact values T(n) for n <= 15
    and T(30) >= 7, T(32) >= 8, T(64) >= 18 in NOTE.md? (ii) Construct, by hand, rounds designed to defeat the
    potential: a flipped path ending at a vertex whose single out-neighbour is tall; a vertex x with a tall second
    out-neighbour named against a tall y; many in-neighbours of a path vertex; ties in h with both orders of the
    other slot. Compute Phi before and after for at least two such rounds and report the numbers.
    (iii) Is there any reason the adaptive Adversary of Theorem 4's construction (ternary family) cannot be run
    against rule L? What does rule L do at the second round of that construction?

For each question give CONFIRMED / REFUTED / UNSUPPORTED with the line of the attachment or file:line, and the
replacement wording you would use where something is wrong or unclear.
