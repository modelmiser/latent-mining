<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the follow-up instance that built the certificate for 11 bins, verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Certificate for g(11) ≤ 3 (11 bins are not enough for load 4)

**Result.** The certificate exists and my checker accepts it: on 11 bins the Rule can keep every load below 4 against FIFO expiry. A by-product: the cores of `back2.cpp` and `vb.cpp` (3,723,730 and 3,722,933 entries) reduce to the same file byte for byte, so the two programs' winning sets are equal state for state, not only in count.

The harness refused to let me write `REPORT.md`, so this text is the only copy of the report. Everything else is in `(staging)/`; nothing was written under `(workspace)`.

## The certificate

`cert/vb_11_4.cert`: 3,334,554 states, one per line, newest ball first (90 MB, sha256 `35f5203d68c2cec608cebd0beeef9fe2a13ed7c6e22af303b8e265ff4488e99d`). It is the set C of minimal elements of the Adversary's winning set: the dumped core with every line that has a proper prefix in the core removed, then sorted. That form does not depend on search order.

- A = non-final states having some line of C as their j newest balls.
- S = non-final states not in A. S is the Rule's safe set.

## Soundness proof (full text: `PROOF.md`)

Properties 1–3 (empty state in S, loads ≤ 3, closed under Expire) follow from the form of S once the empty state is not a line. For property 4, suppose a state q in S has an offer with both answers bad. Three cases, each covered by a finite family:

- **Both answers make load 4:** q is a seed. The checker walks all minimal seeds and requires each in A.
- **Both answers land in A:** the longer of the two witnessing lines is a line c of C. The checker tests every line: if c with its newest ball flipped is in A or final, c minus its newest ball must be in A.
- **One answer lands in A, the other makes load 4:** the checker walks the extensions of each line up to the first moment the other offered bin reaches load 3, and tests there.

In each case the test forces q into A, a contradiction.

## What the checker enumerates, and timing

`check.cpp` (about 210 lines, written from the game definition, no code shared with the two programs) has three checking modes.

| Mode | What it walks at n = 11 | Time | Verdict |
|---|---|---|---|
| `min` | 3,183,950 minimal seeds; 22,716,542 states from the lines; 4,294,591 tests | 40 s, 0.56 GB | ACCEPT |
| `full` | every state of A (974,272,116, equal to both programs' counts), no reduction lemma | 567 s | ACCEPT |
| `brute` | every state, properties 1–4 literally | small n only | not run at 11 |

## Validation on known cases

- **n = 6, L = 3:** all three modes accept. `explicit.py` expands S to 6,651 states and checks 1–4 literally; the referee's `wcheck.py` accepts that list. All 3,843 states of `solver-a/rule_n6_L3.txt` lie inside S; S is larger because it is the largest safe set.
- **n = 7, L = 3 (Adversary wins):** every certificate tried was rejected by all modes: the true core, the core without the empty state, the n = 6 certificate, and 300 random ones.
- **n = 8, 9, 10 with L = 4:** accepted. `brute` agrees at 8 and 9 (139,229,839 states at n = 9); n = 10 has `min` and `full` only. The two programs' cores also reduce to identical files at n = 10.
- **Differential test:** on 1,560 random damaged certificates (n = 4 to 8), `min`, `full` and `brute` gave the same verdict every time; on 300 more at n = 9, `min` and `full` agreed (`brute` not run there).
- **Enumerator:** the child generator's state counts match an independent labelled-forest formula (`forestcount.py`) for n = 6 to 9, all 30 rows.
- **Prunings off:** same verdicts at n = 6 to 9.

## Damaged n = 11 certificates

- Nine single lines removed: all rejected, seven via the seed test and two (non-seed lines, chosen for that) via the offer test.
- Lines added: the empty state, `01`, `01 02`, `01 23 24 05`, `01 23 45 67 89` were rejected.
- One added line was accepted, correctly: `01 02 03 45 46 47 58` is itself a seed, already in A, so the set is unchanged. That was a badly chosen test, not a checker fault.
- Wrong parameters: n = 12, n = 10, L = 5, L = 3, and the n = 10 certificate at n = 11 were all rejected.

## What remains trusted

- That `check.cpp` implements the definitions in `PROOF.md` (relabelling, components, the 128-bit key, the hash set). This is tested, not proved.
- The proof itself, which is two pages.
- The seed pruning bound is the same bound the two programs use. It is proved in `PROOF.md` and checked with pruning off only up to n = 9.
- g++ and the machine.

## Not done

- No reviewer from another model family has read the proof or the checker.
- The certificate cannot go under 300 kB. Regenerate it with `DUMP=file gen/vb_dump 11 4 23` (about 7 min) or `DUMP=file gen/back2_dump 11 4` (about 13 min), then `certtool.py minimize`; both give the same sha256. These generators are copies of the original programs with one dump statement added, and are not trusted by the checker.
- No literal check at n = 10 or 11 (too many states).

Logs are in `out/`; about 470 MB of dumps, certificates and one damaged test file sit in `out/`, `cert/` and `tmp/`.

Files are in `(staging)/`:
- PROOF.md
- check.cpp
- certtool.py
- explicit.py
- difftest.py
- forestcount.py
- cert/vb_11_4.cert
- gen/vb_dump.cpp
- gen/back2_dump.cpp
- out/check_min_11_4.log
- out/check_full_11_4.log
- out/damage_11.log
- out/damage_11_nonseed.log
- out/difftest.log
- out/count_check.txt
