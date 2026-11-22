<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The organiser's note of what was done with the findings. Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Dispositions, round 1 (experiment 9)
Q1, Q2, Q3, Q5, Q6 CONFIRMED by all five.
- REAL BUG, RECORDED: solver-b L2(c) false (Grok). Counterexample checked by hand: 1 4 5 2 6 + y=3 -> 1 2 4 5 3 6. README no longer counts L2 as a proof; note N1 at top of solver-b/proofs.txt. Upper bound rests on solver-a T3, which I re-read case by case (holds; all-ones case trivial).
- FIXED: README lower-bound explanation lacked "deletion-proof" hypothesis (Grok, GPT, Opus Q7).
- FIXED: "three independent programs", "tested word by word for n <= 5", "upward slip", n >= 3 qualifier, base-case provenance, Yao-Yao mention, prior-art identification unverified.
- NOTE: proof-file blemishes listed in notes at top of each proofs.txt.
- NOT DONE: relabel solver-a/out_cross.txt runs; replace referee's two non-broken witnesses (mentioned in README).
No CRITICAL. The one real-bug MODERATE is in a redundant proof and is now disclosed. Stop.
