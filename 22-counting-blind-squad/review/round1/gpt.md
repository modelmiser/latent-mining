<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — With A=U1, B=U2: n=2: GQ/11→GA/11→GB/11→GG/00→AA/00→FF@5; n=3: GQQ/110→GAQ/121→GGA/011→AGB/121→BQG/121→GAG/121→GGG/000→AAA/000→FFF@8; n=4: GQQQ/1100→GAQQ/1210→GGAQ/0121→AGGA/1111→BGGB/1111→GGGG/0000→AAAA/0000→FFFF@7; n=6 reaches AAAGGG/001100 and then FFBGAA at t=12, a partial-fire failure. The table exactly decodes the witness line (`solver-a/witnesses.txt:1-14`; `22-counting-blind-squad/README.md:50-62`) — none

Q2: CONFIRMED — The brute simulator counts only existing differing neighbours, writes all successors before copying, rejects partial fire, accepts full fire, and uses Brent cycle detection (`solver-b/brute.cpp:16-31`); it fixes d(Q,0)=Q, enumerates 3s−4 free entries and all s^(3s−4) assignments, and maximises the contiguous prefix from n=2 (`solver-b/brute.cpp:35-58`); no relevant cap was reached (`reports/solver-a.md:20-22`) — none

Q3: CONFIRMED — An unread entry cannot affect a deterministic run; branching assigns the first required entry, and the lowest-unused-unnamed-state rule selects one representative per renaming orbit (`solver-b/lazy.cpp:20-56`). The unpruned referee branches over every value (`verifier/ref.cpp:112-129`), so each full table follows one path; the 7^17 weight sum is only an accounting checksum, not by itself proof of unique coverage. Bad symmetry pruning can only lower the maximum, while simulator or branching bugs can move it either way. The solvers share a tree; referee variants share a simulator but alter symmetry/order (`reports/referee.md:20-27`) — none

Q4: REFUTED — All four proofs have no invalid step: propagation gives t≥n−1, d(Q,1)=F causes partial fire while the n=3 end remains Q, d(Q,1)=Q prevents propagation (`solver-b/proofs.txt:1-13`); Lemma 2’s light-cone induction holds (`solver-a/proofs.txt:11-20`), and P3’s case split is complete (`solver-b/proofs.txt:15-21`). But the README attributes the general n≥3 d(Q,1) result to both solvers although A states only n=3 (`solver-a/proofs.txt:7-10`; `22-counting-blind-squad/README.md:46-47`) — “The time bound was proved by both; B proved the general d(Q,1) statement, while A proved its n=3 case.”

Q5: CONFIRMED — State visibility, endpoints, initial state, simultaneous updates, synchronisation and prefix definition match (`chain/3-question.txt:3-16`; `22-counting-blind-squad/README.md:20-35`). A counting rule embeds into an ordinary radius-1 rule by ignoring neighbour identities, and README uses this only for the three-state prior-art implication (`22-counting-blind-squad/README.md:87-91`) — add “L(s)=∞ if one rule handles every n” to the formal definition.

Q6: REFUTED — Numerical status entries, 96=4×24, minimum times and 1,244/336 counts match (`reports/referee.md:5-15`; `solver-b/brute5_single_out.txt:1-9`). Defects: “no argument” overlooks P3; “counterexamples” omits that 2,487,460 counts (rule,n) pairs; and A’s s=4 Lemma-2 check could fail despite not exercising the equal-time branch (`22-counting-blind-squad/README.md:65-69,95-101`; `reports/referee.md:33-45`) — “Except for L(3), values are computation-only; 2,487,460 (rule,n) pairs; s=3 was vacuous, while s=4 tested 511 pairs but not the equal-time branch.”

Q7: CONFIRMED — Values are consistently labelled computed, limitations and the open infinity question are explicit (`22-counting-blind-squad/README.md:11-16,65-103`), and the top-level row agrees (`README.md:53`) — none

F: MODERATE — `22-counting-blind-squad/README.md:99-101` — claims A’s s=4 Lemma-2 test could not fail, although 511 pairs tested the first disjunct — confidence: high — use the Q6 wording

F: LOW — `22-counting-blind-squad/README.md:46-47` — overattributes the general d(Q,1) proof to solver A — confidence: high — use the Q4 wording

F: LOW — `22-counting-blind-squad/README.md:65-66` — “no argument” contradicts the hand proof of L(3) — confidence: high — say “Except for L(3)”

F: LOW — `22-counting-blind-squad/README.md:96` — 2,487,460 is pairs, not plainly rules — confidence: high — say “(rule,n) pairs”

F: LOW — `22-counting-blind-squad/README.md:30-32` — omits the explicit infinity convention — confidence: high — add it

F: LOW — `22-counting-blind-squad/README.md:68-69` — “full table” is ambiguous; retained artifacts show matching histograms, not a per-rule table — confidence: medium — say “full longest-prefix histogram”

Unasked: none.
