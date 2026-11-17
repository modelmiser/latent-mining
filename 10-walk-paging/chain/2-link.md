## 1. KNOWN

Without the neighbour, the game is classical paging: the Sleator–Tarjan argument gives ratio exactly k, and the adversary needs only k+1 pages. The finiteness bound k(m+1) looks right: LRU without the neighbour faults at most k times per phase, and each phase after the first costs any offline server at least one fault, however much a fault loads. For k = 1 the neighbour is useless and F(1, m) = m. Paging where one fetch brings a whole block is studied ("block-aware caching", "granularity change"; I am fairly but not fully sure of those names); the overlapping-pair version I cannot place. Weaknesses of the question as given: it is three questions, not one; the adversary can simply never request a page adjacent to anything Online holds, so Online's neighbour right is probably worthless and the answer likely collapses to "classical bound with Offline up to twice as fast" — unsure, not checked. Computation over arbitrary integers is also awkward.

## 2. SUCCESSOR QUESTION

Pages are the integers. A cache is a set of at most k pages, initially empty. Service rule: when page p is requested and p is in the cache C, nothing happens and C may not change (a hit). When p is not in C (a fault), the owner of the cache picks q in {p-1, p+1} and replaces C by any set C' with p in C', |C'| <= k, and C' a subset of C ∪ {p, q}. So a fault loads p and, optionally, one adjacent page; any pages may be evicted at a fault; nothing else may be loaded.

A walk is a finite sequence of pages p_1, p_2, ..., p_n with p_1 = 0 and |p_{i+1} - p_i| = 1 for every i (consecutive requests are always adjacent pages, never equal). For a walk s, opt(s) is the minimum number of faults over all ways of serving s by the service rule from the empty cache, with the whole of s known in advance.

Game W(k, m), integers k >= 2 and m >= 1, perfect information, two players: Adversary and Online. Online owns one cache, initially empty. Round 1: the request is page 0. In every later round the Adversary, who sees Online's current cache and the whole history, either stops or names p in {last request - 1, last request + 1} such that opt(history followed by p) <= m; any other move is illegal. Online then serves the named page by the service rule, knowing the history and its own cache but nothing about the future; at a fault its choice of q and of C' is its move. The payoff is the total number of faults Online has incurred, counted when the Adversary stops, or the limit of that count if the Adversary never stops. The Adversary maximises the payoff and Online minimises it. (The payoff never exceeds k(m+1), so the value exists.)

Let F(k, m) be the value of W(k, m).

Question. Determine F(k, m) exactly for all k >= 2 and m >= 1, as a closed formula or a recurrence in k and m. In particular give the sequences F(2, m) and F(3, m) for all m, and the rational number r(k) = lim F(k, m)/m (m growing) as a function of k.

## 3. WHY IT IS PROBABLY UNCHARTED

Forcing requests to walk the line means the adversary can no longer dodge Online's prefetch: every fault becomes a guess of direction, and the neighbour right matters to both sides. Paging with requests constrained to a graph is the access-graph model (Borodin, Irani, Raghavan, Upfal), where paths and trees are understood asymptotically; I know of no treatment combining it with a two-page fetch, nor exact fault-budget values. Doubts: the path may be simple enough that r(k) falls out of a short potential argument, or reduces to access-graph paging on a path with pages paired.

## 4. HOW TO CHECK AN ANSWER

Solve W(k, m) exhaustively for k = 2, 3 and m up to about 5. A position is Online's cache plus, for each offline cache reachable on the history, its least fault count (capped at m), all taken relative to the current page; this is finite. Compute the value by fixed-point iteration (hit-only cycles add nothing). Compare with the claimed formula; a general claim needs an explicit Online strategy (upper bound) and Adversary strategy (lower bound) with proofs.
