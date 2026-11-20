# referee report

**Headline g(3)=4: CONFIRMED.** The b=3 hand proof has no gap in either version; run as a program it produced a valid violation in all 569,600 trials. For general n only g(n)≥4 is settled.

My files are in `verifier/`.

## Verdicts

CONFIRMED, proof read with no gap and mechanised:

- b=2 fails for n≥3, any s (A Theorem 1, B T2).
- b=3 fails for n≥3, any s (A Theorem 2, B T3).
- Hence g(n)≥4 for n≥3.

CONFIRMED, witness checked with my checker:

- g(3)≤4: A's witness (3/7/6 states, 104 configurations) and B's (7/7/7, 182).
- g(4)≤8 and g(5)≤14.
- n=2, b=3, s=6 (A) and n=2, b=4, s=4 (B): yes. My search also finds one for each.

CONFIRMED, recomputed with my own search (`vsynth.cpp`, `vsynth2.cpp`):

- n=2: none for b=2 with s≤5, yes at s=6; none for b=3 with s≤4; none for s=3 with b≤8.
- n=3: none for s=3 with b≤5 (b=6–8 follow from n=2); none for s=4 with b=2, 3, 4.

UNSUPPORTED:

- n=2, b=3, s=5: none (B only). See disagreements.
- g(n)≤2^n − 1 (A) and g(n)≤2^(n−1) (B): labelled sketches; only n≤5 checked.

I differ from neither solver in any entry I computed.

## The b=3 proof

Claim: for any three automata over Z_3, some schedule violates safety or progress. I read both versions line by line: **no gap found**. Both are constructive, so I wrote each as a program (`refuter.py`) that follows the written steps and outputs the schedule the proof names. A separate simulator that knows nothing of the proof judges that schedule.

- 561,600 refutations in two seeded runs, plus 8,000 on hand-built automata with chains up to 7 levels. No schedule was rejected; the builder never got stuck.
- The automata: uniform 3-state ones, random ones up to 8 states, all 940 "flag-like" automata with at most 4 states, their one-entry mutants, and negated mixtures.
- Every branch of both proofs fired. Thinnest: A Step 3 "e=1, rho=1" (once); two hold chains both longer than 1 (4 times in the random runs, more in the hand-built set).
- The check can fail. With Claim D switched off, the builder gets stuck. Of 2,700 lassos with one step deleted, 2,246 were rejected; I did not inspect the survivors.

This is testing on automata with at most about 30 states, not a formal proof. Other items marked COMPLETE (A Lemma 0; B L0, L1, T1, T4, T5): no gap found.

## Disagreements between the solvers

- **n=3, b=4, s=4.** A timed out; B says none but left no log of the triple run. Mine: none, 16.6M nodes. A guided run shows my search accepts both b=4 witnesses and rejects a broken one.
- **n=2, b=3, s=5.** A timed out; B says none. My search covered the branches where rem is an add (none; one of the four by negation symmetry) and timed out at 1500 s where rem is a read. B's pruning reads as sound and its four part-logs are complete. Not an asked instance.
- **n=2, b=3, s=6.** A yes, B not computed. A is right.
- **General upper bound.** 2^n − 1 against 2^(n−1). Neither is proved; B's is stronger.
- **Problem reading.** No disagreement: both read progress literally and allow unreached states. Both answered the problem as written; the general form of g(n) is unanswered by both.

## Errors found

- A's `proofs.txt` ends by listing b=4, 8, 14 as the machine-checked instances of the 2^n − 1 sketch. Those use a different weighting; the report correctly names b=7 and b=15.
- A's Step 7 detour through J(1, f_j − f_i + 1) is unnecessary, since Step 5 already reaches J(f_i, f_j). Harmless.
- A calls its headline witness "7-state"; the processes have 3, 7 and 6 states.
- Mine: `broken2_progress.txt` breaks safety, not progress. Progress rejection was seen on A's `bad_progress.txt`.

## Witness checker

`vcheck.py`, written from the statement, accepts all 18 solver witnesses and rejects all 8 broken files (four from the solvers, four of mine). It rejects 274 of 369 one-entry mutants of B's headline witness and 195 of 225 of A's.

## Not checked

- n=2, b=3, s=5 in full; n=3, b=4, s=5 (my run timed out, no claim).
- The solution counts 19300 and 116680.
- The "least modulus for the Burns family" values 2, 4, 8, 14.
- Burns' correctness for general n.
- The solvers' code, beyond reading `synth.cpp`, `enum2.cpp`, `mc.h`.

One smoke test briefly ran as a third process, against the two-core rule.

## Follow-up tasks

1. **g(4).** Decide whether four processes have a solution with b in {4, 5, 6, 7}. Start with b=4: extend the hidden-process argument to rule it out, or find a witness with at most 8 states.
2. **Least s for n=3, b=4.** It is none at s=4 and yes at s=7; decide s=5 and s=6.
