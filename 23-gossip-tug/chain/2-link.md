## KNOWN

The question is well-posed: a finite perfect-information game with integer outcome, so h(n) and d(n) exist. With freely planned calls the minimum is 2n-4 for n >= 4 (the classical gossip/telephone problem; Tijdeman, Baker-Shostak, Hajnal-Milner-Szemeredi, early 1970s). The legality rule is essentially the "Learn New Secrets" protocol of the dynamic/epistemic gossip literature (van Ditmarsch and co-authors), where I believe the longest legal sequence on the complete graph is n(n-1)/2. Hence 2n-4 <= h(n), d(n) <= n(n-1)/2 for n >= 4. For n = 2 the value is 1; for n = 3 every play has exactly 3 calls. Whether the alternating Hastener/Delayer game itself has been studied: unsure. No flaw found; its weakness is only that strict alternation gives both players the same kind of power, so the answer may be a bland interpolation.

## SUCCESSOR QUESTION

There are n >= 2 agents, numbered 1..n. Agent i starts knowing exactly the set K_i = {i} (its own secret). A call is an unordered pair {a,b} of distinct agents. A call {a,b} is LEGAL in the current position if and only if a is not in K_b or b is not in K_a (at least one of the two does not yet know the other's secret). Making the call replaces both K_a and K_b by their union K_a ∪ K_b; nothing else changes. The position is TERMINAL when every K_i = {1..n}. An agent a is ACTIVE if some legal call contains a. (Consequences of the rules: a non-terminal position has an active agent; no pair is ever called twice; every play lasts at most n(n-1)/2 calls.)

Two players, Namer and Chooser, both see the whole position (all sets K_i) at all times. Play proceeds in rounds until the position is terminal. In each round: first Namer names any one active agent a; then Chooser, knowing a, picks any agent b such that {a,b} is legal; then the call {a,b} is made. Neither step may be skipped. The outcome is the total number of calls made. One player (the Hastener) wants the outcome as small as possible, the other (the Delayer) as large as possible.

Let A(n) be the outcome under optimal play when Delayer is Namer and Hastener is Chooser. Let B(n) be the outcome under optimal play when Hastener is Namer and Delayer is Chooser. (Sanity check: A(2) = B(2) = 1 and A(3) = B(3) = 3, since for n = 3 every play has exactly 3 calls.)

Question: determine A(n) and B(n) exactly for every n. In particular: (1) give the exact values for n = 2, ..., 7 (as far as exhaustive search reaches); (2) decide whether A(n) = 2n - 4 for all n >= 4 (the minimum number of calls when calls are freely planned), and if not, give a closed formula for A(n) or at least the limit of A(n)/n; (3) decide whether B(n) = n(n-1)/2 for all n, and if not, give a closed formula for B(n) or at least its growth order (n, n log n, or n^2) with the leading constant; (4) decide whether A(n) <= B(n) for all n.

## WHY IT IS PROBABLY UNCHARTED

Splitting each call between two adversaries (one fixes an endpoint, the other the partner) is a picker-chooser twist I have not seen applied to gossip; the literature I know studies planned, random, or protocol-driven calls, not split control. The classical 2n-4 schemes need both endpoints chosen together, so they do not transfer. Doubts: one direction may collapse to an easy answer (a hub strategy might give A(n) = 2n-4 or 2n-3 quickly; Delayer as Chooser might trivially force all pairs), and the adversarial-gossip literature is larger than I can recall.

## HOW TO CHECK AN ANSWER

Minimax with memoisation over positions (n-tuples of subsets, i.e. reflexive 0/1 matrices), canonicalised under agent relabelling, alternating a Namer layer and a Chooser layer; this gives exact A(n), B(n) for n up to about 6 or 7. Check n = 2, 3 against the stated values and confirm 2n-4 <= value <= n(n-1)/2. A claimed formula must match all computed values; a proof needs an explicit Chooser strategy with a potential argument for one bound and an explicit Namer strategy for the other.
