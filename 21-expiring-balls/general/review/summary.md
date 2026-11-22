<!-- Archived by the organiser on 2026-11-21. Machine-made summary of the three non-Anthropic seats (verdict table and findings). Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Panel 20261121-170550 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $199.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q2** (split) | REFUTED | CONFIRMED | CONFIRMED |
| **Q5** (split) | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q2, Q5
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 4 MODERATE, 16 LOW
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:54` — attributes a removed-line rejection to the organiser, but `general/organiser_check.txt:1-15` contains no such run — confidence: high — attribute deletion tests to `out_damage.txt`, not the organiser
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:109-110` — “every deleted line and every swapped move” is false for k=5; only 150 of 3,676 lines were sampled (`general/out_damage.txt:7-11`) — confidence: high — say “every sampled line”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:110-111` — the untouched-bin-renaming conclusion covers only 400 sampled offers (`general/out_damage2.txt:1`) — confidence: high — explicitly call it a sample
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:56` — “known Robin Hood argument” overstates the repository’s literature evidence; the exact constant was not found or checked (`prior-art.md:28,50-52`) — confidence: high — describe only the asymptotic method as known
- **LOW** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:55` — “510 MB” has no retained table or size log — confidence: high — omit the size or add a recorded checksum/size
- **LOW** [cursor:gpt-5.6-sol-xhigh] `general/PROOFS.md:94,131` — references nonexistent `REPORT.md` — confidence: high — cite the corresponding `out_*.txt` files
- **LOW** [cursor:gpt-5.6-sol-xhigh] `general/PROOFS.md:20-25` — Lemma T omits the spanning/touches-every-bin invariant later used in exact touched-bin counts — confidence: medium — state that its construction produces a spanning tree
- **LOW** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:207-208` — “confirmed … the general bounds” is now ambiguous between the old and new bounds — confidence: medium — specify the original 2k and 2^k bounds
- **LOW** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:115-116` — four small values do not establish “exponential values” — confidence: high — say “exponential-looking first four values”
- **LOW** [cursor:grok-4.7-medium] `21-expiring-balls/README.md:106` — “150-line checker”, file is 155 lines — high — say 155
- **LOW** [cursor:grok-4.7-medium] `21-expiring-balls/README.md:55` — “510 MB” cannot be checked; the table is absent (2,689,236 positions is in `out_check_k6_n41.txt:1`) — high — cite the count only
- **LOW** [cursor:grok-4.7-medium] `21-expiring-balls/README.md:109` — “every deleted line and every swapped move” is exhaustive only for the 101-line k=4 table (`out_damage.txt:1-6`); k=5 is a 150-line sample plus one organiser deletion — high — say “every deletion tried (all 101 lines of the k=4 table; 150 of the k=5 table)”
- **LOW** [cursor:grok-4.7-medium] `21-expiring-balls/README.md:57` — “between quadratic and (5/8)·2^k” drops the +1; the proof is `≤ 5·2^{k-3}+1` — high — “about (5/8)·2^k, precisely 5·2^{k-3}+1”
- **LOW** [cursor:grok-4.7-medium] `21-expiring-balls/general/PROOFS.md:93` and `:131` — both point at `REPORT.md`, which is not in the folder; `out_rh_test.txt:5-6` shows two assertion failures with no parameters, so the “reversed tie-break” label is not in the log — high — cite `out_rh_test.txt` and record the command
- **LOW** [cursor:grok-4.7-medium] `21-expiring-balls/general/out_damage.txt:12` — the “21 unnamed, 0 other” line is not printed by `damage.py:27-28` — medium — generate it from `damage2.py`’s classifier
- **LOW** [cursor:grok-4.7-medium] `21-expiring-balls/general/check_adv.py:79` — comment says the position is parsed again; only the move string is reread — high — delete the comment
- **LOW** [cursor:gemini-3.8-flash-high] `21-expiring-balls/README.md:54` — claims organiser reran checker "reject on 20 and with a line removed", but `general/organiser_check.txt:5-8` tested target 6 (`check_adv.py 6 21`) rather than line deletion — confidence: high — change wording to "reject on 20 bins and with target 6; line deletions tested in out_damage.txt"
- **LOW** [cursor:gemini-3.8-flash-high] `21-expiring-balls/README.md:109-110` — states "every deleted line ... rejected", but for the 3,676-line \(k = 5\) table `general/damage.py:8` and `general/out_damage.txt:11` only tested a random sample of 150 deleted lines, not all of them — confidence: high — clarify as "every tested deleted line (all 101 for k=4, sample of 150 for k=5)"
- **LOW** [cursor:gemini-3.8-flash-high] `21-expiring-balls/general/PROOFS.md:94,131` — text cites `(see REPORT.md)` and `(REPORT.md)`, but no `REPORT.md` file exists in `general/` — confidence: high — remove the missing file references or commit `REPORT.md` to `general/`
- **LOW** [cursor:gemini-3.8-flash-high] `21-expiring-balls/README.md:87-88` — section "How the adversary uses expiry" says "Nobody has shown that a clean (k−1)-star can always be forced within that many bins", which is slightly stale now that `general/PROOFS.md` Round 3 proves a clean \((k-1)\)-star can be forced on \(5\cdot 2^{k-3}\) bins for every \(k \ge 5\) — confidence: medium — qualify that Round 3 achieves this for all \(k \ge 5\), while matching \(k + 2^{k-1}\) total bins remains open for \(k \ge 6\)

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
