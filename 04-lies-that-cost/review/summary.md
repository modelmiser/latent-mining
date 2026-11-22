<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-080645 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $4.124; this month $120.20 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $3.420, input 42 + cache write 134,037 + cache read 671,008 tokens — cache hit 83%
- claude:claude-opus-5-5: $0.704, input 28 + cache write 49,507 + cache read 302,378 tokens — cache hit 86%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q5** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | REFUTED |
| **Q8** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | REFUTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q5, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 7 MODERATE, 22 LOW
- **MODERATE** [claude:claude-fable-5-1] `README.md:46`, `reports/open-upper-referee.md:42` — cites `open-upper-referee/run1.txt` as second-program evidence for k=2…13, but the file contains no `t1 k=2`/`t1 k=3` lines (starts at k=4); the k=2,3 portion of "two programs" is unsupported by the saved log (true on rerun: 0 lost) — high — regenerate run1.txt or cite refcmp_out.txt/verifier ms for k=2,3
- **MODERATE** [claude:claude-opus-5-5] README.md:69, open-upper-referee/run1.txt:1 — The T1 row claims a second-program check for k=2..13 and cites run1.txt. That log has no T1 run for k=2 or k=3, the headline case; "wide" (run1.txt:27) reaches only j ≤ 12. The independent evidence for k=2,3 at j=13,14 is only `open-upper/refcmp_out.txt`, which compares against a one-lie DP and comes from a different program. — high — Cite refcmp_out.txt for k=2,3, or rerun `ref t1 2` and `ref t1 3` and save the output.
- **MODERATE** [claude:claude-opus-5-5] README.md:82-84 — The text says "Lemma S shows such a question exists… whenever there are at least 2j−2 candidates". It drops the hypotheses b ≥ a−1, weight ≤ 2^j and m ≥ 2, and the niceness hypothesis is needed (run2.txt:72 fails without it). — high — "…whenever the position is nice (b ≥ a−1), within the weight bound a(j+1)+b ≤ 2^j, and has at least 2j−2 (and at least 2) candidates."
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:45` — the cited logs do not substantiate independent T1 coverage for k=2,3 at j=13,14 — confidence: high — save those runs or narrow the claim
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `open-upper-referee/ref.cpp:15` — memo keys truncate every count to one char, so counts such as 44 and 300 collide and the n≤300 root sweep can reuse another state’s verdict — confidence: high — encode full-width integers in the key
- **MODERATE** [cursor:gemini-3.8-flash-high] 04-lies-that-cost/open-upper-referee/run1.txt:1 — `run1.txt` omits T1 runs for k=2 and k=3 (starts at k=4 due to output redirection overwrite), leaving T1 unlogged for k=2,3 in the referee's primary log file — high — re-run `./ref t1 2 >> run1.txt` and `./ref t1 3 >> run1.txt` to restore missing logs
- **MODERATE** [cursor:gemini-3.8-flash-high] 04-lies-that-cost/README.md:48 — Results table asserts headline Theorem 2 follows from rows above but omits Table T2 (P(3) for k=2,3 and P(5) for k=2), without which root exceptions are unproved from the table — high — add Table T2 row to README results table citing `open-upper/proofs.txt:59` and `open-upper/table_out.txt:24-29`
- **LOW** [claude:claude-fable-5-1] `README.md:57-58` — "more answers than remain in the game": Lemma S gives k·ell ≥ j−1, i.e. at least as many, equality allowed — high — "at least as many answers as remain"
- **LOW** [claude:claude-fable-5-1] `README.md:71-72` — "two candidates can be kept alive for ever" reads as all n for k=1; true only for n∈{2..7,9}, false for n=8 and 10..700 — medium — "…for ever when n is one of 2…7, 9"
- **LOW** [claude:claude-fable-5-1] `README.md:45` — "both outcomes are again such states": outcomes need not have ≥2(j−1)−2 candidates; "such" must mean nice with weight ≤2^{j−1} only — medium — "both outcomes are again nice with the weight bound at j−1"
- **LOW** [claude:claude-fable-5-1] `open-upper/proofs.txt:107` — "1310 states" per k conflicts with 960 (T1) and 1884 (table_out); third unexplained count (referee noted) — high — state which window gives 1310 or delete
- **LOW** [claude:claude-fable-5-1] `open-upper/proofs.txt:30-36` — Lemma S never states ell ≥ 1; true (at j=1 the only state is (0,2) with ell=1) and only cosmetically needed — high — add "ell ≥ 1 in all cases"
- **LOW** [claude:claude-fable-5-1] `README.md:80-82` — "checked against the literal definition only for n ≤ 4" understates: referee also ran 4,000 random histories n≤8 and literal game n≤5 (`reports/referee.md:10-13`) — high — "exhaustively for n ≤ 4, by sampling to n ≤ 8"
- **LOW** [claude:claude-opus-5-5] open-upper/proofs.txt:31-50 — Lemma S never states ell ≥ 1. It is not needed, because k·ell ≥ 2ell ≥ j−1 is all (ii) uses, but the proof reads "obligation k·ell" as "blocked". — medium — Add "(if j ≥ 2 then ell ≥ 1)".
- **LOW** [claude:claude-opus-5-5] README.md:79-81 — "that is more answers than remain in the game" is stronger than what the proof gives: k·ell ≥ j−1 can hold with equality. — high — "at least as many answers as remain".
- **LOW** [claude:claude-opus-5-5] README.md:94-95 — "two candidates can be kept alive for ever" reads as general for k=1, but Q_1(8)=6 and Q_1(n)=P(n) for 10 ≤ n ≤ 700. — medium — "…and for n ∈ {2,…,7,9} the answerer can keep two candidates alive for ever."
- **LOW** [claude:claude-opus-5-5] README.md:36-37 — "proved by hand apart from a finite table" omits T2: the n=3,5 roots also rest on computation. — high — "…apart from a finite table (T1) and three small root cases (T2) settled by program."
- **LOW** [claude:claude-opus-5-5] README.md:69 vs open-upper/proofs.txt:3,62-63 — The two files name different second programs: the README says the reader's ref.cpp, while proofs.txt says the earlier referee's verifier/ms. — high — Name both programs and the k ranges each covers.
- **LOW** [claude:claude-opus-5-5] open-upper/table_out.txt:16-20 — The log is filtered: the k=1 control reports lost=30 but shows only 2 LOST lines, and per-j lines are missing for k ≥ 2. — medium — Save the unfiltered log, or say the log is filtered.
- **LOW** [claude:claude-opus-5-5] README.md:103-105 — "checked against the literal definition only for n ≤ 4" understates the evidence: 4,000 random histories with n ≤ 8 were also run (reports/referee.md:11). — high — "exhaustively for n ≤ 4, randomly for n ≤ 8".
- **LOW** [claude:claude-opus-5-5] README.md:67-68 — "I checked it by hand" is unverifiable, since no trace is saved. — low — Drop it or attach notes.
- **LOW** [claude:claude-opus-5-5] README.md:73 — The "Where" cell for the Q_1 = P row is empty. — high — Cite reports/referee.md:24 and the solver-a/solver-b k=1 output files.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:71` — the k=1 sentence can be read as applying for every n, contradicting finite Q_1(8) — confidence: medium — specify “when n=2”
- **LOW** [cursor:grok-4.7-medium] `README.md:46` — cites `run1.txt` for T1 at k=2 and k=3; that file has no `t1` line for either k, so the second program’s saved log does not show the headline table — high — cite `table_out.txt` and the referee report for k=2,3, and `run1.txt` for k≥4
- **LOW** [cursor:grok-4.7-medium] `open-upper/proofs.txt:57-58` — “k·ell≥13≥number of questions left” is false if that number is the state’s j=14 (13≱14); it is true of the questions left after the lie — high — “after any lie at most 13 questions remain, and k·ell is 0 or ≥13”
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:24` — “|L|≤1 satisfies (ii) vacuously” is the wrong word: the ∀ over s∈L is not vacuous when |L|=1; the window simply cannot contain another lie — high — “a single lie is never inside its own later window”
- **LOW** [cursor:gemini-3.8-flash-high] 04-lies-that-cost/open-upper-referee/run2.txt:2 — `run2.txt` omits literal check for Q_2(5) <= 6, leaving one of three T2 base cases unlogged in the referee's literal run — high — add `literal Q_2(5)` to `open-upper-referee/lemmas.py` and re-run into `run2.txt`
- **LOW** [cursor:gemini-3.8-flash-high] 04-lies-that-cost/open-upper/proofs.txt:30 — Lemma S statement omits explicit hypothesis `ell >= 1`, which is needed to ensure liars get obligation k·ell >= 2 (holds implicitly from m>=2, 2·ell>=j-1) — high — add `ell >= 1` explicitly to Lemma S statement
- **LOW** [cursor:gemini-3.8-flash-high] 04-lies-that-cost/open-upper/proofs.txt:62 — Proof text cites 1310 states per k for T1, whereas T1 as defined (a+b <= 2j-3, j <= 14) has 960 states (1310 corresponds to envelope a+b <= 2j) — high — clarify in `proofs.txt` that 960 is the exact count for T1 and 1310 is the tested superset

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
