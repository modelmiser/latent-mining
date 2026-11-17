# solver-b report

Files: `solver-b/`.

## Headline

**c(k) = 1 − 1/k for every k ≥ 3, PROVED** (limit exists; strategy and matching upper bound complete). c(3) = 2/3, c(4) = 3/4. The bet "c(4) = 5/8 exactly" is false; "c(4) ≥ 5/8" and "c(k) → 1" are true.

Finite form, proved for k ≥ 3, n ≥ max(5, k+1):

    n − ⌈n/k⌉  ≤  F(n,k)  <  n − n/k + 1

So F = n − n/k exactly when k | n; otherwise F is one of two consecutive values. Confidence: high (short proofs, lemmas tested against a set-based simulator).

**Also PROVED:** F(n,1) = F(n,2) = ⌊n/2⌋ for all n ≥ 3, so c(k) = max(1/2, 1 − 1/k) for all k ≥ 1.

**Conjecture (computed only):** F(n,k) = n − ⌈n/k⌉ for all 3 ≤ k ≤ n. Every exhaustive entry agrees. First entry the proofs leave open: F(10,4) (bound allows 8, search gives 7).

## 1. Tables

Rows n = 3..11, columns k = 1..4:

    n=3:  1 1 2 2
    n=4:  2 2 2 3
    n=5:  2 2 3 3
    n=6:  3 3 4 4
    n=7:  3 3 4 5
    n=8:  4 4 5 6
    n=9:  4 4 6 6
    n=10: 5 5 6 7
    n=11: 5 5 7 8*

- All COMPUTED by exhaustive memoised game search (`decide.cpp`), except F(11,4)=8*: that search hit the 1500 s timeout, and the value is PROVED instead (strategy ≥ 8, Theorem U ≤ 8).
- Cross-checks, all agreeing: `slow.py` (direct definition, unpruned) n ≤ 6; `fast.cpp` (value search on bitmasks) n ≤ 9; `decide` with half-speed pruning off, n ≤ 8.
- Larger k, COMPUTED for n ≤ 8, k = 5..2n (`bigk.log`, `results.txt`): F(n,n) = n−1, F(n,2n) = 2⌊n/2⌋; for even n the value n appears only at k = 2n.
- F(3,3) = 2 > 1, so k = 3 already delays at n = 3.

## 2. Witnesses

- `surv_n{n}_k{k}.txt`: explicit D_1..D_{F−1} leaving nobody complete, for every computed entry (77 files).
- `check.py`: independent set-based checker; all 77 pass (`check_log.txt`).
- Deliberately wrong objects, all rejected: k=4 witness checked at k=3 ("ILLEGAL: budget"); witness extended one round ("COMPLETE"); repeated link ("ILLEGAL: link destroyed twice in a row").
- "Next value impossible": exhaustive search, plus Theorem U where tight, plus Theorem K2 for k ≤ 2.
- `construct.py n k` writes the general strategy (e.g. `sawtooth_n10_k4.txt`, `sawtooth_n30_k5.txt`).
- `verify_construct.py 60 12` simulated it for all n ≤ 60, k ≤ 12: on the 524 instances with k ≥ 3 it achieves exactly n − ⌈n/k⌉, and Theorem U's bound is at most 1 higher. `bounds.py` prints the bracket per (n,k).

## 3. Strategy (sawtooth)

T = n − 1 − ⌈n/k⌉; k base points spaced ≤ ⌈n/k⌉.

- Odd rounds: destroy k links (i+1 → i), each stepping one position against the flow every two rounds (tracks one diagonal).
- Even rounds: the mirror image on links (i−1 → i).

This builds k overlapping delay ramps per direction; the two sawtooths are aligned so their sum is constant, T + 1 − ⌈n/k⌉. Exactly k destructions per round, no link repeated.

## 4. Proofs (`proofs.txt`, each marked COMPLETE)

- **Lemma E:** K_t(i) is an arc with extents L, R obeying a simple recursion; directions interact only through the budget.
- **Lemma D:** deficit a = t − R in a moving frame is monotone, satisfies A(x) ≤ A(x−1)+1, a hit sets A(x) = A(x−1)+1, and A ≤ ⌈t/2⌉ (consecutive ramp steps would hit one link twice running). Nobody complete at T iff a_i + b_i ≥ 2T − n + 2 for all i.
- **Lemma Φ:** sum of A over tops (A(x+1) ≤ A(x)) ≤ number of hits in that direction (needs k < n).
- **Theorem U:** h = ⌈T/2⌉, m = 2T−n+2. If nobody is complete at T and m ≥ h+1, then 2nh ≤ kT(2h+1−m). Corollary: F < n − n/k + 1 for k ≥ 3.
- **Theorem L:** the sawtooth gives F ≥ n − ⌈n/k⌉.
- **Theorem K2:** F(n,1) = F(n,2) = ⌊n/2⌋.
- **Section P:** search prunings are safe (useful links only, restricted previous set, dihedral symmetry, half-speed bound); an unsafe one could only lower F, and the unpruned solvers agree.
- `lemma_test.py` checks E, D, Φ on random legal plays (7767 positions); random plays rarely build ramps, so it is a weak test of Φ.

No sketches are presented as proofs. Only the conjecture is unproved (the odd-T case of Theorem U is loose by at most 1).

## 5. Interpretation

Nothing needed an alternative reading. Links are directed and budgeted separately; "complete at round t" is tested after round t's deliveries; F = 1 + (largest T the adversary can keep everyone incomplete through). The headline theorem assumes n > k; for k ≥ n only computed values are given. The stated bounds ⌊n/2⌋ ≤ F ≤ 2⌊n/2⌋ and monotonicity in k hold in all data.

## 6. Code → numbers

- `decide.cpp`: tables, `surv_*.txt`.
- `fast.cpp`, `slow.py`: cross-checks (`witness_*.txt`, `fast_9_4.log`).
- `check.py`: witness verification.
- `construct.py`, `bounds.py`, `verify_construct.py`: strategy and proved brackets.
- `lemma_test.py`: lemma checks. `results.txt`, `bigk.log`: collected values.

C++ searches ran under nice 15, at most 2 at once, 8 GB ulimit, 1500 s timeout; the small Python scripts ran niced, some without ulimit or timeout.
