<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-070226 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q6** (split) | REFUTED | CONFIRMED | CONFIRMED |
| **Q7** (split) | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q6, Q7
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 1 CRITICAL, 11 MODERATE, 9 LOW
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `certificate/check.cpp:171,213` — `parts=0` reaches modulo by zero and crashes — confidence: high — validate `parts>0` and `0<=part<parts`
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `certificate/check.cpp:212-215` — incomplete shards emit globally worded ACCEPT, including for losing cases — confidence: high — emit PARTIAL PASS or aggregate all shards
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `certificate/PROOF.md:125-126` — claims `explicit.py` participated in damaged-certificate testing, but `difftest.py` invokes only check.cpp’s min/full/brute modes (`certificate/difftest.py:30-31`) — confidence: high — remove the claim or retain explicit.py run logs
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:47,103-106` — byte identity and SHA have no supporting artifact or log — confidence: high — add cmp/hash transcripts or qualify as unretained
- **MODERATE** [cursor:grok-4.7-medium] `21-expiring-balls/README.md:113-114` — the bullet says damaged 11-bin certificates are rejected, but `damage_11.log:23-24` prints ACCEPT after adding `01 02 03 45 46 47 58` — high — say that added line was still accepted
- **MODERATE** [cursor:grok-4.7-medium] `21-expiring-balls/README.md:105-106` — byte-for-byte identity and the sha256 are not stored anywhere except this sentence; `vb_11_4.log:2` and `b2_11_4.log:2` still show different core sizes (3,722,933 vs 3,723,730) — high — report the hash from a file in the repo or drop the claim
- **MODERATE** [cursor:grok-4.7-medium] `21-expiring-balls/README.md:90-93` — “costs the adversary one bin” treats any-removal N(4)=11 as known, while `README.md:142-144` says 11–13 bins have no certificate — high — state 12 as certified and 11 as solver A only
- **MODERATE** [cursor:grok-4.7-medium] `certificate/check.cpp:213-215` — `walkSeeds` runs only for part 0, and any other part still prints ACCEPT and returns 0, so a seed-only failure is a pass if that part is skipped — high — do not print ACCEPT unless every part, including seeds, has been run
- **MODERATE** [cursor:grok-4.7-medium] `certificate/PROOF.md:107-108` — pruning “was compared” in `REPORT.md`, and that file is not in the tree; `difftest.log:80-83` only shows NOPRUNE accepts — high — cite the logs that exist or remove the citation
- **MODERATE** [cursor:gemini-3.8-flash-high] `21-expiring-balls/certificate/out/difftest.log:50-79` — Contains 29 spurious lines logging `MISMATCH` for the state count test against Cayley labelled forests, directly contradicting `certificate/out/count_check.txt` where all rows match — confidence: high — Strip the broken comparison run from `difftest.log` or replace it with the clean output from `count_check.txt`.
- **MODERATE** [cursor:gemini-3.8-flash-high] `21-expiring-balls/README.md:46,106` — Claims the two programs' winning sets reduce byte-for-byte to the same certificate (`sha256 35f5203d…e99d`), but no reduction run, diff, or checksum log for 11 bins exists in `certificate/out/` — confidence: high — Add a reproduction log showing the minimization and sha256 hash comparison to `certificate/out/`.
- **MODERATE** [cursor:gemini-3.8-flash-high] `21-expiring-balls/README.md:90-93` — Unhedged statement "for k = 4 the order of expiry costs the adversary one bin" is stated as fact, despite any-removal on 11 bins resting solely on an uncertified computation by solver A — confidence: high — Add qualification: "assuming the uncertified 11-bin value for the any-removal game holds".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `certificate/PROOF.md:115-116` — exact full-mode count fails with duplicate lines — confidence: high — deduplicate `cert`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `certificate/PROOF.md:107-108` — cites nonexistent REPORT.md — confidence: high — cite `out/difftest.log`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `certificate/out/difftest.log:50-79` — unexplained MISMATCH records coexist with the successful count log — confidence: high — label or remove the failed comparison transcript
- **LOW** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:92-93` — wrongly attributes any-removal 11 solely to solver A — confidence: high — mention the referee’s independent n=10 confirmation
- **LOW** [cursor:grok-4.7-medium] `certificate/out/difftest.log:50-79` — every count line is marked MISMATCH, while `count_check.txt:1-30` marks the same state counts MATCH on the labelled column — medium — split the logs and say which column was compared
- **LOW** [cursor:grok-4.7-medium] `certificate/check.cpp:172` — `getenv("NOPRUNE") != 0` turns pruning off whenever the variable is set, including `NOPRUNE=0` — high — test the value
- **LOW** [cursor:grok-4.7-medium] `certificate/PROOF.md:17` — “lowers loads” is stronger than the rule used (a prefix’s loads are ≤ the original loads, which is what non-finality needs) — high — say loads do not increase
- **LOW** [cursor:gemini-3.8-flash-high] `21-expiring-balls/README.md:116` — Text claims a "218-line checker", but `certificate/check.cpp` has 217 lines — confidence: high — Update line count to 217.
- **LOW** [cursor:gemini-3.8-flash-high] `21-expiring-balls/certificate/PROOF.md:69-70` — For Case F1 with \(j = 1\), \(\text{dropNewest}(c_1)\) is the empty state which is never in A, rendering the prose deduction technically incomplete; soundness holds because the checker rejects any certificate containing such a 1-ball line whose flipped counterpart is in A — confidence: medium — Clarify in PROOF.md that 1-ball lines with flipped partners in A are explicitly rejected by \(T^+\) at load time.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
