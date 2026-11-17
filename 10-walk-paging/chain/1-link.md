## KNOWN

The model is, I believe, "granularity-change caching" (Beckmann, Gibbons, McGuffey, around 2021-22; fairly but not fully confident of the attribution). "Block-aware caching" (Coester, Levin, Naor, Talmon) is a relative that charges per block; unsure how its bounds transfer. Easy facts: with at least k+1 blocks the adversary can request one page per block, so block loading is useless and the classical lower bound k survives. Any classical k-competitive algorithm that ignores neighbours is at most kB-competitive, since block loading saves the optimum at most a factor B. So the answer lies in [k, kB]; the exact value for all k, B: unsure. Flaws in the seed: universe size is unspecified, the additive constant in "competitive" is unspecified, and a ratio defined by a limit is not directly checkable by finite computation. "What does a good algorithm do" has no unique answer.

## SUCCESSOR QUESTION

Pages are the integers. A cache is a set of at most k pages. Two players, Online and Offline, each own a cache, both initially empty, and both obey the same service rule: when page p is requested and p is in the cache, nothing happens and the cache may not change (a hit); when p is not in the cache (a fault), the owner replaces its cache C by any set C' with p in C', |C'| <= k, and C' a subset of C ∪ {p, q}, where q is one of p-1 or p+1 chosen by the owner at that moment. So one fault brings in the requested page and, optionally, ONE adjacent page on either side; the pairs overlap, there is no fixed partition into blocks. Any pages may be evicted at a fault; nothing may be loaded except p and that one neighbour.

For a finite request sequence s, let opt(s) be the minimum number of faults over all ways of serving s by these rules from the empty cache with the whole of s known in advance.

Game G(k, m), for integers k >= 1 and m >= 1, with perfect information. In each round the Adversary, who sees Online's current cache and the whole history, either stops or names a page p such that opt(history followed by p) <= m; naming any other page is illegal. Online then serves p by the service rule, knowing the history but not the future. The payoff is the total number of faults Online has incurred when the Adversary stops. The Adversary maximises it, Online minimises it; Online's strategy is deterministic.

Let F(k, m) be the value of G(k, m) (it is finite: at most k(m+1)).

Question. Determine F(k, m) exactly for all k >= 1 and m >= 1, as a closed formula or a recurrence, and with it the limit r(k) = lim F(k, m)/m as m grows. As a required part of the answer, decide for each k whether r(k) changes when Online is forbidden ever to load the neighbour (Offline keeping the right), and whether r(k) equals the corresponding limit for the partitioned variant in which the only permitted neighbour of p is p XOR 1 (blocks {2i, 2i+1}) for both players.

## WHY IT IS PROBABLY UNCHARTED

Published work on block or granularity-change caching uses a fixed partition into blocks and asymptotic ratios; I know of none with overlapping neighbourhoods, where a page belongs to two possible "blocks" and partition-based phase arguments break. The budgeted exact-value form F(k, m), with its additive behaviour in m, is also not what the literature states. Doubts: the answer might collapse to something dull (for instance r(k) = 2k - 1 or k by a short argument), and the overlapping variant may reduce to the partitioned one by a simulation I have not seen. The k(m+1) bound is my own sketch, unverified.

## HOW TO CHECK AN ANSWER

Solve G(k, m) exhaustively by minimax for k <= 3 and m <= 4 or so. A state is Online's cache plus the map from each reachable Offline cache to the fewest faults reaching it (entries above m dropped); normalise by translation and reflection; hits may be ignored as Adversary moves. Only pages within distance 2 of already-requested pages, or one fresh far-away page, need be considered, which itself needs a short proof. Compare the table with the claimed formula, then prove the formula by an explicit Online strategy and an explicit Adversary strategy. Repeat for the two variants.
