# latent-mining

Experiments in getting a language model to produce a small result it could not
recall: a question is invented by a chain of model instances, then solved and
attacked by other instances that share no context.

Everything here was produced by instances of one model family (Claude). Read
each experiment's status table before relying on anything: "proved by hand"
means a model wrote the proof and other instances of the same model checked it.

## Method

1. **Seed.** One hard, open-ended question.
2. **Chain.** Three instances in turn. Each sees only the previous question and
   must hand on a successor that is fully formal, has small cases a program can
   settle exactly, and has a general form whose answer is a formula or theorem.
   The last link reduces it to one headline question.
3. **Solve.** Two instances work independently: exact computation on small
   cases, then conjectures, then proofs, each tagged computed / proved / guessed.
4. **Attack.** A referee instance reads the proofs line by line, re-implements
   the computation from the definition, and runs checks that are able to fail.
5. **Prior art.** A separate instance searches the literature.
6. **Follow-up.** Open ends go to further instances, and short proofs into Lean.

The checkability rule in step 2 is what makes the rest possible. Without it the
chain produces prose that nothing can test.

## Experiments

| # | Folder | Subject | Headline | Status |
|---|---|---|---|---|
| 1 | [01-late-thermostat](01-late-thermostat/) | regulating a level from a delayed one-bit signal | a blind clock is within 1 of a controller with full memory | headline proved by hand for every delay, not machine-checked; one supporting bound is in Lean; the exact clock value for odd delays is open past 13 |
| 2 | [02-move-on-free](02-move-on-free/) | a memory allocator that may move blocks only when something is freed | how much that right is worth | open. Proved by hand: an allocator that packs its first blocks together cannot use the right to get below 7/4 of live memory asymptotically, the best lower bound known with no moves at all; with only the smallest blocks freed the cost is exactly 5/3 asymptotically. Nothing machine-checked |
| 3 | [03-switch-cost](03-switch-cost/) | online scheduling with a one-tick penalty for leaving a job unfinished | over a long horizon, no online rule beats never preempting, even against a benchmark that pays the same penalty | proved by hand for every maximum job length, not machine-checked; the two general lower bounds were each written by one instance and re-derived once; small cases confirmed by exhaustive search |
| 4 | [04-lies-that-cost](04-lies-that-cost/) | twenty questions against a liar who must be honest for a while after each lie, longer after an evenly split question | if the honest stretch is at least twice the smaller side of the split, unlimited lying is no better than a single lie, for every number of candidates | proved by hand apart from a finite table settled by two programs; the upper bound was written by one instance and read by one other; not machine-checked |
| 5 | [05-ring-destruction](05-ring-destruction/) | processes on a ring flooding what they know while an adversary destroys up to k messages a round, never the same link twice running | the adversary can hold off the first fully informed process until round n − ⌈n/k⌉ and no longer (3 ≤ k < n); with k ≤ 2 it gains nothing | proved by hand, not machine-checked; the exact upper bound was written by one instance and read by one other; computed for n ≤ 12 |
| 6 | [06-one-feedback-bit](06-one-feedback-bit/) | sending bits through a channel that may delete one, with a single yes/no feedback bit at a moment fixed in advance | feedback straight after the first bit is not always best: at 6 bits, feedback after the second bit carries 13 messages against 12 | the counterexample is an explicit list checkable in a few lines; the table for up to 8 bits is by exhaustive search; the general pattern is open |
| 7 | [07-charged-pebbles](07-charged-pebbles/) | the reversible pebble game on a line, with each pebble allowed to be forgotten k times | one pebble can be delivered cleanly to exactly node (pk + 2)·2^(p−2) | proved by hand for every p and k, not machine-checked; exhaustive search agrees in every case reached. Not new in substance: the upper bound follows from a recurrence in a preprint of October 2026 (see the folder) |
| 8 | [08-patient-automaton](08-patient-automaton/) | two players steering an automaton whose state is unknown, one of them seeing the other's moves late | seeing them arbitrarily late still beats not seeing them: a 4-state automaton is winnable at every finite delay and not when blind | example proved by hand and its strategy checked by programs; that no smaller automaton does this is by exhaustive search; not machine-checked as a proof |
| 9 | [09-slipped-comparator](09-slipped-comparator/) | sorting networks of adjacent comparators in which one comparator may be wired one position off | exactly n − 1 extra comparators are needed, and a slipped comparator is no worse than a missing one | proved by hand for every n ≥ 3, not machine-checked; computed for n ≤ 7; one of two upper-bound proofs was found wrong in review; the upper bound may be known in Coxeter combinatorics |
| 10 | [10-walk-paging](10-walk-paging/) | a cache serving requests that walk along a line of pages, where a fault may also load one neighbour | against walks an offline cache serves with m faults, the online cache can be held to exactly max(m + 1, 2m + 1 − ⌊k/2⌋) faults and no fewer | proved by hand, both bounds by two different arguments, not machine-checked; computed for cache sizes up to 14 |
| 11 | [11-glimpse-budget](11-glimpse-budget/) | a cop hunting an unseen robber on a binary tree with a limited number of looks | a look that shows only the direction needs h − 1 uses, one that shows the position ⌊h/2⌋, and they differ from height 3 | proved by hand for every height on two small computed base cases, not machine-checked; one lower bound has a single author; computed for heights up to 8 |
| 12 | [12-repairable-loop](12-repairable-loop/) | a circular bit string containing every n-bit code that must be restorable with one edit after an adversary deletes or doubles a bit | two extra bits suffice for n = 3, 4, 5, never for n ≥ 6, and the extra length needed is unbounded, at least of order 2^n/n | small cases by exhaustive search; the two general theorems proved by hand by one instance and read by one other, not machine-checked; exact values unknown from n = 6 and no good upper bound |
| 13 | [13-blind-stacks](13-blind-stacks/) | sorting numbers through small stacks without ever seeing a value, learning only whether a move was accepted | with two stacks of two, the number of arrival orders one strategy handles obeys N(n) = 5N(n−1) − 5N(n−2); every order can be handled exactly when there are at most k + 1 items for k stacks | recurrence proved with a small exact computation, two ways, on a hand-proved reduction tested up to 10 items; not machine-checked |
| 14 | [14-forest-orientation](14-forest-orientation/) | orienting the edges of a growing forest with at most two out-edges per vertex, paying for each edge reversed | an adversary can force n/3 reversals up to a term of order log n, and no more: n/3 − (7/3)·log₂(n+1) + 5/6 ≤ T(n) ≤ (n − 1)/3. The upper bound is the algorithm of Gupta, Kumar and Stein (2014), for which they prove 2n, with a tie-breaking rule; with ties broken against it that algorithm can be made to pay about n/2; write-up in NOTE.md | both bounds are machine-checked in Lean 4 (core only, one 89-line specification audited by a five-seat panel); small cases by exhaustive search; no human mathematician has checked it, and the literature search was done by models |

## License

MIT, copyright ModelMiser. See [LICENSE](LICENSE).
