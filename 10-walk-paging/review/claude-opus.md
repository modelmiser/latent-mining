<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED — Traced in solver-b/proofs.txt:10–33. The strategy is well defined: round 1 loads {0,1}, the window stays an interval around the current page, and pair faults run while |W| ≤ k−2, so f1 = h whenever f2 ≥ 1 (both parities, and k = 2, 3). The invariant in (1) holds. In (2), the "k+j distinct pages" step is right but loosely argued: after t0 offline holds at most k−1 pages other than x0, and the k+j−1 pages after x0 all fall in (t0,t1], so ≥ j entries. The intervals are disjoint and the arithmetic h+f2 ≤ 2·opt+1−h checks. — Replacement for :30: "offline holds ≤ k−1 pages other than x0 after t0, and the k+j−1 pages beyond x0 are all requested in (t0,t1]".

Q2: CONFIRMED — :38–61. Stage 0 is legal: g pairs, 2g ≤ k, no eviction. Online needs ⌈(2g−1)/2⌉ = g more faults. The m<h stop and m=h (zero cycles) cases work. Cases (i) and (ii) check for |O| = k and k−1: C′ holds b+1 and b+2, so it keeps ≤ k−2 pages of [a,b]. The offline choice only extends a common prefix, so it is one consistent schedule. The count is h+1+2(m−h). — none.

Q3: CONFIRMED — solver-a/proofs.txt:13–18. Lemma 2's charging is injective, with one charge per prefetch load. Lemma 3 (:24–28) uses Belady only for no-prefetch schedules; the far end is a furthest-in-future page under any tie-break. Theorem U (:40–46) follows: t* exists before fault j+1, and N's cache at t* is inside V, which is inside I. — none.

Q4: CONFIRMED — :65–90. Zero-fault ranks lie in {3..k−1} with no two adjacent, so at most ⌈(k−3)/2⌉ = j−1. Rank k (even k) and the first block (odd k) land on the uncached frontier page. Stop-before-stage-2 handles odd k with m = j. The cases k = 2 and m = 1 work. — none.

Q5: CONFIRMED (B's write-up has a flaw) — A's Lemma D (:97–101) is correct. B's invariant at solver-b/proofs.txt:69 says "credit ≥ |V\R|", which is false at the start (credit 0, |V\R| > 0). The code's prunings are licensed: solver-a/game.py:28–34 and :36–44, solver-b/game.py:14–22. A also restricts offline to inclusion-maximal moves, which (a) licenses. — "credit ≥ |V_t\R_t| − |V_0\R_0|".

Q6: CONFIRMED — Both proofs count round 1. Hits are no-ops, legality includes the new request, the bounds hold on every prefix, endless free steps give infinitely many Online faults (B :61, A :56), and the least N follows. — none.

Q7: CONFIRMED (with caveat) — verifier/table_mode2.txt and table_m6.txt cover all 78 entries k = 2..14, m = 1..6, all EQ. The README table matches the formula and the logs. But these runs are mode 2, i.e. pruned (wk.cpp:11); unpruned runs only go to (8,4) and (9,3). — "computed exhaustively for k ≤ 14, m ≤ 6, with search pruning proved safe".

Q8: REFUTED (wording only) — see findings 1–4 and 6–8. The headline and status cells otherwise hold.

F: MODERATE — README.md:64 — "replayed each adversary against every online reply for k ≤ 13, m ≤ 6" is false. adva.txt and advb*.txt run in mode 1, which only tries inclusion-maximal Online replies (wk.cpp:10, :207, :219). Lemma D justifies that for game values, not for testing a fixed adversary. All replies were tried only in 12 small cases each (adva_mode0.txt, advb_mode0.txt). — high — "…against every inclusion-maximal Online reply for k ≤ 13, m ≤ 6, and every reply for 12 small cases".

F: MODERATE — README.md:131–135 — "did not find the two combined" leaves out prior-art.md:15–16: Gaysinsky–Itai–Shachnai do combine access-graph walks with prefetching (different objective). — high — "…found one work combining access-graph requests with prefetching (Gaysinsky, Itai, Shachnai 2001; execution-time objective), but none with fault counts or an exact game value".

F: LOW — README.md:35 — "computed exhaustively" hides that the large cases rely on the pruning lemma. — high — add "with pruning proved safe; unpruned to (8,4)".

F: LOW — solver-b/proofs.txt:69 — the credit invariant is misstated (Q5). — high — as in Q5.

F: LOW — solver-b/proofs.txt:75 (also reports/solver-b.md) — "agrees on all k=2..4, m=1..3" has no log in solver-b/; table_py_noprune.txt starts at k=3, m=2. — high — drop it, or cite verifier/small.txt.

F: LOW — README.md:104 — "steps one page past an end of the window, where online must fault" leaves out that the end is chosen so the outside page is uncached (proofs.txt:48). For |O| = k−1 the other end can be a hit. — high — "past an end whose outside page online does not hold".

F: LOW — README.md:95–98 — "away from the side the online cache prefetched" (it may not have prefetched) and "pays min(m,h) more faults" (it is at least that). — high — "away from an uncached neighbour of 0 … at least min(m,h) more faults".

F: LOW — README.md:101 — "(k − 1 once, for odd k)": O has k−1 pages from the end of stage 0 until the first cycle, possibly over several free steps. — medium — "(k − 1 until the first cycle, for odd k)".

F: LOW — README.md:66 — "(one entry timed out)": solver A's (7,5) timed out (solver B covers it), but solver B also timed out on four m = 6 entries. — medium — "solver A timed out on (7,5), which B covers; B timed out at k = 5..8, m = 6".

F: LOW — README.md:80–81 — "once m ≥ k/2" is not sharp for odd k; m ≥ ⌊k/2⌋ is enough. — high — "once m ≥ ⌊k/2⌋".

F: LOW — README.md:118–119 — "two independent proofs": both upper bounds analyse the same strategy, and everything is from one model family. — medium — "two different arguments".

F: LOW — README.md:120–122 — leaves out the referee's second finding on adv.py: legality is checked only on nodes popped before the first terminal. — medium — add that.

F: LOW — solver-b/proofs.txt:25 — garbled phrase "|W|>=k-1+... precisely". — high — delete it.

Not asked:
1. The README's claim that outward prefetching "loses without bound" was checked at only four (k,m) points (verifier/strat_broken.txt); there is no proof.
2. The referee report (reports/referee.md) repeats the "every Online reply" overstatement from the first finding.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
