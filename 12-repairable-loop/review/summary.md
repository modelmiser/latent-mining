<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-134150 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $4.192; this month $140.80 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $3.051, input 32 + cache write 77,002 + cache read 636,945 tokens — cache hit 89%
- claude:claude-opus-5-5: $1.141, input 34 + cache write 72,967 + cache read 605,575 tokens — cache hit 89%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q1** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | REFUTED | REFUTED | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q1
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 10 MODERATE, 28 LOW
- **MODERATE** [claude:claude-fable-5-1] `README.md:11` — "at least like 2^n/(19n)" overstates the proved rate by a factor 8; proved is (2^(n−3)−19n²)/(19n) ≈ 2^n/(152n), and even the optimised (**) gives ≈ 2^n/(130n) — high — "growing at least like 2^n/(152n)"
- **MODERATE** [claude:claude-opus-5-5] README.md:33 — says the excess grows "at least like 2^n/(19n)". The proved rate is 2^(n−3)/(19n) = 2^n/(152n), so the README overstates it by a factor of 8 (proofs.txt:113 has it right) — high — "growing at least like 2^n/(152n)"
- **MODERATE** [claude:claude-opus-5-5] README.md:65 (also proofs.txt:30) — says length 66 was "computed three ways". Solver B's search covers only universal w (solver-b/witnesses.txt:29: "L=66, w NOT universal: NOT excluded"). So only two computations cover the full claim: solver A's, which rests on P8 pruning validated only empirically, and the referee's, which rests on Lemma 5 plus the referee's own run-count lemma — high — "computed twice in full (solver A's pruned search; the referee's enumeration via Lemma 5), and for universal loops a third time by solver B"
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:98-100` — coincidences are not fewer than 2^(n−3); G4 also has linear and repeat terms — confidence: high — state the full bound from `open-growth/proofs.txt:134-158`
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:43`, `open-growth/proofs.txt:30` — solver B excluded only universal length-66 loops, so there were not three complete exhaustive exclusions — confidence: high — say “two complete searches; one universal-only cross-check”
- **MODERATE** [cursor:grok-4.7-medium] `README.md:11` — “at least like 2^n/(19n)” overstates the proved constant by 8; the closed form is ∼ 2^n/(152n) (`open-growth/proofs.txt:113`) — high — replace with “at least on the order of 2^n/(152n)”
- **MODERATE** [cursor:grok-4.7-medium] `README.md:50-51` — 9, 418, and 272775 are not values of the displayed closed form (negative, 325, 235440) — high — attribute them to `(**)` before loosening, and give the closed form its own numbers
- **MODERATE** [cursor:grok-4.7-medium] `README.md:96-99` and `open-growth/proofs.txt:162` — “fewer than 2^(n−3) coincidences” is false; the pattern sum alone is 2^(n−3)+16(k−3) — high — quote the full G4 upper bound
- **MODERATE** [cursor:grok-4.7-medium] `open-growth/proofs.txt:30` — “three independent exhaustive runs” at n = 6; solver B did not rule out non-universal loops (`solver-b/proofs.txt` is not the source: `reports/solver-b.md:18`) — high — “two exhaustive runs (solver A and the referee); solver B only for universal loops”
- **MODERATE** [cursor:gemini-3.8-flash-high] 12-repairable-loop/README.md:11 — States excess grows "at least like 2^n/(19n)", but the derived formula \((2^{n-3}-19n^2)/(19n)\) has leading term \(2^n/(152n)\), an 8x error contradicting `open-growth/proofs.txt:113` — confidence: high — Replace `2^n/(19n)` with `2^n/(152n)`.
- **LOW** [claude:claude-fable-5-1] `README.md:99-101`, `open-growth/proofs.txt:162` — "fewer than 2^(n−3) coincidences … leaves at least 2^(n−3) lone bits": the proved bad-singleton bound with k=n−4 is 2^(n−3)+16k−48+2(k−1)(e+n−1) > 2^(n−3) — high — "about 2^(n−3) (plus lower-order terms), leaving nearly 2^(n−3)"
- **LOW** [claude:claude-fable-5-1] `README.md:46,112-113` — "says nothing below n = 15 / vacuous below n = 15": the closed bound is negative at n = 15 (first positive at 16); (**) is what gives 9 at n = 15 and a trivial e ≥ 1 at n = 14 — high — "the closed form is vacuous up to n = 15; the inequality (**) gives e ≥ 9 at n = 15 and nothing new below"
- **LOW** [claude:claude-fable-5-1] `open-growth/proofs.txt:108` — "for all n" is true only because the bound is negative for n ≤ 15 (and the proof needs n ≥ 9 for the sum formula) — high — "for all n (non-trivial from n = 16)"
- **LOW** [claude:claude-fable-5-1] `README.md:64` — "stands alone between two different bits" misreads: the neighbours equal each other and differ from the lone bit — medium — "a bit whose two neighbours both differ from it"
- **LOW** [claude:claude-fable-5-1] `open-growth/proofs.txt:149` — "at most k−1 singletons" per repeat cell; proved count is k−2 (referee defect 2); harmless — high — "k−2"
- **LOW** [claude:claude-fable-5-1] `README.md:43,120` — solver B's "third" n=6 computation covers universal w only (`solver-b/witnesses.txt:28`); the README says so at :43 but :120 "three computations" could be read as three complete ones — medium — "two complete computations plus B's universal-only run"
- **LOW** [claude:claude-opus-5-5] README.md:120-123, proofs.txt:162, reports/open-growth.md:25 — say the junction coincidences number "fewer than 2^(n−3)", leaving "at least 2^(n−3)" lone bits. In fact the d-sum alone is 2^(n−3)+16k−48 > 2^(n−3), and there is a further repeat term 2(k−1)(e+n−1). The formal proof is unaffected — high — "about 2^(n−3) + O(n·e) of them, which leaves about 2^(n−3) − O(n·e) lone bits"
- **LOW** [claude:claude-opus-5-5] README.md:105 (also proofs.txt:86-87 and :102, open-growth.md:23) — "a de Bruijn cycle plus one alternating pair". Step 6 proves a wider class: a 0 added at one place and a 1 inserted into a different 00 elsewhere (the test's family2). The n=4,5 structure check tested only the local form — high — "a de Bruijn cycle with one 0-singleton lengthened and one 1 inserted into a 00 run (for n = 4, 5: one alternating pair inserted)"
- **LOW** [claude:claude-opus-5-5] README.md:26-27 — "A mechanic may then make one edit of the same kind" contradicts chain/3-question.txt:10, which allows either kind — high — "one edit of either kind"
- **LOW** [claude:claude-opus-5-5] README.md:68 and :134-135 — "says nothing below n = 15". The displayed closed formula is also vacuous at n=15 (its value is negative, first positive at n=16). Only (**) with the best k gives e ≥ 9 at n=15 — high — "(**) says nothing below n = 15; the closed formula nothing below n = 16"
- **LOW** [claude:claude-opus-5-5] proofs.txt:113 — "first non-trivial at n = 14", but e ≥ 1 there is weaker than Theorem 1's e ≥ 3 — high — "first improvement on Theorem 1 at n = 15"
- **LOW** [claude:claude-opus-5-5] README.md:94 — "216 + 20 such loops" mixes strings (n=4) with necklaces/normalised words (n=5) — medium — state the units
- **LOW** [claude:claude-opus-5-5] proofs.txt:149 — the repeat-cell term is k−1 where k−2 is proved; valid but loose — high — use k−2
- **LOW** [cursor:gpt-5.6-sol-xhigh] `open-growth/proofs.txt:25-26` — C3 is false for n=1 — confidence: high — add n≥2
- **LOW** [cursor:gpt-5.6-sol-xhigh] `open-growth/proofs.txt:141-143` — the 2^(d−1) factor count omits the residue-class argument — confidence: high — insert the referee’s sentence
- **LOW** [cursor:gpt-5.6-sol-xhigh] `open-growth/proofs.txt:148-150` — `[t+2,t+k−1]` has k−2, not k−1, cells; the bound remains valid but loose — confidence: high — use k−2
- **LOW** [cursor:gpt-5.6-sol-xhigh] `open-growth/proofs.txt:110,134,158` — application silently needs |w|>2n+2 and k=n−4≥2; small n instead follow from M(n)≥2^n — confidence: high — state these cases explicitly
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:11` — “like 2^n/(19n)” suggests the wrong explicit coefficient; the proved simple bound is 2^n/(152n)−n — confidence: high — say Ω(2^n/n)
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:72` — “216+20 loops” mixes n=4 strings with n=5 necklaces — confidence: high — label the counting units
- **LOW** [cursor:grok-4.7-medium] `open-growth/proofs.txt:149` — a repeat cell meets s in [t+2, t+k−1], which is k−2 indices, not k−1; the written bound stays valid because it is larger — high — “at most k−2”
- **LOW** [cursor:grok-4.7-medium] `open-growth/proofs.txt:89` — “length n (as in Step 5)” cites the n+2 conclusion; length n is the de Bruijn case mult(0^n)=1 — high — “length n, by the same count as Step 5 with mult(0^n)=1”
- **LOW** [cursor:grok-4.7-medium] `open-growth/proofs.txt:108` — “for all n”, while k = n−4 is allowed only for n ≥ 6; for n < 6 the right-hand side is negative — high — “for all n ≥ 6 (and, being negative, for n < 6 as well)”
- **LOW** [cursor:grok-4.7-medium] `open-growth/proofs.txt:146` — “len(w) − #distinct repeats” does not say #distinct n-words; the formula in line 135 does — medium — “len(w) − (number of distinct n-words) repeat cells”
- **LOW** [cursor:gemini-3.8-flash-high] 12-repairable-loop/README.md:63 — Defines a lone bit as "a bit that stands alone between two different bits"; differing neighbours mean one neighbor matches the bit, forming a run of length \(\ge 2\) rather than a singleton — confidence: high — Replace with "between two equal bits opposite to it".
- **LOW** [cursor:gemini-3.8-flash-high] 12-repairable-loop/README.md:99-101 — States coincidences across the gap number "fewer than 2^(n-3)", but the coincidence sum evaluates to \(2^{n-3} + 16k - 48 > 2^{n-3}\); the surplus comes from subtracting it from \(2^{n-2}-2\) total singletons — confidence: high — Clarify that coincidences are \(2^{n-3} + O(n)\) against \(\approx 2^{n-2}\) singletons.
- **LOW** [cursor:gemini-3.8-flash-high] 12-repairable-loop/open-growth/proofs.txt:141-143 — Asserts without proof that \(w[-1] \ne w[0]\) is non-trivial leaving at most \(2^{d-1}\) factors; proof gap exists in original text and was only patched in referee report — confidence: high — Incorporate referee's proof that \(-1\) and \(0\) lie in \([p, d-1]\) with distinct residues mod \(d\).
- **LOW** [cursor:gemini-3.8-flash-high] 12-repairable-loop/open-growth/proofs.txt:150 — Claims repeat cell \(t\) accounts for at most \(k-1\) singletons in \([t+2, t+k-1]\), but the integer interval contains exactly \(k-2\) elements — confidence: high — Replace `k-1` with `k-2`.
- **LOW** [cursor:gemini-3.8-flash-high] 12-repairable-loop/open-growth/proofs.txt:108 — Claims the closed bound holds "for all n", but \((2^{n-3}-19n^2)/(19n)\) is negative for all \(n \le 15\) (yielding \(-179/285\) at \(n=15\)); \(e \ge 9\) at \(n=15\) requires (**) with \(k=11\) — confidence: high — Clarify that closed bound is non-trivial for \(n \ge 16\), while unsimplified (**) yields \(e \ge 9\) at \(n=15\).

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
