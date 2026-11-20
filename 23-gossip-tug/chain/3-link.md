## KNOWN

The freely planned minimum is 2n-4 calls for n >= 4 (classical gossip theorem; Tijdeman, Baker–Shostak, Hajnal–Milner–Szemerédi). The legality rule is the "learn new secrets" protocol of the dynamic-gossip literature; that its longest play is n(n-1)/2: unsure of citation.

Flaw: the received question has four headlines, and the B part is easy. With Hastener naming, name agent 1 until inactive (n-1 calls, a star, whatever Delayer picks); the last partner then knows everything and nobody else knows its secret; name it n-2 times, each call legal only with an uninformed agent, who becomes fully informed. So B(n) <= 2n-3, hence B(n) is 2n-4 or 2n-3, and "B(n) = n(n-1)/2" is false from n = 4. By hand B(4) = 5. The hard part is A(n): no linear upper bound is evident.

## FINAL QUESTION

There are n >= 2 agents, numbered 1..n. Agent i holds a set K_i of secrets; initially K_i = {i}. A call is an unordered pair {a,b} of distinct agents. A call {a,b} is LEGAL in the current position if and only if a is not in K_b or b is not in K_a (at least one of the two does not yet know the other's secret). Making the call replaces both K_a and K_b by K_a ∪ K_b; nothing else changes. The position is TERMINAL when every K_i = {1..n}. An agent a is ACTIVE if some legal call contains a. (Consequences: every non-terminal position has an active agent; no pair is called twice; every play lasts at most n(n-1)/2 calls.)

Two players, Delayer and Hastener, both see the whole position (all sets K_i) at all times. Play proceeds in rounds until the position is terminal. In each round: first Delayer names any one active agent a; then Hastener, knowing a, picks any agent b such that {a,b} is legal; then the call {a,b} is made. Neither step may be skipped. The outcome is the total number of calls made. Delayer wants it as large as possible, Hastener as small as possible. Let A(n) be the outcome under optimal play by both (a finite perfect-information game, so A(n) is well defined).

Headline question (yes/no): is A(n) = 2n - 3 for every n >= 4?

If the answer is no, give the smallest n at which it fails and the exact function A(n) (or, failing that, the exact value of lim A(n)/n, or a proof that A(n)/n is unbounded). In either case, give the exact values A(4), A(5), A(6), A(7).

UNVERIFIED beliefs:
- A(2) = 1, A(3) = 3 (for n = 3 every play has exactly 3 calls).
- A(n) >= 2n - 4 for n >= 4 (the classical minimum number of calls when calls are freely planned).
- A(4) = 5 by hand: after the first call {a,b}, Delayer names a, which rules out the 4-call schedule; Hastener then seems able to finish in 5 against every continuation.
- No linear upper bound on A(n) is known to me; the trivial one is n(n-1)/2.
- Delayer naming one fixed agent until it is inactive, then others, yields only 2n - 3 against a sensible Hastener, so that line alone does not beat 2n - 3.

Guess (not a result): yes, A(n) = 2n - 3 for all n >= 4, because Delayer's only evident gain is breaking the four-cycle that every (2n-4)-call schedule needs, while Hastener's free choice of partner seems enough to keep producing a fully informed agent with almost every call.

## WHY IT IS PROBABLY UNCHARTED

Dynamic-gossip work studies protocols run by the agents themselves or by a random or single adversarial scheduler, and counts extremal or expected lengths. I know of no study of a split-control game where one side names a caller and the opponent picks the partner. Doubts: the game is simple enough that someone may have treated it under "adversarial scheduling" or as a Maker–Breaker variant; and my hand analysis of n = 4 could be wrong, which would move the conjectured formula.

## HOW TO CHECK AN ANSWER

For fixed n, a position is the tuple (K_1..K_n) of bitmasks. Compute by memoised recursion: value(terminal) = 0; otherwise value(P) = max over active a of min over legal b of 1 + value(P after {a,b}). A(n) = value(initial). Canonicalise positions under agent permutations to reach n = 7; cross-check n <= 5 without canonicalisation. Sanity: reproduce A(2) = 1, A(3) = 3, and, with unrestricted choice of calls, minimum 2n-4 and maximum n(n-1)/2.

A general "yes" needs two proofs: an explicit Hastener strategy guaranteeing at most 2n-3 calls against every naming sequence, and a Delayer strategy forcing at least 2n-3. A "no" needs one verified n with A(n) != 2n-3.
