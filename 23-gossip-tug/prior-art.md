# Prior-art report: Delayer/Hastener gossip game

The call rule is exactly the undirected call set of the "Learn New Secrets" (LNS) protocol on the complete graph. The two-player game was not found.

## CLOSEST PRIOR WORK

1. **Epistemic Protocols for Distributed Gossiping**, Apt, Grossi, van der Hoek, 2016, https://arxiv.org/pdf/1606.07516 (downloaded, section read). Defines LNS ("agent i calls agent j if i is not familiar with j's secret"); "in each terminating computation at least 2n − 4 calls are made"; "the longest possible sequence contains at most n(n−1)/2 calls", with an explicit one. Call rule matches; no players, only shortest and longest executions.
2. **Dynamic Gossip**, van Ditmarsch, van Eijck, Pardo, Ramezanian, Schwarzentruber, 2015, https://arxiv.org/pdf/1511.00867 (downloaded, sections read). "For n agents, any sequence between the minimum of 2n − 4 and the maximum of (all possible pairs) n(n − 1)/2 can be realized [2]"; LNS executions of length 4 and 6 for 4 agents. No minimax value; main results concern non-complete networks.
3. **Propositional Gossip Protocols under Fair Schedulers**, Livesey, Wojtczak, 2022, https://www.ijcai.org/proceedings/2022/56 (downloaded, grepped). The scheduler "can be assumed to be demonic", picking calls to "maximize the number of calls made before termination". One-sided adversary; complexity results, no game value.
4. **Greedy Gossiping**, 2025, https://arxiv.org/pdf/2506.17804 (authors not recorded; skimmed). Maximum total knowledge after m < 2n−4 calls. Single-player optimisation.
5. **Reachability and Expectation in Gossiping**, van Ditmarsch, Kokkinis, Stockmarr, 2017, https://link.springer.com/10.1007/978-3-319-69131-2_6 (snippet only). Expected duration of LNS. Random, not adversarial, call choice.
6. **The Expected Duration of Sequential Gossiping**, EUMAS 2017 (authors not shown in my results), https://link.springer.com/doi/10.1007/978-3-030-01713-2_10 (snippet only). Bounds on expected duration. Same difference.

## VERDICTS

- **Headline (A(n) = 2n−3 false; A(4) = 6):** NOT FOUND. A length-6 LNS execution for n = 4 is FOUND (item 2); that Delayer can force it is not.
- **A(2..10) = 1, 3, 6, 9, 12, 16, 19, 23, 27; A(11) ≥ 30:** NOT FOUND.
- **P1, A(n) ≥ 2n−3:** NOT FOUND as stated. Near the known fact that 4-cycle-free call graphs need 2n−3 calls (cited in item 2), but not a corollary of it.
- **P0 and sanity values (no pair twice; shortest 2n−4, longest n(n−1)/2):** FOUND (items 1, 2).
- **Solver-b lemmas:** Lemma 1 (a call adds at most one knower of a secret) LIKELY FOLKLORE; Lemmas 2, 3 NOT FOUND, elementary.
- **V(m,r) table and fits:** NOT FOUND.

**OEIS** (JSON search queried directly):
- A(n), nine terms, and from 3: no hits.
- 6, 9, 12, 16, 19, 23, 27 (n = 4..10): one hit, A053096 (phi iterations on primorials: 1, 2, 4, 6, 9, 12, 16, 19, 23, 27, 31, 35). Unrelated, earlier terms disagree: coincidence in my judgement, though its next term 31 equals solver-b's guess for A(11).
- V rows m=2, m=3: generic Beatty-type matches only; m=4: no hits.
- V column r=3 (8, 10, 13, 15, 17, 20, 22, 24): A047282 (≡ 1, 3, 6 mod 7), consistent with the floor((7m+11)/3) fit. Column r=4: no hits.
- Even-n terms 1, 6, 12, 19, 27: A051936 (n(n+1)/2 − 9), the report's own quadratic.

**Queries (17 listed):** LNS maximum calls; gossip two-player game, adversary chooses caller; dynamic gossip LNS execution length; expected duration sequential gossiping; OEIS A(n); NODUP/NOHO; LNS combinatorial game; demonic scheduler LNS; gossip Maker-Breaker/saturation game; Apt Wojtczak fair scheduler; token/spider/call-me-once length; Livesey Wojtczak; LNS longest sequence; rumor spreading adversary-selects-node; reachability and expectation; adaptive-adversary telephone 2n−3; minimax gossip.

## WHO TO ASK

A specialist in epistemic/dynamic gossip protocols (van Ditmarsch, Apt, Wojtczak), or a combinatorial-games person working on saturation (prolong/shorten) games.

## NOT CHECKED

- Sources read in full: 0. Relevant sections read in items 1, 2; items 3, 4 grepped; items 5, 6, West's NODUP paper and the NOHO literature from snippets only.
- Reference [2] of Dynamic Gossip not obtained.
- Theses and puzzle forums not searched; no hit for the game is weak evidence only.
