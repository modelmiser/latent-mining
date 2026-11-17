# solver-a report

All files are in `solver-a/` (map: `FILES.txt`).

## 1. Tables

| n | M(n) | status | method |
|---|------|--------|--------|
| 1 | 4 | COMPUTED | every loop up to length 6, `slow.py`; agrees with `fast brute` |
| 2 | 4 | COMPUTED | every loop up to length 7, same |
| 3 | 10 = 2^3+2 | COMPUTED | every loop up to length 11, same |
| 4 | 18 = 2^4+2 | COMPUTED | every necklace of length 12..22, no pruning (`fast brute`, `brute_n4.txt`) |
| 5 | 34 = 2^5+2 | COMPUTED | normalised DFS, lengths 32..36 (`fast dfs`, `dfs_n5.txt`) |
| 6 | 67 <= M(6) <= 78 | lower COMPUTED, upper by witness | see below |

Number of repairable loops (necklaces for n<=4; normalised words for n=5):
n=3: 3, 10 at L=10, 11. n=4: 12, 24, 62, 308, 1172 at L=18..22. n=5: 20, 40, 108 at L=34..36.
Every repairable loop of minimum length is universal (n<=5). Non-universal repairable loops exist (n=2, L=6; n=4, L>=20).

n=5 pruning: longest 0-run first, last cell 1 (removes rotations only); at most L-2^n+(n-1) repeated windows in a prefix (the pass move forces >= 2^n-(n-1) distinct words). An unsafe prune biases M upward; this DFS reproduces the unpruned n=4 counts.

n=6:
- Length 64, 65: impossible by proof (section 4).
- Length 66: no repairable loop. COMPUTED, exhaustive, but with an extra "needy cell" pruning (`proofs.txt` P8), run separately for k = 0..5 missing words (`n6_L66_k*.txt`, about 80 minutes on 2 cores). That pruning is validated only by reproducing the unpruned result sets on 13 (n,L) pairs with n<=5, including cases with non-universal solutions (`validation3.txt`). For k>=2 no branch reached a leaf, so those runs rest entirely on the pruning argument.
- Length 67..77: not decided. Annealing reached cost 1 (one unanswerable loss) at 76 and 77 and found nothing below 78. A random DFS over universal loops found nothing at 66..80, which is weak evidence only.
- n=7 annealing did not converge.

## 2. Witnesses (`witnesses.txt`, checked by `check_witness.py` using only `slow.py`; output `check_witness.out`)

- n=1: 0011. n=2: 0011. n=3: 0001011101. n=4: 000010011010101111.
- n=5: 0000010010101101111100011001110101.
- n=6, length 78: 000001001011011111100110001010100011110100111000011010101110110010010111010010.
- `strategy_n*_L*.txt` lists, for each witness, every adversary move with a winning mechanic reply.
- Deliberately wrong objects (de Bruijn cycles, a length-9 loop, perturbed witnesses) are rejected by the same checker.
- "Next better value impossible": proof for 2^n and 2^n+1 (n>=3); exhaustive files above otherwise.

## 3. Headline

M(n) = 2^n + 2 for n = 3, 4, 5, and this is a proved lower bound for all n >= 3. It is not the general formula: M(6) >= 67 = 2^6 + 3, and the best loop I found has length 78 = 2^6 + 14. I have no closed formula and no theorem that determines M(n).

Is M(n) - 2^n bounded? Not settled. My belief is that it is unbounded (roughly 75% confidence), based on the jump at n=6 and on the mechanism in sketch P7. The data do not distinguish growth like 2^n/poly(n) from the brief's guess (1+d)·2^n; I make no conjecture on the rate. The brief's reason for its guess ("deleting a length-1 run destroys about n words") is false as stated: next to long runs or periodic stretches most of those words reappear at the junction, which is exactly how the n<=5 witnesses work.

## 4. Proofs (`proofs.txt`)

- P0 COMPLETE: M(n) >= 2^n.
- P1 COMPLETE: M(n) <= 2^(n+1) (de Bruijn twice; the two occurrences of a word are 2^n > n-1 apart, one edit hits at most one).
- P2 COMPLETE: one edit creates at most n-1 new words.
- P3 COMPLETE: the brief's lemma. For universal w only losses of isolated cells matter.
- P4 COMPLETE: a universal loop of length 2^n or 2^n+1 has exactly 2^(n-1) runs.
- P5 COMPLETE: **M(n) >= 2^n+2 for all n >= 3**, universal or not. A loss in a run of length >= 2 forces 2^(n-1) runs in w; a loss of an isolated cell forces 2^(n-1)+2. So w has only one kind of run, and both cases fail by length or by the need for 0^n and 1^n.
- P7 SKETCH, not a proof: unboundedness. The counting step is tight by one word, and the missing step (the mechanic must rebuild a factor of length 2n-2 from a near-copy and pay for what his own edit destroys) is not done.
- P8: soundness argument for the n=6 pruning; complete as an argument, implementation checked only empirically.

## 5. Interpretation

- Taken literally: the mechanic may edit after an adversary pass, so w need not be universal. All values above are for this reading.
- Under the alternative reading "w must be universal", the values for n<=5 are unchanged, and M(6) >= 67 needs only the k=0 run (`n6_L66_k0.txt`).
- n=1, 2 are degenerate (M = 4 for both) and do not fit 2^n+2.
- Window wrap for m < n never arises for n >= 2 at the lengths in play.

## 6. Code

- `slow.py`: n<=3 tables, reference definition. `check_witness.py`: witnesses and strategies.
- `fast.cpp`: n=4 (`brute`), n=5 (`dfs`), cross-checks for n<=3.
- `dfs2.cpp`: n=6, L=66, k=0. `dfs3.cpp`: n=6, L=66, k=1..5, and `validation3.txt`.
- `anneal.cpp`: n=6 witnesses of length 78 and 80. `shrink.cpp`, `randsearch.cpp`: heuristic, no claims rest on them.
