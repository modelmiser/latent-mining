<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — canonicalization, legal offers, FIFO expiry, both Rule answers, and DFS cycle rejection are correctly implemented (`general/check_adv.py:40-66,90-146`); acyclicity prevents Rule from looping forever in the finite state graph — none
Q2: REFUTED — acceptance/rejections are logged (`general/organiser_check.txt:1-9`), but exhaustive damage claims are false: k=5 used samples of 150 lines and 400 offers (`general/out_damage.txt:7-11`; `general/damage2.py:10`) — “sampled deletions, swapped moves, and altered offers were rejected except equivalent untouched-bin renamings”
Q3: CONFIRMED — Lemma C, all FIFO cleanups, induction, and maxima 21 and 41 trace correctly (`general/PROOFS.md:27-38,40-85`) — none
Q4: CONFIRMED — forest edge count, stamp argument, distinct neighbours, and optimization prove the bound, including edge cases; the any-removal interval follows (`general/PROOFS.md:97-127`; `solver-a/proofs.txt:20-27`) — none
Q5: REFUTED — the mathematical values and top-level asymptotics follow from `general/PROOFS.md:80-92,125-127`, and monotonicity follows because first reaching load k passes through k−1; however several evidence/provenance sentences are unsupported (`21-expiring-balls/README.md:54-56,109-111`) — retain the bounds, but qualify the tests as sampled and remove unsupported provenance/size claims

F: MODERATE — `21-expiring-balls/README.md:54` — attributes a removed-line rejection to the organiser, but `general/organiser_check.txt:1-15` contains no such run — confidence: high — attribute deletion tests to `out_damage.txt`, not the organiser
F: MODERATE — `21-expiring-balls/README.md:109-110` — “every deleted line and every swapped move” is false for k=5; only 150 of 3,676 lines were sampled (`general/out_damage.txt:7-11`) — confidence: high — say “every sampled line”
F: MODERATE — `21-expiring-balls/README.md:110-111` — the untouched-bin-renaming conclusion covers only 400 sampled offers (`general/out_damage2.txt:1`) — confidence: high — explicitly call it a sample
F: MODERATE — `21-expiring-balls/README.md:56` — “known Robin Hood argument” overstates the repository’s literature evidence; the exact constant was not found or checked (`prior-art.md:28,50-52`) — confidence: high — describe only the asymptotic method as known
F: LOW — `21-expiring-balls/README.md:55` — “510 MB” has no retained table or size log — confidence: high — omit the size or add a recorded checksum/size
F: LOW — `general/PROOFS.md:94,131` — references nonexistent `REPORT.md` — confidence: high — cite the corresponding `out_*.txt` files
F: LOW — `general/PROOFS.md:20-25` — Lemma T omits the spanning/touches-every-bin invariant later used in exact touched-bin counts — confidence: medium — state that its construction produces a spanning tree
F: LOW — `21-expiring-balls/README.md:207-208` — “confirmed … the general bounds” is now ambiguous between the old and new bounds — confidence: medium — specify the original 2k and 2^k bounds
F: LOW — `21-expiring-balls/README.md:115-116` — four small values do not establish “exponential values” — confidence: high — say “exponential-looking first four values”

Unasked: no additional result-changing defect found.
