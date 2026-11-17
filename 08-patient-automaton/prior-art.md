# Prior-art search

Areas: synchronization games (synchronizing automata); delay games / games under delayed control (reactive synthesis); preset vs adaptive synchronizing sequences.

## CLOSEST PRIOR WORK

1. **P(l)aying for Synchronization** — Fominykh, Martyugin, Volkov, 2012/2013. https://arxiv.org/abs/1205.2174
   Shows: introduces the alternating-letter synchronization game; the winner is decided on pairs in O(n²k) time. Differs: full information only, i.e. exactly the d = 0 game; no delay, no blind variant, no counts.
2. **Adversarial Synchronization** — Lipin, Volkov, 2026. https://arxiv.org/abs/2601.18362
   Shows: Bob may answer with words of length < k; the classes A_1 ⊋ A_2 ⊋ … ⊋ A_ω are strictly nested and on n states collapse exactly at k = C(n,2). Differs: the parameter is Bob's word length, not Alice's information; Alice sees everything.
3. **On the Existence of Reactive Strategies Resilient to Delay** — Fränzle, Kröger, Winter, Zimmermann, 2023 (LMCS 2025). https://arxiv.org/abs/2305.19985
   Shows: the controller sees environment actions δ steps late; winning is monotone in δ, and beyond an exponential "decisive" delay (safety, reachability, parity) winning implies winning under every delay. Differs: general graph games; the blind limit is never compared with "wins under every delay".
4. **How Much Lookahead Is Needed to Win Infinite Games?** — Klein, Zimmermann, 2015. https://finkbeiner.groups.cispa.de/publications/2015-how-much-lookahead-is-needed-to-win-infinite-games/
   Shows: exponential lookahead is necessary and sufficient in ω-regular delay games. Differs: says nothing about an opponent who sees the whole future.
5. **Degrees of Lookahead in Regular Infinite Games** — Holtmann, Kaiser, Thomas, 2012. https://arxiv.org/abs/1209.0800
   Shows: for regular conditions a continuous winning operator exists iff a bounded-delay one does. Differs: the patient phenomenon is the complementary gap (a response to every input word, but no continuous one), which I did not see stated there.

## VERDICTS

- **A patient automaton exists:** NOT FOUND. The abstract phenomenon may be delay-games folklore; I found no citation, so that is opinion.
- **Least n is 4; the two witness classes; 96 patient automata; mod-3 strategy:** NOT FOUND.
- **s(n) = ∞ for n ≥ 4 by padding:** NOT FOUND; trivial given the above.
- **|W_d(n)| eventually constant in d (seen at d = 2, 6):** EASY COROLLARY of item 3's decisive-delay theorem, assuming the game encodes as a reachability game under delayed control on the subset automaton (not verified). The values 2 and 6: NOT FOUND.
- **Uniform T; reduction to (image, pending letters):** LIKELY FOLKLORE (König's lemma; the standard delayed-control queue construction, judged from snippets).
- **Count tables:** NOT FOUND, including the d = 0 row, which counts Alice's wins in item 1's game.

OEIS, queried directly, no match: rows 12, 405, 28960, 3645945 / 12, 405, 27952, 3318105 / 12, 405, 27808, 3240105 / 12, 405, 27712, 3199305; the n = 4, 5 and 6 columns. The minimal-T sequence 4, 4, 6, 7, 7, 9, 10, 10, 12, 13, 13, 15 matches A117571, expansion of (1+2x²)/((1−x)(1−x³)), from its fourth term: a generic period-3 staircase.

Web queries (16): synchronization game Synchronizer/Spoiler; the same with delay or lookahead; with partial information; on subclasses; enumeration of automata Alice wins; counts of synchronizing two-letter automata; OEIS terms; delay-game lookahead bounds; continuous uniformization without bounded lookahead; safety games won under every delay vs open-loop; safety games under delayed information; synchronizing strategies under partial observability; reset words robust to inserted letters; conformant vs contingent planning with delayed observations; adaptive vs preset synchronizing sequences with delay; directing words of nondeterministic automata.

## WHO TO ASK

A synchronization-games specialist (the Volkov group) for the automata result; a delay-games specialist (Zimmermann, Winter, Fränzle) for whether the blind-limit gap is known.

## NOT CHECKED

- No source read in full. Item 2: 19 of 34 pages read directly. Item 3: about 85%, through a summarising fetch, so its theorems are second-hand. Items 1, 5: abstract or summary. Item 4: snippets only.
- Not checked: full text of item 1 and the Fernau–Haase–Hoffmann(–Volkov) papers for small-case counts; the original Chen et al. paper; non-English literature.
