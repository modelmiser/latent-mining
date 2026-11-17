# Link 3 (reply, verbatim)

## 1. KNOWN

Flaw: at n = 2 the move right is asymptotically worthless. The no-move adversary for sizes {1, 2} allocates M 1s, frees every other one, then allocates 2s. All holes are single cells, so the 2s need about M/2 new cells and the heap is about 3M/2. This matches Robson's bound for power-of-two sizes as I recall it (unsure of the exact additive term). The strategy frees only 1s, so no allowance is ever granted, and mu_2 = 3/2 with or without the right. Sub-questions 1 and 2 are therefore dead, up to an additive constant.

For n = 3 this escape fails. An adversary who frees only 1s gets about 5M/3 (keep every third 1, then allocate 3s). The stronger-looking attack through 2s (free alternate 2s, about 7M/4 without moves) is defeated by the granted 1-moves. The no-move constant for {1, 2, 3} is unknown to me.

## 2. FINAL QUESTION

Repair, one change: the smallest case becomes sizes {1, 2, 3}, and the right is measured against two no-move benchmarks.

Cells are 0, 1, 2, .... Fix M >= 3. Blocks have size 1, 2 or 3. An adversary who sees the layout issues requests forever:
- allocate(s): legal only if (live cells) + s <= M. The manager must immediately place the block on s contiguous free cells.
- free(b): the live block b vanishes.

There are three games on these requests.
- **N (no moves):** blocks never move.
- **F (move on free):** immediately after free(b) with |b| = s, the manager may perform a sequence of moves whose moved sizes total at most s - 1. A move lifts one live block and puts it on any contiguous cells that are free once it is lifted. The cells b vacated count as free. Unused allowance is lost. So freeing a 1 grants nothing, freeing a 2 grants one 1-move, and freeing a 3 grants one 2-move or two 1-moves. 3s never move.
- **O (only 1s freed):** game N, except the adversary may free only blocks of size 1.

For G in {N, F, O}, let h^G(M) be the least H such that some manager serves every legal infinite request sequence inside cells 0 ... H-1. Trivially h^O(M) <= h^F(M) <= h^N(M), because in O no allowance ever arises and in F the manager may decline to move.

**Headline.** Which of the following holds for all sufficiently large M?
- (a) h^F = h^N: the right is worthless.
- (b) h^F = h^O: the right is so strong that the adversary's best play never triggers it.
- (c) h^O < h^F < h^N.

Give the answer in asymptotic form: with mu^G = limsup h^G(M)/M, determine whether mu^O < mu^F < mu^N, and find mu^F exactly.

**Supporting sub-question.** Tabulate h^N, h^F, h^O for 3 <= M <= 8, and report the least M at which each inequality is strict.

**Guess:** (c), with mu^F strictly below mu^N and nearer to mu^O (perhaps 5/3 = mu^O < mu^F). The adversary can free 2s harmlessly only when few 1s are live, which costs him the fine-grained fragmentation he needs.

## 3. WHY IT IS PROBABLY UNCHARTED

Work on compaction budgets (Bendersky–Petrank style, as I recall; unsure of details) grants budget as a fraction of allocated volume and studies asymptotics in n. Here the budget is s - 1 per free, triggered by the adversary, at fixed tiny n. Exact no-move values for non-power-of-two size sets such as {1, 2, 3} are, I believe, not tabulated.

Doubts: the answer may be a dull (c) with ugly constants. The limits may not exist, hence limsup. Small M may be dominated by rounding.

## 4. HOW TO CHECK AN ANSWER

Fix (G, M, H). A position is a tiling of cells 0 ... H-1 by free cells and blocks of size 1, 2, 3 with live cells <= M. Block identities are irrelevant. The count grows like 2.55^H: about 5·10^5 at H = 14 and 3·10^6 at H = 16. That makes M <= 8 feasible, and M = 9 borderline.

Compute the greatest fixpoint of safe positions: a position is safe if every legal allocate has a safe placement, and every free has a move sequence within allowance (F only) that leads to a safe position. h^G(M) is the least H for which the empty layout is safe.

A general claim needs an explicit adversary strategy (lower bound) and a manager invariant preserved under all requests (upper bound), both agreeing with the tables.
