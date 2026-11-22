<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-032652 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $5.268; this month $106.99 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $3.860, input 30 + cache write 102,685 + cache read 612,407 tokens — cache hit 86%
- claude:claude-opus-5-5: $1.408, input 30 + cache write 103,471 + cache read 716,849 tokens — cache hit 87%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q4** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q5** (split) | CONFIRMED | CONFIRMED | UNSUPPORTED | CONFIRMED | CONFIRMED |
| **Q7** (split) | CONFIRMED | CONFIRMED | REFUTED | REFUTED | CONFIRMED |
| **Q8** (split) | CONFIRMED | REFUTED | REFUTED | CONFIRMED | CONFIRMED |
| **Q9** (split) | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q4, Q5, Q7, Q8, Q9
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 4 MODERATE, 24 LOW
- **MODERATE** [claude:claude-opus-5-5] README.md:7-9 — the headline "exactly P−1" has no horizon qualifier, but the table shows S_5(3)=1 and F_5(3)=1 — high — add "for long enough horizons".
- **MODERATE** [claude:claude-opus-5-5] open-s/proofs.txt:196 — the claim that node counts agree for P=3 is not supported: 3850 vs 1064 (results.txt:7,14), from different run modes — medium — restrict the claim to P=2, 4 or rerun P=3 with memo on.
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `03-switch-cost/open-s/proofs.txt:75-78` — cash-in can release a zero-length job, making ADV(P) illegal as written — confidence: high — omit the remainder job when it is zero
- **MODERATE** [cursor:grok-4.7-medium] `03-switch-cost/README.md:57` and `:68` — S_9(4) is printed as 2 and the P=4 horizon as 10..15, but solver A leaves 2..3 and the only unrestricted upper bound is a Lemma D run the referee did not read — high — footnote those cells as solver-B-only
- **LOW** [claude:claude-fable-5-1] `open-s/proofs.txt:107` — says tight units have deadline ≤ Δ; they are released at t ≤ Δ so deadline ≤ Δ+1. Harmless (needs only ≤ Δ+ρP, ρ ≥ 1). — high — write "≤ Δ+1".
- **LOW** [claude:claude-fable-5-1] `open-s/proofs.txt:77` — "one job of length f mod P" is a zero-length job when P | f; `play.py:157` skips it, the text does not say so. — high — add "if f mod P > 0".
- **LOW** [claude:claude-fable-5-1] `open-s/proofs.txt:196`, `reports/open-s.md` claim 5 — "node counts agree exactly for P = 2, 3, 4" but `results.txt` shows Python P=3 at 3850 nodes vs C++ 1064 (the Python line is apparently the no-memo run; no memoised Python P=3 count is recorded). — medium — record the memoised Python P=3 run or restrict the claim to P = 2, 4.
- **LOW** [claude:claude-fable-5-1] `03-switch-cost/README.md:124-125` — says S_7(4)=1 rests on Lemma D; `solver-a/proofs.txt:106` reports P=4 T≤7 with DOM off agreeing. Over-cautious, not an overclaim. — medium — drop T=7 from that list.
- **LOW** [claude:claude-fable-5-1] `03-switch-cost/README.md:64` — S_9(2) left blank though `solver-b/results.txt:40,50` computes it (=1, also forced by Theorems 1–3). — high — fill in 1.
- **LOW** [claude:claude-fable-5-1] `open-f/proofs.txt:13`, `open-s/proofs.txt:8` — "for all T ≥ …" silently uses the solvers' monotonicity lemma (same jobs legal at larger T); not cited. — high — cite Theorem 2 / Lemma 2.
- **LOW** [claude:claude-opus-5-5] open-s/proofs.txt:209-210, reports/open-s.md:49 — the "9 deviations" figure for P=6 comes from the non-proved Δ=45 — high — state which Δ was used.
- **LOW** [claude:claude-opus-5-5] README.md:57, open-s/proofs.txt:222 — "needs ~4P²/~2P²" reads as necessary, but 4P² is only sufficient and 2P² is the witness-plan threshold — high — reword.
- **LOW** [claude:claude-opus-5-5] README.md:64 — S_9(2)=1 is computed (solver-b/results.txt:40,50) but shown blank, against "blank = not computed" — high — fill it in.
- **LOW** [claude:claude-opus-5-5] README.md:68,124-125 — S_9(4)=2 is shown exact, though its upper bound is one solver's DOM run. Also, S_7(4) was computed with DOM off (solver-a/proofs.txt:103-106), so listing it under Lemma D is inaccurate — medium — mark the S_9(4) entry; drop T=7 from the list.
- **LOW** [claude:claude-opus-5-5] open-s/proofs.txt:77 — the text calls for "one job of length f mod P" even when that length is 0, which is an illegal p=0 job; the code omits it (play.py:157) — high — add "if nonzero".
- **LOW** [claude:claude-opus-5-5] README.md:52,54 "I re-derived it" vs open-s/proofs.txt:221 / reports/open-s.md:60 "nobody else has read the proof" — no file records the re-derivation — medium — point to a file, or say "read".
- **LOW** [claude:claude-opus-5-5] reports/open-s.md:36 — the Python match for P=5 was at Δ=60 with a 64-bit hash, not the proved Δ=56 — medium — say so.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `03-switch-cost/README.md:55-57` — general minimality of the triangular free-promise horizon is worded as fact — confidence: high — label it sufficient/conjecturally minimal
- **LOW** [cursor:gpt-5.6-sol-xhigh] `03-switch-cost/README.md:47-49` — the table’s cited sources omit the unrestricted upper computation establishing S₉(4)=2 — confidence: high — cite `solver-b/table_run_S4_L2_full.txt`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `03-switch-cost/open-f/proofs.txt:23-27` — \(a_j\) is undefined if J_j is never run, though the intended stall rule and code make the construction total — confidence: medium — define \(a_j=\infty\) when absent
- **LOW** [cursor:gpt-5.6-sol-xhigh] `03-switch-cost/open-f/proofs.txt:98-100` — Corollary A′ omits that k is a nonnegative integer and its one-line proof does not state the trivial k=0 or k≥P−1 branches — confidence: high — state the domain and split those cases
- **LOW** [cursor:grok-4.7-medium] `03-switch-cost/README.md:59` — “blank = not computed” hides S_9(2)=1 and F_10(2)=1, both in `solver-b/results.txt` — high — fill 1s or change the legend
- **LOW** [cursor:grok-4.7-medium] `open-s/proofs.txt:77-78` — “one job of length f mod P” is a length-0 job when f≡0 (mod P). P=2, idle at tick 0, cash-in at t=2 with c=1 gives f=4 — high — omit the zero job, as `open-s/play.py:157` already does; the unit count f is unchanged
- **LOW** [cursor:grok-4.7-medium] `reports/referee.md:71` — says the general equality is conjecture only, contradicting the later proofs a reader meets first in the README — high — date the report as pre-follow-up
- **LOW** [cursor:grok-4.7-medium] `03-switch-cost/README.md:147` — the S_15(4)≥3 tree is not in the tree; the “at most 15” claim rests on `reports/referee.md:33` — high — keep the referee citation next to the horizon sentence
- **LOW** [cursor:gemini-3.8-flash-high] 03-switch-cost/solver-a/bad2.txt:1 — `bad2.txt` is an unmutated duplicate of `strat_S_P3_T9.txt` rather than a corrupted tree; running verify on it succeeds instead of failing — high — delete `bad2.txt` or replace it with a corrupted strategy
- **LOW** [cursor:gemini-3.8-flash-high] 03-switch-cost/solver-a/tables.txt:30-31 — Unhandled `std::bad_alloc` crash dump logged into published table output during unpruned search at P=2, T=9 — high — prune or catch allocation errors before dumping table logs
- **LOW** [cursor:gemini-3.8-flash-high] 03-switch-cost/open-f/proofs.txt:147 — Negative control "staircase/short" in `controls.log` has misbuilt stall timing that triggers an illegal-job parameter error instead of testing the intended scheduler mechanism — high — adjust stall timing in `open-f/control_loose.py`

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
