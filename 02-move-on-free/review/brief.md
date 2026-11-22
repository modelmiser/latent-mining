<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Review brief: latent-mining, experiment 2 (move on free)

Repository: `latent-mining`. Read `02-move-on-free/README.md` first (it defines the game: games N, F, O;
h^G(M); mu^G), then the proof files it cites. Do not reason from the README alone: trace each proof in
the cited file. Assume the author is competent and wrong until you have traced the argument yourself.
Everything was written by instances of one model family; you are the outside check.

The README claims the following are PROVED BY HAND. For each, say CONFIRMED / REFUTED / UNSUPPORTED,
cite file:line, and for anything not CONFIRMED give the concrete counterexample or the missing step
and the replacement wording you would use.

Q1. `02-move-on-free/solver-a/proofs.txt` P1: h^O(M) >= 3*L(M) - 2 >= (5M-14)/3, for every manager.
Q2. `02-move-on-free/solver-a/proofs.txt` P2: first fit in game O never uses a cell >= floor((5M+2)/3).
    Also `02-move-on-free/solver-b/proofs.txt` P5 (same statement, different proof). Check both.
Q3. `02-move-on-free/solver-b/proofs.txt` P6: h^N(M) >= 7M/4 - 17/4 for every manager in game N.
    Is any assumption about where the manager places blocks used silently?
Q4. `02-move-on-free/solver-a/proofs.txt` P3: h^N(M) <= 43M/18 + 12 (hexad manager). Check the three
    inequalities, the claim that the maximum is at the all-tight vertex, and the final step from the
    number of non-empty hexads to H.
Q5. `02-move-on-free/open-f-lower/proofs.txt` L1 and L2. L1: a layout with waste W forces
    H >= M - 2 + W in all three games. L2: in game F, a manager's reply to a freed 2 is at best a
    deletion of one 1. Is "the adversary frees the moved 1 at once" always legal and does it restore
    exactly the stated layout, including when the moved 1 lands in the hole the freed 2 left?
Q6. `02-move-on-free/open-f-lower/proofs.txt` Theorem A: in game F, any manager whose first M 1s are
    on consecutive cells needs H >= 7M/4 - 7. Trace the counting of "counted runs", "interior" and
    "clean" 2s, the bound |F| >= (y-3-2k)/2, W_chain >= k + 2|F|, the pin runs, and the arithmetic.
    Check degenerate cases: no lower chain, no upper chain, y < 3, small M. Is the bound claimed
    "for every M >= 3" actually true at small M?
Q7. `02-move-on-free/open-f-upper/proofs.txt` Theorems R and R'. Taking Step 5's computed inequality
    (v <= M/3 + 7r/4 + 27/4) as given, are the remaining hand steps sound? Separately: the README
    says R and R' are implied by Theorem A. Is that correct as a statement about cells needed?
Q8. The README section "What the table cannot tell you" makes three numeric claims (the N/O gap
    cannot show before M about 59; Theorem A's bound passes 5M/3 only for M > 84; floor(7M/4) - 1
    fits all exact values for M <= 12 and is wrong for game O at M = 24 and every M > 32).
    Check each against the bounds in the proof files and the table.
Q9. Status wording. Does any row of the Results table in `02-move-on-free/README.md`, or the row
    for experiment 2 in the top-level `README.md`, claim more than the files support? In
    particular: "three independent programs agree for M <= 11, two for M = 12, 13"; "exhaustive
    check of first fit for M <= 16"; "played out for M = 3 ... 60 against five such managers";
    and the top-level phrase "the right is worth nothing to an allocator that packs its first
    blocks together". Is mu^F stated as open everywhere it should be?
Q10. Anything else in this experiment's files that is stated as proved and is not, or any
    contradiction between `open-f-upper/proofs.txt` and `open-f-lower/proofs.txt` (they were written
    independently; note they use different adversaries).
