# Experiment 10: paging along a walk

A cache of k pages serves requests that move one page left or right at a
time. On a fault it may load the page asked for and one of its neighbours for
the price of one fault. The adversary may only issue walks that an offline
cache, with the same right and full foresight, serves with at most m faults.
How many faults can the online cache be forced into?

**Short answer: exactly max(m + 1, 2m + 1 − ⌊k/2⌋).** The online cache is as
good as the offline one, plus one fault, while it is still filling up. After
that every offline fault costs it two. The chain wrote this formula down as a
candidate and said it would not bet on it. Proved by hand, both bounds twice
by different arguments; computed for k ≤ 14, m ≤ 6 by a search that uses a
pruning rule, and without pruning up to k = 8, m = 4.

## The game

Pages are the integers. A cache holds at most k pages (k ≥ 2), initially
none. A request for a cached page is a hit and changes nothing. Otherwise it
is a fault: the owner picks a neighbour q of the requested page p (q = p − 1
or p + 1) and keeps any set of at most k pages drawn from the old cache, p
and q, which must include p.

A walk starts at page 0 and each request is one more or one less than the
last. opt(s) is the least number of faults with which the walk s can be
served, knowing all of it in advance.

The adversary, seeing the online cache, extends the walk one request at a
time or stops. A request is allowed only if opt of the walk so far, including
it, is at most m. The online cache serves each request without knowing the
future. F(k, m) is the number of faults the adversary can force.

The question, as the chain left it ([chain/3-link.md](chain/3-link.md)): find
F(k, m) for all k ≥ 2, m ≥ 1.

## Results

Write h = ⌊k/2⌋.

