<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261120-005400 — (repository) — round 1

Seats: cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $0.000; this month $198.21 of $200.00 (Cursor spend not metered: check Cursor's usage page)


| Q | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|
| **Q4** (split) | CONFIRMED | REFUTED | CONFIRMED |
| **Q6** (split) | CONFIRMED | REFUTED | CONFIRMED |
| **Q7** (split) | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q4, Q6, Q7
Reports: cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 3 CRITICAL, 2 MODERATE, 11 LOW
- **CRITICAL** [cursor:gpt-5.6-sol-xhigh] `verifier/vb.cpp:21,50`; `solver-b/back2.cpp:11,48` — unrestricted n is written into fixed 12-element state arrays; vb can overflow for n≥13 and back2 for n≥14 — confidence: high — reject unsupported n or allocate dynamically
- **CRITICAL** [cursor:gemini-3.8-flash-high] 21-expiring-balls/verifier/vb.cpp:51 — Invoking `./vb 11 4` per usage default (`lg = 22`) overflows the table at 70% load (`cnt = 2,936,013` vs core size 3,722,933), aborting with `exit(3)` (`table full`) — confidence: high — Increase default `lg` in line 51 from 22 to 23
- **CRITICAL** [cursor:gemini-3.8-flash-high] 21-expiring-balls/solver-a/verify12.py:64 — Opening `adv_n7_L3.txt` using a bare relative path crashes with `FileNotFoundError` when run outside `solver-a/` — confidence: high — Resolve path relative to `__file__` via `os.path.join(os.path.dirname(__file__), 'adv_n7_L3.txt')`
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:46,94-95` — statewise agreement is claimed although only cardinalities agree; the stored cores even differ (`verifier/out_vb_11_4.txt:2`, `verifier/out_back2_rebuilt_11_4.txt:2`) — confidence: high — say “same attractor cardinality”
- **MODERATE** [cursor:grok-4.7-medium] `verifier/out_vb_11_4.txt:2` vs `solver-b/out_11_4_back2.txt:3` — cores 3722933 vs 3723730 while seeds and the 974272116 count match, so at least one core is not exactly the minimal winning set — high — report both core sizes and stop saying the sets were compared
- **LOW** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:91-92` — “results are computations” overlooks the explicit hand-checkable adversary strategies — confidence: high — distinguish computational lower bounds from explicit upper strategies
- **LOW** [cursor:gpt-5.6-sol-xhigh] `21-expiring-balls/README.md:101-104` — literature-reading scope misstates `prior-art.md:54` — confidence: high — use the replacement in Q7
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:45` — “5 bins untouched” is false when x accepts {x,f} (6 of 12 bins still free) — high — say “5 if P5 runs to the end; 6 if x accepts early”
- **LOW** [cursor:grok-4.7-medium] `README.md:78` — “the same threat twice” skips the junk-expiry branch — high — use the Q6 replacement
- **LOW** [cursor:grok-4.7-medium] `README.md:52` — tie-break omits “untouched bins last” (`solver-a/greedy.cpp:4`) — high — add that phrase
- **LOW** [cursor:grok-4.7-medium] `README.md:4` — “not already linked” is weaker than different components — medium — say “in different components”
- **LOW** [cursor:grok-4.7-medium] `verifier/vb.cpp:17,51` — default 2^22 slots abort once the core exceeds 0.7·2^22, below the n=11 core — high — document the slot count used for the 974,272,116 run
- **LOW** [cursor:grok-4.7-medium] `README.md:101-102` — “three papers” and “semester project” are not what `prior-art.md` records — medium — name Ozdayi, Bender (introduction), and Armon as the search does
- **LOW** [cursor:gemini-3.8-flash-high] 21-expiring-balls/README.md:46 — Describing 974,272,116 states as "the same winning set" obscures that these are visited nonterminal states in the backward attractor and that the empty state is outside — confidence: high — Clarify to "backward attractor of 974,272,116 nonterminal states (with the empty state outside)"
- **LOW** [cursor:gemini-3.8-flash-high] 21-expiring-balls/README.md:66-74 — The 7-bin strategy prose omits the shortcut branch where expiring once immediately after step 1 yields a clean 2-star if x holds the two youngest balls — confidence: high — Note the optional step-1 early expiry shortcut from `solver-a/proofs.txt` P5
- **LOW** [cursor:gemini-3.8-flash-high] 21-expiring-balls/solver-a/x:1 — Stray leftover test output file `x` from capfifo execution was left uncommitted in `solver-a/` — confidence: high — Remove `solver-a/x` or add it to `.gitignore`

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
