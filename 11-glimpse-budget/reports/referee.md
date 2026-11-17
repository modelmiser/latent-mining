# Referee report: glimpse cop/robber on complete binary trees

My code and logs are in `verifier/`.

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| d = e = 0 for h ≤ 2; e(3) = 1, d(3) = 2; **h\* = 3** | CONFIRMED | Recomputed; strategies replayed against explicit robbers; losing certificates checked. |
| e = 0,0,0,1,2,2,3,3,4,4 (h ≤ 9); d = 0,0,0,2,3,4,5,6,7 (h ≤ 8) | CONFIRMED | Recomputed from every start; no entry differs from either solver. |
| d(9) = 8 (A only) | UNSUPPORTED by me | Not recomputed; follows from the proof. |
| d(h) ≤ h-1, e(h) ≤ floor(h/2) | CONFIRMED | Both proofs read, no gap. |
| d(h) ≥ h-1 for h ≥ 3 | CONFIRMED | A's and B's proofs read, no gap; certificate D(3,1) checked. |
| **e(h) ≥ floor(h/2) for all h ≥ 3 (A)** | **CONFIRMED: the proof is complete** | Read line by line, no gap; every lemma tested mechanically. |
| d(h) - e(h) = ceil(h/2) - 1, unbounded | CONFIRMED | Follows. |
| Belief-set reduction equals the game as written | CONFIRMED | Argument read; tested against explicit trajectories. |

## Disagreements between the solvers

1. **Status of e(h) ≥ floor(h/2).** A claims a proof; B has only e(h) ≥ e(h-1) and a conjecture. A is right; B did not find the argument.
   - **Blind lemma and seeded version, general m ≥ 3.** The joint induction closes for clauses F, W and K. Tested over all blind walks: W_full on T_3..T_8; W_seed in T_4..T_7 for every v with m ≥ 3 and every cop start outside X (1.9M timed states). Zero violations.
   - **Answer at each exact glimpse.** Cop outside X: a child of v. Cop at v: a grandchild. Cop below child v_i: the other child. In all 1.9M states the answer is in the belief, at distance ≥ 2, and heads a subtree (T_(m-1) or T_(m-2)) the cop is outside.
   - **Cop not at the root, glimpses left.** Handled. Lam(j) quantifies over every cop position outside X and any belief containing {v, v1, v2}. After the answer u and the cop's move c', the belief N[u] minus c' is such a state with j-1 glimpses.
   - **Lam(j).** Computed for every seed in T_3..T_8, j ≤ 3: the cop wins 0 seeds whenever m ≥ 2j+2, and some at m = 2j+1, so the bound is sharp.
   - **Base m = 2.** A's 8 states on T_3 are blind-lost, as are the corresponding seeds in every T_h up to h = 8.
2. **No value conflicts.** A reaches h = 9, B h = 8. Certificate sizes differ (D(3,1): 250 against 334 states) by convention only.

## D lower bound and "an outside glimpse tells nothing"

The true statement: against a robber confined to X, a direction glimpse taken outside X has a forced answer, so every X-position stays possible.

- **A, section 4.** No gap. Non-emptiness of R_t makes "towards X" a legal answer; it needs m-1 ≥ 3, which holds.
- **B, L1.** No gap.
- **Test** on random states with the cop outside H (h = 3, 4, 5): whenever the cop wins by glimpsing now with j+1 glimpses, the projected cop wins with j. D: 0 violations in 5,900 cases where the premise held. E: 67 violations, as B says it must fail. Projection and monotonicity: 0 violations in about 43,000 states per game.

## Reduction to the belief game

Both solvers use the same reduction, and it is correct. A deterministic cop's history fixes his actions; the belief is exactly the set of end vertices of robber trajectories consistent with that history; finiteness gives the uniform N.

- **Trajectory test.** On T_3 and T_4 the belief update matched brute-force enumeration of all robber trajectories on random histories, both games (0 mismatches in about 27,000 round-checks). A deliberately broken update is caught.
- **Strategies.** My checker replays each strategy against an explicit robber vertex with true answers, not belief sets.
- **Reading.** Both solvers read the statement the same way; I found no ambiguity that affects the answer.

## Witnesses

My checker accepts all 20 of A's files and all 15 of B's.

- 6 of my 8 broken witnesses are rejected: dropped line, changed move, E strategy run as D, E(3,0) relabelled E(3,1), cut budget, wrong leaf.
- The other 2 are accepted and are valid (a mirrored glimpse answer; a quicker first move).
- Single-line deletions of the h = 3 certificates are all rejected, except 24 redundant lines of B's D(3,1).
- All 4 of A's mutants are rejected, and 4 of B's 5 `bad/` files.

## Errors found (all minor)

- B's `bad/E_h3_k1_wrongmove.txt` is a valid strategy; B's own checker accepts it. `run_check.log` records only one of the four claimed rejections.
- A's W_full(K) says "that leaf or q1"; it must be q1. Harmless.
- A's `lemma_check.py` never tests clause K and uses one v per depth. Mine tests both.
- A's "every entry PROVED" should read "proved given finite certificates".
- The statement's beliefs are all true; its guess "h\* is 4 or 5" is false.

## Not checked

- d(9).
- Neither solver's C++ was audited; I only recomputed.
- Lam(j) only for h ≤ 8, j ≤ 3; projection only on random states.
- My proof reading is by the same model family as the solvers.
- One of my test runs hung on a bug in my own control script; it is under `timeout 600` and expires by itself. I fixed the script and re-ran it.

## Follow-up tasks

1. Have a different model family, or a proof assistant, check A's `proofs.txt` sections 3 and 5 (W_full, W_seed, Lam): the only general claim not proved by both solvers.
2. Determine d and e on complete t-ary trees of height h for t ≥ 3. Conjecture: the same h-1 and floor(h/2) pattern.
