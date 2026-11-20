# solver-a report

**Headline: g(3) = 4.** Lower bound: a written proof that b = 2 and b = 3 admit no solution for any n ≥ 3 and any s. Upper bound: a machine-checked 7-state witness with b = 4. For general n only g(n) ≥ 4 (n ≥ 3) is proved; no formula.

All files are in `solver-a/`.

## 1. Tables

Does a solution with at most s states per process exist?

- NONE / YES = COMPUTED by exhaustive search (`synth.cpp`: lazy backtracking over partial transition tables, complete up to state renaming; a partial table is rejected once the part of the configuration graph it fixes has a safety or progress violation, which is monotone).
- PROVED = `proofs.txt`, all s.
- t/o = search not finished in 1400 s; no claim.

n = 2:

| b \ s | 3 | 4 | 5 | 6 |
|---|---|---|---|---|
| 2 | NONE | NONE | NONE | YES |
| 3 | NONE | NONE | t/o | YES |

n = 3:

| b \ s | 3 | 4 | 7 | all s |
|---|---|---|---|---|
| 2 | NONE | NONE | | NONE, PROVED |
| 3 | NONE | NONE | | NONE, PROVED |
| 4 | NONE | t/o | YES | |
| 5 | NONE | not run | YES | |

Every instance the problem asks for (n = 2, s ≤ 4, b ≤ 3; n = 3, s = 3, b ≤ 4) is NONE, COMPUTED. The least s for n = 2, b = 2 is exactly 6.

Checks:

- `brute.py` (every tuple of fully specified automata, no pruning, judged by the independent Python checker) finds 0 solutions for (n,b,s) = (2,2,3), (2,3,3), (3,2,3), agreeing with synth. Only NONE answers were cross-checked this way; YES answers rest on `checker.py` accepting the witness.
- The checker was watched failing: `bad_safety.txt` and `bad_progress.txt` (one-entry mutations of the n = 2 witness) are rejected with a schedule.
- Pruning in synth: canonical state numbering; no "add 0" for non-crit states (covered by the lazy read branch); crit is "add c → rem". An unsafe pruning would bias towards NONE.

## 2. Witnesses (plain text; format in the `checker.py` header)

- `w_n2_b2_s6.txt`, `w_n2_b3_s6.txt`: n = 2, 6 states, b = 2 and b = 3.
- `w_n3_b4_burns.txt`: n = 3, b = 4, 7 states. Burns' one-flag algorithm with flag weights 1, 1, 2 mod 4, hand-written; checker OK, 104 reachable configurations.
- `w_burns_n3_b5.txt`, `w_burns_n3_b7.txt`: same construction mod 5, mod 7.
- `w_burns_n4_b8.txt` (weights 1,1,2,4), `w_burns_n5_b14.txt` (weights 1,1,2,5,5): checker OK. These are the least moduli for this construction only (`burnsmin.py`).
- Impossibility for b = 2 and b = 3: `proofs.txt`.

## 3. Headline and confidence

g(3) = 4, confidence about 90%. The residual risk is an error in the b = 3 proof, which I wrote and re-read but did not machine-check. The exhaustive NONE for n = 3, b = 3, s ≤ 4 agrees with it but is weak evidence.

The problem's guess (4) is right; its stated reason is not the proof. "Weights 1,1,2 mod 5" improves to mod 4. g(2) = 2 holds.

General n:

- PROVED: g(n) ≥ 4 for n ≥ 3.
- COMPUTED upper bounds: g(4) ≤ 8, g(5) ≤ 14.
- SKETCH: g(n) ≤ 2^n − 1 (relies on Burns' correctness for general n, not re-proved; machine-checked only for n = 3, b = 7 and n = 4, b = 15).
- Whether g(n) grows with n is open to me; no conjectured formula.

## 4. Proofs (`proofs.txt`)

- **Theorem 1, COMPLETE: b = 2, n ≥ 3 impossible.** Two processes are stopped just after their first add (the adds cancel), a third enters alone, and the first then sees exactly its solo view and enters too.
- **Theorem 2, COMPLETE: b = 3, n ≥ 3 impossible.**
  - All first adds are equal (WLOG +1), solo crit has R = 1, and exit restores R = 0.
  - Using a hidden third process, each process is forced to behave like a flag in a two-party duel: after raising and seeing one other, its next change must be to lower; after lowering and seeing nobody, its next change must be to raise.
  - Each duel stage is therefore "hold for ever" or "lower", and stage states of two processes can be combined at any offset, with the third process as sparring partner.
  - Two processes that each have a hold stage can be brought to deadlock; two that have none livelock in lockstep. Among three processes one such pair exists.
- The b = 3 argument does not extend to b ≥ 4 as written.

## 5. Interpretation

- Progress read literally: a process may stay in rem for ever and is then never scheduled. Finite test used: a reachable crit-free strongly connected set of configurations with constant nonempty active set S, containing a step of each member of S (rem is entered only from crit, so the active set cannot shrink).
- "At most s states": unreached states and read branches are allowed (`unreached` / −1 in witness files).
- b ≥ 2 is part of the definition, so g(2) = 2 needs only the witness.
- No alternative reading was computed.

## 6. Code

- `synth.cpp` (`synth n b s`): all table entries except n = 2, b = 3, s = 6. Logs of the timed-out runs: `run_n2_b3_s5.out`, `run_n3_b4_s4.out`.
- `synthfam.cpp`: restricted flag-count family; produced `w_n2_b3_s6.txt`.
- `checker.py`: witness verification. `brute.py`: cross-checks.
- `burnsgen.py`, `burnsmin.py`: weighted-Burns witnesses and their least moduli.
