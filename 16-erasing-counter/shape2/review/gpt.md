<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — legality and transition/L calculations follow directly at `16-erasing-counter/shape2/PROOFS.md:19-40`; examples match `shape2/out_cex.txt:1-17`, and the depth-4 result is `shape2/out_root1.txt:1` — none

Q2: REFUTED — MD additionally covers no-home cycles and states component-wise bounds (`shape2/PROOFS.md:73-80`), whereas Z1 assumes a home state and states only the aggregate bound (`shape/PROOFS.md:80-87`); MD’s proof itself is sound — “Theorem MD(B) is the same through-home counting argument as Theorem Z1”

Q3: CONFIRMED — NC replaces disjoint coordinates with noninterference (`shape2/PROOFS.md:189-208`), yielding pruned-map cycles, Theorem E overflow uniqueness and the count (`shape2/PROOFS.md:210-225`); the depth-5 consequence follows from lower-depth D and the no-home bound (`shape2/PROOFS.md:227-232`, `shape/PROOFS.md:48-69`) — none

Q4: CONFIRMED — MD_i prevents prefix/tail data access, while tail writes to prefix indices become empty Θ* steps (`shape2/PROOFS.md:241-253,270-297`); EC supplies Θ*’s unique all-zero overflow target, Θ* is legal, and Theorem E is applied to Θ* (`shape2/PROOFS.md:250-253,291-299`) — none

Q5: CONFIRMED — the recurrence expansion and product accounting are valid, including small boundary cases (`shape2/PROOFS.md:316-330`); K** is well-founded through smaller pruned subtrees, contains Z’s recursive class and K*’s recursive RD rule, and the stated strict example works (`shape2/PROOFS.md:339-348`); depth 5 uses only computed depths ≤4 (`shape2/PROOFS.md:331-336`) — “K** strictly contains Z’s class and K*, the recursively certified MD/RD closure”

Q6: REFUTED — counts match the outputs (`shape2/out_nc.txt:1-3`, `out_tail.txt:1-3`, `out_lemma_t.txt:1-2`), but the README omits NC’s home-state scope and EC’s home entries (`16-erasing-counter/README.md:65-66`), and its claimed exhaustive localization is not proved by the stated structural MD_i theorem (`shape2/PROOFS.md:241-247,352-357`) — “NC applies to through-home cycles; T requires all entries from home or prefix modes to use one tail mode; other cases remain unclassified”

F: MODERATE — `16-erasing-counter/shape2/PROOFS.md:354-357` — failure of structural MD_i does not imply actual prefix-mode communication, so the two-case localization lacks a cycle-level T/pruning lemma — confidence: high — prove that lemma or describe these as candidate mechanisms

F: MODERATE — `16-erasing-counter/README.md:65` — “every cycle” overstates Theorem NC, which assumes a home state — confidence: high — say “every cycle through home”

F: MODERATE — `16-erasing-counter/README.md:66` — the summary mentions prefix jumps but omits that home-to-tail entries must land on the same mode — confidence: high — explicitly include home entries

F: MODERATE — `16-erasing-counter/README.md:153` — one example refutes only the proposed W_(r−1) per-node bound, not every possible node-by-node bound — confidence: high — say “refuted this natural per-node bound”

F: LOW — `16-erasing-counter/README.md:158` — MD and Z1 are not literally the same statement — confidence: high — say “same through-home counting argument”

F: LOW — `16-erasing-counter/shape2/PROOFS.md:112-116` — e_i is used as though it exists although Theorem E gives “at most one”; the zero-overflow case is implicit — confidence: high — define e_i only when an empty step exists

F: LOW — `16-erasing-counter/shape2/PROOFS.md:384` — “exhaustive” tests only selected numerical values per chain (`shape2/lemma_t.py:40-58`) — confidence: high — say “systematic boundary cases”

F: LOW — `16-erasing-counter/shape2/out_root1.txt:10` — the saved output ends in a traceback and includes unrelated stale cases — confidence: high — regenerate it from current `root1.txt`

Unasked: no additional theorem-level defect found.
