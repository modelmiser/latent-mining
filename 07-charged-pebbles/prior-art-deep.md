# Deep prior-art check: charged pebbles (erasure-budget line game)

**Headline: the upper bound (a) is a short corollary of a preprint posted one month ago** (Thumann and Soloveichik, 17 Oct 2026). That paper does not state the closed form. Bound (b) and the per-pebble version were not found.

## Sources read (PDF text extracted locally)

1. Li, Vitányi 1996, "Reversibility and adiabatic computation", https://arxiv.org/abs/quant-ph/9703022. Read §1.1 and all of §2 (Lemmas 1–3, Theorem 1, Corollary 1); not §3 onward (Kolmogorov cost, no pebbles).
2. Li, Tromp, Vitányi 1998, "Reversible simulation ... by pebble games", https://arxiv.org/abs/quant-ph/9703009. Read §5–§9 in full.
3. Vitányi 2005, "Time, space, and energy in reversible computing", https://arxiv.org/abs/cs/0504088. Read §5.3–§5.6 in full.
4. Buhrman, Tromp, Vitányi 2001, https://arxiv.org/abs/quant-ph/0101133. Read §3.2 and every theorem statement. No erasure game (one sentence that an illegal deletion "is erased irreversibly").
5. Královič 2004, https://www.numdam.org/item/ITA_2004__38_2_137_0.pdf. Read every theorem/lemma statement and searched the text. No erasures anywhere.
6. Knill 1995, https://arxiv.org/abs/math/9508218. Read the game definition and Theorems 2.1–2.3. No erasures. Theorem 2.3: clean finish at n is possible iff n ≤ 2^(S−1), our f = 0 case.
7. **Thumann, Soloveichik 2026, "Energy-optimal scheduling for clocked reversible computing", https://arxiv.org/abs/2610.24435. Read in full, appendices included.** Found by forward citation.
8. Kornerup, Sadun, Soloveichik, https://arxiv.org/abs/2110.08973, and Quist, Laarman, https://arxiv.org/abs/2401.10579: definitions and theorem statements. Ghosts must be re-pebbled and are not budgeted; no ghost-count versus reach theorem.
9. Gidney 2019, https://algassert.com/post/1905 (machine summary only): no bound on ghost count.
10. Stumm, Walther, "New algorithms for optimal online checkpointing" (restates Griewank–Walther), https://tu-dresden.de/mn/math/wir/ressourcen/dateien/forschung/publikationen/pdf2010/new_algorithms_for_optimal_online_checkpointing.pdf: Theorem 2.1 only.
11. Text-searched only, no erasure-budget content: https://arxiv.org/abs/1604.05510 (Komarath et al.), https://arxiv.org/abs/1605.08448 (Demaine et al.), https://arxiv.org/abs/2001.02481, https://arxiv.org/abs/2609.15062, https://arxiv.org/abs/2510.08403, https://arxiv.org/abs/2610.09434.

Forward citations: Semantic Scholar lists for items 1–3 (73, 23, 45 citing papers), titles scanned, arXiv-hosted pebbling ones opened.

**Not opened:** https://www.preprints.org/manuscript/202509.0116 (HTTP 403 again); Chan's thesis (escholarship returned HTML); Frank's 1999 thesis; Perumalla's book; Kornerup's thesis; Blocki et al. on parallel reversible pebbling; the original Griewank–Walther paper.

## Strongest prior statements, in (q, f)

