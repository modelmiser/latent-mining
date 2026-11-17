# Referee report

Own code and outputs: `verifier/` (`ref.py` literal definition; `ver.cpp` brute force + Lemma-5 route; `*_checks.txt`, `*_tests.txt`).

## Verdicts

| claim | verdict | how |
|---|---|---|
| M(1)=M(2)=4, M(3)=10 | CONFIRMED | recomputed, all 2^L strings, `ref.py` and `ver brute` agree (`ref_tables.txt`, `brute_small.txt`) |
| M(4)=18 | CONFIRMED | every necklace, L=15..24 (`brute_n4.txt`) |
| M(5)=34 | CONFIRMED | third method (below): none at 32, 33; 20, 40, 108, 240, 1440 necklaces at 34..38, all universal |
| M(n) >= 2^n+2, all n>=3 | CONFIRMED | A's P4/P5 and B's Theorem 1 read, no gap in either |
| M(n) <= 2^(n+1) | CONFIRMED | proofs read; DD checked n=1..6 |
| M(6) <= 76 (B) | CONFIRMED | witness checked from the definition, all 153 adversary moves |
| M(6) >= 67 (A) | CONFIRMED | independent exhaustive computation, 0 repairable loops of length 66 |
| M(n)-2^n unbounded | UNSUPPORTED | both solvers give labelled sketches only |

So 67 <= M(6) <= 76. No table entry of mine differs from either solver.

## Length 66, n=6

Lemma 5 does settle it, and cheaply. Universal loops of length 2^n+e are Eulerian circuits of the de Bruijn graph plus a balanced multiset of e extra edges. A short lemma (mine, extending P4): a universal loop of length 64..67 has 32 or 34 runs, and playing "loss in a run >= 2" and "loss of a singleton" against w forces every repairable w of length 66 to have exactly 34 runs. That leaves, up to complement:
- length 65 (extra 000000) with a foreign cell splitting a run: 67,108,864 loops (= 2^26), 2.2e9 candidates;
- length 66 (extra 010101, 101010): 113,246,208 loops, identical to B's count;
- length 67 (extra 000000+010101+101010, or the 3-cycle 001001, 010010, 100100) minus a cell of a run >= 2: 298,319,872 loops, 5.0e9 candidates.

All 7.29e9 candidates tested, 0 repairable (`n6_L66_route_summary.txt`, 64 chunks, all exit 0, about 80 CPU-minutes). The same program reproduces brute force for n=3 (L=8..16) and n=4 (L=16..22, including non-universal solutions), and its run-count and complement filters reproduce the full sets at n=4 and n=5.

A's P8: read line by line, no gap; `dfs3.cpp` implements what it states. Tested per k (stronger than A's union-over-k check): in 39 (n,L,k) cells, n=2..5, L up to 24, the pruned search contains every loop with exactly k missing words found by my unpruned search, including k=2, 3 cells with solutions (`dfs3_vs_unpruned.txt`). A's claim is established twice independently.

## Proofs marked COMPLETE

- A P0-P3, P6: no gap (P0 ignores L=1, harmless).
- A P4 (universal of length 2^n or 2^n+1 has 2^(n-1) runs): no gap; 0 violations in 433 loops.
- A P5 (no repairable loop of length 2^n or 2^n+1, n>=3, by run count): no gap.
- A P8: no gap (above).
- B Fact A, B (singleton counts): no gap; 0 violations mechanically. Fact C: no gap; smoke-tested only.
- B Theorem 1 (same bound via singletons, n>=6): no gap.
- B Theorem 2, Lemma 4, Lemma 5: no gap; Lemma 5 confirmed by route = brute.
- B Theorem 3 (surplus windows alternate): no gap; holds on all 20 n=5 loops.

## Disagreements

1. M(6) lower bound, 67 (A) vs 66 (B): A is right.
2. M(6) upper bound, 78 (A) vs 76 (B): B's witness is valid; A's 78 and 80 are valid too.
3. Lower-bound proof: A proves n>=3; B proves n>=6 and computes n=3, 4, 5. Both correct.
4. Counts: A in necklaces, B in strings; consistent (x L).

Both read the game literally (w need not be universal), as I did. No easier neighbour was answered.

## Errors and weaknesses

- A's validation of the pruning compares only the union over k. With K fixed `dfs3` drops true solutions with other k (n=4, L=22, K=0 returns 1159 of 1172), so the union test could mask a per-k fault. The per-k test passes.
- A's k>=2 runs at n=6 reach no leaf: a check that could not fail by itself. Now covered independently.
- A: "non-universal repairable loops exist (n=2, L=6; n=4, L>=20)" is incomplete: also n=3, L>=13. None for n=5 through L=38.
- B's title says "repairable universal loops"; the object is repairable loops.
- All 13 strategy tables check (`witness_checks.txt`).
- My checker rejected nine broken objects (bit flip, cell removed, swap of the 76-witness; de Bruijn cycles; others).

## Not checked

- Candidates of length 66 with a run count other than 34 are excluded by my lemma (and by A's run), not enumerated by me.
- The n=6 run uses a fast rejection bound validated against the plain test only at n=4, 5.
- A's `dfs2.cpp` (k=0) was not read; its result is reproduced.
- Sketches P7 and Sketch 6, annealing claims, B's no-pass variant, n=6 lengths 67..75.

## Follow-up tasks

1. Decide whether a repairable loop of order 6 and length 67 exists (then 68): same route with universal loops of length 66..68; first derive run-count and singleton-count restrictions on the surplus multiset, since the unrestricted candidate set is near 10^11.
2. Prove or refute: for every n >= 6 no loop of length 2^n+2 is repairable. Theorem 3 and the 34-run lemma generalise and fix the surplus structure; what is missing is a singleton (context 001011) whose loss provably cannot be repaired.
