## KNOWN

Synchronization games of this kind were introduced, I believe, by Fominykh and Volkov ("P(l)aying for synchronization", around 2012, later with Martyugin). What I recall with reasonable confidence: the Synchronizer wins from the full state set iff he wins from every pair of states, so the winner is decidable in polynomial time by a backward search on the pair graph; consequently the game length is polynomially bounded (the pair criterion plus a merge-pairs-one-at-a-time argument gives a cubic-type upper bound, and I think a quadratic-order lower bound is known). The exact maximum as a function of n, the extremal automata, and the "Spoiler moves every k-th letter" variant: unsure; I do not know them to be settled. The question is well posed and the answer is finite (at most polynomial in n), not trivial.

## SUCCESSOR QUESTION

Let A be a complete deterministic finite automaton with state set Q, |Q| = n, and input letters {a, b}; for a word v and a set S of states, S·v is the image of S under v. The Synchronizer must commit in advance, without seeing anything, to a word w = w_1 w_2 ... w_m over {a, b}. The Spoiler, who knows A and w, then chooses letters u_1, ..., u_{m-1} in {a, b}, and the played word is the interleaving w_1 u_1 w_2 u_2 ... u_{m-1} w_m (length 2m-1). Call w a blind winning word for A if |Q·(w_1 u_1 w_2 ... u_{m-1} w_m)| = 1 for every one of the 2^(m-1) choices of u. (The final state may depend on u.) Let b(A) be the least m for which a blind winning word of length m exists, and let B(n) be the maximum of b(A) over all n-state two-letter automata A that have a blind winning word.

(1) Determine B(n) exactly for n = 2, 3, 4, 5.
(2) Determine the growth of B(n): give a formula, or at least decide whether B(n) is bounded by a polynomial in n, and describe an extremal family of automata.
(3) Call A adaptively winnable if, in the game where the two players alternate letters (Synchronizer first), each seeing all earlier letters, the Synchronizer has a strategy forcing the image of Q under the played word to become a single state after finitely many letters. Every automaton with a blind winning word is adaptively winnable. What is the least n for which some n-state two-letter automaton is adaptively winnable but has no blind winning word, and how many such automata (up to renaming states) exist at that n?

## WHY IT IS PROBABLY UNCHARTED

Removing the Synchronizer's feedback swaps the quantifiers (one word against all Spoiler replies), so the pair criterion that makes the original polynomial no longer applies: the natural state space is families of subsets, and B(n) could conceivably be exponential. I know of no paper on this exact "oblivious" game. Doubts: it can be recast as a universal-completion problem for partially specified words, or as a kind of directing word for a nondeterministic automaton with correlated choices; work on directable NFAs or synchronization under partial observation may already contain bounds I do not remember.

## HOW TO CHECK AN ANSWER

For fixed A, track the antichain of maximal possible images: start with {Q}; a Synchronizer letter x maps family F to the maximal sets among S·x (first step) or S·u·x for S in F, u in {a, b}. Breadth-first search over antichains gives b(A) exactly (images are monotone, so maximal sets suffice). Enumerate all n^(2n) automata up to isomorphism for n ≤ 5 to get B(n) and, with the standard pair-graph backward search for adaptive winnability, the count in (3). A general claim needs a proof of the upper bound plus an explicit family verified by the same search.
