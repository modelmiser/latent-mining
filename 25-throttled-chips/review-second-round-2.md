<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 25 page after the `tight/` follow-up. No CRITICAL defects. Four MODERATE (stale or inconsistent), seven LOW. Nothing was written under (workspace) and no scripts were run; the checks were read-only against the files.

**Numbers checked, all correct**
- Crossover: 5h − 28 = 2h − 1 at h = 9, so 2h − 1 is strictly better for h ≤ 8 (README:15); `tight/PROOFS.md:88` ("beats from h = 10") agrees.
- 18 rounds = (5h − 10) − (5h − 28).
- Heights: t/5 + 4.8 after a round and + 5.6 after an add (`PROOFS.md` B5). `out_rule_main.txt` has max 5η − t of 12 after a round and 16 after an add, so t/5 + 3.2 is right.
- 89,243 plays matches `lists=89243`.
- `out_C.txt` gives C(3) = 7 and C(h) = 5h − 10 for h = 4..12; the page's "4 ≤ h ≤ 14" is attributed to `reports/` and is supported at `referee.md:11` and `solver-a.md:13`.
- The B4 table has ten cases (j = 0..9), and three superseded lower-bound rows are marked (53, 55, 57).

**MODERATE**
1. **README:188–190** — "Its tests exercise the first two lemmas only." This contradicts row 51 and the Review paragraph (README:337–338), which say the height bound was asserted on every play and never tight. The panel correction reached the table but not this bullet. Replace with: "Its tests bind only on the first two lemmas; the height bound is asserted on every play too but is never close to tight (t/5 + 3.2 seen against 5.6), so the last two lemmas rest on the argument."
2. **README:68–69, 78–79** ("Why 5h is enough") — "solver B's P3 is the sharper version" and "incompatible once m reaches 5(h − 1)" present 5h − 5 and 5h − 6 as the best upper bound, with no pointer. Append: "(Superseded for h ≥ 4: the same potential, counted exactly, gives 5h − 10; see `tight/PROOFS.md` Part A.)"
3. **README:158** — "The constant 35.2 is not optimised … brought the constant down to 28." This conflates two constants: 35.2 is a height constant, now 5.6, while 28 replaces 176. Replace with: "brought the height bound down to t/5 + 5.6 (so 5h − 176 becomes 5h − 28)".
4. **README:56 (bold row) and README:110** — "every lag is at most 2 + H_n" is still true but no longer the best bound. `tight/PROOFS.md` B2 (lines 132–137) proves every lag is at most max(1, H_n) after a round, with n the number of points whose target is at least 6/5, a subset of Theorem U's n. Add to row 56: "(sharpened to max(1, H_n) in `tight/PROOFS.md` Lemma B2)". README:210 ("about ln t + 4.1") needs the same pointer.

**LOW**
5. **README:110** — "(Both are superseded below: the lag is at most 2 + H_3t.)" names only the lag. Add: "and the height is at most t/5 + 5.6".
6. **README:192** — the bullet "The constant bound has one author and is new" describes 5h − 176 with no superseded marker. Retitle: "The first constant bound (5h − 176, superseded by 5h − 28)…".
7. **README:191** — "at h = 6 and 7, on 3 to 6 neighbouring points" is broader than `out_search_h6.txt`, which has widths 3, 5, 6 at h = 6 and 3, 5 at h = 7. Use: "on 3, 5 or 6 neighbouring points at h = 6 and 3 or 5 at h = 7".
8. **README:191, leftovers list** — two leftovers are missing. `tight/PROOFS.md:4` ("nobody else has read it") and `tight/REPORT.md` ("read by nobody else"; tests "not … the height bound") are still in the files. Add them, as was done for `lag/`.
9. **README:16** — "the single-point list, which needs at most 5h − 10 chips … whatever h is": 5h − 10 holds only for h ≥ 4 (C(3) = 7). Insert "(for h ≥ 4)". The 18-round claim is true for every h, since the gap is at most 2 for h ≤ 3.
10. **README:64** — the conjecture row is true, but half of it is now proved. Add: "(C(h) ≤ 5h − 10 is proved; C(h) ≥ 5h − 10 beyond h = 14 and G = C are open)".
11. **README:345, 357, 219** — three wording slips in Files and one bullet:
    - `tight/` "a solver for the single-point list": `solve.cpp` also checks arbitrary lists and runs the random search.
    - `constant/` "the follow-up that proves the constant bound": should read "the first follow-up (5h − 176)".
    - "Slips in the follow-up's files": should read "in `constant/`".

**Clean**
- Short answer, apart from item 9.
- Results rows 50–55, 57–63: superseded marks, bold rows and statuses are right; row 54's "two rows above" still holds.
- Bullet at README:179–184.
- All Review paragraphs, read as history.
- Top-level `README.md:56` (row 25): consistent with the page.
