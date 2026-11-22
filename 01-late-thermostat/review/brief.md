<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining, experiment 1 ("the late thermostat")

This repository is about to be made public. It states theorems about a small
control game. Every author and every checker so far was an instance of one model
family, so assume the authors are competent and wrong until you have traced each
argument yourself. Read the files in the repo; do not reason from this brief or
from the READMEs alone. No verdict without evidence.

## The game (01-late-thermostat/README.md)

Fix a delay d >= 1. Integer level x_0 = 0 (and x_s = 0 for s <= 0). Each tick t:
the controller is shown o_t = sgn(x_{t-d}); it adds u_t in {-1,0,+1}; an adversary
who sees everything including u_t adds w_t in {-1,0,+1}, with w_t != 0 implying
w_{t+1} = 0; x_{t+1} = x_t + u_t + w_t. RELAY: u_t = f(o_t) for one fixed map.
CLOCK: u_t = f_t(o_t), the sequence f_t fixed in advance. HISTORY: u_t any
deterministic function of o_0..o_t. R(d), C(d), H(d) = least B such that some
controller of the class keeps |x_t| <= B for ever.

## Claims

- T1. R(d) = floor(3d/2)+1 for all d. Proof: 01-late-thermostat/solver-a/proofs.txt, Theorem 1.
- T2. C(d) <= d+1 for all d. Proofs: solver-a/proofs.txt Theorem 2 (the README says
  its case II contains a false intermediate inequality for odd d) and
  solver-b/proofs.txt P2.
- T3. H(d) >= 2*floor(d/2)+1 for all d. Proof: solver-a/proofs.txt Theorem 3.
- TA. H(d) <= d for all odd d, by the controller u_t = -clamp(K*o_t + S_t, -1, +1),
  K = (d+1)/2, S_t = u_{t-d}+...+u_{t-1}. Proof: open-h/proofs.txt Theorem A.
  Formalisation: lean-h/HistoryUpper.lean.
- Headline: 0 <= C(d) - H(d) <= 1 for every d.

## Questions

Q1. Is Theorem 3's proof (solver-a/proofs.txt) correct for ODD d? Trace Steps 1-4.
    In particular: is the liminf argument sound, is every switched play legal under
    the rest rule, and does fact (K) give what Step 2 uses?
Q2. Is Theorem 3's proof correct for EVEN d? Trace Steps 1-6, especially the phase
    shift (Step 5) and the count of odd ticks in Step 6.
Q3. Is T2 (C(d) <= d+1) established by at least one complete proof in the repo?
    Check solver-b/proofs.txt P2 case by case, and say whether solver-a Theorem 2
    case II can be repaired as the README implies.
Q4. Is Theorem 1 (R(d) = floor(3d/2)+1, and uniqueness of the optimal map) correct,
    including part (d), that every map with f(+) >= 0 or f(-) <= 0 is unbounded?
Q5. Is Theorem A's hand proof (open-h/proofs.txt) correct, including the padding
    for t < d and both uses of "d is odd"?
Q6. Does lean-h/HistoryUpper.lean state Theorem A faithfully? Check the index
    shift (real tick t = index t + d + 1), the padding hypotheses x0 and u0, the
    controller hypothesis ctrl, and whether the hypotheses of `Play` could be
    unsatisfiable (which would make the theorem vacuous). Is the README's sentence
    about what the Lean theorem does and does not cover accurate?
Q7. Does the headline 0 <= C(d) - H(d) <= 1 for every d follow from T2 and T3 as
    stated, and is "the gap is 0 for even d" justified?
Q8. Do the status cells in the results table of 01-late-thermostat/README.md
    claim more than the files support anywhere? Consider each row, and the top
    README's table. Give replacement wording for any cell you would change.
Q9. Is anything in the "What to distrust" section of 01-late-thermostat/README.md
    wrong, or is a material caveat missing?
