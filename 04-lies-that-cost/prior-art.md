# Prior-art report

## CLOSEST PRIOR WORK

1. **Searching with Local Constraints on Error Patterns** — Czyzowicz, Lakshmanan, Pelc, 1994, Eur. J. Comb. 15(3):217–222. https://dblp.uni-trier.de/rec/journals/ejc/CzyzowiczLP94.html. Adaptive game with at most one lie in every r consecutive answers (fixed r ≥ 3); a strategy whose length is asymptotically optimal (ratio to lower bound → 1), as summarised in Pelc's survey §5.1.3. Differs: the window is a constant, not k·ell_s, and no exact values are given. Paper itself not read.
2. **Solution of Ulam's problem on searching with a lie** — Pelc, 1987, JCTA 44(1):129–140. https://dblp.uni-trier.de/rec/journals/jct/Pelc87.html. Exact one-lie value: least q with n ≤ 2^q/(q+1) (even n) or n ≤ (2^q−q+1)/(q+1) (odd n), as restated by OEIS A286496 and Osthus–Watkinson. This is U1 only; no honesty-window rule.
3. **Renyi-Ulam Games and Forbidden Substrings** — Beluhov, 2016. https://arxiv.org/abs/1609.07367. Winnability criteria for restrictions on the truth/lie string; restates the one-forbidden-substring classification (Seeker wins iff the string is 0, 1, 01 or 10) and classifies pairs. Differs: restrictions depend on lie positions only, never on the question; winnability, not question counts.
4. **Searching games with errors — fifty years of coping with liars** — Pelc, 2002, TCS 270:71–109. https://www.gwern.net/doc/statistics/order/comparison/2002-pelc.pdf. Survey; the pages read (§5.1.2–§7.3) list segment-bounded, prefix-bounded, forbidden-pattern and half-lie models. None makes the restriction depend on the split.
5. **The Interval Liar Game** — Doerr, Lengler, Steurer, 2006, ISAAC (LNCS 4288). https://www.dsteurer.org/paper/liar.pdf. All lies inside one block of k consecutive rounds; about log n + log log n + k questions. Differs: lies clustered rather than separated; costs k more than one lie. Snippet only.
6. **Searching with lies under error cost constraints** — Ahlswede, Cicalese, Deppe. https://www.math.uni-bielefeld.de/ahlswede/homepage/public/221.pdf. q-ary game where each wrong answer has a cost from a channel matrix and total cost is capped; asymptotic bounds. Differs: cost depends on the (true, given) answer pair, not on split size; a budget, not a forced honesty period.

## VERDICTS

- **U1(n) = P(n)**: FOUND (Pelc 1987). Threshold sequence 4, 8, 16, 28, 50, 92, 170, 314, 584 is OEIS A286496 (https://oeis.org/A286496); later terms 1092, 2048 agree with the reports' ranges.
- **U1(n) ≥ P(n)**: FOUND, the lower half of Pelc's theorem.
- **Q_k(n) ≥ U1(n), and the weight bound inside the Q_k game**: LIKELY EASY COROLLARY (Berlekamp-style volume bound; one-lie plays are legal). Not found stated for this game.
- **Position = multiset of obligations**: LIKELY FOLKLORE; same reduction as Beluhov's positions on a restriction graph.
- **Q_1(2) = ∞, Q_2(2) = 3**: LIKELY EASY COROLLARY. With two candidates every splitting question has ell = 1, so the rule is "no two consecutive lies" (forbidden 11, an Obscurer win in the one-substring classification) or "one lie per 3 answers". I did not verify that the classification covers n = 2 from the start position.
- **Q_1(n) = ∞ for n ∈ {3,4,5,6,7,9}, finite for n = 8 and 10..317**: NOT FOUND.
- **Headline Q_2(n) = U1(n) (computed to 2805), same for k = 3,4,5,8, conjectured E_k**: NOT FOUND. No source has a split-dependent honesty window.
- **Sequences**: Q_2 / U1 values 3,5,5,6,6,6,6,7,… gave no OEIS match as a run; it is the inverse of A286496.

Queries (17): Ulam game no two consecutive lies / after a lie must tell truth; Pelc one-lie n(q+1) ≤ 2^q; one lie per sliding window; OEIS 4,8,16,28,50,92,170,314,584 (web and oeis.org); OEIS 3,5,5,6,6,6,6,7,…; lie cost depends on question size; Pelc survey consecutive-lie patterns; IMO 2012 liar game generalisation; forbidden lie pattern Czyzowicz Lakshmanan Pelc; window restriction question count; local constraints on error patterns; lies depend on query / balanced split; burst lies; feedback codes isolated errors; cooldown / recovery period after a lie; one lie per block exact count; Ravikumar–Lakshmanan known lie patterns; interval liar game.

## WHO TO ASK

A combinatorial search / liar-games specialist (Rényi–Ulam games, error-correcting codes with noiseless feedback).

## NOT CHECKED

- Read at length: Beluhov (pp. 1–14), Pelc survey (pp. 89–96 only), Ahlswede et al. (pp. 1–3), OEIS A286496. None read cover to cover; everything else is snippets.
- Unread: Pelc 1987, Czyzowicz–Lakshmanan–Pelc 1994 and their forbidden-pattern paper (paywalled), Cicalese's monograph, Deppe's survey, the rest of Pelc's survey.
- The split-dependent rule has no standard name, so NOT FOUND is weak evidence of novelty.
