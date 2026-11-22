<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `chain/3-question.txt:4-6`, `solver-b/proofs.txt:23-40`, `23-gossip-tug/README.md:55-73` — none
Q2: CONFIRMED — `solver-a/proofs.txt:9-25`, `23-gossip-tug/README.md:77-88` — none
Q3: CONFIRMED — `solver-b/proofs.txt:5-21`, `solver-b/ab2.cpp:64-70`, `solver-b/reports/solver-b.md:20` — none
Q4: REFUTED — `23-gossip-tug/README.md:46-47,97-98,130-132`, `solver-a/check.log:7-9` — "Up to n = 6 strategy files for both players are present in the repo; at n = 7 only Delayer files are present (solver A's n = 7 Hastener file and both solvers' n = 8 Delayer files exceed 300 kB and were omitted from the tree)."
Q5: REFUTED — `23-gossip-tug/README.md:34`, `chain/3-question.txt:11`, `solver-a/reports/solver-a.md:29` — "Without the players, the shortest possible run has 2n − 4 calls for n ≥ 4 (1 for n = 2, 3 for n = 3) and the longest n(n − 1)/2 (no pair can call twice)."
Q6: CONFIRMED — `23-gossip-tug/README.md:54-92`, `solver-b/proofs.txt:7-8,23-40`, `solver-a/proofs.txt:9-25` — none
Q7: REFUTED — `23-gossip-tug/README.md:108`, `solver-a/reports/solver-a.md:44` — "The differences between consecutive values from n = 2 on are 2, 3, 3, 3, 4, 3, 4, 4 (or 3, 3, 3, 4, 3, 4, 4 from n = 3 on)."
Q8: REFUTED — `README.md:54`, `solver-a/proofs.txt:34`, `23-gossip-tug/README.md:14,46-47,51` — "| 23 | [23-gossip-tug](23-gossip-tug/) | the gossip problem as a game: one player names who must call next and wants many calls, the other picks the partner and wants few | under best play 1, 3, 6, 9, 12, 16, 19, 23, 27 calls are made for 2 to 10 people; at least 2n − 3 in general, and strictly greater for 4 ≤ n ≤ 10 | computed, with checkable strategy files up to 6 people (and Delayer at 7); the general lower bound proved by hand; the exact function and its growth are open |"

F: MODERATE — `23-gossip-tug/README.md:46-47,97-98,130-132` — Four large files (>300 kB) were omitted from the repo but the README only accounts for two from solver B; solver A's `cert_n7_H.txt` and `cert_n8_D.txt` are missing, leaving n = 7 without an H witness and n = 8 with no witness in the repo — confidence: high — document that solver A's large certificates were omitted and adjust status text
F: MODERATE — `README.md:54` — Root table states "and more than that from four people on" as a proved fact for all n ≥ 4, but A(n) > 2n − 3 is open for general n (verified only for 4 ≤ n ≤ 10) — confidence: high — rephrase to "strictly greater for 4 ≤ n ≤ 10"
F: LOW — `23-gossip-tug/README.md:34` — Scope omitted on classical gossip length 2n − 4, which fails for n = 2 (gives 0 vs 1) and n = 3 (gives 2 vs 3) — confidence: high — add "for n ≥ 4"
F: LOW — `23-gossip-tug/README.md:108` — Indexing mismatch: lists eight differences starting with A(3) − A(2) = 2 while stating "from n = 3 on" — confidence: high — change to "from n = 2 on" or drop 2
F: LOW — `23-gossip-tug/solver-b/bad5.txt:1-93` — Deliberate error test file `bad5.txt` is byte-identical to `table5.txt`, so it does not test negative validation — confidence: high — introduce a corrupted row into `bad5.txt`

### Important Items Not Asked in the Questions

1. **Silent Fallback in Referee Witness Checker (`verifier/g6.cpp:236`)**: In `g6.cpp` mode `certD`, when a position reachable under Delayer's strategy is omitted from the file, the checker silently substitutes the static leaf lower bound `(nonfull(c) + 1) / 2` (`missing++`) instead of raising an error or requiring an explicit leaf annotation (unlike `solver-a/check.py:58` which aborts with `REJECT: reachable position missing from certificate`). A truncated certificate whose missing positions satisfy the leaf bound will be accepted as complete.
2. **Unproved Twin Reduction Pruning (`solver-b/ab2.cpp:52-57`, `verifier/g6.cpp:300`)**: Both solver B and the referee rely on pruning symmetric twin agents during search to establish A(10) = 27 and A(11) bounds, but neither solver nor referee provides a written proof in `proofs.txt` demonstrating that twin pruning does not eliminate optimal minimax lines in games with alternating asymmetric player moves.
