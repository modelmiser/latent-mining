<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `solver-b/proofs.txt:4-19` derives the exact consistent assignments, branching sum, flag clearing, and well-founded recursion; `solver-b/fast.py:16-28` preserves buried flags and permits OUT from every stack — none

Q2: CONFIRMED — `solver-b/proofs.txt:22-30` handles \(n=k+1\): after all \(k\) refusals, the unarrived item is 1 and fits atop \(S_1\) — none

Q3: CONFIRMED — `solver-b/proofs.txt:32-49` legitimately constructs a fixed order against a deterministic strategy, handles nontermination, and counts exactly \(j\) items in the shorter suffix — none

Q4: CONFIRMED — `solver-b/proofs.txt:53-65` supplies the matrix/induction argument; `solver-b/prove_k2.py:58-87` uses integer arithmetic and checks slack 1–9, 121 zeros, and the exact quadratic-field sign — none

Q5: CONFIRMED — `solver-a/prove.py:14-15,41-61` checks all 49 states over \(u=3..8\), recurrence at \(u=5..8\), and fixed-action margins \(d(4)..d(8)\); the induction is valid, but `solver-a/proofs.txt:61` overclaims one base level — “The recurrence holds for every state for \(u\ge6\), and for the empty state also at \(u=5\).”

Q6: REFUTED — README’s unrestricted formulas fail at \(n=1,2\), while `solver-b/proofs.txt:67-69` supplies the missing domains; the generating function is correct — “Odd formula for \(j\ge1\), even formula for \(j\ge2\), with \(N(2,1)=1,N(2,2)=2\).”

Q7: CONFIRMED — table values match `verifier/flag_out.txt:1` and the literal-rule runs at `verifier/def_out2.txt:11,15,17,21` — none

Q8: REFUTED — `README.md:45,96,112-117` omits closed-form domains, calls normal-form actions “all moves,” suppresses the OEIS shift, and overstates novelty relative to `prior-art.md:10,43` — use the corrections below

F: CRITICAL — `verifier/defsolve.cpp:12-13,59-61` — unchecked \(k>6\), \(n>10\), or missing arguments cause out-of-bounds access/undefined behavior; duplicated in `deflemma.cpp` — confidence: high — validate argc and bounds or use dynamic storage

F: MODERATE — `README.md:45` — closed forms are false for \(n=1,2\) — confidence: high — add domains and exceptions from `solver-b/proofs.txt:68`

F: MODERATE — `solver-a/proofs.txt:61` — \(f(s,5)\) is not proved for multi-action states because `prove.py:58-60` never checks \(d(3)\) — confidence: high — restrict the all-state conclusion to \(u\ge6\); retain \(u=5\) only for the root

F: LOW — `README.md:96` — solver A forces probing at `prove.py:20-21`, so it does not check every literal move — confidence: high — say “all normal-form actions”

F: LOW — `solver-a/table.py:5,10-11` — using \(N(2,0)=1\) prints the wrong generating-function numerator — confidence: high — set the generating-function constant term to zero

F: LOW — `README.md:112-114` — OEIS indexing is omitted — confidence: high — write \(N(2,n)=3\,A052936(n-2)\)

F: LOW — `README.md:117` — “chain’s invention” exceeds a search whose author calls novelty evidence weak (`prior-art.md:43`) — confidence: medium — say “not found in the limited search”

F: LOW — `README.md:42` — “every state” relies on hash-only memo keys (`verifier/defsolve.cpp:7,31`) — confidence: high — disclose the 128-bit collision caveat

Unasked: the verifier CLI memory-safety defect above is the only severe non-mathematical issue found.No follow-up needed; the completed audits do not change the prior verdicts.
