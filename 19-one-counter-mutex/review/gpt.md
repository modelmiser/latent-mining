<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — L1(a–d) follows the exact progress/safety definitions; safety schedules need no fairness, so parking p in crit and j mid-protocol is legitimate (`chain/3-question.txt:10-16`; `solver-b/proofs.txt:14-27`) — none

Q2: CONFIRMED — the two distinct exclusions exhaust Z₃\\{0}, forcing E_k={0} and a_j=e_i; scaling counter values/adds by 2 is an isomorphism, and the crit successor must restore reachable all-rem R to 0 (`solver-b/proofs.txt:40-46`) — none

Q3: CONFIRMED — the helper is paused whenever its observed counter would differ; (i) R=0 lets the resting third process enter beside the parked critical helper; (iii) values are 0→1→2→1→0; (iv) add 2 would make R=0 beside that helper (`solver-b/proofs.txt:48-72`) — none

Q4: CONFIRMED — p’s constructed history has net contribution 0 and it remains unscheduled; Steps 4–5 have exactly p,q active and moving forever, h permanently resting, and no crit. “Lockstep” is by levels, allowing unequal finite runs to L_m/P_{m+1}; finite/infinite is exhaustive (`solver-b/proofs.txt:75-95`) — none

Q5: CONFIRMED — a triple comprising two long finite hold-chain automata and one cyclic yield-chain automaton, with arbitrary read/add-0 delays and unreachable padding, is defeated by Step 4; making two chains cyclic invokes Step 5. The repository’s generator exercises precisely long finite/cyclic chains (`verifier/chains.py:6-19`; `solver-b/proofs.txt:80-94`) — none

Q6: CONFIRMED — vcheck exhausts reachable configurations for safety, then exact nonempty active sets and SCC-internal edges containing every active process (`verifier/vcheck.py:150-189`). Both witnesses obey rem/crit structure (`solver-a/w_n3_b4_burns.txt:3-20`; `solver-b/w_n3_b4_s7.txt:2-26`), and reported checks give 104/182 configurations (`reports/referee.md:17`) — none

Q7: REFUTED — the proof summary is materially faithful, but “reads 2 forever” ignores possible add-0 steps, reachable is omitted before “all in rest,” and Solver A does explain the 1,1,2 decoding (`README.md:65,80,94`; `solver-a/proofs.txt:102-103`) — “Every reachable all-rem configuration has R=0”; “thereafter each sees only 2 and never changes R”; “Solver A briefly explains decoding; full correctness is model-checked.”

Q8: REFUTED — the numerical cells match the reports, but both README status wordings say “nothing settled beyond three” despite their own proved lower bound and checked n=4,5 upper bounds (`README.md:43,112`; `../README.md:49`; `reports/referee.md:12-18`) — “No exact g(n) is known for n≥4; 4≤g(4)≤8 and 4≤g(5)≤14.”

F: CRITICAL — `solver-b/mc.h:12-16,85-93`; `reports/solver-b.md:33-38` — the default C++ checker allocates 8 read branches but parses the included b=14 witness, causing out-of-bounds memory access and potentially checking corrupted transitions — confidence: high — use dynamically sized storage or reject b>MAXB before parsing

F: MODERATE — `README.md:112`; `../README.md:49` — “nothing settled beyond three” contradicts established bounds — confidence: high — say no exact value is settled beyond three

F: LOW — `README.md:94` — claims neither solver explains decoding, but Solver A does — confidence: high — acknowledge its brief explanation

F: LOW — `README.md:80` — hold loops may execute add 0 rather than repeatedly read — confidence: high — say “sees only 2 and never changes R”

F: LOW — `README.md:65` — omits that the all-rem configuration must be reachable — confidence: high — insert “reachable”

Unasked: the C++ b=14 overflow is the main omitted issue; it does not invalidate g(5)≤14 because the Python/referee checker evidence is separate.
