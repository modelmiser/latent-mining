<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Round 5: a new upper bound, T(n) <= (n-3)/2, proposed as Theorem 3

The repository's note (`14-forest-orientation/NOTE.md`) currently proves T(n) <= 9n/16 - log2 n for n >= 19
(Theorem 2) for the greedy rule of Gupta, Kumar and Stein. Attachment A is a NEW draft, not yet in the
repository, claiming that the same greedy rule pays at most (n-3)/2, by a potential argument with two lemmas.
Attachments B and C are the two enumeration programs the draft cites.

The author is the only reader of this proof so far. Assume the author is competent and wrong until you have
traced every case yourself. Do not reason from the summary; check each inequality line by line. Read
NOTE.md sections 1, 2 and 5 for the definitions of the game, of T(n), and of the algorithm.

Q1. Lemma A (attachment A). Is the proof correct and complete? Check in particular: (a) the description of
    the out-neighbours after the flip; (b) the claim that the out-trees of c_i, z and q contain no path vertex;
    (c) Step 1 (no h decreases), including the bound g_{i-1} >= k-i+1; (d) every numbered case in Step 3,
    including boundary cases k = 1 and k = 2 and the case where w_k already had one out-neighbour;
    (e) whether any edge's term is forgotten (edges INTO path vertices, edges into q, edges in the other tree).
    Give a concrete counterexample merge if any case fails.

Q2. Lemma B (attachment A). Is the induction correct? Check: a vertex of in-degree 0 exists; deleting it
    changes no other h; the three sub-cases for H(T) are exhaustive and each inequality holds; the base case.

Q3. Theorem 3 (attachment A). Does "total cost <= Phi(T) <= (n-1)/2 - H(T)", and then "<= (n-3)/2 if any cost
    was paid", follow from Lemmas A and B for the game as defined in NOTE.md? Does it hold for every
    tie-breaking of the greedy rule, and does it bound T(n) as NOTE.md defines T(n) (including the case where
    the final forest is one tree, and small n)? Is "L <= 1/2" justified?

Q4. Consistency. Does the claimed bound contradict anything in the repository: the exact values of T(n) listed
    in NOTE.md, Theorem 1 (the lower bound), or the table of f(n) in `upper/greedy_recurrence.py`? Try to build
    by hand a small greedy run whose cost exceeds (n-3)/2 or exceeds Phi of the final tree.

Q5. The comparison sentence: "the new bound is smaller once n/16 > log2 n - 3/2, that is from about n = 80".
    Is this right, and is "the best statement is the minimum of the two" right?

Q6. Attachments B and C. Do the programs check what the draft says they check (all oriented trees with
    out-degree <= 2 up to the stated sizes, all endpoint pairs with h(p) <= h(q), all shortest paths, the
    potential as defined)? Name any gap between the programs and the lemmas.

For each question give CONFIRMED / REFUTED / UNSUPPORTED with the line of the attachment or file:line, and the
replacement wording you would use where something is wrong or unclear.
