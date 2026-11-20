# Prior art: f(w), sliding-window count with a (w+1)-state automaton

Standard names: Basic-Counting over a sliding window with additive error (streaming); finite-memory estimation and prediction of individual sequences (information theory); saturating counters. The "lit state on every cycle" rule and the exact budget of w+1 states were found nowhere.

## CLOSEST PRIOR WORK

1. **Efficient Summing over Sliding Windows** — Ben-Basat, Einziger, Friedman, Kassner, 2016. https://arxiv.org/pdf/1604.02450
   Shows: deterministic Wε-additive Basic-Counting needs at least max(log W, ⌊W/⌊2Wε+1⌋⌋) bits (Lemmas 3, 4); 1/(2ε)+2 log W+O(1) bits suffice.
   Differs: asymptotic, in bits, every configuration must answer; no exact optimum at a fixed state count.
2. **Statistical Inference with Limited Memory: A Survey** — Berg, Ordentlich, Shayevitz, 2023. https://arxiv.org/pdf/2312.15225
   Shows: surveys S-state machines, including the "imaginary sliding window" (S states estimating the ones in a window of length S−1) and saturable counters.
   Differs: randomized machine, mean-square risk on i.i.d. input, not worst-case spread.
3. **Deterministic Finite-Memory Bias Estimation** — Berg, Ordentlich, Shayevitz, 2021. https://arxiv.org/pdf/2206.09390
   Shows: minimax risk Θ(1/S) for deterministic S-state Bernoulli estimators.
   Differs: stochastic input, order of growth only.
4. **Finite-memory universal prediction of individual sequences** — Meron, Feder, 2004. https://search.isc.ac/Inventory/49/1010500.htm
   Shows (abstract only): regret bounds for deterministic K-state predictors on adversarial sequences; keywords include "saturated counter".
   Differs: regret against the best constant predictor, not per-state window ambiguity.
5. **Automata Theory on Sliding Windows** — Ganardi, Hucke, König, Lohrey, Mamouras, 2018. https://arxiv.org/pdf/1702.04376
   Shows: space classification for window membership in a regular language.
   Differs: exact membership, not approximate counting.
6. **Estimating a Probability Using Finite Memory** — Leighton, Rivest, 1986. https://people.csail.mit.edu/rivest/pubs/LR86.pdf
   Shows (snippet only): counter-like n-state estimators are as good as arbitrary ones for mean-square error.
   Differs: probabilistic setting.

## VERDICTS

- **Conjecture f(w) = ⌊(w−1)/2⌋ for w ≥ 3; values f(1..9):** NOT FOUND.
- **Upper bound f(w) ≤ ⌊(w−1)/2⌋ via the split-end and bouncy counters (P1, Theorems A, D):** NOT FOUND. The plain saturating counter is classical, but no worst-case window-spread analysis of it turned up.
- **Theorem B (g ≤ s forces ≥ w−s+1 states) and lemma P3:** EASY COROLLARY of Lemma 4 of item 1. Its pumping argument (0^W, then ones; the repeated configuration is also reached with an all-ones window) gives more than W(1−2ε) states, which is w−s+1 at s = 2Wε. This translation is mine and unchecked line by line.
- **f(w) ≥ 1 for w ≥ 2:** LIKELY FOLKLORE. For the all-lit e(w) it follows from Lemma 3 of item 1 (exact counting needs 2^W configurations); for f it needs a small extra step I did not find published.
- **e(w), N\*(w,s), counts of optimal machines:** NOT FOUND. Lemma 3 would give only about e(w) ≥ w/log₂(w+1) − 1 (my derivation).

Queries (18): automaton approximating ones in sliding window; basic counting deterministic lower bound; finite-memory prediction saturating counter; Ganardi Hucke Lohrey approximate counting; deterministic finite-memory bias estimation; approximate counting automaton states; "w+1 states" up-down counter; Meron Feder saturated counter; two-bit counter optimality; "infinitely often" abstain automaton; Leighton Rivest finite memory; minimum-state DFA window sum; up/down counter error bound; definite automata; Basic-Counting state complexity; moving average adversarial; suffix weight ambiguity; Ben-Basat follow-up constants.

## SEQUENCES (OEIS, looked up)

- f = 0,1,1,1,2,2,3,3,4: 33 hits, first 10 seen, all coincidental (prime partitions and similar).
- e = 0,1,1,2,2,2,3,3,4: 27 hits, first 10 seen, coincidental.
- Counts 2,61,77,5,3573,91: no match.
- N\* rows concatenated (7,3,2,10,5,3,2,8,4,3,2): no match.

## WHO TO ASK

A streaming-algorithms researcher on sliding-window lower bounds, or an information theorist in finite-memory estimation of individual sequences.

## NOT CHECKED

- Meron–Feder and Leighton–Rivest were seen as abstracts or snippets only; a deterministic saturated-counter optimality result there could be closer than I can tell.
- OEIS hits beyond the first page; hardware literature beyond one search.
- Sources read in full: none. Item 1's introduction, lower-bound and algorithm sections were read from extracted PDF text; items 2, 3 and 5 were downloaded and read only in keyword-located excerpts.
