## Prior-art search: delayed sign-feedback band game

**Bottom line:** I did not find this game or any of the three formulas in print. The RELAY value looks like a discretisation of a textbook delayed-relay calculation; the HISTORY value and the headline "memory is worth at most 1" were not found anywhere.

### Closest prior work

1. **Simpson, "Twenty Hopf-like bifurcations in piecewise-smooth dynamical systems" (2019)** — https://arxiv.org/pdf/1905.01329. Theorem 10.2 (I read the full text): a delayed relay's limit-cycle amplitude is asymptotically proportional to the delay µ. Differs: continuous-time, no adversary, no exact constant.
2. **Shustin, Fridman, Fridman, Castaños, "Robust semiglobal stabilization of the second order system by relay feedback with an uncertain variable time delay" (2008)** — https://hal-centralesupelec.archives-ouvertes.fr/hal-00218262. Shrinks oscillation magnitude using only the delayed sign, via adaptive relay gain. Differs: continuous second-order plant, gain adaptation rather than a fixed ±1 alphabet, no minimax band value.
3. **Stépán and Insperger, "Stability of time-periodic and delayed systems — a route to act-and-wait control" (2006)** — https://mm.bme.hu/~insperger/j2006_AnnRevCont.pdf. Feedback is switched on and off periodically to tame delay; this is the nearest relative of the "relay active every second tick" idea. Differs: linear feedback and pole placement, no sign-only sensing, no adversary or amplitude bound.
4. **Fränzle, Kröger, Winter, Zimmermann, "On the Existence of Reactive Strategies Resilient to Delay" (2023)**, building on Chen et al., ATVA 2018 — https://ar5iv.labs.arxiv.org/html/2305.19985. Safety games where the controller sees the play δ steps late are decidable. Differs: generic finite-graph algorithmics; no integer-line instance, no closed form, no clock-versus-history comparison.
5. **Schreuer, "Stochastic pursuit-evasion games with information lag. I. Perfect observation" (J. Appl. Prob., 1975)** — https://www.cambridge.org/core/journals/journal-of-applied-probability/article/abs/stochastic-pursuitevasion-games-with-information-lag-i-perfect-observation/0AF4B548F840C9838D676E83B516E8FB. Discrete-time scalar pursuit with a lagged pursuer. Differs: full (not sign) observation, mean-square payoff, linear strategies.
6. **Kjellqvist, "Minimax dual control with finite-dimensional information state" (2023)** — https://arxiv.org/pdf/2312.05156. Minimax control of a discrete integrator with partial observation. Differs: the controller sees magnitude and not sign, there is no delay, and the cost is ℓ2-gain.

### Verdicts

- **RELAY, floor(3d/2)+1: LIKELY FOLKLORE.** In continuous time the relay keeps pushing the wrong way for d after the true sign flips, at speed 1 plus the adversary's average 1/2, giving about 3d/2. That derivation is mine, and Simpson's theorem only supports "proportional to delay". The exact floor and +1 were not found.
- **CLOCK, at most d+1 by half-duty relay: NOT FOUND.** Act-and-wait is the same idea in spirit, but nothing there gives a 3d/2 to d improvement for sign feedback.
- **HISTORY, 2·floor(d/2)+1: NOT FOUND.** The delayed-safety-game work would let you compute it per d, but gives no formula.
- **Headline (clock within 1 of history): NOT FOUND.** No result on memoryless-with-clock versus full history under delayed 1-bit feedback turned up.

### Whom to ask

- A relay / sliding-mode-with-delay specialist (the Fridman–Shustin school) for the RELAY and CLOCK values.
- Someone in reactive synthesis under delay (the Fränzle/Zimmermann group) for the HISTORY value and the memory question.

### What I could not check

- No paywalled full texts (IEEE, SIAM, Automatica); Shustin et al., Schreuer, Kjellqvist and Delchamps were judged from abstracts or snippets.
- The act-and-wait PDF body did not extract, and the Fränzle paper was read only for its first 100k characters.
- The claim that δ bits of memory suffice in delayed safety games came secondhand from a search snippet.
- I did not search the Russian differential-games literature (Petrosyan, Chernousko) beyond one query that returned nothing relevant, nor the pre-1990 relay literature (Tsypkin).
- I did not verify any of the three formulas myself.

### Queries run (about 34)

- Relay and delay: relay time-delay limit-cycle amplitude with bounded disturbance; Fridman–Shustin delayed relay (three variants); discrete `x(k+1)=x(k)-sgn(x(k-d))`; `x'=-sgn(x(t-τ))` amplitude and period; duty-cycle / dither relay with dead time; intermittent or open-loop switching schedule for delayed sign; act-and-wait (two variants); discrete sliding-mode quasi-sliding band with delay; sigma-delta / bang-bang PLL loop delay.
- Minimax and quantised control: minimax delayed sign-only integer game; Delchamps quantised feedback; Wong–Brockett containability; Nair–Evans / Tatikonda–Mitter data rate with delay; Bertsekas–Rhodes delayed measurements; Bernhard partial-information minimax; binary-sensor delayed ultimate bound; rate-limited adversary invariant set; Competitive Control with Delayed Imperfect Information.
- Games: pursuit-evasion delayed information on the integer line; information-lag pursuit (Elion / Ferguson); Petrosyan / Chernousko information delay; cops and robbers or lion and man with delay (two variants); balancing game with delayed feedback; safety games under delay; value of memory versus time-varying memoryless.
- Stale information: Mitzenmacher old information; Pender delayed-information queues; delayed binary congestion feedback; delta modulation / one-bit tracking with delay.
