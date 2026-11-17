# Prior art: charged pebbles, clean delivery

The setting is Bennett's reversible pebble game on a line plus a budget of irreversible erasures. Solver B's "relaxed game" is exactly the "m-erasure pebble game" of Li and Vitányi (1996). The per-pebble charge is the only new ingredient.

## Closest prior work

1. **Reversibility and Adiabatic Computation: Trading Time and Space for Energy** — Li, Vitányi, 1996. https://arxiv.org/abs/quant-ph/9703022
   Shows: defines the m-erasure pebble game; Lemma 3 is a strategy with n pebbles and 2m−1 erasures reaching node m·2^(n−1). Differs: no impossibility bound with erasures, about half the reach per erasure, no clean-delivery goal.
2. **Reversible Simulation of Irreversible Computation by Pebble Games** — Li, Tromp, Vitányi, 1998. https://arxiv.org/abs/quant-ph/9703009
   Shows: complete characterisation of reachable configurations without erasures (maximum node 2^n − 1), plus an erasure strategy. Differs: the exact results are erasure-free.
3. **Time, Space, and Energy in Reversible Computing** — Vitányi, 2005. https://arxiv.org/abs/cs/0504088
   Shows: Lemma 2, "a winning strategy with n+2 pebbles and m−1 erasures for T_G = m·2^n", using two alternating springboards. Differs: with p = n+2, f = m−1 this reaches (f+1)·2^(p−2), against the reports' clean delivery at (f+2)·2^(p−2) and visit to (f+4)·2^(p−2) − 1; no upper bound.
4. **Tight Bounds on the Spooky Pebble Game** — Kornerup, Sadun, Soloveichik, 2021. https://arxiv.org/abs/2110.08973
   Shows: asymptotically tight pebble/time trade-offs on the line when irreversible removals leave ghosts. Differs: ghosts must be cleaned up later, are unlimited in number, and the bounds are asymptotic.
5. **Spooky pebble game blog post (exact title not captured)** — Gidney, 2019. https://algassert.com/post/1905
   Shows: introduces ghosts; three pebbles suffice on any line. Differs: as item 4.
6. **An analysis of Bennett's pebble game** — Knill, 1995. https://arxiv.org/abs/math/9508218
   Shows: a recursion for time-optimal reversible pebbling of a chain; a later paper (https://arxiv.org/pdf/2609.15062) cites it for "finite iff n ≤ 2^(S−1)". Differs: no erasures.

## Verdicts

- **C(p,0) = 2^(p−1) (Lemma W):** FOUND. Immediate from the configuration characterisation in item 2 (restated as Corollary 3 of item 3).
- **Lower bound C(p,k) ≥ (pk+2)·2^(p−2):** the method is the springboard strategy of item 3, Lemma 2, so EASY COROLLARY in technique; the exact constant and clean-delivery form were NOT FOUND (the published strategy reaches less).
- **C(2,k) = 2k+2:** NOT FOUND as a statement; elementary.
- **Upper bound for p ≥ 3 (headline equality, conjectures R and J):** NOT FOUND. Items 1 and 3 state no impossibility result for the game with erasures; their optimality claims concern the erasure-free game.
- **Per-pebble charge and the unequal-charge side result:** NOT FOUND.

Queries (13): reversible pebble game limited erasures trade-off; spooky pebble game tight bounds line; reversible pebbling path clean final configuration; "n+2 pebbles" "m−1 erasures"; OEIS terms with pebbling; hybrid reversible/irreversible pebbling bounded removals; Knill pebble game recurrence; spooky pebbling number of measurements; erasure lower bound given pebbles; "(f+2)"/"(m+1)" persistent pebbling budget; exact characterisation erasure lower bound; pebbles with limited lifetime or charge; Landauer erasure budget checkpointing chain.

## Integer sequences

- 4, 10, 16, 22, … (p=3): OEIS A016957, 6n+4. No pebbling reference.
- 8, 24, 40, 56, … (p=4): OEIS A051062, 16n+8. No pebbling reference.
- 2, 4, 6, … (p=2): the even numbers; not looked up.

All are arithmetic progressions, so OEIS carries little weight.

## Who to ask

A reversible-computing or pebbling-complexity theorist: the authors of items 1–3, or of item 4, whose line lower-bound technique is the nearest tool.

## Not checked

- No source was read in full. I read extracted excerpts of the erasure sections of items 1 and 3, and machine summaries of items 2, 4 and 5; item 6 by abstract and secondary citation only.
- Quist and Laarman's spooky-pebbling paper (https://arxiv.org/abs/2401.10579) and a 2025 preprint on an erasure "pebbling price" (https://www.preprints.org/manuscript/202509.0116, fetch refused) were seen in search snippets only.
- Forward citations of items 1–3 were not traced; a later paper proving the erasure upper bound could exist.
