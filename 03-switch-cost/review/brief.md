<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining, experiment 3 (switch cost)

Repository: `latent-mining`. Read `03-switch-cost/README.md` first (it defines the game: jobs,
machine, order of play, the same-cost and free promises, S_T(P), F_T(P)), then the proof files it
cites. Do not reason from the README alone: trace each proof in the cited file. Assume the author
is competent and wrong until you have traced the argument yourself. Everything was written by
instances of one model family; you are the outside check. The two general lower bounds (Q3, Q4)
are new, were each written by a single instance, and are the claims most likely to be wrong.

For each question say CONFIRMED / REFUTED / UNSUPPORTED, cite file:line, and for anything not
CONFIRMED give the concrete counterexample (a scheduler behaviour, or a release that breaks the
promise) or the missing step, and the replacement wording you would use.

Q1. Upper bound, `03-switch-cost/solver-b/proofs.txt` Theorem 1 (and `solver-a/proofs.txt`
    Theorem 1): non-preemptive earliest-deadline-first keeps every lateness <= P - 1 under the FREE
    promise, for all T. Does the argument handle jobs released while the machine is in the middle
    of a job? Is S_T <= F_T argued correctly?
Q2. `03-switch-cost/solver-b/proofs.txt` Theorems 3 and 3': S_4(2) >= 1 and F_3(2) >= 1 by hand.
    Check both branches (scheduler runs the first job at tick 0; scheduler declines).
Q3. `03-switch-cost/open-f/proofs.txt` Theorem A (staircase): F_T(P) >= P - 1 for all P >= 2,
    T >= P(P+1)/2. Trace (1) the facts about s_j including the stall case, (2) legality and the
    hindsight schedule for the free promise at EVERY release, (3) the saturation lemma, (4) the
    lock lemma, (5) the downward induction. Try to break it with a concrete scheduler for P = 3:
    one that refuses to start a job, one that leaves a job after one unit, one that idles.
Q4. `03-switch-cost/open-s/proofs.txt`: S_T(P) >= P - 1 for all P >= 2, T >= (P-1)(4P-1), under
    the SAME-COST promise. Trace the rule ADV(P), the Round Lemma case by case, and section 3 (every
    release keeps the same-cost promise: the witness plans must never switch away from an
    unfinished job and must meet every deadline, including stream units, tight units, decoys and
    filler). Specific things to attack: (a) at the start of a round the scheduler may be locked on,
    or hold a partly run copy of, an EARLIER decoy; does any case of the Round Lemma silently assume
    all its other jobs are unit jobs? (b) in mode A/R the adversary releases a stream unit "if PLAN0
    has no inner job" before seeing the scheduler's action in that tick; is the rule well defined as
    an online rule? (c) the bound B <= (rho+1)(3P-1) <= Delta; (d) the cash-in: is f >= 0, are the
    filler jobs legal, and does Lemma 0 give the stated lateness; (e) some cases conclude lateness
    >= P, which exceeds the upper bound P - 1: is that a contradiction or only a statement about a
    scheduler that played badly? (f) P = 2 as a degenerate case.
Q5. Does S(P) = F(P) = P - 1 for every P >= 2 follow from Q1 and Q4 as the README says?
Q6. `03-switch-cost/open-f/proofs.txt` Corollary A' and Theorem B: sound?
Q7. Tables. Does the table in `03-switch-cost/README.md` agree with `solver-a/tables.txt`,
    `solver-b/results.txt` and `reports/referee.md`? Is any entry shown as exact that the files
    support only as a bound? Does Corollary A' agree with every F entry?
Q8. Status wording. Does any row of the Results table, the "Short answer", the section "What this
    does and does not say", or the experiment 3 row of the top-level `README.md` claim more than
    the files support? In particular the phrases "no online rule beats never preempting", "exactly
    P - 1", and the statements about horizons (4, 9, at most 15 against 3, 6, 10; "about 4P^2",
    "about 2P^2 by computation for P <= 5").
Q9. Anything else in this experiment's files stated as proved that is not, or any inconsistency
    between the reports in `03-switch-cost/reports/` and the proof files.
