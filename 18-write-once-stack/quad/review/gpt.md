<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — the definition makes POP illegal on empty and checks only after operations (`18-write-once-stack/chain/3-question.txt:1-5`); the scheme is monotone, configuration-only, and has Q(0ⁿ)=EMPTY (`18-write-once-stack/quad/PROOF.md:27-55,135-137`) — none

Q2: CONFIRMED — the pair invariant is valid (`18-write-once-stack/quad/PROOF.md:61-65`); freeze cleaning leaves N=(b), preserves the required suffix, and no-LL implies s≥3q (`18-write-once-stack/quad/PROOF.md:96-101,110-116`) — none

Q3: CONFIRMED — every non-waste death contributes one charge, waste kills hit only mixed quads, and the four rows give exactly s=4q−|R|−w; the freeze-converted quad proves w≤q−1 (`18-write-once-stack/quad/PROOF.md:68-93`) — none

Q4: CONFIRMED — configuration alone determines j*, zones, N, O and R (`18-write-once-stack/quad/PROOF.md:36-55`); the push and both pop cases preserve I1/I2, with |O|≥3 preventing a pop at j* (`18-write-once-stack/quad/PROOF.md:118-136`); q=2 exhaustively survives all 209 sequences through t=5 (`18-write-once-stack/quad/organiser_rerun.txt:3`) — none

Q5: CONFIRMED — ignoring extra cells establishes monotonicity, so the 4q+1 theorem gives the stated corollary (`18-write-once-stack/quad/PROOF.md:7-12`; `18-write-once-stack/solver-a/proofs.txt:89`); arithmetic gives strict improvements at 17, 21–23 and ≥25, with ties at 13, 18, 19, 24 (`18-write-once-stack/README.md:52`) — none

Q6: UNSUPPORTED — the checker’s legal-transition loop and quad implementation agree with the definition (`18-write-once-stack/quad/checker.py:20-53`; `18-write-once-stack/quad/quad.py:46-101`), but the saved outputs omit t=3q for q=1–3 and t=3q+1 for q=1–2 (`18-write-once-stack/quad/organiser_rerun.txt:2-8`; `18-write-once-stack/quad/run_quad.log:5-18`) — “The recorded runs cover 3q−1 for q=1…6 and the full 3q/3q+1 comparison only for q=4…6.”

Q7: REFUTED — the page still calls ⌊n/2⌋+2 the best general bound and describes that obsolete gap (`18-write-once-stack/README.md:8-12,88-96`), while its full q≤6 run and n=29/6-GB provenance are not recorded (`18-write-once-stack/README.md:52,127`; `18-write-once-stack/quad/run_quad.log:19-92`) — replace the old-bound passages by the maximum of the established bounds and qualify the computational claims by the runs actually logged.

F: MODERATE — `18-write-once-stack/quad/PROOF.md:141-146` — claims a complete q=1…6 3q/3q+1 run matrix absent from both logs — confidence: high — add the missing outputs or narrow the claim

F: MODERATE — `18-write-once-stack/README.md:8-12` — calls the superseded ⌊n/2⌋+2 bound “best” immediately before stating a stronger bound — confidence: high — say “previous bound” and state the combined maximum

F: MODERATE — `18-write-once-stack/README.md:88-96` — describes the open gap using the obsolete lower endpoint — confidence: high — use the strongest current lower bound

F: MODERATE — `18-write-once-stack/README.md:127` — the traceback records MemoryError but not q=7, n=29, command, or 6-GB limit — confidence: high — preserve a command-and-resource transcript

F: LOW — `18-write-once-stack/quad/checker.py:20-53` — validates monotonicity only on reachable legal transitions, not E on every configuration/operation as required of a top-scheme; `quad.py` itself is globally monotone — confidence: high — document the limitation or enumerate all inputs

F: LOW — `README.md:48` — experiment 18’s headline/status omits the smaller-n improvements and the quad proof/check evidence — confidence: high — update the row

F: LOW — `18-write-once-stack/README.md:120` — “Its attempt” attributes the R(9) timeout to the follow-up, whereas only Solver B’s earlier timeout is evidenced (`reports/solver-b.md:19`) — confidence: medium — attribute it to Solver B

Unasked: no additional mathematical defect found in the 3q−1 proof.
