<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — `solver-b/proofs.txt:14-24` proves pointwise domination for every fixed legal pair sequence; its triangle inequality covers arbitrary outside/multi-edge flips, and maximizing over such sequences covers adaptive Adversaries — “For every legal pair sequence, a normal strategy costs no more; hence the game values agree.”

Q2: CONFIRMED — the pair is legal because shadow and real forests share G, the discrepancy path always exists, and the invariant transfers the forced cost (`reports/open-limit-referee.md:16`) — “Correct explicit strategy transfer, but unnecessary for value equality because Lemma 1 already dominates every legal pair sequence.”

Q3: CONFIRMED — slots and h satisfy h=P+1 (`open-limit/proofs.txt:11-13`); reachability proves Lemma A(b) (`:31-35`); free/low-slot/high-slot exhaust normal replies and establish all costs/types (`:38-49`); unavailable replies merely remove options — none.

Q4: CONFIRMED — both parity calculations are explicit at `open-limit/proofs.txt:52-62`; singleton P=0 and (*) give the main induction (`:67-70`) — “Inductive from every normal-form or shadow position.”

Q5: CONFIRMED — cleanup legality, telescoping, and all size/F bounds are valid (`open-limit/proofs.txt:72-80`); values are −3.371, 8.114, 67.487 at n=16,64,256, first positive at 34 and first above B at 189 (`reports/open-limit-referee.md:30`) — none.

Q6: CONFIRMED — block/superadditive proofs (`solver-b/proofs.txt:26-44`), binary-tree size and induction (`:46-55`), and P6 (`solver-a/proofs.txt:70-86`) are sound; required W₂ and n=30 certificates exist at `solver-a/cert_W2_14_ge4.txt:1`, `solver-a/cert_W2_16_ge5.txt:1`, `solver-b/cert_n30_k7.txt:1` — none.

Q7: CONFIRMED — keeping either pair means choosing either surviving real witness (`open-limit/proofs.txt:88-97`); (***) and cases exhaust all pair types (`:109-127`); README correctly limits the ceiling to the reduced game (`README.md:47`) — none.

Q8: REFUTED — the numerical claims reconcile with `reports/referee.md:11-32`, `open-limit/pairdp_49_66.txt:14-19`, and `open-limit-referee/run_t3_true.txt:62-67`, but several explanatory statements overreach — “Keep the numbers/statuses; apply the wording fixes below.”

F: MODERATE — `README.md:39` — “nearest” strengthens Lemma 1 to shortest paths, which it never proves — confidence: high — replace with “an unsaturated vertex, with all preceding path vertices saturated.”

F: LOW — `solver-b/proofs.txt:13-14` — wording suggests comparison against the same adaptive strategy, while the proof compares identical pair sequences — confidence: high — state pointwise sequence domination.

F: LOW — `README.md:45,91-103` — calls shadow transfer a missing necessary step; it is correct but redundant for game-value equality — confidence: high — call it an explicit constructive transfer.

F: LOW — `README.md:9` — omits max(0,·), giving negative “optimal” values for n≤3 — confidence: high — write B(n).

F: LOW — `README.md:60` — k blocks force k−1 merge payments, not one per block — confidence: high — write “one per inter-block merge.”

F: LOW — `README.md:110` — not everything beyond the small table relies on Lemma 1; the upper bound, block proof, T(16), and solver-A’s T(30) route do not — confidence: high — restrict this warning to the pair/open-limit route.

F: LOW — `open-limit/proofs.txt:111-113` — “no one-witness strategy” exceeds what the defined pair abstraction proves — confidence: high — say “no strategy represented by this pair game.”

F: LOW — `README.md:131` — Hiken–Wein’s quoted no-lower-bound statement concerns update time, but this context suggests flip lower bounds — confidence: medium — specify “nontrivial update-time lower bound.”

Unasked: The pair potential does not yield a real-game upper bound; `open-limit/proofs.txt:140-148` gives a 9-vertex counterexample, so L≤1/3 remains wholly open.Correction to my prior review:

Q5: REFUTED — cleanup and constants remain sound, but the bound first exceeds B at n=179, also at 183, 186, and 187; n=189 is only the point after which it exceeds B for every n — replace with “first exceeds B at 179, and exceeds it for all n ≥ 189.”

F: LOW — `README.md:52` — “only from n=189” incorrectly suggests first crossing — confidence: high — use the wording above.Additional findings omitted from my original list:

F: MODERATE — `README.md:80-84` — only main-phase equal-index merges preserve the displayed potential; cleanup incurs bounded loss — confidence: high — distinguish main-phase preservation from cleanup loss.

F: LOW — `README.md:51-53` — “certified values” mislabels several n=30…66 reduced-game computations — confidence: high — say “computed and certified lower bounds.”

F: LOW — `README.md:57-60` — joining tails either costs a flip or leaves saturation; it does not unconditionally create saturation for free — confidence: high — state the two alternatives.

F: LOW — `README.md:71-77` — “every reply” needs the normal-form/shadow qualifier — confidence: medium — add that qualifier.

F: LOW — `README.md:86-89` — only solver B’s files document the failed fixed 4-ary schedule — confidence: high — attribute that obstacle only to solver B.

F: LOW — `README.md:123-130` — prior-art wording omits amortized results, describes a total \(3(n-1)\) bound as “three flips per edge,” and says eleven papers although 17 were text-scanned (`prior-art-deep.md:5-18`) — confidence: high — state those scopes precisely.

F: LOW — `README.md:137-139` — says the n=11 table is omitted, but `solver-a/table_n11.txt` is present — confidence: high — list only n=12 and n=13 as omitted.
