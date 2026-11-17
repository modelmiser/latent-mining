## KNOWN

Without prefetch this is paging on a path access graph, where LRU keeps an interval and (I believe) coincides with Belady's rule, so it is exactly optimal. A prefetching schedule with f faults can be simulated without prefetch using at most 2f faults (load the companion only when it is first requested). Hence Online, playing LRU and never prefetching, faults at most 2m: F(k,m) <= 2m for every k. I also expect an adversary to hold ratio 2 for every fixed k (punish a prefetch by reversing, punish its absence by continuing), so the received r(k) is probably the constant 2 and is easy. What remains open is the exact additive term. Flaw: the claim "payoff never exceeds k(m+1)" is false for arbitrary Online play (a self-evicting Online faults forever), so the value must be defined as a guarantee. No citation offered: unsure.

## FINAL QUESTION

Pages are the integers. A cache is a set of at most k pages (integer k >= 2), initially empty.

Service rule. When page p is requested and p is in the cache C, it is a hit and C stays unchanged. Otherwise it is a fault: the cache owner picks q in {p-1, p+1} and replaces C by any C' with p in C', |C'| <= k, and C' a subset of C ∪ {p, q}. (A fault loads p and optionally one neighbour; any pages may be evicted; nothing else may be loaded.)

A walk is a finite sequence p_1, ..., p_n of pages with p_1 = 0 and |p_{i+1} - p_i| = 1 for all i. opt(s) is the minimum number of faults over all ways of serving the walk s by the service rule from the empty cache, with all of s known in advance.

Game W(k, m), integers k >= 2, m >= 1, perfect information. Online owns one cache, initially empty. Round 1: the request is page 0 and Online serves it. In each later round the Adversary, seeing the whole history and Online's cache, either stops (the game ends) or names p in {last request - 1, last request + 1}; naming p is legal only if opt(history followed by p) <= m. Online then serves p by the service rule, knowing the history and its cache but not the future; at a fault its move is the choice of q and C'. Play may be infinite. The payoff is the total number of Online's faults (possibly infinite).

F(k, m) is the least integer N such that Online has a strategy keeping the payoff <= N against every Adversary behaviour.

Headline. Find the exact function F(k, m) for all k >= 2 and m >= 1. Equivalently, find the discount D(k, m) = 2m - F(k, m), the number of faults that the empty start and cache size k save Online below the bound 2m.

UNVERIFIED beliefs:
- F(k, m) <= 2m for all k, m (LRU without prefetch).
- F(k, 1) = 2 for all k.
- F(2, m) = 2m for all m.
- F(3, 2) = 4 and F(4, 2) = 3.
- F(k, m) = m + 1 whenever k >= 2m + 2 (Online never evicts and always prefetches outward).
- For fixed k, F(k, m)/m tends to 2.
- Candidate formula fitting all of the above: F(k, m) = max(m + 1, 2m - floor(k/2) + 1).

Guess (clearly a guess): I would bet that D(k, m) is bounded in m for each fixed k and equals about k/2 for large m, because Online matches the offline player only while its cache is still filling; I would not bet on the candidate formula being exact.

## WHY IT IS PROBABLY UNCHARTED

Access-graph paging and paging with prefetching or read-ahead are both studied, but I know of no treatment combining a path access graph, a one-neighbour prefetch allowed only at faults, and an exact offline-budget game value instead of a competitive ratio. Exact additive terms from a cold start are rarely worked out. Doubts: the model is simple enough that the answer may follow in a page from the LRU/Belady coincidence plus a parity argument, and results on "paging with locality and lookahead" might cover the ratio. The candidate formula rests on four hand-checked cases.

## HOW TO CHECK AN ANSWER

Fix k, m, N. Solve a finite safety game. State: current page, Online's cache, Online's fault count capped at N+1, and the offline profile: the map from each cache (subset of visited pages containing the current page, size <= k) to the minimum faults reaching it, capped at m+1. All requests lie in [-(2m-1), 2m-1]. Compute the set of states from which the Adversary forces count N+1 as a least fixpoint (zero-fault cycles gain nothing). F is the least N for which the start state is outside it. Tabulate k <= 5, m <= 5. A general claim needs an explicit Online strategy with a proof over all legal walks, and an explicit Adversary strategy with opt <= m verified against every Online.