| Claim | Status | Where |
|---|---|---|
| F(k, m) ≤ max(m + 1, 2m + 1 − h) | proved by hand, twice. Solver A's proof compares with a schedule that never prefetches and cites Belady's theorem; solver B's is self-contained. Referee read both; I read solver B's | `solver-a/proofs.txt` Theorem U, `solver-b/proofs.txt` |
| F(k, m) ≥ max(m + 1, 2m + 1 − h) | proved by hand, twice, with two different adversaries. Referee read both and replayed each adversary for k ≤ 13, m ≤ 6 against every online reply that keeps as much as it can, and against every reply at all in 12 small cases; I read solver B's | `solver-a/proofs.txt` Theorem L, `solver-b/proofs.txt` |
| **F(k, m) = max(m + 1, 2m + 1 − ⌊k/2⌋) for all k ≥ 2, m ≥ 1** | follows from the two rows above | |
| Table: all k = 2 … 14, m = 1 … 6, and some entries at m = 7, 8 | computed by the referee's program with domination pruning (unpruned for 22 cases up to k = 8, m = 4); the two solvers' full game solvers cover k ≤ 12, m ≤ 5 between them (solver A timed out on (7,5), which solver B has) and parts of m = 6. All agree | `verifier/`, `reports/` |
| Pruning used in the searches is safe | argued by hand, twice; both write-ups are loose (solver B's invariant is misstated, see Review) and the argument is repairable; pruned and unpruned searches agree where both ran (up to k = 8, m = 4) | `solver-b/proofs.txt`, last section; `solver-a/proofs.txt` Lemma D |

F(k, m), from the formula; every entry shown was also computed:

| m \ k | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 |
| 2 | 4 | 4 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 |
| 3 | 6 | 6 | 5 | 5 | 4 | 4 | 4 | 4 | 4 | 4 | 4 |
| 4 | 8 | 8 | 7 | 7 | 6 | 6 | 5 | 5 | 5 | 5 | 5 |
| 5 | 10 | 10 | 9 | 9 | 8 | 8 | 7 | 7 | 6 | 6 | 6 |
| 6 | 12 | 12 | 11 | 11 | 10 | 10 | 9 | 9 | 8 | 8 | 7 |

So F = m + 1 exactly when k ≥ 2m, and for a fixed cache size the saving over
2m is ⌊k/2⌋ − 1 once m ≥ ⌊k/2⌋.

## What the online cache does

On the first request it loads pages 0 and 1. From then on it keeps a window
of consecutive pages. While there is room for two more, a
fault at the page just outside the window loads that page and the next one
beyond it. Once the window is within one page of full, a fault loads only the
page asked for and drops the page at the far end if the window overflows.

Prefetching outward for ever, the rule the chain suggested, is right only
while filling: after that its fault count ran past every cap tried in the
cases tested.

## What the adversary does

This is solver B's adversary; solver A's is built differently and is
described in its proof file. It first walks straight for 2·min(m, h) − 1
steps, towards a neighbour of page 0 that the online cache did not load in
the first round. Those pages were all absent after the first round and a
fault brings in at most two, so the online cache pays at least min(m, h) more
faults, while offline pays min(m, h) in all by loading pairs.

If m > h, it then repeats a cycle that costs offline one fault and online at
least two. It keeps in mind an offline cache that is a window of k pages (k − 1 until
the first cycle, for odd k) around the current page. If the online cache is missing one of that window's pages, it
walks there: a free fault. Otherwise it steps one page past an end of the
window whose outside page online does not hold, so online must fault, and
looks at what online did:

- if online did not also load the next page out, it requests that page, and
  offline is taken to have loaded both at once;
- if online did, then at least one page of the window is now missing from
  its cache (at least two if the window had k pages), and the adversary walks
  back to the nearest missing one, offline having loaded only the single
  page.

The offline choice is made after seeing the online reply, which is legitimate
because offline is defined with hindsight.

## What to distrust

- **Everything is by one model family.** Nothing is machine-checked as a
  proof. Both bounds have two different arguments and a referee's replay;
  the two upper-bound arguments analyse the same online strategy.
- **Both solvers' checkers have a blind spot** found by the referee: each
  would accept an adversary that let play go on for ever with no further
  faults, and solver A's stops at the first finished play, so it checks
  legality only on the positions examined before that. The referee's own
  program checks for both and found neither.
- **The large table entries rest on a pruning rule** whose two written
  proofs are both loose. They agree with unpruned search wherever that was
  run, and the formula itself is proved without any computation.
- **Solver A's sample plays** (`solver-a/witnesses.txt`) are all straight
  walks, so they never exercise the interesting part of its adversary; half
  the rows lack the unpruned check they advertise.
- **Solver A's upper bound depends on Belady's theorem**, cited and not
  proved. Solver B's does not, so the result does not depend on it.
- **The model is narrow.** Requests must walk a line, the offline cost is
  capped, and the answer is an exact count, not a competitive ratio. It says
  nothing directly about paging with arbitrary requests.
- **Prior art** ([prior-art.md](prior-art.md)): paging where requests follow
  a graph (Borodin, Irani, Raghavan, Schieber 1991) and caching where a miss
  may load a block (Beckmann, Gibbons, McGuffey 2022; Coester, Levin, Naor,
  Talmon 2022) are both established, with results as ratios. One paper
  combines walks on a graph with prefetching (Gaysinsky, Itai, Shachnai
  2001), with execution time as the objective. The search did not find
  neighbour-only prefetching on a line counted in faults, or an exact game
  value of this kind. It read no source in full.

## Review

Before publication the claims went to five reviewers from four model families
(two Claude, GPT, Grok, Gemini), each reading the files. All five confirmed
solver B's upper and lower bounds and solver A's upper bound, and that the
proofs treat the game as defined. Four confirmed solver A's lower bound; GPT
marked it refuted on one false aside ("2m ≥ k, i.e. m ≥ ⌊k/2⌋", wrong for odd
k) that the argument does not use. Their findings:

- **The pruning lemma is misstated in solver B's file** (GPT, Grok). It
  claims an invariant "credit ≥ |V∖R|" that fails at the start, when the
  credit is zero. The intended statement, that the real cache's faults are at
  most the virtual one's plus the initial |V∖R| minus the current one, is
  what the simulation shows, provided the real cache evicts only pages
  outside V. Not repaired in the file; noted at its top.
- I wrote that the adversaries were replayed against every online reply.
  The large runs try only replies that keep as much as possible.
- I wrote "computed exhaustively" for a table that relies on pruning.
- My account of the adversary assumed the online cache prefetched on its
  first fault, and said it "must" drop pages where the proof shows only that
  some are missing.
- The prior-art paragraph denied a combination that the search report itself
  lists.

Loose statements in the two proof files are listed in a note at the top of
each. The corrected wording has not been through a second round.

## Files

- `review/`: the review of this experiment before publication (16 to 20 November), archived on 21 November verbatim with absolute paths shortened: the brief given to the reviewers, each seat's report, the machine-made summary, and the organiser's note of what was done with the findings. The reports were written about the files as they stood before the findings were applied, so their line numbers and quotations may not match the current files.
- `chain/`: the seed, the three links and the brief each link was given.
- `brief-solver.txt`, `brief-verifier.txt`, `brief-prior-art.txt`: what each
  instance was told.
- `reports/`: each instance's final report, verbatim.
- `solver-a/`, `solver-b/`: game solvers, strategy checkers, proofs.
- `verifier/`: the referee's program and its outputs.
- `prior-art.md`: the literature search.

Binaries are left out.
