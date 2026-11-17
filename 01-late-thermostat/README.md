# Experiment 1: the late thermostat

## The game

Fix a delay d >= 1. An integer level starts at x = 0. Each tick t:

1. the controller is shown sgn(x_{t-d}), the sign of the level d ticks ago (0 for t < d);
2. it adds u in {-1, 0, +1};
3. an adversary, who sees everything including u, adds w in {-1, 0, +1}, but
   must play 0 on the tick after any non-zero push;
4. x_{t+1} = x_t + u + w.

The controller wants |x| <= B for ever. Three classes of controller, and the
least B each can guarantee:

- **Relay** R(d): one fixed map from the observed sign to u.
- **Clock** C(d): a map that may change with time but remembers nothing observed.
- **History** H(d): any function of everything observed so far.

## Results

| Quantity | Value | Status |
|---|---|---|
| R(d) | floor(3d/2) + 1 | proved by hand (two independent proofs); computed for d <= 12 |
| H(d), odd d | d | upper bound machine-checked in Lean; lower bound proved by hand |
| H(d), even d | d + 1 | proved by hand (upper bound from the clock) |
| C(d), even d | d + 1 | proved by hand |
| C(d), odd d | d + 1 for d <= 13; in general d or d + 1 | upper bound proved by hand; lower bound by certificate-checked computation for d <= 13, **open** beyond |

Headline: 0 <= C(d) - H(d) <= 1 for every d. The gap is 0 for even d and 1 for
odd d <= 13.

The optimal controllers are simple:

- Relay: u = -sgn(x_{t-d}).
- Clock: the relay on even ticks, nothing on odd ticks.
- History (odd d): u = -clamp(K * o + S, -1, +1), with K = (d+1)/2, o the
  observed sign and S the sum of the controller's own last d moves.

## Where things are

- `briefs.txt`: every brief given to an instance, in order. The questions that
  the chain produced are quoted inside the next brief.
- `solver-a/`, `solver-b/`: the two independent solvers. `solver-a/proofs.txt`
  has Theorems 1 to 3 (relay value, clock upper bound, history lower bound).
- `verifier/`: the referee's re-implementation and its attack on Theorem 3.
- `open-h/`: the history controller for odd d and its proof (`proofs.txt`, Theorem A).
- `lean-h/HistoryUpper.lean`: Theorem A in core Lean 4, no Mathlib.
- `open-c/`: the clock lower bound for d = 7, 9, 11, 13 by a nogood-learning
  solver with independently checked certificates, and an unfinished general proof.

## What to distrust

- All authors and checkers are one model family. Only `HistoryUpper.lean` has
  been checked by something else.
- The Lean theorem quantifies over trajectories that obey the rules; that such a
  trajectory exists for every adversary is not formalised.
- `solver-a/proofs.txt` Theorem 2, case II, contains a false intermediate
  inequality for odd d (the conclusion holds; `solver-b/proofs.txt` P2 proves
  the same statement another way).
- Solver B saw a fragment of solver A's command line, so their clock choices
  were not fully independent.
- For d = 11 and 13 the clock certificates were checked by one checker, not two.
- The literature search found no earlier statement of the clock or history
  values, but did not cover paywalled or older Russian work. The relay value is
  probably folklore; the half-duty clock resembles "act-and-wait" control.
- Certificates and strategy tables above 1.5 MB are left out; the solvers
  regenerate them. Compiled binaries are left out.