- Li–Vitányi Lemma 3: "There is a winning strategy with n pebbles and 2m − 1 erasures for pebble games G with T_G = m2^(n−1)". Winning means pebble T_G then empty the board. q = n, f = 2m − 1: reach (f+1)·2^(q−2). Strategy only.
- Li–Tromp–Vitányi Lemma 1 (= Vitányi 2005 Lemma 2): "a winning strategy with n + 2 pebbles and m − 1 erasures for pebble games G with T_G = m2^n". q = n + 2, f = m − 1: (f+1)·2^(q−2). Strategy only. Ours: (f+2)·2^(q−2).
- The only impossibility results in items 1–6 are erasure-free (maximum node 2^n − 1). Their "upper bounds on space versus irreversible erasure trade-offs" means achievability.
- Li–Tromp–Vitányi's "available" pebble ("another pebble at most 2^f positions to its left", f = free pebbles) is the erasure-free form of the anchor reduction.
- **Thumann–Soloveichik.** Same game: S pebbles excluding node 0, clean finish ("ends with only node n pebbled"), erasure "removes any pebble on node i ≠ n ... regardless of whether its predecessor is present". Theorem 1: "C_λ(n,S) = min_{1≤m<n} [C_λ(m,S) + C_λ(n−m,S−1) + min{F(m,S−1), λ}]", where C_λ is the minimum of (reversible moves + λ·erasures) and F is Knill's reversible cost. On erasure count they say only: "Let I_min(n,S) be the fewest erasures of any winning schedule, so that I_min = 0 up to the wall and I_min ≥ 1 beyond it." For S = 2 they give C_λ(n,2) = n + 1 + (n−2)λ (λ ≥ 1), i.e. I_min(n,2) = n − 2, which is our q = 2 value. Their Lemma 1 as stated gives only n ≤ (I+1)·2^(S−1), weaker than ours.

## Verdicts

- **(a) upper bound: COROLLARY OF Thumann–Soloveichik Theorem 1 plus Knill Theorem 2.3; closed form not stated there.** Derivation (mine, by hand, unchecked by anyone else): letting λ → ∞ in Theorem 1 and using F(m,S−1) < ∞ iff m ≤ B := 2^(S−2) gives I_min(n,S) = min_m [I_min(m,S) + I_min(n−m,S−1) + 1{m > B}]. Induction on S and n, two cases (m ≤ B, m > B), gives I_min(n,S) ≥ ⌈n/B⌉ − 2, i.e. n ≤ (f+2)·2^(q−2). Our game reduces to theirs by deleting all moves above n and all but the last placement of n; their erasure rule is more permissive, so their lower bound transfers. This rests on their Lemma 4, a month-old unrefereed proof by a different method (Knill-style separator projection, not anchors).
- **(a) strategy: COROLLARY OF the same paper, §V.** "A strict period of length B holds x fixed, uses the remaining S − 1 pebbles to compute node x + B ... and then erases x", B ≤ 2^(S−2), started from the clean finish at the wall 2^(S−1): (f+2)·2^(q−2). The count is not written out. The 1998 strategy is one block shorter.
- **(b) visiting bound (f+4)·2^(q−2) − 1: NOT FOUND AFTER READING** items 1–8. Only f = 0 is known (Li–Tromp–Vitányi Corollary 2, 2^n − 1). The Thumann–Soloveichik board stops at n.
- **Per-pebble version: NOT FOUND.** Its upper bound follows from the pooled bound; the rotation strategy and the unequal-charge observation appear nowhere read.
- **Checkpointing: not equivalent.** Griewank–Walther (as restated in item 10): "t(c,l) = rl − β(c+1, r−1)", β(c,r) = C(c+r, c). Discarding a checkpoint is free and unlimited there, the cost is recomputation, and the task is a full reverse sweep; the extremal numbers are binomial, not (f+2)·2^(q−2).

## Is (a) already known?

That it follows from published work: yes, as above. That the closed form is already known to specialists: about 75%, since Soloveichik's group (UT Austin) holds the exact recurrence and plots I_min. That it is stated in print: not found; perhaps 10–15% given what was not opened. Others who would know: Tromp, Vitányi, Knill, Kornerup.

## Venue

If anything is written up, a short arXiv note (cs.CC, cross-listed quant-ph) presenting the closed form of I_min, the visiting bound (b) and the anchor invariant as a second proof, citing Thumann–Soloveichik prominently and ideally sent to them first.
