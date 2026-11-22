<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261120-035303 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $198.21 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q4** (split) | REFUTED | CONFIRMED | REFUTED |
| **Q6** (split) | CONFIRMED | REFUTED | CONFIRMED |
| **Q8** (split) | UNSUPPORTED | REFUTED | REFUTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | REFUTED | REFUTED | REFUTED |
| Q7 | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q4, Q6, Q8
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 10 MODERATE, 10 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:54` — unqualified all-n overclaim — confidence: high — restrict it to computed n=4…10
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `23-gossip-tug/README.md:34-35` — shortest-run formula has the wrong scope — confidence: high — add n≥4 and the n=2,3 exceptions
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `23-gossip-tug/README.md:110-112` — “cannot arise” is concretely false — confidence: high — say “cannot arise in m−1 calls”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `23-gossip-tug/README.md:131-133` — only two omitted files are disclosed, but `solver-a/cert_n7_H.txt` and `cert_n8_D.txt` are also reported and absent — confidence: high — enumerate all omitted witnesses
- **MODERATE** [cursor:grok-4.7-medium] `23-gossip-tug/README.md:34` — shortest run is stated as 2n−4 for the game as defined, but n=2 needs 1 call and n=3 needs 3 (`solver-a/sanity.log:1-2`) — high — restrict the sentence to n≥4.
- **MODERATE** [cursor:grok-4.7-medium] `23-gossip-tug/README.md:111` — “cannot arise in play” is false: a subset can finish among itself; the referee’s point is not reachable in m−1 calls (`reports/referee.md:34`) — high — use the referee’s wording.
- **MODERATE** [cursor:grok-4.7-medium] `README.md:54` — “more than that from four people on” reads as A(n)>2n−3 for all n≥4, which `solver-a/proofs.txt:37` marks unproved — high — say the strict gap is computed for n=4..10 only.
- **MODERATE** [cursor:grok-4.7-medium] `23-gossip-tug/README.md:11` — the bold answer gives A(10)=27 with no mention that the matching upper bound was never run without Lemmas 1–3 and has no certificate (`reports/referee.md:14,55`) — high — put that caveat in the short answer.
- **MODERATE** [cursor:gemini-3.8-flash-high] `23-gossip-tug/README.md:46-47,97-98,130-132` — Four large files (>300 kB) were omitted from the repo but the README only accounts for two from solver B; solver A's `cert_n7_H.txt` and `cert_n8_D.txt` are missing, leaving n = 7 without an H witness and n = 8 with no witness in the repo — confidence: high — document that solver A's large certificates were omitted and adjust status text
- **MODERATE** [cursor:gemini-3.8-flash-high] `README.md:54` — Root table states "and more than that from four people on" as a proved fact for all n ≥ 4, but A(n) > 2n − 3 is open for general n (verified only for 4 ≤ n ≤ 10) — confidence: high — rephrase to "strictly greater for 4 ≤ n ≤ 10"
- **LOW** [cursor:gpt-5.6-sol-xhigh] `23-gossip-tug/solver-a/check.py:52-76` — unused listed rows are not validated — confidence: high — document reachable-row semantics or validate every row
- **LOW** [cursor:gpt-5.6-sol-xhigh] `23-gossip-tug/README.md:46-47` — file wording obscures that in-tree two-sided witnesses stop at n=6 — confidence: high — distinguish produced/accepted files from shipped files
- **LOW** [cursor:grok-4.7-medium] `verifier/g6.cpp:235` — Delayer checking fills a missing position with ⌈(non-full)/2⌉ and can still ACCEPT (`:342`), so the play need not stay in the file; `solver-a/check.py:58` rejects that case — high — fail closed on a missing Delayer line.
- **LOW** [cursor:grok-4.7-medium] `23-gossip-tug/README.md:89-91` — seven rules “each gave exactly 2n−3” has the source text in `rule.cpp:64-72` but no output log — medium — label it as the solver’s report, not a checked result.
- **LOW** [cursor:grok-4.7-medium] `23-gossip-tug/README.md:124` — “three test files” then describes `bad5.txt`, `badcert5_root.txt`, and the strategy-file format, which is not one test file (`reports/referee.md:31-33`) — high — say two weak test files, plus the certificate indexing.
- **LOW** [cursor:grok-4.7-medium] `23-gossip-tug/README.md:46` and `README.md:54` — “strategy files for both players up to 7” but certificates start at n=4 (`reports/solver-a.md:33`); n=2 and 3 have none — high — say both players for n=4..7.
- **LOW** [cursor:grok-4.7-medium] `solver-b/check_table.cpp:16-17` — child states are stored in `int L[8]` / `int K[8]` with no check on n, so n>8 writes off the array — high — reject n>8 or size the arrays to n.
- **LOW** [cursor:gemini-3.8-flash-high] `23-gossip-tug/README.md:34` — Scope omitted on classical gossip length 2n − 4, which fails for n = 2 (gives 0 vs 1) and n = 3 (gives 2 vs 3) — confidence: high — add "for n ≥ 4"
- **LOW** [cursor:gemini-3.8-flash-high] `23-gossip-tug/README.md:108` — Indexing mismatch: lists eight differences starting with A(3) − A(2) = 2 while stating "from n = 3 on" — confidence: high — change to "from n = 2 on" or drop 2
- **LOW** [cursor:gemini-3.8-flash-high] `23-gossip-tug/solver-b/bad5.txt:1-93` — Deliberate error test file `bad5.txt` is byte-identical to `table5.txt`, so it does not test negative validation — confidence: high — introduce a corrupted row into `bad5.txt`

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
