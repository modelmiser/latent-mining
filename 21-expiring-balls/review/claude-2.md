<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of a Claude seat run on the plan login (one of the two on Claude Opus). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Claude seat 2: experiment 21 (expiring balls)

Everything below was checked by reading the files and doing the algebra by hand. My only computation was two short checkers of my own, written outside the repository (scratchpad): one walks `solver-a/rule_n6_L3.txt` over all labelled pairs of the 6 bins, the other replays `solver-a/adv_n7_L3.txt`. Both accept: 3843 states and 0 unanswerable offers; 22 non-terminal states reached, at most 6 balls present.

## Verdicts

- **Q1: CONFIRMED.** README.md:66-74 played by hand. Step 1: 3 offers on {a,b,c,d}, and x has load 2. Step 2: {e,f} is untouched, so the offer is legal. {x,f} crosses components, and giving the ball to x reaches 3, so f must take it. Step 3: expiring the 3 older balls leaves (e,f)→f and (x,f)→f. That is a clean 2-star on {e,f,x}, with 4 bins untouched. Step 4: 3 offers, then {f,z}. These cross components, give 6 balls on 7 bins, and need no Expire. The branch where x = r2 (x already holds the two youngest balls) also uses exactly 7 bins under the README's uniform steps. P5's shortcut (proofs.txt:36-37) is an optional saving and makes no material difference. The certificate follows a different line of play (proofs.txt:42) but is checked on its own.
- **Q2: CONFIRMED.** verifier/wcheck.py:53-63 checks that the empty state is in the set (54), that no state has load L or more (56), and that the set is closed under Expire (57). It also checks every offer: used×used pairs across components, used×first-fresh and fresh×fresh (37-46), with both answers tried (60-62). Treating all fresh bins as one is sound. `canon` (6-13) relabels bins by first appearance in age order, so it is a complete invariant of the bin-permutation orbit. The game is symmetric under bin permutation, so working up to relabelling is sound. The adversary leg (79-99) checks legality and requires the rank to strictly decrease.
- **Q3: CONFIRMED.** I followed proofs.txt:46-57 by hand.
  - Ia uses 7 bins and leaves 5 free. If the Rule gives x its third ball early, Ia simply ends sooner.
  - Ib: the threat on {x,Y} is valid. After Ia's balls expire, b1 (p,q) is the oldest ball. If Y = r2, expiring b1 leaves a clean 3-star on {Y, s', r1, x} with 8 bins untouched.
  - I′ (Y = r1): 5 + 4 + 2 = 11 bins are in use. If a receives, {a,Y} is legal and wins. If f receives, the threat on {f,Y} holds and f holds the 3 youngest balls. After expiry the star is {f, f′, a, Y}, with 4 + 3 + 1 = 8 bins freed.
  - II: P1 places 7 balls with no Expire, then {z, centre}, for 11 balls in all.
  No step fails. The referee's own replay agrees (reports/referee.md:25).
- **Q4: CONFIRMED (method), with a wording caveat.** In verifier/vb.cpp:5-8, three rules (seed, older ball, offer) generate the least fixed point, and that fixed point is exactly the adversary's attractor. A win by Expire is the older-ball rule. A win by an offer has two answers, each either terminal or in A, which is the seed rule or the offer rule applied to whichever answer joins later. Closure under adding an older ball holds trivially, since Expire returns to p. So storing a core and testing suffixes is exact. In back2, a core element is added only when it is not already in A (back2.cpp:45), so each state is visited once and after it joins.
  - The pruning bound is sound. Prepending only merges components. Keeping two components allows at most n−2 balls in all, and each ball adds 1 to one load. back2.cpp:36-40; vb.cpp:45-48, where line 46 is a no-op and errs only on the side of pruning less.
  - What the README says the two programs share (README.md:94-96) is accurate, as is the absence of a certificate.
