# Link 2 (reply, verbatim)

## KNOWN

All of this is from thought alone; nothing was computed.

- **Well-posed.** h(M) is finite and at least M, since a no-move manager already needs only bounded space.
- **Sizes {1, 2} are trivial.** Put 1s at the left end and 2s at the right end. Each free is repaired by moving that class's innermost block into the hole, so h = M.
- **Sizes {1, 2, 3} are not obviously trivial, but fragile.** The same repair keeps every class compact, but three classes share a line with only two ends. A two-ended scheme I tried (3s on the left with 1s stacked on them, 2s on the right) leaves a growing gap under the 1s. I could neither find an M + O(1) scheme nor rule one out. 1s as universal filler may rescue it.
- **The flaw.** "Refill the hole with an equal-sized block" is always available and does most of the work, so the question may collapse.
- **Literature.** The no-move version is classical (Robson); exact constants: unsure.

## SUCCESSOR QUESTION

**The one change:** freeing a block of size s earns s − 1 units of movement, not s. A free can then never pay to move a block of its own size, so the equal-size refill is gone, and 1s earn nothing.

**The game.** Cells are 0, 1, 2, .... Fix n >= 2 and M >= n. Block sizes are {1, ..., n}. An adversary who sees the whole layout issues requests one at a time:

- **allocate(s)**, s in {1, ..., n}: legal only if live cells + s <= M. The manager must immediately place the block on s contiguous free cells. It may not move anything.
- **free(b)**, b a live block of size s: the block vanishes. Immediately, before the next request, the manager may perform a sequence of moves whose moved sizes total at most s − 1. Unused allowance is lost.

A move lifts one live block and places it on any contiguous cells that are free once it is lifted, so overlapping slides are allowed. The just-vacated cells count as free.

Consequences: a block of size n is never moved. Freeing a 1 allows nothing. Freeing a 2 allows moving one 1.

Let h_n(M) be the least H such that some manager serves every infinite legal request sequence within cells 0 ... H−1.

1. For n = 2, determine h_2(M) for 2 <= M <= 10.
2. For n = 2, give a closed form for h_2(M), or at least mu_2 = lim h_2(M)/M. Decide whether mu_2 = 1, mu_2 equals the no-move constant for sizes {1, 2}, or lies strictly between.
3. For n = 3, determine h_3(M) for 3 <= M <= 8.
4. General form: determine mu_n = lim h_n(M)/M. Decide whether mu_n is bounded as n grows, and whether it is strictly below the constant of the same game with no moves at all.

## WHY IT IS PROBABLY UNCHARTED

Compaction budgets in the literature are amortised against allocation volume and can be banked. Here the budget is per-free, cannot be banked, and is strictly sub-size. That gives a hierarchy where each class can only be repaired by frees of larger classes, and the largest class is immovable. I know of no theorem covering this.

Doubts:
- For n = 2, the answer might coincide with the no-move bound, because the adversary can simply avoid freeing 2s when it hurts. That would make part 2 a corollary of Robson-style analysis.
- The limits might not exist.

## HOW TO CHECK AN ANSWER

This is a finite safety game.

- **States:** layouts on H cells, each cell free or covered by a block, with block boundaries recorded.
- **Adversary:** picks any legal request.
- **Manager:** picks a placement, or a move sequence within the allowance.
- **Solve:** compute the manager's winning region by backward fixpoint, deleting states where some request has no surviving response. h_n(M) is the least H whose empty layout survives.

For n = 2, M <= 10 and H <= 15 this is under a million layouts.

A claimed formula must match every computed value. A claimed mu needs an explicit manager invariant and an explicit adversary strategy.
