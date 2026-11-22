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
| R(d) | floor(3d/2) + 1 | proved by hand (two proofs); computed for d <= 12 |
| H(d), odd d | d | upper bound: the safety invariant is machine-checked in Lean, and that a play exists for every adversary is argued on paper only; lower bound proved by hand |
| H(d), even d | d + 1 | proved by hand (upper bound from the clock) |
| C(d), even d | d + 1 | proved by hand |
| C(d), odd d | d + 1 for d <= 13; in general d or d + 1 | upper bound proved by hand; lower bound by hand for d = 1, by exhaustive search (two solvers) for d = 3, 5, by checked certificates for d = 7, 9, 11, 13 (one checker only at 11 and 13); **open** beyond |

Headline: 0 <= C(d) - H(d) <= 1 for every d. This follows from H <= C (a clock
is a history controller), C(d) <= d + 1 and H(d) >= 2*floor(d/2) + 1, all proved
by hand; none of the three is machine-checked. The gap is 0 for even d and 1 for
odd d <= 13.

The relay map u = -sgn(x_{t-d}) is the unique optimal one: every other map is
shown to be unbounded or to reach floor(3d/2) + 2. That the two maps with
f(0) != 0 stay within floor(3d/2) + 2 is computed for d <= 12, not proved.

The optimal controllers are simple:

- Relay: u = -sgn(x_{t-d}).
- Clock: the relay on even ticks, nothing on odd ticks.
- History (odd d): u = -clamp(K * o + S, -1, +1), with K = (d+1)/2, o the
  observed sign and S the sum of the controller's own last d moves.

## Where things are

- `briefs.txt`: every brief given to an instance, in order. The questions that
  the chain produced are quoted inside the next brief.
- `prior-art.md`: the literature search report, with the queries it ran.
- `solver-a/`, `solver-b/`: the two independent solvers. `solver-a/proofs.txt`
  has Theorems 1 to 3 (relay value, clock upper bound, history lower bound).
- `verifier/`: the referee's re-implementation and its attack on Theorem 3.
- `open-h/`: the history controller for odd d and its proof (`proofs.txt`, Theorem A).
- `lean-h/HistoryUpper.lean`: Theorem A in core Lean 4, no Mathlib. Checked with
  `lean HistoryUpper.lean` on Lean 4.36.0 (`lean-toolchain`); it prints
  `'history_upper' depends on axioms: [propext, Classical.choice, Quot.sound]`.
- `open-c/`: the clock lower bound for d = 7, 9, 11, 13 by a nogood-learning
  solver with independently checked certificates, and an unfinished general proof.

## What to distrust

- All authors and checkers are one model family. Only `HistoryUpper.lean` has
  been checked by something else. That includes the nogood solver, its two
  certificate checkers and the hand argument that the certificates are sound
  (`open-c/proofs.txt` section 1.2).
- The headline rests on hand proofs only.
- The Lean theorem quantifies over trajectories that obey the rules; that such a
  trajectory exists for every adversary is not formalised.
- Errors found in `solver-a/proofs.txt` are listed in an errata block at the top
  of that file and the text below it is left as the solver wrote it. The main
  one: Theorem 2, case II, uses an inequality that is false for odd d and odd l.
  The conclusion holds, and `solver-b/proofs.txt` P2 proves the same statement
  another way. The "NOT PROVED" list at the end of that file predates `open-h/`
  and `open-c/`.
- Solver B saw a fragment of solver A's command line, so their clock choices
  were not fully independent.
- For d = 11 and 13 the clock certificates were checked by the C++ checker only;
  the Python check of d = 11 was stopped at 80,000 of 199,469 nogoods.
- The literature search (`prior-art.md`) found no earlier statement of the clock
  or history values, but did not cover paywalled or older Russian work. The
  relay value is probably folklore; the half-duty clock resembles
  "act-and-wait" control.
- Certificates in the repo: `open-c/cert_7_7.txt` (d = 7, exact game, passed
  both checkers) and `open-c/cert_7_7_KR3.txt`. The certificates for d = 9, 11
  and 13 (13 MB to 170 MB) and the history strategy tables above 1.5 MB are
  left out. To regenerate one, build `ngclock3.cpp` and run
  `ngclock3 d d certfile 4000000000 KR` with KR = 1000 for the exact game or 3 for the
  filtered one (see the header of `ngclock2.cpp` and `open-c/results.log`), then
  check it with `ngcheck.cpp`. Compiled binaries are left out.

## Review

Before publication the claims above were put to a panel of five reviewers from
four model families (two Claude, one GPT, one Grok, one Gemini), each reading
the repository. All five confirmed the proofs of Theorems 1 to 3 and A and the
faithfulness of the Lean statement; the corrections they asked for are the
errata and the status wording above.

## Review archive

`review/`: the review of this experiment before publication (16 to 20 November), archived on 21 November verbatim with absolute paths shortened: the brief given to the reviewers, each seat's report, the machine-made summary, and the organiser's note of what was done with the findings. The reports were written about the files as they stood before the findings were applied, so their line numbers and quotations may not match the current files.
