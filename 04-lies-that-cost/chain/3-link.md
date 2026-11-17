## KNOWN

The received game is well-posed: the position reduces to a finite state (for each alive candidate, its least remaining honesty obligation), so Q(n) is a finite reachability-game value. Any play with at most one lie is valid, so Q(n) >= U1(n) trivially. U1(n) is solved (Pelc, 1987; formula below from memory, unsure of the exact odd-n term). The claim Q(2) = Q(3) = infinity looks right: with two alive candidates, obligations (0,1) flip to (1,0) forever. Flaws: the received text asks several things at once (values for n <= 8, the finiteness set, a formula, an asymptotic comparison), and the small-n infinities are an artefact of a 1:1 split costing only one honest answer, which obscures the real issue: whether cheap late lies ever beat the one-lie bound.

## FINAL QUESTION

Fix integers n >= 2 and k >= 1. A secret x lies in U = {1, ..., n}. A questioner asks yes/no questions one at a time; question t names a set S_t ⊆ U ("is x in S_t?"), chosen knowing all earlier answers. The responder is adversarial, sees everything, may lie, and need not fix x in advance. Each lie forces a period of honesty.

Define by induction on t the set V_t of valid pairs (x, L), x in U, L ⊆ {1, ..., t} (the lie times), and the alive set C_t = {x : (x, L) in V_t for some L}. V_0 = {(x, ∅)}, C_0 = U. For question s put
  ell_s = min( |S_s ∩ C_{s-1}| , |C_{s-1} \ S_s| ).
(x, L) is in V_t iff
(i) for every s <= t, the answer to question s is the true answer for x exactly when s is not in L; and
(ii) for every s in L, L contains no element of {s+1, ..., s + k*ell_s}.
The responder must keep C_t nonempty (always possible). The questioner wins at the first t with |C_t| = 1.

Q_k(n) is the least q such that the questioner has an adaptive strategy winning within q questions against every responder (infinity if none). U1(n) is the least q such that the questioner can always identify x when the responder may lie at most once in the whole game (same questions, no other rule).

HEADLINE (yes/no): Is Q_2(n) = U1(n) for every n >= 2? If no, give the least n with Q_2(n) > U1(n).

General form: for each k, determine the set E_k = {n >= 2 : Q_k(n) = U1(n)}.

UNVERIFIED:
- Q_k(n) >= U1(n) for all k, n (every play with at most one lie satisfies (ii)), so the headline asks only whether Q_2(n) <= U1(n).
- U1(n) is the least q with n(q+1) <= 2^q for even n, and n(q+1) + (q-1) <= 2^q for odd n (Pelc's solution of the one-lie Ulam game).
- Q_1(2) = Q_1(3) = infinity (two candidates survive by alternating lies), so E_1 omits 2 and 3.
- Q_2(2) = 3 = U1(2) and Q_2(3) = 5 = U1(3) (hand analysis).
- The game position is captured by the multiset, over alive candidates, of the least remaining honesty obligation.

Guess (not a result): yes, because a lie cheap enough to be repeated before the game ends needs a lopsided or tiny split, and weight-balanced one-lie strategies seem able to avoid offering those until too few questions remain.

## WHY IT IS PROBABLY UNCHARTED

Ulam–Rényi variants in the literature bound lies by a total count, a fraction of each prefix, or fixed-length windows. Here the honesty period is priced by how evenly the lied-to question splits the currently alive set, so the questioner sets the price of each lie; I know of no such model. Doubts: it may be a disguised case of a "lies in windows" or cost-budget model; and the headline could fail at some small n (5 to 9), making the answer a quick computation rather than a theorem.

## HOW TO CHECK AN ANSWER

For fixed n, k: states are multisets of obligations (integers 0..k*floor(n/2)), one per alive candidate. A question picks a sub-multiset; each answer kills obligated candidates who would have lied, sets liars' obligation to k*ell, and decrements the others. Compute by depth-bounded minimax whether the start state (n zeros) reaches a one-candidate state within q = U1(n) moves; compute U1(n) independently by the same search with states (truthful count, one-lie count). Run n = 2..12. A general "yes" needs an explicit strategy for all n plus a proof of the state reduction; a "no" needs one n, verified by the search, with minimality.
