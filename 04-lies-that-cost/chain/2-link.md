## 1. KNOWN

The received question is a Rényi–Ulam searching game with a time-based constraint on lie patterns. The counting (sphere-packing, Berlekamp-style) bound A(q) <= floor(2^q / P(q)) is sound: for a fixed x, distinct lie patterns give distinct answer strings. The stated P(1..4) = 2, 3, 5, 8 check by hand, but the sequence is not Fibonacci (I get P(5) = 12). Since a pattern with m lies needs about m^2/2 questions, m <= about sqrt(2q), and a volume estimate suggests the form in part (3) with c near 1/sqrt(2); so part (3) is probably routine asymptotics. Small cases are slack: for n = 2 all questions are equivalent, and at q = 4 the patterns {1,3} and {2,4} are complementary, so A(4) = 1 although the bound gives 2. The one-lie game is solved exactly (Pelc, 1987, I believe). The question is well-posed but its lie rule ignores question content.

## 2. SUCCESSOR QUESTION

A secret x lies in U = {1, ..., n}. A questioner asks yes/no questions one at a time; question t names a set S_t ⊆ U ("is x in S_t?"), chosen knowing all earlier answers. Both players see everything except x. The responder is adversarial, may lie, and need not fix x in advance. There is no cap on the number of lies; instead, each lie forces a period of honesty whose length depends on how evenly the lied-to question split the candidates still alive.

Formally, define by induction on t the set V_t of valid pairs (x, L), with x in U and L ⊆ {1, ..., t} (the lie times), and the alive set C_t = {x : (x, L) in V_t for some L}. V_0 = {(x, ∅) : x in U}, so C_0 = U. For a question S_s put

  ell_s = min( |S_s ∩ C_{s-1}| , |C_{s-1} \ S_s| ),

computed from the candidates alive just before question s is answered. (x, L) is in V_t iff
(i) for every s <= t, the answer to question s is the true answer for x exactly when s is not in L; and
(ii) for every s in L, L contains no element of {s+1, ..., s+ell_s}.

So a lie on question s forces truthful answers to the next ell_s questions; a lie on a question with all alive candidates on one side costs nothing, and a lie on a question splitting the alive candidates a : b costs min(a, b) honest answers. The responder must answer so that C_t stays nonempty (always possible). The questioner wins at the first t with |C_t| = 1.

Let Q(n) be the least q such that the questioner has an adaptive strategy guaranteeing a win within q questions against every responder, and Q(n) = infinity if no such q exists. (Q(1) = 0. Unverified sanity claim: Q(2) = Q(3) = infinity, because every lie then costs at most one honest answer and the responder can keep two candidates alive by alternating.)

Let U1(n) be the least q such that the questioner can always identify x when the responder may lie at most once in the whole game (same questions, no other rule).

(1) Determine Q(n), finite or infinite, for every n from 1 to 8.
(2) Determine the set of n with Q(n) finite, and give a formula for Q(n) on that set; in particular, decide whether Q(n) = U1(n) for all sufficiently large n, and if not, determine Q(n) - U1(n) as a function of n.

## 3. WHY IT IS PROBABLY UNCHARTED

Lie budgets, lie fractions, half-lies and fixed lie-pattern constraints are studied; I know no model where the price of a lie is set by the balance of the question lied to, measured on the shrinking alive set. That makes the endgame self-defeating: narrowing the candidates cheapens lies exactly when they matter, and candidates alive only under a lie hypothesis act as useful padding. Since one lie is always admissible, Q(n) >= U1(n); whether the endgame costs extra is unclear to me. Doubts: Q might be infinite for all n >= 2 (dull), or collapse to U1(n) by an easy padding argument.

## 4. HOW TO CHECK AN ANSWER

For small n, the game is finite-state: a state records, for each candidate, dead or its least remaining forced-honesty count (0 to floor(n/2)); smaller counts dominate. That is at most (2 + floor(n/2))^n states with 2^n questions each. Compute the set of questioner-winning states and their minimax depth by backward induction (attractor computation); states outside the attractor give infinity. Compare with a claimed formula, and with U1(n) from an independent one-lie solver. A general claim needs an explicit questioner strategy plus a responder strategy (or potential argument) matching it.
