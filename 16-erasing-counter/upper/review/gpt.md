<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — the transition erases path bits then optionally restores one path bit (`chain/3-question.txt:1-9`), giving exactly −c or 1−c (`upper/PROOFS.md:15-23`) — none

Q2: CONFIRMED — home states and excursion types follow from Z≤1; λ0 cannot write a data index because its path indices are precisely Z (`upper/PROOFS.md:34-46`) — “λ0 necessarily writes some z_j; it cannot write a data index.”

Q3: CONFIRMED — K records retained path indices, repeated modes without K-growth terminate at a dummy, and idx(F)⊆idx(T)\Z (`upper/PROOFS.md:47-59`); induction permits arbitrary depth (`upper/PROOFS.md:2-4`) — “Define U(i,K,V), where V is the modes entered since K last grew.”

Q4: CONFIRMED — the invariant accounts for erased K-indices, alpha transitions, unreachable dummies, and final data bits (`upper/PROOFS.md:61-74`); distinct home states give H distinct x_t and exact period H (`upper/PROOFS.md:38-40`) — none

Q5: CONFIRMED — the two transition formulas, three cases, unique empty exit, and state count are valid (`upper/PROOFS.md:90-104`); iterating RD gives an offending LCA at depth <r−4, hence ≤r−5 (`upper/PROOFS.md:110-116`) — none

Q6: CONFIRMED — replacing root-writing leaves preserves subtree-bit transitions, so RD applies to read-once subtrees (`upper/PROOFS.md:124-132`); the original construction confirms the modified T0 cycle and unique free-leaf use (`solver-a/proofs.txt:14-30`) — none

Q7: CONFIRMED — the state count and missing-node bound yield the displayed inequality (`upper/PROOFS.md:142-150`); the values are 6, 64, 10240, and 402653184 — none

Q8: CONFIRMED — direct application of the transition definition reproduces 01→00→10→01 and the listed ten-state orbit; the derived M and L values are 1,1 and 3,1 respectively (`upper/PROOFS.md:153-164`) — none

Q9: REFUTED — README claims every read-once tree has longest cycle exactly L (`16-erasing-counter/README.md:61`), but `(1 [] [])` is read-once with M=1 and L=2; the proof establishes only M(T)≤L(T), with one attaining labelling per shape (`upper/PROOFS.md:120-132`) — “Every read-once tree satisfies M(T)≤L(T), and every shape has a read-once labelling attaining L(T); hence the depth-r restricted optimum is W_r.”

F: MODERATE — `16-erasing-counter/README.md:61` — Theorem RO is overstated; `(1 [] [])` is a concrete counterexample — confidence: high — use the Q9 replacement

F: LOW — `16-erasing-counter/upper/PROOFS.md:47-55` — U(i,K) depends on unrecorded branch-local mode history, although the intended construction is recoverable — confidence: high — add a mode-history parameter V

F: LOW — `16-erasing-counter/README.md:143-146` — the claim that the organiser read the proofs and traced both examples has no supporting record elsewhere in the repository — confidence: high — remove it or cite a review record

Unasked: no additional correctness defect found.
