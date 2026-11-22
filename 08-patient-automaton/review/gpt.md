<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — direct recomputation gives exactly both pair-image and round tables in `solver-a/proofs.txt:4-9` — none

Q2: CONFIRMED — the strategy waits until \(t\ge d+2\), handles both first letters, maintains the stated shifted mod-3 invariants, and strikes only at pair 34 (`solver-a/proofs.txt:11-23`; `solver-a/witnesses.txt:5-9`) — none

Q3: CONFIRMED — every family transition is correct; the induction may choose replies using the entire fixed word, and the first-round images contain F4 or F1 as required (`solver-a/proofs.txt:25-37`; `README.md:26-27`) — none

Q4: CONFIRMED — new states add no new first-letter image and thereafter remain inside the original four states, preserving both the winning strategy and every blind counterexample (`solver-a/proofs.txt:40-42`; `solver-b/proofs.txt:64-68`) — none

Q5: CONFIRMED — move order, visibility through \(y_{t-1-d}\), winning after \(x_T\), and uniform \(T\) match (`README.md:15-27`); the queue reduction correctly delays each adversarial choice until all \(d\) letters originally visible to it are fixed (`solver-a/proofs.txt:51-55`; `solver-b/proofs.txt:14-20`) — none

Q6: CONFIRMED — Lemmas A and B otherwise close: the false \(M_{r-1}=13\) cases have \(M_1=123\), which also excludes 12, and the height convention affects only wording (`solver-b/proofs.txt:30-59`; `reports/referee.md:12-16,31-32`) — “If \(r=2\), use \(M_1=123\); otherwise \(M_{r-1}=13\).”

Q7: REFUTED — the headline counts and n=6 qualification are supported (`solver-b/proofs.txt:70-78`; `solver-b/n6_sym.txt:1-11`), but “any other choice collapses” and “a never lets … leave the cycle” are literally false: 34 under an a-round can become the still-nonsingleton pair 12 (`solver-a/proofs.txt:7-8`; `README.md:70-79`) — “Any deviation enters 12 and is collapsed by the next a; only an indefinitely surviving play follows the cycle.”

Q8: REFUTED — C5 is correctly labelled computed (`solver-a/proofs.txt:47-49`), but retained output explicitly gives \(W_0=W_\infty=405\) only for n=3 (`verifier/chk_out.txt:6-9`), while solver B says its n=2–4 table output existed only in the terminal (`reports/solver-b.md:90-92`) — “The reports of three implementations agree; retained logs do not independently document n=2 and n=3 more than once.”

F: MODERATE — `solver-b/verify.py:31-54` — the claimed exact blind checks start from \(Q.a\), so they explore only words prefixed by a and omit words beginning b — confidence: high — start from `{Q}`

F: MODERATE — `README.md:44,92` — “three independent programs” is not backed by retained n≤3 outputs — confidence: high — say “three implementations report agreement”

F: MODERATE — `solver-b/proofs.txt:37-39` — \(M_{r-1}=13\) is false for d=0,1 with \(y_1=a\), though the conclusion survives — confidence: high — handle \(M_1=123\) separately

F: LOW — `solver-b/verify.py:21-24` — the “literal” strategy checker initializes at \(Q.a\), not Q; fixture-specific equality masks this — confidence: high — initialize with all state keys

F: LOW — `README.md:73-79` — deviations from the cycle reach pair 12 before collapsing, contradicting the literal explanation — confidence: high — say deviations collapse on the next a

F: LOW — `solver-b/proofs.txt:9-12` — “height + 1” depends on counting levels rather than edges — confidence: high — define height or use maximum prefix length + 2

F: LOW — `verifier/chk_out.txt:36` — it labels a correctly accepted equivalent strategy “CHECKER BROKEN,” contradicting `reports/referee.md:34-35` — confidence: high — replace the negative control

Unasked: the incomplete `solver-b/verify.py` blind checker does not invalidate the patient-automaton result because the hand proof and separate exact census solvers remain.
