## Open-F lower side: mu^F > 5/3 is NOT PROVED

I found an adversary (P6R) that provably forces 7M/4 − 7 against one natural class of managers and beat every manager I could build, but I have no proof against every manager.

### Best adversary: P6R
1. Allocate M 1s.
2. Free the residue class with fewest 1s, keeping two classes alive (X ≥ 2M/3 1s).
3. Allocate y = ⌊(M−X)/2⌋ 2s.
4. Free a 2 whenever that raises the thinning potential by ≥ 2; if the manager replies by moving a 1, free that 1 at once.
5. Thin the 1s to a waste-optimal subset (exact 3-state DP).
6. Allocate 3s.

The known "cancels exactly" argument for the 7/4 adversary assumes the adversary commits to one class before freeing 2s. P6R does not commit. A moved 1 can be freed at once at no cost, so a reply is at best a deletion, and with two classes alive a deletion costs the adversary 1, not 2. A freed 2 between two other 2s gains 2.

Variant P6RS picks the kept set from a menu by look-ahead, including "blockers" (1s of the dropped class next to a reserved gap).

### Proof status
- **COMPLETE — L1:** layout with waste W (sum of free-run lengths mod 3 below the top) forces H ≥ M − 2 + W.
- **COMPLETE — L2, L3:** a reply to a freed 2 is at best a deletion; thinning 1s is free.
- **COMPLETE — Theorem A:** if after step 2 no two adjacent free cells lie between the lowest and highest remaining 1 (true for any manager that puts the first M 1s on consecutive cells), P6R forces H ≥ 7M/4 − 7. Played out for M = 3..60 against five such managers: 0 violations in 290 games.
- **COMPUTED only:** against managers that reserve 3-cell gaps among the first 1s, in a periodic model that gives the manager foresight of the freed set:

| First layout | Slope dW/dM (2/3 = held) |
|---|---|
| [6], [6,6] | 0.75 |
| [6,6,6,6] and [6]×8 (the latter not exhaustive) | 0.7083 |
| [9], [4,5] | 0.7222 |
| [5] and [5,5] | 0.70 |
| Scan of 350 layouts (≤ 30 ones per period, span ≤ 1.7M) | minimum 0.70; none at or below 2/3 |

- **NOT PROVED:** any bound above 5M/3 against arbitrary managers. Missing: non-periodic first layouts, arbitrary placements of the 2s, and a hand proof of the gap case.

### What P6R forces in play (extent − ⌈5M/3⌉)

| Manager | M=48 | M=96 | M=144 |
|---|---|---|---|
| First fit + move | +2 | +7 | +12 |
| Potential-greedy | +5 | +13 | +21 |
| Best designed manager (WB) | +2 | +4 | +8 |

WB knows the script, reserves gaps, plans all 2s against the adversary's own end phase, and replies with the best move. Two WB rows at M = 144 were not run. These managers are heuristic, so this shows P6R beats what I built, not everything.

Exhaustive best response against the fixed adversary, M = 3..16: least surviving H = 3,5,7,8,9,12,12,15,16,17,18,20,22,24. This is ≤ h^F wherever known and below ⌈5M/3⌉; it checks the code, not the slope (the proved bound passes 5M/3 only for M > 84).

### Manager behaviour that limits each attack
- **Solver-b's P6 in F:** cancelled by moving a pin of the committed class (≤ 0 in 33 of 34 games; +2 once, WB[4,4,7] at M = 144).
- **P6R:** reduced from slope 3/4 to 0.70, not stopped, by three things together:
  - reserving 3-gaps whose residue class rotates, so blockers eat the adversary's budget for 2s;
  - putting one 2 in every few gaps, all hidden from the same class, which destroys the redundancy so replies are worth 2 again (one such 2 protects about three tail 2s);
  - replying by lifting a 1 of the class about to be used.
- **P4-safe attacks (never grant a move):** by analysis only, not simulated, they lose because the manager hides re-allocated 1s in the holes the 2s left.
- **Global residue-class potential alone:** cannot beat 5/3, since its three values sum to at most 2M (analysis).

### Negative controls
- P1 and a 1s-only adversary against first fit stay ≤ ⌊(5M+2)/3⌋ (asserted, M = 48, 96, 144).
- P6 in F beats first fit without moves (+3, +10, +15) but not managers with moves, so the harness does detect a defeated attack.
- Exhaustive values never exceed h^F for M ≤ 13.
- The periodic solver returns exactly 2/3 when the attack is absent.

### Belief
mu^F > 5/3 is more likely than not; I am not confident.
- **For:** the mechanism is proved against compact managers, and every escape I constructed stalled at about 1.70M.
- **Against:** the manager side is searched only over periodic layouts with ≤ 30 ones. Going from [6,6] to [6,6,6,6] lowered the slope, so longer periods can help the manager, and I cannot exclude a sequence approaching 5/3. No exact value separates F from O.

### Process notes
- I stopped four of my own runs early; each log says so in its last line.
- To stop one batch whose PID I had not recorded, I used `fuser` on my own log file and checked the command lines before killing. Later jobs had PIDs recorded.
- One short Python sanity check ran without the memory cap and timeout.
- Nothing outside the directory was written; `open-f-upper` was not read.

### Files
Everything is in `./open-f-lower/`:
- proofs.txt
- README.txt
- battery2.log
- scan_patterns.log
