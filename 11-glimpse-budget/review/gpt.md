<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — update order, legal nonempty answers, vertex removal, trajectory realization, and finite-state boundedness match `chain/3-question.txt:4-14`, `solver-a/proofs.txt:8-16`, and `solver-b/proofs.txt:4-16` — none

Q2: CONFIRMED — `solver-a/proofs.txt:31-41` and `solver-b/proofs.txt:24-37` correctly establish capture/Sit(m−2), recurrence \(1+\lfloor(m-2)/2\rfloor=\lfloor m/2\rfloor\), odd-height endgame, and blind height-1/2 cases — none

Q3: CONFIRMED — the joint induction covers the root via children, height-2 vertices via parent/(K), and the \(r_1>r_2\) argument; “full start” permits every cop start (`solver-a/proofs.txt:44-60`) — none

Q4: CONFIRMED — with height interpreted inside X, (F),(W),(K), the entry-time bound, and reuse of W_full hold for arbitrary rooted ambient trees (`solver-a/proofs.txt:62-74`) — “Write height_X(z)=m−depth_X(z), and handle t=0 and t=d+2 explicitly.”

Q5: CONFIRMED — the sibling is needed for the m=2 T₃ base and to make each successor a Ψ-state; all three cop positions satisfy membership, distance, exclusion, and \(m'≥m−2\), and both first-glimpse cases close (`solver-a/proofs.txt:90-107`; `verifier/lam.log:5-8`) — “If no first glimpse occurs, apply the blind corollary.”

Q6: CONFIRMED — the projection uses the full legal T-branch containing the projected H-answer; nonempty \(R_t\) makes “towards X” legal, and the reached winning continuation contradicts L; solver B’s trajectory projection is independent and sound (`solver-a/proofs.txt:20-28,79-87`; `solver-b/proofs.txt:40-59`) — none

Q7: REFUTED — the 250-state certificate has recorded acceptance (`verifier/wcheck.log:13`), and the eight starts are encoded at `solver-a/lemma_check.py:56-65`, but `solver-a/run_all.sh:14` only prints its result and no solver-A output artifact is retained; the README table does match both reports (`reports/solver-a.md:4-8`; `reports/solver-b.md:7-10`) — “Both bases are encoded and independently checked, but solver A’s lemma-check output was not retained.”

Q8: REFUTED — most statuses and the prior-art summary match, but “proved by hand, twice” includes h≤2 cases supported by checked strategy files, `h−1` is unqualified at h=0, “next to u” omits staying at u, and the alleged “that leaf or q1” error is actually a true sufficient disjunction (`README.md:41,67,89,110`; `prior-art.md:25-30,50-55`) — “Say checked strategies for h≤2, restrict the count to h≥1, use closed neighborhood, and remove the false erratum.”

F: LOW — `solver-a/proofs.txt:44,63` — height uses undefined h although W_seed is ambient-tree/X-relative — confidence: high — define height_X=m−depth_X

F: LOW — `solver-a/proofs.txt:66` — (F) invokes S_{t−1} at t=0 — confidence: high — state the seed base separately

F: LOW — `solver-a/proofs.txt:72-73` — reused visit interval is empty when t=d+2 — confidence: high — handle that case directly from leaves in S_{d+1}

F: LOW — `solver-a/proofs.txt:104-107` — final theorem omits the no-first-glimpse branch — confidence: high — invoke the blind corollary

F: LOW — `solver-a/run_all.sh:14` — lemma-check output is not archived — confidence: high — retain a log

F: LOW — `README.md:41` — “proved by hand, twice” overdescribes the h≤2 strategy certificates — confidence: high — distinguish hand bounds from checked strategies

F: LOW — `README.md:67,89` — unqualified h−1 and “next to” are inaccurate at h=0 and omit robber staying — confidence: high — qualify h≥1 and say closed neighborhood

F: LOW — `README.md:110` — labels a correct weaker disjunction as an error — confidence: high — remove that item

Unasked: none.One omitted finding:

F: LOW — `solver-a/proofs.txt:94-96` — the m=2 projection silently relies on monotonicity because projection may yield a belief containing, not equal to, the seed — confidence: high — explicitly invoke `solver-a/proofs.txt:17-18`.

No verdict changes.No verdict change: the proposed projection counterexample fails because the proof’s legal T-answer is the full `{4,5}`; `{4}` is only the contained H-belief. Q6 remains confirmed, though wording could be clearer.

Additional findings:

F: LOW — `README.md:44` — “tested every lemma on 1.9 million states” obscures differing finite/random test limits — confidence: high — state limits per lemma

F: LOW — `reports/solver-b.md:49` — says height 2 “needs” one exact glimpse although e(2)=0 — confidence: high — say the recurrence uses one; a separate blind strategy uses zero