- **Q5: CONFIRMED.**
  - solver-a P2 / solver-b P1: the receiver had load k−1. The other end had load at least k−1, because otherwise the smaller-load rule would have chosen it. In a forest, j present balls at a bin go to j distinct neighbours, so each component has at least k bins, and the two components are disjoint. The argument is about a single moment, so Expire does not matter.
  - P1/P2 (halving) is complete. solver-a P4: T(j)−j = T(j−1) holds. solver-b P3: log2 a + 1 ≤ log2(a+b) because a ≤ b.
- **Q6: Partly UNSUPPORTED.** See F1 and F2.
- **Q7: CONFIRMED, with small exceptions.** The counts match their sources. 97%: referee.md:3. 974,272,116 and 3,183,950: out files. 3,662,464 and 3,521,600: proofs.txt:45, referee.md:25. 32/33 lines and 22/23 non-terminal states: referee.md:60, and my replay gives 22. The 11–13 any-removal point: referee.md:61. Three processes: solver-a.md:79. 64/48 MB: solver-a.md:37, solver-b.md:114. Exceptions: F3, F4, F5.
- **Q8: CONFIRMED.** README.md:23-35 is the same game as chain/3-question.txt:1-8. The top-level README.md:52 row matches the folder.

## Findings

**F1. MODERATE (confidence high).** README.md:85-87: "So for k = 4 the order of expiry costs the adversary exactly one bin." This rests on N(4) ≥ 12, which has no certificate (about 97%), and on A(4) ≥ 11, whose certificate `rule_any_n10_L4.txt` is not in the repository (README.md:121). The sentence is unhedged.
Replacement: "If the computed g(11) = 3 is right, then for k = 4 the order of expiry costs the adversary one bin (12 against 11); for k ≤ 3 it costs nothing."

**F2. LOW (confidence high).** README.md:11: "letting the oldest ball expire helps the adversary, from k = 3 on." This is true, but no file argues it for k ≥ 5. It has a one-line proof. Force load k−1 on N(k−1) bins with expiry, then build a second load-(k−1) bin by P1 on 2^(k−1) untouched bins without Expire, and offer the two. So N(k) ≤ N(k−1) + 2^(k−1), which gives N(k) ≤ 2^k − 1 for all k ≥ 3 (and N(5) ≤ 28).
Replacement: add "(for every k ≥ 3: N(k) ≤ N(k−1) + 2^(k−1) by forcing load k−1 first and then merging on fresh bins, so N(k) ≤ 2^k − 1)".

**F3. LOW (confidence medium).** README.md:46 says the referee's program "finds the same winning set", and README.md:94 says the two programs "agree to the last state". Only the counts were compared: the attractor size and the number of minimal seeds. The cores differ, 3,723,730 against 3,722,933 (solver-b/out_11_4_back2.txt; verifier/out_vb_11_4.txt), as one would expect from an order-dependent core. No comparison of the sets themselves was made.
Replacement: "finds a winning set of the same size (974,272,116 states) from the same 3,183,950 minimal seeds" and "agree exactly in the size of the winning set".

**F4. LOW (confidence medium).** README.md:102-104 presents the value ⌊log₂ n⌋ as "the warm-up algorithm of Bender et al." prior-art.md:9 gives only an upper bound for that algorithm, ⌈log n⌉, from its introduction. The README's "Ozdayi's semester project" description does not appear in prior-art.md:5. Its "relevant sections of three papers" omits the fourth paper, whose introduction was also read (prior-art.md:8, 54).
Replacement: "…is Lemma 2.1 of Ozdayi (arXiv:2001.03015); the matching rule (orient out of the smaller component, maximum ⌈log₂ n⌉) is the warm-up of Bender et al. (2021)."

**F5. LOW (confidence high).** README.md:52 states the tie-break of the greedy rule without "(untouched bins last)", which is part of the definition that was actually tested (proofs.txt:65-66). Add that clause.

**F6. LOW (confidence medium).** README.md:78-79 says the 12-bin strategy "uses the same threat twice". After Ia, the threat is used once in the Ib branch and twice in I′ (proofs.txt:47-55).
Replacement: "uses the same threat once or twice more".

No CRITICAL finding. The headline values 7 (certified) and 12 (upper bound replayed, lower bound computed only) stand as the status cells describe them.
