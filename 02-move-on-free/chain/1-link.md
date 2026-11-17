# Link 1 (reply, verbatim)

## KNOWN

With no moving, Robson (1970s) gave tight bounds: for power-of-two block sizes the worst-case heap is about M(1 + ½ log₂ n), and for arbitrary sizes it is Θ(M log n). The budgeted version as stated — moving cost at most c times the total allocated — is, I believe, the "partial compaction" model of Bendersky and Petrank (POPL 2011), sharpened by Cohen and Petrank (around 2013). They give upper and lower bounds on H(M, n, c) that are asymptotic and not matching; I am unsure of the exact forms. So the original is charted in outline but not exactly. It has no easy answer.

## SUCCESSOR QUESTION

Cells are 0, 1, 2, …. Block sizes are restricted to {1, 2, 3}. Fix an integer M ≥ 3.

An adversary issues requests one at a time, seeing the whole layout:

- **allocate(s)**, s ∈ {1, 2, 3}: legal only if live cells + s ≤ M. The manager must immediately place the block on s contiguous free cells.
- **free(b)**: removes a live block b of size s. Immediately afterwards, before the next request, the manager may perform a sequence of moves whose total moved size is at most s. Unused allowance is lost; it cannot be banked.

A move lifts one live block and puts it on any contiguous cells that are free once the block is lifted, so overlapping slides are allowed. No moves are allowed at any other time.

Consequence: freeing a 1 lets the manager move at most one 1; freeing a 2 lets it move one 2 or up to two 1s; freeing a 3 lets it move one 3, or a 2 and a 1, or up to three 1s.

Let h(M) be the least H such that some manager can serve every infinite legal request sequence using only cells 0 … H−1.

1. Determine h(M) for 3 ≤ M ≤ 12.
2. Determine λ = lim h(M)/M, or prove it does not exist. In particular, decide whether h(M) = M + O(1).
3. General form: the same game with sizes {1, …, n} gives λₙ. Find λₙ as a function of n, and say whether it is bounded.

## WHY IT IS PROBABLY UNCHARTED

Known partial-compaction bounds use a bankable budget amortised against allocation. Here the allowance is tied to each free, is non-bankable, and is size-gated, so a stream of small frees can never pay for moving a large block. That breaks the standard amortised-compaction upper bounds, and Robson's adversary no longer applies unchanged, since holes can be refilled.

Doubts: with sizes {1, 2} segregating the two classes at opposite ends gives h = M trivially. I have not checked whether a cleverer three-class scheme also gives M + O(1) for {1, 2, 3}; if so, only part 3 is interesting. Similar "move on delete" rules may exist in storage-reallocation literature.

## HOW TO CHECK AN ANSWER

For fixed M and H this is a finite safety game. Positions are layouts of blocks of size 1–3 in H cells with at most M live cells. The adversary picks a request; the manager picks a placement, or a move sequence after a free. Compute the manager's winning region by backward fixpoint: iteratively delete positions where some legal allocation has no surviving placement, or some free leaves no surviving reachable layout. h(M) is the least H whose empty layout survives. The state space is small for M ≤ 12. Asymptotic claims need an explicit manager strategy and an explicit adversary strategy with matching bounds.
