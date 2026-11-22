<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-171327 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q2** (split) | REFUTED | REFUTED | CONFIRMED |
| **Q5** (split) | CONFIRMED | REFUTED | CONFIRMED |
| **Q6** (split) | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q2, Q5, Q6
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 8 MODERATE, 8 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/shape2/PROOFS.md:354-357` — failure of structural MD_i does not imply actual prefix-mode communication, so the two-case localization lacks a cycle-level T/pruning lemma — confidence: high — prove that lemma or describe these as candidate mechanisms
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/README.md:65` — “every cycle” overstates Theorem NC, which assumes a home state — confidence: high — say “every cycle through home”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/README.md:66` — the summary mentions prefix jumps but omits that home-to-tail entries must land on the same mode — confidence: high — explicitly include home entries
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/README.md:153` — one example refutes only the proposed W_(r−1) per-node bound, not every possible node-by-node bound — confidence: high — say “refuted this natural per-node bound”
- **MODERATE** [cursor:grok-4.7-medium] `shape2/PROOFS.md:354-357` — double entry is listed as uncovered, but with disjoint data Theorem MD already gives `≤W_r` at depth 5 — high — state the open case as violate `(NC)` and, at every split, violate `(MD_i)` or `(EC)`
- **MODERATE** [cursor:grok-4.7-medium] `README.md:153` — "only the total is bounded" states the open equality `D(r)=W_r` as fact; the same slip is `shape2/PROOFS.md:361` — high — "the root-bit charge fails, so a node-by-node induction has no theorem yet"
- **MODERATE** [cursor:grok-4.7-medium] `README.md:159` — "Theorem MD is the same statement as Theorem Z1" omits case (A) and Z1's home hypothesis (`shape/PROOFS.md:80`) — high — "MD(B) matches Z1; MD(A) is separate"
- **MODERATE** [cursor:grok-4.7-medium] `README.md:67` — "reproduced by the organiser's simulator" has no output in the tree; `shape/organiser_check_out.txt` never mentions 258 — high — delete that clause; the count is the mode-1 term `21·6=126` of the chain in `root1.txt:2`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/README.md:158` — MD and Z1 are not literally the same statement — confidence: high — say “same through-home counting argument”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/shape2/PROOFS.md:112-116` — e_i is used as though it exists although Theorem E gives “at most one”; the zero-overflow case is implicit — confidence: high — define e_i only when an empty step exists
- **LOW** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/shape2/PROOFS.md:384` — “exhaustive” tests only selected numerical values per chain (`shape2/lemma_t.py:40-58`) — confidence: high — say “systematic boundary cases”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `16-erasing-counter/shape2/out_root1.txt:10` — the saved output ends in a traceback and includes unrelated stale cases — confidence: high — regenerate it from current `root1.txt`
- **LOW** [cursor:grok-4.7-medium] `shape2/out_root1.txt:5-9` — the cited run dies with `AssertionError` after printing 258/126, and the file has trees that are not in the current `root1.txt` — high — regenerate from `root1.txt` only
- **LOW** [cursor:grok-4.7-medium] `shape2/PROOFS.md:70` — "when K ∩ nothing is shared" does not state a hypothesis — high — "when `K` shares no index with the spine"
- **LOW** [cursor:grok-4.7-medium] `shape2/PROOFS.md:87` — case (A) stops at `2^k`; Lemma A already gives `≤k` for every tree — high — cite Lemma A, or say the `2^k` bound is only what this argument gives
- **LOW** [cursor:gemini-3.8-flash-high] `shape2/out_root1.txt:5-9` — dirty artifact: output file contains extraneous runs and an unhandled Python traceback (`AssertionError` in `root1.py:7`) from executing against `cex_mut.txt` trees after `root1.txt` — confidence: high — re-run `python3 root1.py root1.txt > shape2/out_root1.txt` to keep only the clean single-tree output

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
