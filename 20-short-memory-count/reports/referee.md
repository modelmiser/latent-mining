# referee report

My files: `verifier/`: `ref.cpp` (my search), `chk.py` (definitional checker: explicit (state, window) closure, brute-force simple cycles), `lemmas.py` (lemma tests), `mkbad.py` + `bad/` (broken witnesses), `runs.log` (raw outputs).

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| f(1..6) = 0,1,1,1,2,2 | CONFIRMED | recomputed unpruned over every machine with ≤ w+1 reachable states (w=6: 262,279,982) |
| f(7) = 3, f(8) = 3 | CONFIRMED | recomputed, pruned, two search orders; witnesses checked |
| f(9) = 4 (B only) | CONFIRMED | recomputed, pruned, one order, none at s=3 (7.9M nodes); witness checked |
| e(1..9) = 0,1,1,2,2,2,3,3,4 | CONFIRMED | recomputed: unpruned to w=6, pruned for w=7..9 |
| Counts with g = f: 2, 61, 77, 5, 3573, 91; w=7, s≤3: 143,863 | CONFIRMED | recomputed; g-histograms for w≤5 equal B's |
| f(w) ≤ ⌊(w−1)/2⌋, w ≥ 3 (A P1; B Theorems A, D) | CONFIRMED | proofs read, no gap; per-state ranges tested from every start state |
| f(w) ≥ 1, w ≥ 2 (A P2; B Theorem C) | CONFIRMED | proofs read, no gap |
| g ≤ s forces ≥ w−s+1 reachable states (A P3; B Theorem B) | CONFIRMED | proofs read, no gap; 6000 random machines |
| B's N\* table | CONFIRMED in part | all 27 `nstar_wit.txt` lines, `w7n9.txt`, `w9n11_s3.txt` checked; 13 "one state fewer fails" entries recomputed |
| Author's guess f(w) = ⌊w/2⌋ | REFUTED | f(4)=1, f(6)=2, f(8)=3, f(10)=4 |
| f(w) = ⌊(w−1)/2⌋ for all w ≥ 3 | UNSUPPORTED | no lower-bound proof beyond f ≥ 1; computed for w ≤ 10 |

**New, mine only:** f(10) = 4. No 11-state machine has g ≤ 3 (13,891,314 nodes, 24m44s; one program, one search order, no second confirmation); B_10 has g = 4.

The author's other beliefs hold: the 0..w counter has g = ⌊w/2⌋, the trailing-ones machine g = w−1 (w ≤ 10), e(2) = 1.

## Disagreements between the solvers

No value differs. Differences:

- **Family.** A uses S_w for all w ≥ 3; B uses the short counter for odd w and B_w for even w. For even w, S_w and B_w are the same transition table (tested w = 4..12). Both are correct.
- **Reach.** A stops at w=8, B at w=9; A searched e(w) directly, B squeezed e(6..9). Same values.
- **Confidence.** A 85%, B 65%. B's caution is better founded: one extra state beats the bound at w=7 and w=9, and I confirmed both machines.
- **Reading.** Identical: unreachable padding allowed, C uses strings of length ≥ w only, game reading equals the definition. I agree. Both answered the problem as written; neither delivered the headline (closed formula with proof), and both say so.

## Proofs marked COMPLETE

- **A P1:** g(S_w) ≤ ⌊(w−1)/2⌋. No gap found. The inequality m(j)+m(j′) ≥ (j+j′+2)/2 is loose but true; the "equality only at j=j′=1" step holds.
- **A P2:** g = 0 is impossible for w ≥ 2. No gap found.
- **A P3:** a state entered on s+1 zeros starts a 1-path of w−s+1 distinct states. No gap found. "It equals e" should read min(e, w).
- **B Lemmas 0–3:** g ≤ s iff the unlit states are acyclic; pruning and numbering are safe. No gap found.
- **B Theorem A:** the counter with ⌊w/2⌋+2 states has every spread ≤ ⌊w/2⌋. No gap found; L(k) bounds tested for all K ≤ w ≤ 10.
- **B Theorem B:** as A's P3. No gap found.
- **B Theorem C:** as A's P2. No gap found. "k ≥ w" is in fact k = w.
- **B Theorem D:** g(B_w) ≤ w/2 − 1 for even w ≥ 4. No gap found; the three stated ranges tested for w = 4..12 from every start state.

## Witnesses

- Accepted by `chk.py`: A's `witness_w1..14.txt`; B's `witnesses.txt`, `bw_small.txt`, `nstar_wit.txt`, `w7n9.txt`, `w9n11_s3.txt`.
- Rejected: my 10 broken witnesses (s lowered, over-wide state lit, edge changed, too many states, wrong w, three wrong g values, transition out of range), A's three `bad_*` files, and the one altered line in B's `bad.txt`.

## Errors and weak points

- B's `results.txt` gives N\*(9,3) as "≥ 11"; the report's "= 11" is right.
- B's w=9 certificate is self-referential: the 5525 prefixes come from the program under test. The logs are consistent (5525 distinct prefixes, all NONE, nodes sum to 1,703,509,895). My search replaces it.
- A and B report identical node counts at w=7 and w=8: the same search tree, so their agreement there is one check, not two.
- My broken witness a3 was rejected for the wrong reason (it lit the spread-2 state). A's `bad_w4_lit.txt` exercises the unlit-cycle path.
- B exceeded the 2-core limit for a few minutes, by its own account.

## Not checked

- A's `witness_w15.txt`; B's B_14 and B_16.
- The remaining N\* entries and B's w=11 probe.
- The solvers' code line by line (I read only B's `fast3.cpp`).
- f(9) and f(10) by a second independent program: my two orders share one code base, and pruned-versus-unpruned agreement stops at w=6.

## Follow-up tasks

1. **Decide f(11).** Does a 12-state machine with window 11 have g ≤ 4? Exhaustive pruned search split by prefix; my guess is 50–100 times the w=10 run. A machine found refutes the conjecture; none gives f(11) = 5.
2. **Prove f(w) ≥ 2 for all w ≥ 5.** Start from the w−s+1 path lemma applied to the 0-loop and the 1-loop together. B's 8-state machine with g = 1 at w=5 (`nstar_wit.txt`) shows the proof must use the state count.
