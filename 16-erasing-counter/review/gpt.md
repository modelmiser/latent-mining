<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — The legal recursive tree, three transitions, distinct \(W^2+W\) states, exact first return, free-leaf invariant, and \(r=0\) auxiliary base are established at `solver-a/proofs.txt:10-31`; Solver B independently starts at \(r=1\) at `solver-b/proofs.txt:7-19`; `solver-a/sylvester_r3.txt:1-18` instantiates the 42-cycle — none

Q2: CONFIRMED — Squaring the recurrence gives \(W_r\ge2^{2^{r-1}}\), while \(2^d\) memory states give \(\log_2D(r)\le d=2^r-1\) (`solver-a/proofs.txt:34-38`) — none

Q3: REFUTED — Remark 3’s argument is valid and has no non-following step (`solver-a/proofs.txt:65-70`), but Lemma 2(c) falsely says every no-creation step reads one 1 (`solver-a/proofs.txt:51-52`), and its setup omits the legal single-leaf tree (`solver-a/proofs.txt:43`) — “If such a step reads no 1, it fixes the state and the cycle has length 1; otherwise every step reads exactly one 1 and writes one. Handle the single-leaf tree separately.”

Q4: CONFIRMED — Both trajectory searches enumerate every reached-node choice and canonical start representative, pruning only non-start revisits/minimum-weight violations (`solver-b/lazy.cpp:20-39`; `verifier/lazy.cpp:20-48`); complete outputs report 1806 (`solver-b/lazy4_summary.txt:1-16`; `verifier/lazy_r4.out:1-4`). Unsafe pruning could only miss a larger cycle, while the valid 1806 witness prevents an overestimate — none

Q5: CONFIRMED — The README’s \(W+1\) steps for each of \(W\) rounds, distinctness, position count, and \(0.676\) asymptotic (`README.md:62-86`) match `solver-a/proofs.txt:13-38` and the explicit one-node-per-position invariant in `solver-b/proofs.txt:6-19` — none

Q6: REFUTED — “Over every tree” overstates Solver A’s canonical trajectory search (`README.md:44-48`; `reports/solver-a.md:22-24`); the 2010 author order differs from `prior-art.md:7-9`; and the alleged 100-second wait loop (`README.md:125`) has no repository source — “exhaustively covered, using symmetry/trajectory representatives where stated”; cite “Jansens et al. (2010)”; remove or source the wait-loop claim

Q7: CONFIRMED — The top-level row says lower bounds for all \(r\), exact values through \(r=4\), several exhaustive \(r\le3\) searches, and one search design run twice at \(r=4\), matching the folder (`README.md:46`; `16-erasing-counter/README.md:8-15,44-57`) — none

Q8: CONFIRMED — Memory size, legal trees, erase-then-write transition, and cycle definition agree exactly (`16-erasing-counter/README.md:19-34`; `chain/3-question.txt:1-16`) — none

F: MODERATE — `solver-b/check.py:24,33` — nonbinary start characters are silently treated as zero, so malformed witnesses can be accepted — confidence: high — validate every character is `0` or `1`

F: MODERATE — `solver-a/proofs.txt:51-52` — false “exactly one 1” assertion; an empty all-zero-path leaf gives a zero-read fixed point — confidence: high — add the fixed-point exception

F: MODERATE — `README.md:13-14` — “no better upper bound is known” is unsupported by a literature search that read no source fully (`prior-art.md:35-37`) — confidence: high — say “we found no better upper bound”

F: LOW — `solver-a/proofs.txt:43` — Lemma 2 assumes a root although a single-leaf tree is legal — confidence: high — dispatch the identity-map case first

F: LOW — `README.md:44-48` — literal “every tree” wording hides canonical/trajectory enumeration — confidence: high — say “representatives covering every cycle”

F: LOW — `reports/solver-a.md:26` — “24 relabelings” means symmetry-reduced representatives; the referee reports 720 with root fixed (`reports/referee.md:23,35`) — confidence: high — state the symmetry convention

F: LOW — `reports/solver-b.md:26` — `bad_idx.txt` is rejected for an off-path write, not a repeated path index — confidence: high — correct the test description

F: LOW — `README.md:109` — bibliographic author order disagrees with `prior-art.md:7` — confidence: high — use “Jansens et al.”

F: LOW — `README.md:125` — wait-loop claim has no repository evidence — confidence: high — remove or source it

F: LOW — `verifier/lazy.cpp:3` — comment says states are XOR-encoded, but only visitation keys are (`verifier/lazy.cpp:39-45`) — confidence: high — correct the comment

Important unasked issue: Solver A’s checker uses removable `assert` validation throughout (`solver-a/verify.py:18-43`); explicit checks would remain effective under `python -O`.
