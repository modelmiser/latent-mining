## KNOWN

With w larger than the number of questions this is Ulam's game with one lie, solved exactly (Pelc): for even n the least q with n(q+1) <= 2^q, with a small correction for odd n. For general w the question looks well-posed and finite: each candidate only needs to remember how long ago its last lie was, so states are short count vectors and q(n, w) is computable by dynamic programming. The counting (Berlekamp-style volume) bound gives q >= log2(n) / (1 - log2(lambda_w)), where lambda_w is the largest root of x^w = x^(w-1) + 1 (golden ratio for w = 2), since admissible lie patterns grow like lambda_w^q. I expect this bound to be tight up to lower-order terms, as it is for a fixed number of lies, but I am unsure whether that is published. Related work I am confident exists: prefix-bounded lies (Pelc; Spencer and Winkler, "Three thresholds for a liar"). So the seed is probably "linear in log n with a computable constant", not deep.

## SUCCESSOR QUESTION

A secret x lies in {1, ..., n}. A questioner asks q yes/no questions in sequence, each of the form "is x in S?", where the set S may depend on all earlier answers. The responder may lie any number of times, but each lie buys a longer period of forced honesty: if the lies occur at question numbers t_1 < t_2 < ... < t_m, then t_{k+1} - t_k > k for every k (after the k-th lie, the next k answers must be truthful; the first lie may come at any time). Call a subset of {1, ..., q} with this property an admissible lie pattern, and let P(q) be the number of admissible lie patterns (P(1)=2, P(2)=3, P(3)=5, P(4)=8). The responder is adversarial and need not fix x in advance: it only has to keep at least one pair (x, admissible lie pattern) consistent with all answers given.

Let A(q) be the largest n for which the questioner has a strategy that, after exactly q questions, always leaves exactly one x consistent with the answers. Counting gives A(q) <= floor(2^q / P(q)).

(1) Determine A(q) exactly for every q from 1 to 12.
(2) For which q does A(q) = floor(2^q / P(q)) hold? In particular, does equality hold for all sufficiently large q, for infinitely many q, or only finitely many?
(3) Let Q(n) be the least q with A(q) >= n. Prove or disprove: Q(n) = log2(n) + (c + o(1)) * sqrt(log2 n) * log2(log2 n) for a constant c, and determine c.

## WHY IT IS PROBABLY UNCHARTED

The number of lies available in q questions is about sqrt(2q): neither a fixed number (classical Ulam, where the overhead is e*log log n) nor a constant fraction (Spencer-Winkler thresholds, where the rate changes). I know of no treatment of this intermediate regime, nor of a lie budget that the responder's own lies tighten. The constraint is also not a fixed-memory sliding window, so constrained-channel capacity arguments do not apply directly. Doubts: growing-error-count variants of Berlekamp's bound may cover part (3) at first order, and part (2) may simply fail for small-number reasons.

## HOW TO CHECK AN ANSWER

Parts (1) and (2), small cases: a candidate's status is (k, r) = (lies so far, forced-truth answers remaining), with r <= k and k <= 4 for q <= 12; a game state is the vector of candidate counts per status plus questions left. Memoised minimax over these vectors (the questioner chooses how many candidates of each status go into S) gives A(q) exactly; n <= about 16 suffices. P(q) is a simple recurrence. Part (3): a matching proof pair, the volume lower bound with an asymptotic count of P(q), and an explicit strategy, sanity-checked against the computed table.
