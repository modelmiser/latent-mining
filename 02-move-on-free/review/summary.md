<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261116-214318 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $4.472; this month $101.73 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $3.397, input 34 + cache write 84,976 + cache read 673,875 tokens — cache hit 89%
- claude:claude-opus-5-5: $1.075, input 20 + cache write 66,388 + cache read 239,030 tokens — cache hit 78%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q8** (split) | CONFIRMED | CONFIRMED | REFUTED | REFUTED | CONFIRMED |
| **Q9** (split) | REFUTED | REFUTED | UNSUPPORTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q10 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q8, Q9
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 7 MODERATE, 20 LOW
- **MODERATE** [claude:claude-fable-5-1] `README.md:33`, `02-move-on-free/README.md:6-7` — "worth nothing … (proved)" overstates Theorem A (see Q9) — high — reword as above.
- **MODERATE** [claude:claude-fable-5-1] `02-move-on-free/README.md:46` — R' row omits hypothesis M ≡ 0 (mod 12) (`open-f-upper/proofs.txt:143`); stated for all M — high — add "M ≡ 0 mod 12".
- **MODERATE** [claude:claude-opus-5-5] README.md:6, ../README.md:33 — "the right is worth nothing to packing allocators (proved)" overstates Theorem A; first fit goes from 11/6 without moves to 7/4 with compaction — high — use the Q9 wording.
- **MODERATE** [claude:claude-opus-5-5] open-f-lower/proofs.txt:8-10 — headline says Theorem A covers every manager "whose first M 1s leave no room for a 2"; the hypothesis is about the layout after step 2. Counterexample: 1s on even cells; freeing one class opens 3-cell gaps, so the hypothesis fails — high — restate it with the after-step-2 hypothesis (line 66).
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `02-move-on-free/README.md:63` — “about 59” misses the sharp crossover M=54 — confidence: high — state 54 or identify the coarse arithmetic
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:35` — “worth nothing” overstates a matching lower-bound slope as a value comparison — confidence: high — use the Q9 replacement
- **MODERATE** [cursor:grok-4.7-medium] `02-move-on-free/README.md:64` — “cannot show before M is about 59” is false: P6 vs `ceil(5M/3)` separates at `M=54` (`90.25>90`) — high — say the bounds first force `h^N>h^O` at `M=54`, and for every `M>59`
- **LOW** [claude:claude-fable-5-1] `open-f-upper/proofs.txt:10-12` — "effect invisible below M=384 … even M=100" is contradicted by Theorem A (≥7M/4−5.75 > ⌈5M/3⌉ from M≈70) and by `open-f-lower` battery (+7..+16 at M=96): the dp4 figure is the optimum against one script, not the manager's need — high — say "against script S".
- **LOW** [claude:claude-fable-5-1] `open-f-lower/proofs.txt:8-10, 294` — headline/status paraphrase "no room for a 2 between the first M 1s" is weaker than the theorem's hypothesis (no free pair *after step 2*); e.g. 1s on even cells satisfy the paraphrase but not the hypothesis — high — use the line 66-68 wording.
- **LOW** [claude:claude-fable-5-1] `02-move-on-free/README.md:64-68` — numeric claims imprecise (first integer separation M=54; ⌊7M/4⌋−1 also wrong at 27, 28, 30-32) — high — see Q8.
- **LOW** [claude:claude-fable-5-1] `open-f-lower/proofs.txt:44-48` — L3 tagged COMPLETE but rests on correctness of `core.thin_best` (code, unproved) — medium — tag COMPUTED.
- **LOW** [claude:claude-fable-5-1] `open-f-upper/proofs.txt:91` — "odd rank (2nd, 4th, …)" is self-contradictory naming; argument needs the *top* 2 freed, which is what is used — high — say "even rank".
- **LOW** [claude:claude-fable-5-1] `02-move-on-free/README.md:39` — solver A has no direct F run at M=12 (`t12_*.log` only N, O); value follows by O ≤ F ≤ N sandwich — medium — note it.
- **LOW** [claude:claude-opus-5-5] README.md:67-68, solver-b/proofs.txt:89 — "every M > 32" should be "M = 24, 27, 28 and every M ≥ 30" — high — reword.
- **LOW** [claude:claude-opus-5-5] README.md:64-65 — "about 59": integer bounds already separate at M=54 — high — give 54 and 60.
- **LOW** [claude:claude-opus-5-5] README.md:46 — R′ row omits "M ≡ 0 (mod 12)" — high — add it.
- **LOW** [claude:claude-opus-5-5] open-f-upper/proofs.txt:91 vs 161 — "odd rank" means 2nd, 4th, …, last in R but 1st, 3rd, … in R′ — high — use one convention.
- **LOW** [claude:claude-opus-5-5] open-f-lower/proofs.txt:66-69 — the theorem says "P6R forces", but the proof uses a "proof version" of step 4 that differs from the coded step 4 (line 56) — medium — name the proof version in the statement.
- **LOW** [claude:claude-opus-5-5] open-f-upper/proofs.txt:9-12, 17 — "invisible for M ≤ 192" and "greedy rules reach 1.70M" hold only for that adversary; Theorem A forces ≥ 161 > 160 at M=96 and ≥ 1.75M − 7 on these first-fit managers — medium — say "against this adversary".
- **LOW** [claude:claude-opus-5-5] ../README.md:33 — "at least 7/4 of live memory" is asymptotic; the proved bound is 7M/4 − 17/4 — low — say "asymptotically".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:77-81` — the LP exposition omits the one-/two-type cases and writes equality where only a historical-maximum upper bound is needed — confidence: high — add those trivial cases and write H≤6·maxₜN(t)
- **LOW** [cursor:gpt-5.6-sol-xhigh] `open-f-upper/proofs.txt:91,161` — “odd rank” means even-numbered blocks in R but odd-numbered blocks in R′ — confidence: high — use explicit one-based ranks consistently
- **LOW** [cursor:grok-4.7-medium] `open-f-upper/proofs.txt:11` — “NEGATIVE for `M≤192`” but `dp4_all.log:14` has the theorem row at `M=192` equal to `5M/3` (`320-320=0`) — high — “negative for `M<192`, zero at `192`”
- **LOW** [cursor:grok-4.7-medium] `reports/solver-a.md:11-12` — “all entries are COMPUTED” by exhaustive fixpoint, then `h^F(12)` and two `M=13` losses are inferred — high — say those three cells are inferred from `h^O≤h^F≤h^N`
- **LOW** [cursor:grok-4.7-medium] `open-f-upper/proofs.txt:10` — “first exceeds `5M/3` at `M=384`” is only sampled at `48,96,192,384,…` (`dp4_all.log`); intermediates such as `288` are absent — medium — “first computed point past `192` is `384`”
- **LOW** [cursor:gemini-3.8-flash-high] `README.md:34` — Top-level summary claims "the right is worth nothing to an allocator that packs its first blocks together", but against first fit in game N the adversary forces \(11M/6\) (`open-f-upper/proofs.txt:64`, `solver-b/proofs.txt:128`) whereas in game F first fit + compaction achieves \(7M/4\) (`open-f-upper/proofs.txt:65`), so moves improve first fit by \(M/12\) rather than being worth nothing — confidence: high — rephrase to "the right cannot lower the memory cost below the no-move lower bound of 7/4 for an allocator that places its first M 1s on consecutive cells"
- **LOW** [cursor:gemini-3.8-flash-high] `02-move-on-free/solver-a/proofs.txt:81` — Phrasing "A newly opened hexad has index <= the number of non-empty hexads" is off-by-one under 0-indexing without stating that opening index \(j\) requires \(j\) lower hexads to be non-empty (so \(j \le N_{\max} - 1\)) — confidence: high — clarify that 0-indexed hexads never reach index \(N_{\max}\) because opening hexad \(j\) requires all lower \(j\) hexads to be non-empty

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
