<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261118-085600 — (repository) — round 6

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $6.436; this month $163.55 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $5.414, input 50 + cache write 296,474 + cache read 835,852 tokens — cache hit 74%
- claude:claude-opus-5-5: $1.021, input 38 + cache write 67,314 + cache read 699,595 tokens — cache hit 91%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q2** (split) | CONFIRMED | CONFIRMED | REFUTED | UNSUPPORTED | CONFIRMED |
| **Q4** (split) | — | REFUTED | REFUTED | REFUTED | CONFIRMED |
| **Q5** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q2, Q4, Q5
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 2 CRITICAL, 9 MODERATE, 25 LOW
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `upper/compare_bounds.py:2,10-14` — the documented optional \(N<130\) causes an out-of-range traceback — confidence: high — validate \(N\ge130\) or bound diagnostics by \(N\)
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `upper/pool_search.cpp:40-46,69` — \(K=0\) dereferences an empty vector, causing undefined behavior — confidence: high — reject nonpositive \(K\)
- **MODERATE** [claude:claude-fable-5-1] README.md:234-241 — (n−3)/2 bound attributed to "the greedy algorithm" in a paragraph about solver B's smaller-tree rule, for which Lemma 4 fails — high — name rule R.
- **MODERATE** [claude:claude-fable-5-1] NOTE.md:32-35, NOTE.md:605-606, README.md:16-17, :240-241, top README.md:45 — "needs a different algorithm"/"settled at rate 1/2" without the adversarial-tie qualifier; contradicts Open question 1 — high — add qualifier.
- **MODERATE** [claude:claude-opus-5-5] NOTE.md:32-35, 14-forest-orientation/README.md:16-17 and :240-241, README.md:45 — These say rule R is "settled at rate 1/2" and that doing better "needs a different algorithm". That rests on adversarial ties and contradicts NOTE.md:503-504 and :696-699 — high — add "or a tie-breaking rule" and say that the ties go against the algorithm.
- **MODERATE** [claude:claude-opus-5-5] NOTE.md:30-32 — "Theorem 2 … smaller for most n below 94" compares f with g, not the stated formula. As stated, Theorem 2 is below g only for 19 ≤ n ≤ 75 (not at 74) — high — say "the recurrence f behind Theorem 2".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `NOTE.md:32-35`; `14-forest-orientation/README.md:17,237-241` — adversarial ties do not prove that every fixed tie rule has rate \(1/2\) — confidence: high — add the fixed-tie alternative
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `NOTE.md:461-464`; `upper/compare_bounds.py:10-14` — finite computation is presented as an unbounded comparison — confidence: high — restrict to 4000 or add the 16-step induction
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `upper/greedy_exact.cpp:3-5,321-337` — \(S<HI\) silently activates abstraction while still printing ostensibly exact values — confidence: high — enforce \(S\ge HI\)
- **MODERATE** [cursor:grok-4.7-medium] `NOTE.md:32-35` and `14-forest-orientation/README.md:16-17` — “needs a different algorithm” does not follow from an adversarial-tie lower bound; a fixed tie-break of rule R is still open (`NOTE.md:503-504`) — high — use the wording already at `NOTE.md:696-699`
- **MODERATE** [cursor:grok-4.7-medium] `NOTE.md:463` — “g(n)≤f(n) for every n≥94” is stronger than `compare_bounds.py:10-11`, which stops at N — high — limit the claim to 94≤n≤4000
- **LOW** [claude:claude-fable-5-1] NOTE.md:30-32 — "Theorem 2 is the smaller bound for most n below 94" is true of f, false of the stated 9n/16 − log₂n (smaller only for n ≤ 75) — high — say "the recurrence f".
- **LOW** [claude:claude-fable-5-1] NOTE.md:463 — "g(n) ≤ f(n) for every n ≥ 94" unproved beyond 4000 — high — bound the range.
- **LOW** [claude:claude-fable-5-1] NOTE.md:464-465 — "exact values … smaller than both": equal at n = 1–5, 8 — high — "no larger than either".
- **LOW** [claude:claude-fable-5-1] NOTE.md:457 — pool_search parameters giving k = 6 not recorded; README/NOTE give no command line — medium — record them.
- **LOW** [claude:claude-fable-5-1] NOTE.md:501 — c_0 used, c_j defined for j ≥ 1 only — high — "with c_0 = 0".
- **LOW** [claude:claude-fable-5-1] NOTE.md:468-469, :502 — A_1 also needs its second tie broken (h=0 vs 0); "one tie" per level includes this — medium — say so.
- **LOW** [claude:claude-fable-5-1] pool_search.cpp:88 prints "Lemma A" (old name) — high — rename.
- **LOW** [claude:claude-opus-5-5] NOTE.md:463-464 and :32 — "for every n ≥ 94" and "from 126 on" are only computed to 4000. The step for larger n is missing; it is easy: D(n)=f(n)−(n−3)/2 satisfies D(a+b) ≥ D(a)+D(b)−3/2+c(a) — high — add that argument or say "up to 4000".
- **LOW** [claude:claude-opus-5-5] NOTE.md:457 — "paths of length up to 6": no run parameters or log are in the repository. With n ≤ 140 I reached only k=4 — medium — record the command and output.
- **LOW** [claude:claude-opus-5-5] NOTE.md:505-506 — G(13) and G(14) rest on one program run, and its output is not saved — medium — attach the log.
- **LOW** [claude:claude-opus-5-5] NOTE.md:468-469, :502 and README.md:57 — The adversarial tie needed to build A_1 is not mentioned — high — state it.
- **LOW** [claude:claude-opus-5-5] NOTE.md:491 — H = j is asserted, but it needs Lemma 5 — high — cite it.
- **LOW** [claude:claude-opus-5-5] ternary_family.py:31 — A_1 is not played, so its two rounds are not checked — high — play them.
- **LOW** [claude:claude-opus-5-5] pool_search.cpp:89 — The program prints "Lemma A" (the old name). It crashes if run without arguments — high — rename it and check argc.
- **LOW** [claude:claude-opus-5-5] NOTE.md:461/496 vs :538/546 — g and G each mean two different things in Section 5, and "Theorem 3" also names open-limit/proofs.txt Theorem 3 — high — rename one of each.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `NOTE.md:500-503`; `14-forest-orientation/README.md:57` — tie count and \(c_0\) are misstated/undefined — confidence: high — define \(c_0\) and describe recursive ties
- **LOW** [cursor:gpt-5.6-sol-xhigh] `upper/ternary_family.py:22-40` — base rounds are skipped and all verification disappears under Python `-O` — confidence: high — replay \(A_1\) and use explicit failures
- **LOW** [cursor:gpt-5.6-sol-xhigh] `upper/lemma5_check.py:20-34` — “all trees up to N” omits the singleton — confidence: high — check \(n=1\)
- **LOW** [cursor:grok-4.7-medium] `NOTE.md:491` — H=j is not proved, only h(root)=j via I(j); it is true for j=2 and j=3 by hand (degree <2 forces h=0) — high — add the one-line bound that no h exceeds j
- **LOW** [cursor:grok-4.7-medium] `NOTE.md:370` — “for some” does not state the induction; the step from g_0 is valid if read as one — high — “for each”
- **LOW** [cursor:grok-4.7-medium] `NOTE.md:20` vs `open-limit/proofs.txt:156` and `README.md:53` — two different Theorem 3s; round 5’s rename was not done — high — rename the pair-game result in citations
- **LOW** [cursor:grok-4.7-medium] `NOTE.md:457` — “paths of length up to 6” is not in `pool_search.cpp`; only a run of `byk` could show it — high — say “whatever lengths that run produced”
- **LOW** [cursor:grok-4.7-medium] `compare_bounds.py:10-11` — if N<126 the “from 126” check is vacuous and the script can still print OK — medium — refuse N<4000, or require the ranges to lie inside 1..N
- **LOW** [cursor:gemini-3.8-flash-high] `14-forest-orientation/NOTE.md:34` — Summary states "an upper bound on L below 1/2 needs a different algorithm", but line 504 concedes fixed tie-breaking for rule R remains undetermined and Theorem 4 relies on adversarial tie-breaking in round 2 — confidence: high — reword to state that rule R with adversarial tie-breaking cannot beat 1/2, or that beating 1/2 requires a different algorithm or an algorithmic tie-breaking rule.
- **LOW** [cursor:gemini-3.8-flash-high] `14-forest-orientation/upper/greedy_exact.cpp:322` — Usage error message prints `"usage: hybrid S D lo hi [CAP] [MEM_GB] [MINUTES]\n"`, retaining the old program name from which it was adapted — confidence: high — replace `hybrid` with `greedy_exact`.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
