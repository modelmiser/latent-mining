<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-171359 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $3.959; this month $144.76 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.966, input 42 + cache write 76,052 + cache read 847,163 tokens — cache hit 92%
- claude:claude-opus-5-5: $0.993, input 28 + cache write 65,183 + cache read 449,717 tokens — cache hit 87%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q5** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q8** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q5, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 7 MODERATE, 29 LOW
- **No parseable F: lines from:** claude:claude-opus-5-5 — read their reports by hand
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:39` — “nearest” strengthens Lemma 1 to shortest paths, which it never proves — confidence: high — replace with “an unsaturated vertex, with all preceding path vertices saturated.”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:80-84` — only main-phase equal-index merges preserve the displayed potential; cleanup incurs bounded loss — confidence: high — distinguish main-phase preservation from cleanup loss.
- **MODERATE** [cursor:grok-4.7-medium] `README.md:57-60` — the simple attack is told as if the block join is free and the total is one flip per block; for two blocks the proved total is 1, not 2, and a flip inside the block leaves no saturated vertex to name — high — "k blocks force k−1 flips. If the block join is free it creates a saturated vertex; if not, that flip is already the payment the invariant charges."
- **MODERATE** [cursor:grok-4.7-medium] `README.md:81-82` — "each merge preserves" paid+F≥|C|/3 is false for cleanup: merging a high-index tree with a singleton at index 0 can make P negative (the log term is exactly that drop) — high — "Same-type merges preserve the potential. Cleanup merges can lower it by at most 2/3 each."
- **MODERATE** [cursor:grok-4.7-medium] `open-limit/proofs.txt:111-113` — "no one-witness strategy proves more than 1/3" is stronger than Theorem 3, which bounds only the pair game; a witness that is not one of the two endpoints is outside that game (`:142-146`) — high — "No strategy in this pair game proves more than L≥1/3."
- **MODERATE** [cursor:gemini-3.8-flash-high] `solver-b/proofs.txt:11-13` — Lemma 1 claims normal-form optimality against every adversary, but its proof fixes the adversary's move sequence; it fails to cover adaptive adversaries without a shadow simulation — confidence: high — Qualify Lemma 1 to fixed move sequences and explicitly invoke the shadow orientation transfer for adaptive strategies.
- **MODERATE** [cursor:gemini-3.8-flash-high] `open-limit/proofs.txt:81` — Remark (i) claims the invariant is inductive from every position, but non-normal replies destroy witness types in real positions — confidence: high — State that inductiveness holds from every position in the normal-form game or on the shadow orientation.
- **LOW** [claude:claude-fable-5-1] `solver-b/proofs.txt:12` — Lemma 1 stated "against every Adversary strategy" but proved per fixed pair sequence; conclusion holds via L being a function of pair history, unstated — high — restate as per-sequence plus one-line history argument.
- **LOW** [claude:claude-fable-5-1] `README.md:93-104`, `reports/open-limit-referee.md:15` — the "missing step" belongs to Lemma 1's last sentence and is closable without any shadow; README presents the shadow as required — high — "one way to close the omitted step".
- **LOW** [claude:claude-fable-5-1] `README.md:52` — "exceeds B(n) only from n = 189": first exceedance is n = 179; 189 is the permanent threshold — high — see Q5 wording.
- **LOW** [claude:claude-fable-5-1] `README.md:60-61` — "one flip per block": k blocks give k−1 flips — high — "one flip per block after the first".
- **LOW** [claude:claude-fable-5-1] `README.md:116-117` and `:46` — 30 ≤ n ≤ 35 also follow from certificates (T(30) ≥ 7, T(32) ≥ 8 + monotonicity); only 36–63 rest solely on the pair DP — high — narrow the range.
- **LOW** [claude:claude-fable-5-1] `README.md:44` — "two routes sharing no lemma": both rest on a simulation lemma (P1 / Lemma 1) with identical proof idea and the same adaptive-Adversary subtlety — medium — "sharing no stated lemma, though both use a simulation reduction".
- **LOW** [claude:claude-fable-5-1] `open-limit/proofs.txt:111-113` — "no strategy that tracks one witness … proves more than 1/3" overstates Theorem 3 (abstract game with Lemma B's cost bounds only) — high — scope to the pair game.
- **LOW** [claude:claude-fable-5-1] `open-limit/proofs.txt:81` — Remark (i) "inductive from every position": only the step is position-free, and only under normal replies — medium — qualify.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:13-14` — wording suggests comparison against the same adaptive strategy, while the proof compares identical pair sequences — confidence: high — state pointwise sequence domination.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:45,91-103` — calls shadow transfer a missing necessary step; it is correct but redundant for game-value equality — confidence: high — call it an explicit constructive transfer.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:9` — omits max(0,·), giving negative “optimal” values for n≤3 — confidence: high — write B(n).
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:60` — k blocks force k−1 merge payments, not one per block — confidence: high — write “one per inter-block merge.”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:110` — not everything beyond the small table relies on Lemma 1; the upper bound, block proof, T(16), and solver-A’s T(30) route do not — confidence: high — restrict this warning to the pair/open-limit route.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `open-limit/proofs.txt:111-113` — “no one-witness strategy” exceeds what the defined pair abstraction proves — confidence: high — say “no strategy represented by this pair game.”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:131` — Hiken–Wein’s quoted no-lower-bound statement concerns update time, but this context suggests flip lower bounds — confidence: medium — specify “nontrivial update-time lower bound.”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:52` — “only from n=189” incorrectly suggests first crossing — confidence: high — use the wording above.Additional findings omitted from my original list:
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:51-53` — “certified values” mislabels several n=30…66 reduced-game computations — confidence: high — say “computed and certified lower bounds.”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:57-60` — joining tails either costs a flip or leaves saturation; it does not unconditionally create saturation for free — confidence: high — state the two alternatives.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:71-77` — “every reply” needs the normal-form/shadow qualifier — confidence: medium — add that qualifier.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:86-89` — only solver B’s files document the failed fixed 4-ary schedule — confidence: high — attribute that obstacle only to solver B.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:123-130` — prior-art wording omits amortized results, describes a total \(3(n-1)\) bound as “three flips per edge,” and says eleven papers although 17 were text-scanned (`prior-art-deep.md:5-18`) — confidence: high — state those scopes precisely.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:137-139` — says the n=11 table is omitted, but `solver-a/table_n11.txt` is present — confidence: high — list only n=12 and n=13 as omitted.
- **LOW** [cursor:grok-4.7-medium] `open-limit/proofs.txt:136` — "an optimal Algorithm must let h grow" drops the normal-reply hypothesis of Corollary 7 (`:130`) — medium — "An optimal normal-form Algorithm must let h grow."
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:14` — "the same Adversary sequence" is easy to read as non-adaptive only; the invariant itself does not use that restriction — medium — "on the pairs named in the coupled game, which may depend on D_L."
- **LOW** [cursor:grok-4.7-medium] `README.md:110` — "everything above the small table" reads as if the n≤15 row does not rest on the normal form, but n=14 and 15 were budget-searched with normal replies only (`reports/referee.md:11`) — medium — "The n=14 and n=15 entries use normal-form search plus Lemma 1; the machine check of Lemma 1 stops at n=11."
- **LOW** [cursor:gemini-3.8-flash-high] `README.md:39` — Normal form describes path reversal as ending at the "nearest" free slot, but Lemma 1 allows any path through saturated vertices ending at the first unsaturated vertex — confidence: high — Change "nearest vertex with a free slot" to "first unsaturated vertex along the path".
- **LOW** [cursor:gemini-3.8-flash-high] `open-limit/proofs.txt:111-113` — Claim that no one-witness strategy can prove more than $L \ge 1/3$ is proved only for the pair game reduction, not for all possible single-witness real-forest strategies — confidence: high — Restrict the impossibility claim to the pair game as defined (as `README.md:46` already does).
- **LOW** [cursor:gemini-3.8-flash-high] `open-limit/proofs.txt:134` — Corollary 7 claims an optimal algorithm must let $h$ grow without bound, but this is established only for normal-form algorithms — confidence: high — Add the qualification "under normal-form replies".
- **LOW** [cursor:gemini-3.8-flash-high] `README.md:43` — Upper bound $T(16) = 3$ rests on a single unverified solver run (`solver-a/run_n16.txt`) that neither referee verified due to memory limits — confidence: high — Add explicit caveat in text that $T(16) \le 3$ is unverified by an independent program.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
