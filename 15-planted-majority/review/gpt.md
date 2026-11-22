<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — every opposite-sign pair and required split is defined; the third net move lowers absolute weight by 2c; W≥K for every plant; terminality excludes mixed signs and a wrong weightless agent; all agents, including the plant, then output correctly (`15-planted-majority/solver-a/proofs.txt:39-63`) — none

Q2: REFUTED — the sets are closed (for M=3 the cubic set is {1,2,3,6,7,8,9,18,27}), but displayed identities use a zero summand at c=1 and use e_M outside its declared range (`15-planted-majority/solver-a/proofs.txt:65-76`) — restrict those identities to c≥2 and handle M, M², and e_M separately

Q3: REFUTED — the sub-population, padding, Q0, zero-sum-cycle and Gordan arguments work, but δ=min_t is undefined when T is empty (`15-planted-majority/solver-a/proofs.txt:94-105`) — first observe that T=∅ already contradicts instances (2,0,qA) and (0,2,qA), then assume T≠∅

Q4: REFUTED — the scheduler, planted-instance interpretation, output alternation, km bound and final contradiction are sound, but stage j=2m+1 attempts to activate nonexistent h_{2m+2} (`15-planted-majority/solver-a/proofs.txt:108-121`) — activate h_{j+1} only when j<2m+1

Q5: REFUTED — Lemma R’s terminal-or-swap-self-loop argument is sound (`15-planted-majority/solver-a/proofs.txt:11-22`), and G(4)’s search dependence is accurately reported (`15-planted-majority/reports/referee.md:44`), but solver A did not complete literal-model G(3) (`15-planted-majority/reports/solver-a.md:87`) — attribute literal G(3) computation only to solver B and the referee

Q6: REFUTED — “number of agents … less than K” is false at a+b=K−1, and adding qB cannot flip an existing B majority (`15-planted-majority/README.md:89-91,107-109`) — say “number of tokens is at most a+b<K” and “add agents of the opposite starting opinion”

Q7: REFUTED — the exact 4M−1 bound is A’s, while B proves 4M (`15-planted-majority/solver-b/proofs.txt:50-53`); literal G(3) is over-attributed (`15-planted-majority/README.md:54,57`); the panel and solver-A process claims lack report support (`15-planted-majority/README.md:130,162-164`) — correct those attributions or mark them unrecorded

Q8: REFUTED — the top-level row is consistent, but the folder README omits the formal G(s)=1 and G(s)=∞ conventions (`15-planted-majority/README.md:36-37`; `15-planted-majority/chain/3-question.txt:9`) — add both conventions

F: LOW — `15-planted-majority/solver-a/proofs.txt:66,72-75` — boundary identities silently use 0 or undefined e_M — confidence: high — state parameter ranges and boundary cases

F: MODERATE — `15-planted-majority/solver-a/proofs.txt:104` — empty T makes δ undefined — confidence: high — dispose of T=∅ first

F: LOW — `15-planted-majority/solver-a/proofs.txt:109-110` — final stage activates h beyond its range — confidence: high — condition activation on j<2m+1

F: MODERATE — `15-planted-majority/README.md:36-37` — G(2)=1 is undefined under the abbreviated definition — confidence: high — restore both conventions

F: LOW — `15-planted-majority/README.md:45-46` — “every difference” includes zero unless u>v is stated — confidence: high — quantify u>v in S

F: LOW — `15-planted-majority/README.md:90-91,107-109` — two explanatory sentences are literally false at boundary/opposite majority — confidence: high — use the Q6 wording

F: LOW — `15-planted-majority/README.md:54,57,130,162-164` — exact-bound, model, panel, and process attributions exceed their sources — confidence: high — narrow or remove them

Unasked: no additional correctness defect found.
