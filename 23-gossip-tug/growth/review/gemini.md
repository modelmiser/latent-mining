<!-- Archived by the organiser on 2026-11-21. Report of the Gemini seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `23-gossip-tug/chain/3-question.txt:1-10`, `23-gossip-tug/growth/PROOFS.md:3-7,40-43,73-82` — none
Q2: CONFIRMED — `23-gossip-tug/growth/PROOFS.md:12-27` — none
Q3: CONFIRMED — `23-gossip-tug/growth/PROOFS.md:29-39` — none
Q4: CONFIRMED — `23-gossip-tug/growth/PROOFS.md:40-88` — none
Q5: CONFIRMED — `23-gossip-tug/growth/PROOFS.md:90-94` — none
Q6: CONFIRMED — `23-gossip-tug/growth/dcheck.cpp:1-49`, `23-gossip-tug/growth/dcheck.log:1-16` — none
Q7: CONFIRMED — `23-gossip-tug/README.md:15-18,55,98-120,123-128`, `README.md:53` — none

ROUNDS MODE.
F: LOW — `23-gossip-tug/README.md:79` — Section heading "Why at least 2n − 3 in general" does not mention that the bound is superseded for \(n \ge 4\) by the \(3n - 6\) proof directly following it — confidence: high — Retitle to "Why at least 2n − 3 in general (superseded for n ≥ 4)" or add a brief cross-reference note.
F: LOW — `23-gossip-tug/growth/PROOFS.md:30` — Range \(l_1..l_{m-1}\) is empty when \(n = 3\) (\(m = 1\)), leaving \(p\) as the sole non-top agent, which is correct in count (\(m = 1\)) but degenerates syntactically — confidence: high — Clarify with "(empty when n = 3, leaving only p)".
F: LOW — `23-gossip-tug/growth/PROOFS.md:100` — Lemma 2 is presented as a standalone numbered lemma immediately following Theorem 1 despite having no role in the lower bound proof — confidence: medium — Subordinate Lemma 2 under the "Upper bound: NOTHING PROVED" section.

### Important points not asked about

1. **Strategy \(D^*\) is tight at \(3n - 6\) and cannot explain \(A(n)\) for \(n \ge 7\)**: `growth/dcheck.log` confirms \(D^*\) achieves exactly \(3n - 6\) calls for all \(n = 3, \dots, 11\). However, true optimal play diverges upward starting at \(n = 7\) (\(A(7) = 16 > 15\), \(A(8) = 19 > 18\), etc.). `growth/star.log` confirms that achieving these higher values requires leaving at least two agents untouched during Phase 1.
2. **Checker scalability limit**: In `growth/dcheck.log`, running \(n = 12\) threw `std::bad_alloc` and dumped core because memoizing raw state vectors in `std::map` exhausts RAM once the reachable state space exceeds \(\approx 6.4 \times 10^6\) positions.
3. **Failure of greedy Hastener heuristics**: `growth/PROOFS.md:109-130` records that natural Hastener rules ("lift to the top", "first fit", greedy union) fail to establish sub-quadratic upper bounds, with policy S1 allowing the Delayer to force the trivial maximum \(\frac{n(n-1)}{2}\) calls.
