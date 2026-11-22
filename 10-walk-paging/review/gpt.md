<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `10-walk-paging/solver-b/proofs.txt:10-34` handles round 1, both parities, runs, entry counts, disjoint intervals and arithmetic correctly — none  
Q2: CONFIRMED — `10-walk-paging/solver-b/proofs.txt:38-61` gives legal prefix schedules and covers Stage 0, free steps, both cycle branches, cache sizes and parities — none  
Q3: CONFIRMED — Lemmas 1–2 are valid (`solver-a/proofs.txt:8-18`); Belady is used only after conversion to no-prefetch demand paging (`:20-28`), yielding Theorem U (`:31-48`) — none  
Q4: REFUTED — `solver-a/proofs.txt:80` falsely equates \(2m\ge k\) with \(m\ge j\); counterexample \(k=5,j=2,m=2\). The remaining counting and legality argument at `:67-89` works — “Stage 2 occurs for \(m\ge j\) when \(k\) is even and \(m\ge j+1\) when odd; odd \(m=j\) is covered by Stage 1.”  
Q5: REFUTED — the domination lemma is true and both licensed prunings are used, but `solver-b/proofs.txt:69-72` defines credit as virtual minus real faults while requiring it initially to exceed \(|V\setminus R|\), impossible when that set is nonempty — define credit as \(|V_0\setminus R_0|+\text{virtual faults}-\text{real faults}\), and specify eviction from \(R\setminus V\) on a virtual hit  
Q6: CONFIRMED — the formal rules explicitly cover unchanged hits, forced round 1, prefix legality, stopping, infinite payoff and least \(N\) (`chain/3-question.txt:3,7-9`), and the proofs use those conventions — none  
Q7: CONFIRMED — exhaustive values for \(k=2\ldots14,m=1\ldots5\) appear in `verifier/table_mode2.txt:1-65`, and all \(m=6\) values in `verifier/table_m6.txt:1-13`; they match the README table and formula — none  
Q8: REFUTED — “every online reply” is false because the main replay uses inclusion-maximal moves (`verifier/wk.cpp:10-11,223`; `verifier/adva.txt:1-72`); `README.md:73-74` is undefined when Online prefetches neither neighbour; and “did not find the two combined” (`README.md:112-113`) conflicts with access-graph prefetching in `prior-art.md:15-16` — say “every inclusion-maximal reply (all legal replies in 12 smaller cases)”; “choose an absent adjacent side”; and “did not find this exact neighbour-prefetch bounded-opt game”

F: MODERATE — `solver-a/proofs.txt:80` — false odd-\(k\) case equivalence — confidence: high — use the parity-specific condition above  
F: MODERATE — `solver-b/proofs.txt:69-72` — stated credit invariant cannot hold initially — confidence: high — include initial credit and the required eviction argument  
F: MODERATE — `README.md:42` — replay did not enumerate every legal Online reply at the claimed range — confidence: high — qualify it as inclusion-maximal replies  
F: MODERATE — `README.md:112-113` — prior-art summary denies a combination that `prior-art.md:15-16` reports — confidence: high — narrow the claim to the exact model  
F: MODERATE — `solver-b/verify.py:131` — revisiting a state returns success, accepting zero-fault infinite plays — confidence: high — detect hit-only cycles  
F: MODERATE — `solver-a/adv.py:61-64` — stopping at the first terminal leaves other branches’ legality and zero-fault cycles unchecked — confidence: high — exhaust reachable states and detect cycles  
F: LOW — `solver-a/proofs.txt:98-100` — \(D\) is never initialized and the capacity-preserving eviction is omitted — confidence: high — define \(D=C\setminus C'\) and evict outside protected pages  
F: LOW — `README.md:63-66` — strategy description omits round 1 — confidence: high — add “load \(\{0,1\}\) on round 1”  
F: LOW — `README.md:73-74` — assumes a first-round neighbour was retained — confidence: high — choose either absent side  
F: LOW — `README.md:79-80` — “\(k-1\) once” is misleading during repeated free steps — confidence: medium — say “until the first cycle completes”  
F: LOW — `README.md:68-69` — “checked by program” overstates a capped diagnostic; `verifier/wk.cpp:122,130` labels reaching CAP as unbounded — confidence: medium — say “observed as unbounded in the tested cases”  
F: LOW — `solver-a/game.py:57,97` — displayed cap differs from the cap used — confidence: high — print `2*m+3`  
F: LOW — `solver-a/mkwit.py:21,24` — witness header is glued to data and advertised recomputation is skipped for many rows — confidence: high — insert a newline and label unchecked rows explicitly  

Unasked important: no additional headline-level defect found; the two checker soundness defects above are the most consequential non-proof issues.
