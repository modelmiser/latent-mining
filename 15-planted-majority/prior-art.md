# Prior-art search: planted majority against an unfair scheduler

Standard names: population protocol for exact majority, one transient fault, no fairness, "busy beaver" state complexity.

## Closest prior work

1. **Time- and Space-Optimal Silent Self-Stabilizing Exact Majority in Population Protocols**, Kanaya, Eguchi, Sasada, Ooshita, Inoue, 2025, https://arxiv.org/pdf/2503.17652. Shows that no protocol solves self-stabilising exact majority for all n, and that silent ones need at least n states. Differs: every agent starts corrupted and keeps a read-only input, and the scheduler is random; here one agent is corrupted and the bound N <= s fails.
2. **Monadic Presburger Predicates Have Robust Population Protocols**, Czerner, Esparza, Fischer, Guttenberg, Pins, Reilich, CONCUR 2026, https://drops.dagstuhl.de/storage/00lipics/lipics-vol391-concur2026/LIPIcs.CONCUR.2026.28/LIPIcs.CONCUR.2026.28.pdf. Shows that a robust protocol for x >= k needs exactly k states. Differs: adversarial crashes, fair executions, thresholds; its bound excludes x - y >= 0, so not majority.
3. **The Black Ninjas and the Sniper: On Robustness of Population Protocols**, Lossin, Czerner, Esparza, Guttenberg, Prehn, 2024, https://arxiv.org/pdf/2412.11783. Defines crash robustness; robust protocols for threshold and modulo predicates. Differs: crashes, fairness, no state bounds for majority.
4. **Lower Bounds on the State Complexity of Population Protocols**, Czerner, Esparza, Leroux, 2021, https://arxiv.org/abs/2102.11619v3. Defines the busy beaver function for x >= eta and bounds it. Differs: fault-free, fair, thresholds.
5. **Byzantine-Resilient Population Protocols**, Busch, Kowalski, 2021, https://arxiv.org/pdf/2105.07123. Majority with f Byzantine nodes needs margin Omega(f + 1); randomised protocols with O(log^3 n) states. Differs: random scheduler, probabilistic guarantees.
6. **On the Runtime of Chemical Reaction Networks Beyond Idealized Conditions**, Condon, Emek, Harlev, 2023, https://arxiv.org/pdf/2307.00647. Correctness and runtime under a weakly fair adversarial scheduler. Differs: fairness kept, no faults, no state counting.

## Verdicts

- **Headline (no C with G(s) <= C*s; G(4M-1) >= M^2-1, G(8M-5) >= M^3-1, G(4m) >= m^2-1): NOT FOUND.**
- **Closed-set weight-splitting theorem (G(2|S|+1) >= max S - 1) and the budget bound: NOT FOUND.** Cancelling signed weights with a conserved sum are standard in exact majority (e.g. "Fast and Exact Majority in Population Protocols", Alistarh, Gelashvili, Vojnovic, 2015, https://researchonline.lse.ac.uk/id/eprint/85706, abstract only seen); I found no closure condition or plant argument.
- **G(s) finite for every s: NOT FOUND.** Item 1's Theorem 1 is the nearest analogue; its proof (restrict to a sub-population) does not transfer.
- **G(2) = 1, G(3) = 3, G(4) = 3: NOT FOUND.** Nearest: four states are necessary and sufficient for fault-free fair exact majority (Mertzios, Nikoletseas, Raptopoulos, Spirakis, https://arxiv.org/pdf/1404.7671, snippet only).
- **Reduction lemma and monotonicity: NOT FOUND**; routine.
- **Sequence K(m) = 1, 2, 4, 6, 10, 14, 18, 24, 32, 42, 50, 62, 75:** looked up through the OEIS JSON search. The full sequence and three sub-runs returned no entry; a control query (Fibonacci) returned A000045, so the lookup works. A web search pointed at A059254, said to diverge at the eighth term (22 against 24); not opened. Nearest named notion: "stamp chain" (addition chain plus 2-basis), Kohonen and Corander, https://arxiv.org/pdf/1310.7090 (snippet only). No other sequence in the reports has four exact terms.

Queries (22), by topic: Byzantine exact majority under adversarial scheduler; Czerner-Esparza-Leroux bounds; busy beaver protocols; OEIS sequence; silent / always-terminating / no fairness; self- and loosely stabilising majority; fair versus unfair scheduler; succinct protocols; "names trump malice"; transient and crash failures; one arbitrary-state agent; layered termination; addition chains closed under differences; CRN worst-case scheduler; four-state majority bound; exact majority weights; unfair daemon; robust approximate majority; weak fairness; strongly silent WS3; difference-basis sets; single Byzantine agent, deterministic guarantee.

## Who to ask

A population-protocol state-complexity specialist (the Esparza, Czerner, Leroux, Blondin line) or a self-stabilisation researcher (Ooshita, Inoue; Doty, Severson). For K(m), an additive combinatorialist working on addition chains and postage-stamp bases.

## Not checked

Read in full: one source (item 1). Item 2: sections 1, 3, 5. Item 3: introduction. Items 4 to 6: abstract and keyword scan. Everything else: snippets. Not checked: paywalled proceedings, theses, Petri-net termination literature, the solvers' proofs.
