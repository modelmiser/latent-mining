<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `chain/3-question.txt:5-12` matches `solver-a/proofs.txt:9-15` and `solver-b/proofs.txt:4-15` (glimpse, then cop move deleting `c'`, then `N[S1]\{c'}`; D-answer is the neighbour of a nonempty branch; adversary picks any such branch or any vertex in `S`; finite belief graph makes a forced win bounded) — none

Q2: CONFIRMED — `solver-a/proofs.txt:35-40`: direction step lands in `Sit(m-1)`; exact path `x,y,w` puts the robber in `N[v]\{y}` inside the subtree of `w`, then the blind step to `w` captures or leaves `Sit(m-2)`; `Sit(0)` and `Sit(1)` (`:33-34`) cost 0, so the recurrence is `⌊m/2⌋` for odd `m` too (`m=3` ends at `Sit(1)`) — none

Q3: CONFIRMED — `solver-a/proofs.txt:46-59`: root has height `≥3`, so re-entry is from a child; height 2 is never the root when `m≥3` and uses the parent or `(K)`; `r1>r2` with `r1≤t-2` refills `q2` and then both leaves of `q2`. The only start hypothesis is the stated full start, any `c_0` — Replace “that leaf or q1” (`:56`) with “q1” (last visit cannot stay on the leaf).

Q4: CONFIRMED — `solver-a/proofs.txt:62-73`: unique entrance is `v`, so a cop outside `X` needs time `≥ depth+1`; `(F)` and `(W)` follow; `(K)` replays `W_full` after the four leaves are forced in at time `d+1`. True for every tree and every `m≥3` — Say visits lie in `[d+3,t-2]`, not `[d+2,t-1]`.

Q5: CONFIRMED — Sibling is required at `solver-a/proofs.txt:90-96` so `Y` is a `T_3` for the `m=2` base, and it persists: `v2` beside `v1`, the other grandchild beside `u`, and `v_i` beside the other child. `u` is in `S_t` by `(W)` (`:64`, via `(F)` when depth `=t+1`); `dist≥2` and `c'∉X_u`, so `N[u]\{c'}` contains `N_{X_u}[u]` and `m'≥m-2≥2(j-1)+2`. Both first-glimpse cases (`:104-107`) meet `Lam(k-1)` — none

Q6: CONFIRMED — `solver-a/proofs.txt:20-29` and `:79-87`: `R_t` is a blind full-start belief on `T_{m-1}`, `m-1≥3`, hence nonempty, so “towards `X`” is legal and monotone plus projection contradict `L(m-1,j-1)`. `solver-b/proofs.txt:39-53` is a correct separate strategy projection — none

Q7: REFUTED — `lose_D_h3_k1.txt:2-251` is 250 states and `verifier/wcheck.log:13` accepts it; the README table matches `reports/solver-a.md:7-8`, `reports/solver-b.md:9-10`, `verifier/table6.log`, `table7E.log`, `table789.log`. `lemma_check.py:56-65` encodes the 8 starts, but no run of that script is stored — “Part 3 is unchecked stdout; the referee’s `lam.log:5` shows 0 wins on the `T_3`, `m=2` seeds.”

Q8: CONFIRMED — `README.md:41` says the `h≤2` zeros were proved by hand twice; `solver-a/proofs.txt:41` and `solver-b/proofs.txt:59` only cite strategy files (`run_check.log:2,9`). The glimpse paragraphs (`README.md:57-93`) match §§2–5, and the prior-art paragraph matches `prior-art.md:7-15,29-30,54` — “`d(h)≤h-1` and `e(h)≤⌊h/2⌋` are hand proofs; `d(2)=e(2)=0` is a checked strategy from the root, not a written sweep.”

F: MODERATE — `README.md:11` and `:41` — “proved by hand for every h” / “both are 0 for h≤2, proved by hand, twice” is false for `h=2`: §2(a) only clears a parent of two leaves, and both solvers point at strategy files — high — call `h=2` a checked root strategy; keep the hand proofs for the inequalities and for `h≤1`.
F: MODERATE — `reports/solver-b.md:34` — “All four are rejected” does not match `solver-b/run_check.log:16` (only `E_h3_k1_wrongmove2.txt`) and `verifier/wcheck_theirmutants.log` accepts `bad/E_h3_k1_wrongmove.txt` — high — say one altered strategy is legal and the log shows a single rejection.
F: LOW — `solver-a/proofs.txt:56` — “`c_{r1+1}` is that leaf or q1” cannot be the leaf, because `r1` is the last time on a leaf of `q1` — high — “`c_{r1+1}=q1`”.
F: LOW — `solver-a/proofs.txt:72-73` — the visit window `[d+2,t-1]` includes impossible times: depth `d+2` is unreachable before time `d+3`, and `c_{t-1}` is not a leaf — medium — “`[d+3,t-2]`”.
F: LOW — `solver-a/lemma_check.py:21-48` — parts 1–2 assert `(W)` and `(F)` only; clause `(K)` is never checked (the referee’s `lemmas.py:42,71` does check it) — high — add the height-2 child assertion.
F: LOW — `solver-a/proofs.txt:31` — §2 is tagged COMPLETE while `(d)` at `:41` is a named file, not an argument written there — high — tag `(d)` complete-modulo-certificate.
F: LOW — `README.md:44` — “tested every lemma on 1.9 million states” is the blind-walk total in `verifier/lemmas.log:9-12` (F, W, K, and the choice of `u`); it does not execute the post-move check `N[u]\{c'}⊇N_{X_u}[u]` — medium — attribute the 1.9M figure to `W_seed` and the answer choice only.
