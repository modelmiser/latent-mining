<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the follow-up instance (A(n) >= 3n - 6), verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Report: growth of the gossip game (experiment 23)

The harness refused the write of `REPORT.md` (subagents may not write report files), so this text is the report. `PROOFS.md` and the programs were written.

## Strongest statement

**A(n) ≥ 3n − 6 for every n ≥ 3: PROVED** (`PROOFS.md`, Theorem 1). The previous general bound was 2n − 3. The bound is the exact value for n = 3..6 and falls short from n = 7 (16, 19, 23, 27 against 15, 18, 21, 24).

**No upper bound was obtained: FAILED.** Neither A(n) ≤ c·n nor any o(n²) bound. Whether A(n)/n is bounded is still open.

## Goal by goal

**(2) Lower bound: PROVED.** Delayer names one agent c for n − 2 rounds (a star, one agent p left untouched). Then:
- R1: while no expert exists, name an agent that neither knows the whole star nor knows p.
- R2: when no such agent is left, name p.
- R3: once experts exist, name an active expert.

Proof idea: after the star, every knowledge set is a member of a chain, with or without p. Each agent therefore has two tasks (reach the top of the chain, learn p), 2n − 3 tasks in all. A call completes at most two, and under the rule exactly one call of the second phase completes two. So the second phase has at least 2n − 4 calls.

COMPUTED: `dcheck.cpp` plays the rule against every Hastener reply. The minimum is exactly 3n − 6 for n = 3..11 (`dcheck.log`); n = 12 ran out of memory. So this rule gives no more than 3n − 6.

**(1) Upper bound: FAILED.** What I have is negative:
- **Most knowledgeable legal partner:** no better than the trivial bound. COMPUTED n(n−1)/2 for n = 5..8 (`hast.log`, policies 1 and 3); the reason is a SKETCH (S1 in `PROOFS.md`).
- **Lowest-numbered legal partner:** loses about n²/4. SKETCH only (S2), not computed.
- **Two greedy rules**, COMPUTED against an optimal Delayer: "least-known partner, then largest union" gives 9, 13, 17, 22, 26 for n = 5..9; "largest union first" gives 9, 13, 18, 23 for n = 5..8. Neither is optimal, and the runs are too short to call linear or not.
- **The one tool** (Lemma 2, which is solver-b's Lemma 3): an expert whom no non-expert knows ends the game in one call per remaining agent. I found no Hastener rule that provably produces such an expert in O(n) calls. The obstacle is the clique: agents with identical knowledge are pairwise dead, and each needs its own call with an outsider.

**(3) Linear or superlinear: evidence only, leaning linear.**
- COMPUTED (`star.log`, n ≤ 8): after a star with k leaves the game value is unchanged as long as at least two agents are untouched. With one untouched it is 3n − 6; with none, 2n − 3. So the excess over 3n − 6 comes from having two or more untouched agents.
- SKETCH, from solver-b's table V(m,r) (m agents sharing m secrets, r untouched), not my data: the columns fit slope 1, 2, 7/3, 5/2 in m for r = 1..4, i.e. 3 − 2/r. A hand analysis of r = 3 gives the same 7/3: Delayer can make the members of all but two groups call three times. If the pattern holds, a clique member costs under 3 calls however many outsiders there are.
- A(n) − 4n = −10, −11, −12, −12, −13, −13, −13 for n = 4..10.

**Conjecture.** A(n) = 4n − 13 for all n ≥ 8; in particular A(11) = 31 and A(12) = 35. Weaker form: A(n)/n → 4. It rests on three data points (n = 8, 9, 10) and the V-table increments in r (mostly 4, with a few 3s and 5s). Settling A(11) would test it.

## What failed and why

- **Linear upper bound.** Every invariant I tried lets Delayer rebuild a clique after each newcomer, which costs a quadratic number of repeated lifts. I tried "chain plus a fresh top" and disjoint "worlds" each satisfying Lemma 2's hypothesis. The fix that appears to work in the clique analysis (send demoted clique members to different untouched agents) breaks those invariants, and I did not find one that survives it.
- **Extending Theorem 1 to two untouched agents**, which is where the true value lives. The task count no longer bounds the number of calls, because one call can then complete many tasks. Not attempted beyond that.

## Mechanical checks seen failing

- `dcheck` variant 2 (after the first expert, name the lowest active agent instead of an expert): 8, 13, 18 for n = 5, 7, 9, below 3n − 6.
- `dcheck` variant 3 (star to n − 1 leaves, the old P1 strategy): 7, 11, 15, below 3n − 6.
- Variant 1 (R2 names c instead of p) was meant to fail and did not: it still gives 3n − 6. The reason is that c's only legal partner is p, so it is the same call. It is not a test.
- `g.cpp` reproduces A(2..7) = 1, 3, 6, 9, 12, 16. I did not run a deliberately wrong input through `g.cpp`; its only cross-check is agreement with the published values.

## What a reader must still trust

- The proof of Theorem 1 has been read by nobody but me. Its key steps are the closure of the "chain, with or without p" family, the two ways a call can complete two tasks, and Lemma B (an active expert always exists in the last stage).
- `dcheck` treats the n − 2 partners of phase 1 as interchangeable and tries only the lowest. With all partners tried (`FULLPH1=1`) the result is the same for n = 5..8.
- `g.cpp` relabels positions before memoising, and the Hastener policies act on the relabelled position. That is a legitimate strategy, but the numbers belong to that tie-breaking.
- The conjecture and the 3 − 2/r pattern are not established.
- Compute hygiene: no two of my runs overlapped. Three runs were stopped early by PID (a long policy run at n = 10 and two checker loops). The "largest union first" rule at n = 9 and both greedy rules at n = 10 were never completed.

## Files

All in `(staging)/`:
- `PROOFS.md` (Theorem 1, Lemma 2, sketches S1–S3)
- `dcheck.cpp`, `dcheck.log` (strategy checker and its output)
- `g.cpp`, `hast.log` (minimax and Hastener policies)
- `star.py`, `star.log` (values after a star)
