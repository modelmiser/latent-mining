# Prior-art search: first-to-know-everything on a ring with destruction budget k

**Setting, in standard terms.** Full-information flooding on a bidirectional ring under a synchronous message adversary ("mobile omission" or "dynamic link" faults) with at most k losses per round and no link lost twice running (edges 2-recurrent). The value is the adversarial "dynamic radius" (first node to hear from all), which by time reversal is the same as the first node heard by all, i.e. broadcast. The reversal is my inference; the constraint is reversal-symmetric.

## CLOSEST PRIOR WORK

1. **Broadcasting in synchronous networks with dynamic faults** — Chlebus, Diks, Pelc, 1996 (Networks 27(4)). https://iaorifors.com/paper/35049
   - Shows: with at most k faulty links per step, broadcast succeeds iff edge connectivity exceeds k; time is O(d^(k+1)) for fixed k, and tori are near-diameter.
   - Differs: fixed source, links may fail in consecutive steps, so a ring allows only k = 1; no ring results in the abstract.
2. **Asymptotically Tight Bounds on the Time Complexity of Broadcast and its Variants in Dynamic Networks** — El-Hayek, Henzinger, Schmid, 2022/ITCS 2023. https://arxiv.org/abs/2211.10151
   - Shows: when the adversary picks a rooted tree each round, the time until some id is known to all lies between ⌈(3n−1)/2⌉−2 and about 2.4n.
   - Differs: same "first one to reach all" objective, but arbitrary rooted trees on a complete vertex set; no ring, no per-round loss budget, no no-repeat rule.
3. **Linear-Time Data Dissemination in Dynamic Networks** — Schwarz, Zeiner, Schmid, 2017. https://arxiv.org/abs/1701.06800
   - Shows: O(n) dissemination for rooted trees with boundedly many leaves; n−1 rounds for rooted chains, (n−1)/2 for undirected chains.
   - Differs: adversary chooses whole graphs from a class; no failure budget, no rings.
4. **Shortest, Fastest, and Foremost Broadcast in Dynamic Networks** — Casteigts, Flocchini, Mans, Santoro. https://arxiv.org/abs/1210.3277
   - Shows: in time-varying graphs whose edges recur within Δ, foremost broadcast is feasible in O(nΔ) time (snippet only).
   - Differs: this is the general fact behind F ≤ 2⌊n/2⌋; no budget k, no exact ring values.
5. **Exploration of k-Edge-Deficient Temporal Graphs** — Erlebach, Spooner, 2021/2022. https://durham-repository.worktribe.com/output/1210332/exploration-of-k-edge-deficient-temporal-graphs
   - Shows: with at most k edges missing per step and always connected, exploration takes O(kn log n), and Ω(n log k) on some family (snippet only).
   - Differs: same "k missing per step" budget, but an agent exploring, not flooding; connectivity is required instead of no-repeat.
6. **Time Complexity of Broadcast and Consensus for Randomized Oblivious Message Adversaries** — El-Hayek, Henzinger, Schmid, 2023/DISC 2024. https://arxiv.org/abs/2302.11988
   - Shows: adversary fixes k edges per round, the rest of the rooted tree is random; broadcast in O(k + log n) (snippet only).
   - Differs: randomised, and k counts forced edges, not losses.

## VERDICTS

- **⌊n/2⌋ ≤ F ≤ 2⌊n/2⌋:** EASY COROLLARY of the Δ-recurrent-edge bound (item 4) with Δ = 2 on a ring of diameter ⌊n/2⌋.
- **F(n,1) = F(n,2) = ⌊n/2⌋:** NOT FOUND.
- **F(n,k) = n − ⌈n/k⌉ for n > k ≥ 3 (solver A), bracket n − ⌈n/k⌉ ≤ F < n − n/k + 1 (solver B), c(k) = 1 − 1/k:** NOT FOUND. Nothing seen combines a per-round budget with a no-repeat rule; the opposing-ramp strategy was not seen either.
- **k ≥ n values (F(n,n) = n−1, F(n,2n) = 2⌊n/2⌋):** NOT FOUND.

**Sequences**
- k = 3 row (2,2,3,4,4,5,6,6,7,8): OEIS A004523, floor(2n/3). https://oeis.org/A004523
- k = 4 row (2,3,3,4,5,6,6,7,8,9): OEIS A057353, floor(3n/4). https://oeis.org/A057353
- Neither entry has a comment on networks, broadcast or failures; the matches are to the closed formula only.
- Not looked up: the k = 1, 2 row (⌊n/2⌋) and the six-term rows for k = 5..8.

**Queries run (19).** Message adversary / first process to hear from everyone / ring; Santoro–Widmayer mobile omissions; dynamic radius / rooted trees; flooding time on a dynamic ring with k edges removed; two OEIS term searches; no edge removed in consecutive rounds; the ⌈(3n−1)/2⌉−2 lower bound; gossip/broadcast with dynamic faults per step; convergecast / data aggregation with a dynamic-ring lower bound; Chlebus–Diks–Pelc ring; bounded-recurrent edges; radius of nonsplit graphs; dynamic ring with k missing edges and "n − n/k"; full-information flooding game with a loss budget; Dobrev–Vrťo tori/hypercubes; temporal ring diameter with edges every two steps; heard-of predicates with k losses; puzzle/olympiad phrasing; "cannot be faulty in consecutive rounds"; ramp/wave adversary.

## WHO TO ASK

A distributed-computing theorist working on message adversaries and dynamic-network broadcast time (the oblivious-message-adversary / dynamic-radius line), or someone from the older "broadcasting with dynamic link faults" literature.

## NOT CHECKED

- No paper was read in full: 0. I read two OEIS entries, three arXiv abstract pages and one abstract listing through a summarising fetch; items 4–6 were judged from search snippets.
- The full text of Chlebus–Diks–Pelc, Dobrev–Královič–Královič–Santoro ("fractional dynamic faults with threshold", which reportedly treats rings; the Springer page redirected to a login) and Godard–Peters (arXiv 1106.3579) was not opened. A ring lemma could sit inside any of them.
- No Google Scholar, DBLP or forward-citation search; no non-English sources.
- "NOT FOUND" therefore means not found in about 19 web searches, not that the result is new.
