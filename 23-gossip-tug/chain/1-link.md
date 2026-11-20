## 1. KNOWN

The centrally planned two-way minimum 2n - 4 (n >= 4) is classical (Tijdeman; Baker and Shostak; Hajnal, Milner and Szemerédi), as is 2n - 2 for one-way calls (Harary and Schwenk, I believe). Gossip with a bounded number of failed calls has been studied (Berman and Hawrylycz, I believe; exact values for k failures: unsure). Distributed "epistemic" gossip protocols, where an agent may call only someone whose secret it does not know ("Learn New Secrets"), were studied by van Ditmarsch and co-authors; every such execution ends with everyone informed, and its length lies between 2n - 4 and n(n-1)/2 (I believe both extremes are attained). The seed is flawed: it bundles three unrelated variants (adversary, silent failure, degree bound), does not define what the adversary may choose or what it wants, and "what people have to know about the plan" has no unique answer.

## 2. SUCCESSOR QUESTION

There are n >= 2 agents, numbered 1..n. Agent i starts knowing exactly the set K_i = {i} (its own secret). A call is an unordered pair {a,b} of distinct agents. A call {a,b} is LEGAL in the current position if and only if a is not in K_b or b is not in K_a (at least one of the two does not yet know the other's secret). Making the call replaces both K_a and K_b by their union K_a ∪ K_b; nothing else changes. The position is TERMINAL when every K_i = {1..n}. (Facts that follow from the rules: a non-terminal position always has a legal call, no pair can ever be called twice, so every play lasts at most n(n-1)/2 calls.)

Two players, Hastener and Delayer, both see the whole position (all the sets K_i) at all times. They move alternately; a move consists of choosing any one legal call, which is then made. Passing is not allowed. Play stops when the position is terminal. The outcome is the total number of calls made by both players together. Hastener wants this number as small as possible, Delayer as large as possible.

Let h(n) be the outcome under optimal play by both when Hastener makes the first move, and d(n) the outcome under optimal play when Delayer makes the first move.

Question: determine h(n) and d(n) exactly for every n. In particular: (1) give the exact values for n = 2, ..., 7 (as far as exhaustive search reaches); (2) decide the growth order of h(n): is h(n) = O(n), or is it of order n log n, or of order n^2, and what is the limit of h(n)/n or h(n)/n^2 if it exists; (3) decide whether |h(n) - d(n)| is bounded by a constant for all n.

## 3. WHY IT IS PROBABLY UNCHARTED

The literature I know treats the two extremes (shortest and longest executions) and random scheduling (expected length), not an alternating game in which a helper and a spoiler share the scheduler. Neither the 2n - 4 construction nor the n(n-1)/2 construction survives an opponent taking every other move, so the easy answers stop applying. Doubts: saturation games and Maker-Breaker games on graphs are a large field, and a "gossip game" of this shape may exist under another name; I cannot name one. The growth order might turn out to be easy (for example, Delayer forcing quadratic length by a simple pairing strategy).

## 4. HOW TO CHECK AN ANSWER

Small cases: memoised minimax over positions (n-tuples of subsets of {1..n}, canonicalised under permutations of agents, plus whose turn it is); n <= 5 is immediate, n = 6 should fit in minutes, n = 7 may not. Sanity checks: h(2) = d(2) = 1, h(3) = d(3) = 3 (every play with n = 3 has exactly three calls), and 2n - 4 <= h(n), d(n) <= n(n-1)/2 for n >= 4. A general claim needs an explicit Hastener strategy with an upper bound and an explicit Delayer strategy with a matching lower bound; both strategies can be tested against the minimax table.
