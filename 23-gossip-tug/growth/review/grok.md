<!-- Archived by the organiser on 2026-11-21. Report of the Grok seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `chain/3-question.txt:1-6` (legal iff one misses the other's secret; Delayer names any active agent, Hastener any legal partner; ends when every `K_i` is full) and `growth/PROOFS.md:6,19-20,40-49` (non-expert is active; R3 names an active expert) — none

Q2: CONFIRMED — `growth/PROOFS.md:16-27`: each phase-1 partner is untouched (previous partners are dead), so after `n-2` calls `K_c=W`, `K_{l_i}=P_i`, `K_p={p}`; `c` misses `p` on every such round; the family is closed under union, and a call sets level to the max and the flag to the or — none

Q3: CONFIRMED — `growth/PROOFS.md:31-37`: both finish at `max(level)`, so only a strictly lower agent changes level; the flag is or, so at most one agent gains it. `Δ=2` is only (i) split across the two agents or (ii) both gains on the lower agent who called an expert — none

Q4: CONFIRMED — `growth/PROOFS.md:45-87`. R1's `z` supplies neither top nor flag, so no expert and no double while none exists. R2's partner is top-unflagged or flagged-non-top; only the former is a double, and it is the first expert. Monotonicity then blocks R1. Lemma B's unflagged case uses expert `p`; the all-flagged case's first later promotion would call an already-top expert. Each R3 partner is one of those two kinds and drops `T` by 1 — none

Q5: CONFIRMED — `growth/PROOFS.md:89-95`: one double gives `≥ (2n-3)-1` phase-2 calls and `≥ 3n-6`. `n=3`: one phase-1 call, `T=3`, R2 is forced to a top partner, one R3, total 3. `n=4`: every Hastener branch of D* lasts 6, matching `A(4)=6` — none

Q6: UNSUPPORTED — this session cannot execute; missing an independent replay. `growth/dcheck.log:1-9` and `:14-17` already record exactly `3n-6` (`n=3..11` with the phase-1 cut, `n=5..8` without). `growth/dcheck.cpp:14-31,42-43` implements D* with that cut — none

Q7: CONFIRMED — bound, exactness for `n=3..6`, and the excess from `n=7` match `23-gossip-tug/README.md:49-54` and `growth/PROOFS.md:8-10,89-99`; "exactly one call that completes two" matches `:67-68,91`; `4n-13` is marked a guess at `README.md:117-119` (`4·8-13=19`, `4·9-13=23`, `4·10-13=27`); no stronger general upper bound at `growth/PROOFS.md:108-110`. "Why at least `2n-3`" (`README.md:79-95`), the Review paragraph (`:175-197`), and the top-level row (`README.md:54`) still match the weaker proof and the computed values — none

F: MODERATE — `growth/PROOFS.md:101-106` — Lemma 2 is labeled a complete restatement of solver-b's Lemma 3, but `solver-b/proofs.txt:13-18` is `value ≤ u` when each non-expert misses some expert, while Lemma 2 is "exactly `u` calls" when one expert is unknown to all non-experts, and no proof is written — high — prove the stated rule, or cite Lemma 3's actual bound
F: LOW — `23-gossip-tug/README.md:54` — "every reply … for `n=3..11`" is not a full tree: `growth/dcheck.cpp:42-43` keeps one phase-1 partner, `dcheck.log:14-17` is the full tree only for `n=5..8`, and `:10-13` aborts before any larger full run. Symmetry of untouched partners makes the minimum valid anyway — high — say "every reply up to phase-1 symmetry; full tree for `n=5..8`"
F: LOW — `growth/PROOFS.md:10` — "exact for `n=3..6`" sits in a complete-proof header; equality for `n=5,6` is the computed row `README.md:49`, not this argument (`n=3,4` do follow from `3n-6` plus `n(n-1)/2`) — high — limit the parenthetical to `n=3,4` or point at the computed rows
F: LOW — `growth/dcheck.log:18-21` — the banner says the wrong rules must print BELOW, but variant 1 prints OK and still meets `3n-6` — high — drop variant 1 from that claim
F: LOW — `growth/PROOFS.md:25-26` — "flagged iff one of them was" reads as exclusive or; the union and line 27 are inclusive or — medium — "flagged iff at least one was"
F: LOW — `growth/PROOFS.md:64` — "Hence" does not follow from the backward sentence; R1 stays gone because levels and flags never fall — medium — cite line 27 and the R2 case split
F: LOW — `solver-b/proofs.txt:45` — still says `A(n) ≥ 3n-6` is unproved and only computed through `n=10`, which is now false of the experiment — medium — point that line at `growth/PROOFS.md`

The inequality survived the trace: phase-1 shape, closure, `Δ ≤ 2`, a single double, and Lemma B all match the definition, including the `n=3` and `n=4` trees.
