<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. The brief given to every reviewer of this round (attachments, if any, are not archived). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Round 8: the note now claims T(n) = n/3 - O(log n), with both bounds machine-checked

Since round 7 the working tree of the repository has changed (not yet committed; read the files as they are on
disk, and use `git diff` / `git status` to see what is new):

- `14-forest-orientation/NOTE.md`: new title and Summary; a new Section 6 "Proof of Theorem 5"
  (T(n) <= floor((n-1)/3), rule L, Lemmas 6-8); Section 7 "Machine check" now covers two theorems; Sections 8, 9.
- Rule L is now stated by a CODE: flip from an endpoint x with 2h(x) + eps(x) <= 2h(y) + eps(y). The draft you
  may have seen in round 7 compared the sorted slots lexicographically. The proof is otherwise that draft.
- `14-forest-orientation/lean/FO/Upper*.lean` (7 new files, about 2,040 lines), `lean/FO.lean`, `lean/SPEC.md`,
  `lean/PROGRESS.md`, `lean/AXIOMS.txt`, `lean/SORRY.txt`, `lean/BUILD.log`: a Lean 4 proof of
  `upper_third : forall n, 1 <= n -> 3 * FO.T n + 1 <= n`. `lean/FO/Spec.lean` is claimed to be unchanged.
- `14-forest-orientation/upper/third_check.py`, `third_paths.py`, `third_trees.py`, `third_pool.cpp`,
  `third_log.txt`: programs and their recorded output.
- `14-forest-orientation/README.md`, top-level `README.md`.

This is a claim that an open question is settled, written and so far read only by instances of one model. Assume
something is wrong until you have traced it. You cannot run Lean here; audit the Lean by reading. Do not accept a
program's or a document's verdict in place of your own reading.

Q1. NOTE.md Section 6 with the code form of rule L. (i) Is "kappa(x) <= kappa(y) implies h(x) <= h(y)" right, and
    is rule L therefore a way of breaking ties of rule R / of the Gupta-Kumar-Stein algorithm as the note says?
    (ii) Is the stated consequence ("if beta = 0 and eps(y) = 0 then eps(x) = 0; for k >= 1 that is alpha_0 = 0; for
    k = 0 it is: if y has no out-neighbour then x has none") correctly derived from the code rule, and is it all
    that (a) and (d) of Lemma 8 need? (iii) Take ties that the code rule leaves open but the lexicographic rule
    decided (h(x) = h(y), both eps = 1, different larger slots; or both eps = 0): for at least three such rounds on
    concrete trees, with EACH choice of endpoint, compute Phi before and after by hand and report whether it rises
    by at least 3k. (iv) Re-check Lemma 6, Lemma 7 and the proof of Theorem 5 as now written (they were edited
    when moved into the note): any step that no longer follows?

Q2. The Lean statement. Read `lean/FO/Spec.lean`, `lean/FO/UpperSpec.lean` and the last 60 lines of
    `lean/FO/UpperMain.lean`. (i) Does `upper_third` say T(n) <= floor((n-1)/3) for the SAME `T` as
    `T_lower_bound` (`lean/FO/Main.lean`)? (ii) Is `Spec.lean` unchanged (`git diff --stat`, `git status`)?
    (iii) Search the seven new files for anything that could make the theorem true for the wrong reason:
    `sorry`, `admit`, `axiom`, `native_decide`, `unsafe`, `implemented_by`, `extern`, `opaque`, `partial`,
    `instance` declarations that override arithmetic or decidability, `local notation`/`macro`/`syntax`,
    redefinition or shadowing of `T`, `value`, `replies`, `moves`, `cost`, `valid` inside a namespace, or a
    theorem stated about a different function with a similar name. Report what you searched and what you found.
    (iv) Is the last step (from `upper_value` to `upper`) a correct instantiation: value of n-1 rounds from the
    empty position, Phi of the empty position, number of edges?

Q3. Lean against the note. Read the statements (not necessarily the proofs) of `round_gain`, `ruleL_round`,
    `lemma6`, `lemma6_gen`, `upper_value`, and the definitions `wt`/`w2`/`w1`, `eps`, `hh`, `Phi`, `code`, `ruleL`,
    `spath`. (i) Do the Lean weights equal the note's table (check each line, and the one-out-neighbour case)?
    (ii) Is `eps` the note's eps? (iii) Does `round_gain` have the hypotheses the note claims (any endpoint with
    code <= , any shortest path)? Is anything assumed there that a real round of rule L might not satisfy?
    (iv) Is every sentence about the Lean development in NOTE.md Section 7, in the README's "Machine check" and
    in `lean/SPEC.md` (upper-bound part) accurate? Quote any that is not.

Q4. Claims. Read the Summary of NOTE.md, the top of `14-forest-orientation/README.md`, its Results table and
    "What to distrust", and row 14 of the top-level README. (i) Does every statement follow from what is proved:
    "L = 1/3", "T(n) = n/3 - O(log n)", "the rate is exactly 1/3", "both machine-checked ... one and the same
    definition", "the tie-breaking rule is what brings the cost from n/2 down to n/3", "superseded", the bounds
    1/3 <= n/3 - T(n) <= (7/3) log2(n+1) - 5/6, "314 <= T <= 333 at n = 1000", T(1) = T(2) = T(3) = 0? (ii) Is any
    statement left anywhere in these files (not the archival `reports/`, `open-limit/`, `solver-*`) that
    contradicts the new result, e.g. still calling L open or bounded by 1/2? (iii) Is the novelty wording
    (Summary last paragraph, Section 8, Open question 4) appropriately cautious? If you know a reference that
    states an n/3 (or any explicit constant below 2) bound on total flips for incremental forests at out-degree 2
    (also phrased as load 2, or recourse in online load balancing / carpooling on forests), give it.

Q5. Programs. For `third_check.py`, `third_paths.py`, `third_trees.py`, `third_pool.cpp`: does each implement the
    weight table, eps, and the code form of rule L as in the note? Name any real round the enumeration misses
    relative to what NOTE.md says it covers, any way to print ALL OK or exit 0 after a failure, and any figure in
    NOTE.md "Checks by program" that does not match `upper/third_log.txt`.

Q6. Try to break Theorem 5. (i) Consistency with Theorem 1, with Theorem 4 (what does rule L do in the second
    round at each level of the ternary family?), with T(n) = B(n) for n <= 15, T(30) >= 7, T(32) >= 8, T(64) >= 18.
    (ii) Build by hand a tree with a vertex off the flipped path that has several in-neighbours and whose slots
    change in the round; compute Phi before and after. (iii) Is there a position where no endpoint satisfies the
    rule, or where the prescribed flip is not a legal reply?

For each question give CONFIRMED / REFUTED / UNSUPPORTED with file:line, and the replacement wording you would
use where something is wrong or unclear.
