# solver-b report: M(n) for repairable universal loops

All files are in `solver-b/`.

## 1. Tables

| n | M(n) | status | method |
|---|------|--------|--------|
| 1 | 4 | COMPUTED, exhaustive | all 2^L loops, L<=10: `slow.py` (definition, literal) and `brute.cpp`, identical counts |
| 2 | 4 | COMPUTED, exhaustive | same |
| 3 | 10 = 2^3+2 | COMPUTED, exhaustive | same, L<=13 |
| 4 | 18 = 2^4+2 | COMPUTED, exhaustive | `brute` on all 2^L loops, L=16..22; `dfs` and `uni` give the same sets (md5) |
| 5 | 34 = 2^5+2 | COMPUTED, exhaustive | L=32,33: none, under two independent prunings (`dfs`, `uni`); L=34: 20 necklaces, same set from both |
| 6 | 66 <= M(6) <= 76 | lower bound PROVED; upper bound is a heuristic witness, NOT known minimal | Theorem 1; `sa2`, `polish2` |

Counts of repairable loops (all strings): n=3, L=10..13: 30, 110, 300, 910. n=4, L=18..22: 216, 456, 1240, 6468, 25784. n=5, L=34: 20 necklaces.

n=6, L=66, COMPUTED but partial: no *universal* loop of length 66 is repairable (0 of 113,246,208 candidates; Theorem 3 shows that candidate set is complete). Non-universal loops of length 66 are not excluded, so M(6) > 66 is not established.

## 2. Witnesses (`witnesses.txt`, `strategy_n1..n5.txt`, `strategy_n6_L76.txt`)

- n=3: `0001011101`
- n=4: `000011110101011001`
- n=5: `0000011100110001010111110110101001`
- n=6, L=76: `1110100100000011110010101000110011010101100010111011011111101000100111000010`
- Check any of them with `python3 explain.py <n> <loop>`: a separate Python checker that lists every adversary move with all winning replies. It rejects deliberately wrong objects (three examples in `witnesses.txt`, e.g. the n=5 witness with one bit flipped).
- All shortest loops: `n3L10.txt`, `n4L18.txt`, `n5L34.txt`; every one is universal.
- Impossibility is by exhaustive search; reproducible counts are in `witnesses.txt`.

## 3. Headline

**Not settled.**

- PROVED: 2^n + 2 <= M(n) <= 2^(n+1) for every n >= 3. The lower bound is a proof for n >= 6 and a computation for n = 3, 4, 5.
- COMPUTED: M(1) = M(2) = 4 and M(n) = 2^n + 2 for n = 3, 4, 5.
- n = 6: the pattern would need a repairable loop of length 66. No universal one exists. The shortest loop I found has length 76, from a heuristic search that I do not trust to be near-optimal (the first-stage annealer missed the true optimum by 1 at n=5).
- Is M(n) - 2^n bounded? OPEN. My guess is unbounded, roughly 75% confident. The reason is in Sketch 6: the two cheap repair mechanisms that give surplus 2 for n <= 5 do not apply to singletons forced by 001011 (n >= 6) or 1001001 (n >= 7), and the number of such forced contexts grows exponentially.
- M(n) >= (1+d)2^n: no proof and no data. I do not rate it above a coin flip.
- No conjectured formula. "M(n) = 2^n + 2 for n >= 3" fits n <= 5 and I expect it to fail at n = 6.

## 4. Proofs (`proofs.txt`)

- **Theorem 1, COMPLETE**: for n >= 6 no loop of length <= 2^n + 1 is repairable. Let S be the number of length-1 runs. A universal loop of length l has 2^(n-2) <= S <= 2^(n-2) + l - 2^n. Deleting a cell of a length-2 run forces S(w) <= 2^(n-2) + 1; deleting a singleton that has a singleton neighbour forces S(w) >= 2^(n-2) + 2.
- **Theorem 2, COMPLETE**: M(n) <= 2^(n+1). A de Bruijn cycle written twice survives any one edit, so the mechanic passes.
- **Theorem 3, COMPLETE**: for n >= 5, a universal repairable loop of length 2^n + 2 has only alternating words in its two surplus windows.
- **Lemma 4, COMPLETE**: the problem's lemma is correct.
- **Lemma 5, COMPLETE**: a repairable loop is a universal loop, or one plus an inserted cell, or one minus a cell. This is what `uni.cpp` enumerates.
- **Sketch 6, SKETCH only**: why the surplus should grow. The missing step is named there: universality does not control the 2n-1 cells around a singleton, and periodic contexts allow coincidences I could not exclude.

## 5. Interpretation

- I read the statement literally; nothing in it needed changing.
- All UNVERIFIED beliefs hold: the bounds 2^n <= M(n) <= 2^(n+1), M(1) = 4, M(2) = 4, M(3) = 10 with the stated witness, and the lemma.
- n = 1, 2 are degenerate; the regular behaviour starts at n = 3.
- Other reading, "the adversary must edit (no pass)": M is unchanged for n <= 4 (computed with `ADVNOPASS=1 ./brute`, output in `variant_runs.txt`). Not checked for n = 5.

## 6. Code

| file | produced |
|---|---|
| `slow.py` | reference counts, n<=3 |
| `brute.cpp` | n<=4 tables, the no-pass variant |
| `dfs.cpp` | n=5, L=32..34, pruning by repeated windows (`dfs_n5_L34.txt`: 180 strings = the 20 necklaces) |
| `uni.cpp` | n=5, L=32..36 via Lemma 5 (`n5_runs_a.txt`) |
| `uni128.cpp` + `run_chunks.sh` | n=6, L=66 universal case, 128-bit, `ALTONLY=1` (`n6_66u/`) |
| `sa2.cpp`, `polish2.cpp` | n=6 upper bounds (`sa2_n6_*.txt`, `polish2_n6_*.txt`) |
| `explain.py`, `canon.py` | independent checker; necklace canonical form |

`n6_L66_universal.txt` and `n6_L66_universal_v2.txt` are stopped runs, marked INVALID inside. `sa.cpp` is an earlier annealer; its output is not used.
