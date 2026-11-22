<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `chain/3-question.txt:5-14`; `solver-b/proofs.txt:12-19` handle reachable simple cycles, self-loops, and infinite avoidance equivalently — none  
Q2: CONFIRMED — `solver-b/proofs.txt:36-54` correctly derives both L(k) bounds and symmetry; the odd counter has t+2 ≤ w+1 reachable states and spread ≤ t — none  
Q3: CONFIRMED — accounting, parity, start-independent symmetry, all state-0/odd-state cases, and integrality are valid; even inner states transition only to odd states (`solver-b/proofs.txt:86-126`); B₄/B₆ match `solver-b/witnesses.txt:4-6` — none  
Q4: CONFIRMED — Theorems B and C are proofs; arbitrarily large 0ⁿ gives 0∈C(a), and p₀…pₖ are distinct with k≥w−s (`solver-b/proofs.txt:58-84`) — none  
Q5: CONFIRMED — partial C-sets only grow, so the pruning cycle persists; an unsafe pruning can only overstate f, while literal-checked witnesses prevent understatement (`solver-b/proofs.txt:20-34`); provenance matches `verifier/runs.log:1-20` and `reports/referee.md:7-21` — none  
Q6: CONFIRMED — state counts, endpoint jumps, reliable levels, and the even-family identity are exactly represented and tested (`20-short-memory-count/README.md:50-66`; `verifier/lemmas.py:98-115`) — none  
Q7: CONFIRMED — every Status entry and distrust bullet agrees with `reports/referee.md:7-62`, `reports/solver-a.md:12-58`, `reports/solver-b.md:8-65`, and `prior-art.md:27-50` — none  
Q8: REFUTED — the experiment and conjecture wording agree, but root `README.md:48` says “no general lower bound” despite the proved f(w)≥1 (`20-short-memory-count/README.md:42-43`) — replace with “no general lower bound beyond f(w)≥1”

F: MODERATE — `solver-a/proofs.txt:45` — “it equals e” is false for e>w — confidence: high — use min(e,w)  
F: MODERATE — `solver-b/fast3.cpp:4,138-148` — NOSYM10 is documented as skipping a branch but only changes the log label — confidence: high — implement the skip or remove the option  
F: LOW — `README.md:48` — “no general lower bound” contradicts f(w)≥1 — confidence: medium — add “beyond f(w)≥1”  
F: LOW — `solver-b/results.txt:38` — N*(9,3) is left as ≥11 although an 11-state witness proves equality — confidence: high — write “=11”  
F: LOW — `verifier/mkbad.py:14` — the intended unlit-cycle test also lights a spread-2 state, so rejection exercises the wrong check — confidence: high — omit state 2 from the lit set  
F: LOW — `reports/solver-b.md:65` — the stated two-core limit was exceeded — confidence: high — rerun affected jobs under the limit if procedural compliance is required

Unasked: the NOSYM10 no-op is the only additional material issue found.
