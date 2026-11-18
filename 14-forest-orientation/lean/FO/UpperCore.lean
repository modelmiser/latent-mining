/-
U1.  ARITHMETIC CORE of the upper bound T(n) ≤ (n - 1)/3 (../NOTE.md, Section 6; called "the draft" in these files).  No graphs.

Everything is in THIRDS (as in the draft and in third_check.py), so all quantities are integers.
Conventions of this file:
  * a height `h(c)` is a natural number;
  * a flag `ε(c)` is a natural number, 0 if the two slots of `c` are equal, 1 if not
    (lemmas that need it assume `ε ≤ 1`);
  * weights are INTEGERS: the table gives `w = -1` for a vertex with two out-neighbours that
    both have height 0 and flag 0 (a "cherry").  So `Φ` can be negative.

Contents: the weight table (`w1`, `w2`, `wt`); the bounds used by Lemma 6 (`w1_le_*`,
`w2_le_*`); Lemma 7 (`w1_mono`, `w2_mono_left`, `w2_mono_right`, `w2_mono`); the path
inequalities (a)-(d) of Lemma 8 with the carry `pi` (`path_start`, `path_interior`,
`path_end_none`, `path_end_some`, `free_none`, `free_some`); the telescoping
(`rest_gain`, `path_gain`); deliberately false variants, refuted.
-/
namespace FO.Upper

/-- Split every `if` of the goal, then linear arithmetic. -/
macro "ifs_omega" : tactic => `(tactic| ((repeat' split) <;> omega))

/-! ### the weight table -/

/-- Weight of a vertex with exactly one out-neighbour, of height `h` and flag `e`. -/
def w1 (h e : Nat) : Int := if h = 0 then (e : Int) else 1

/-- Weight of a vertex with two out-neighbours of heights `p ≤ q` and flags `ep`, `eq`. -/
def w2s (p ep q eq : Nat) : Int :=
  if q = p then 3 * (p : Int) - 1 + ep + eq
  else if q = p + 1 then 3 * (p : Int) + 1 + ep + eq
  else 3 * (p : Int) + 2 + ep

/-- Weight of a vertex with two out-neighbours `(h1, e1)`, `(h2, e2)`, in any order. -/
def w2 (h1 e1 h2 e2 : Nat) : Int := if h1 ≤ h2 then w2s h1 e1 h2 e2 else w2s h2 e2 h1 e1

/-- The weight as a function of the list of (height, flag) pairs of the out-neighbours.
(Three or more out-neighbours do not occur in a valid position; the value 0 is a dummy.) -/
def wt : List (Nat × Nat) → Int
  | [] => 0
  | [c] => w1 c.1 c.2
  | [c, d] => w2 c.1 c.2 d.1 d.2
  | _ => 0

/-- `γ(c) = 3 h(c) + 1 + ε(c)`. -/
def gam (h e : Nat) : Int := 3 * (h : Int) + 1 + e

/-- `0` if `a = b`, `1` otherwise: the flag of a vertex whose two slots are `a` and `b`. -/
def flagNe (a b : Nat) : Nat := if a = b then 0 else 1

theorem flagNe_le (a b : Nat) : flagNe a b ≤ 1 := by unfold flagNe; split <;> omega

theorem w2_comm (h1 e1 h2 e2 : Nat) : w2 h1 e1 h2 e2 = w2 h2 e2 h1 e1 := by
  simp only [w2, w2s]; ifs_omega

/-! Hand computations of the table (third_check.py `weight`), each can fail. -/
example : wt [] = 0 := by decide
example : wt [(0, 0)] = 0 := by decide              -- one out-neighbour, a leaf
example : wt [(0, 1)] = 1 := by decide
example : wt [(5, 0)] = 1 := by decide
example : wt [(0, 0), (0, 0)] = -1 := by decide     -- the cherry
example : wt [(2, 1), (2, 1)] = 7 := by decide      -- 3·2 − 1 + 1 + 1
example : wt [(2, 1), (3, 0)] = 8 := by decide      -- 3·2 + 1 + 1 + 0
example : wt [(3, 0), (2, 1)] = 8 := by decide
example : wt [(2, 1), (4, 1)] = 9 := by decide      -- 3·2 + 2 + 1, flag of the higher ignored
example : wt [(7, 0), (2, 0)] = 8 := by decide

/-! ### the description with a maximum (third_check.py part (0)), and Lemma 6's bounds

A vertex with one out-neighbour has height 0 and flag 1, so `γ = 2`, `g = 3 h(c) + 1 (h(c) ≥ 1)`
or `2 (h(c) = 0)`.  A vertex with two out-neighbours of heights `h1, h2` has height
`min h1 h2 + 1` and flag `flagNe h1 h2`. -/

/-- `g(v)` for a vertex with two out-neighbours of heights `p ≤ q` (slots `(p, q)`). -/
def g2 (p q : Nat) : Int := if q = p then 3 * (q : Int) + 4 else if q = p + 1 then 3 * (q : Int) + 2
  else 3 * (q : Int) + 1

theorem w1_eq_max (h e : Nat) (he : e ≤ 1) :
    w1 h e = 1 + gam h e - max (gam h e) (if h = 0 then 2 else 3 * (h : Int) + 1) := by
  simp only [w1, gam]; ifs_omega

theorem w2s_eq_max (p ep q eq : Nat) (hpq : p ≤ q) (hp : ep ≤ 1) (hq : eq ≤ 1) :
    w2s p ep q eq = 1 + gam p ep + gam q eq - max (max (gam p ep) (gam q eq)) (g2 p q) := by
  simp only [w2s, gam, g2]; ifs_omega

/-- Lemma 6, one out-neighbour: `w ≤ 1 + γ(c) − max(γ(c), γ(r))` with `γ(r) = 2`. -/
theorem w1_le_one (h e : Nat) (he : e ≤ 1) : w1 h e ≤ 1 := by simp only [w1]; ifs_omega
theorem w1_le_gam (h e : Nat) : w1 h e ≤ 1 + gam h e - 2 := by simp only [w1, gam]; ifs_omega

/-- Lemma 6, two out-neighbours: `w ≤ 1 + γ₁ + γ₂ − max(γ₁, γ₂, γ(r))`,
`γ(r) = gam (min h1 h2 + 1) (flagNe h1 h2)`, as three inequalities. -/
theorem w2_le_left (h1 e1 h2 e2 : Nat) (he1 : e1 ≤ 1) (he2 : e2 ≤ 1) :
    w2 h1 e1 h2 e2 ≤ 1 + gam h1 e1 := by simp only [w2, w2s, gam]; ifs_omega
theorem w2_le_right (h1 e1 h2 e2 : Nat) (he1 : e1 ≤ 1) (he2 : e2 ≤ 1) :
    w2 h1 e1 h2 e2 ≤ 1 + gam h2 e2 := by simp only [w2, w2s, gam]; ifs_omega
theorem w2_le_parent (h1 e1 h2 e2 : Nat) (he1 : e1 ≤ 1) (he2 : e2 ≤ 1) :
    w2 h1 e1 h2 e2 ≤ 1 + gam h1 e1 + gam h2 e2 - gam (min h1 h2 + 1) (flagNe h1 h2) := by
  simp only [w2, w2s, gam, flagNe]; ifs_omega

/-! ### Lemma 7 (monotone)

The pair `(h, ε)` of a vertex with sorted slots `(P, Q)` (unshifted, hole = −1) is
`(P + 1, [P < Q])`.  If the slots increase componentwise to `(P', Q')` then either `P' = P`
and `[P < Q] ≤ [P' < Q']`, or `P' > P`; that is, `2h + ε ≤ 2h' + ε'`.  Conversely the four
single-slot steps of the draft are (S1) `2h+ε ↦ +1`, (S2) `+0`, (S3) `+1`, (S4) `+2`.
So "monotone under every componentwise increase of the slots of an out-neighbour" is exactly
"monotone in the code `2h + ε` of each out-neighbour", which is what is proved here and what
U3 needs: for a vertex off the path the out-neighbours are the same before and after, and the
code of each does not decrease. -/

theorem w1_mono {h e h' e' : Nat} (he : e ≤ 1) (he' : e' ≤ 1) (hle : 2 * h + e ≤ 2 * h' + e') :
    w1 h e ≤ w1 h' e' := by simp only [w1]; ifs_omega

theorem w2_mono_left {h e h' e' h2 e2 : Nat} (he : e ≤ 1) (he' : e' ≤ 1) (he2 : e2 ≤ 1)
    (hle : 2 * h + e ≤ 2 * h' + e') : w2 h e h2 e2 ≤ w2 h' e' h2 e2 := by
  simp only [w2, w2s]; ifs_omega

theorem w2_mono_right {h e h' e' h1 e1 : Nat} (he : e ≤ 1) (he' : e' ≤ 1) (he1 : e1 ≤ 1)
    (hle : 2 * h + e ≤ 2 * h' + e') : w2 h1 e1 h e ≤ w2 h1 e1 h' e' := by
  rw [w2_comm h1 e1 h e, w2_comm h1 e1 h' e']; exact w2_mono_left he he' he1 hle

/-- Lemma 7 for both out-neighbours at once. -/
theorem w2_mono {h1 e1 h1' e1' h2 e2 h2' e2' : Nat} (a1 : e1 ≤ 1) (a1' : e1' ≤ 1) (a2 : e2 ≤ 1)
    (a2' : e2' ≤ 1) (l1 : 2 * h1 + e1 ≤ 2 * h1' + e1') (l2 : 2 * h2 + e2 ≤ 2 * h2' + e2') :
    w2 h1 e1 h2 e2 ≤ w2 h1' e1' h2' e2' :=
  Int.le_trans (w2_mono_left a1 a1' a2 l1) (w2_mono_right a2 a2' a1' l2)

/-- The single-slot steps of the draft, literally, for an out-neighbour `(h, ε)` beside a fixed
one `(h2, e2)` and alone: (S1) `(h,0) → (h,1)`, (S3) `(h,1) → (h+1,0)`, (S4) `(h,1) → (h+1,1)`.
((S2) changes nothing.) -/
theorem lemma7_steps (h h2 e2 : Nat) (he2 : e2 ≤ 1) :
    (w2 h 0 h2 e2 ≤ w2 h 1 h2 e2 ∧ w1 h 0 ≤ w1 h 1) ∧
    (w2 h 1 h2 e2 ≤ w2 (h + 1) 0 h2 e2 ∧ w1 h 1 ≤ w1 (h + 1) 0) ∧
    (w2 h 1 h2 e2 ≤ w2 (h + 1) 1 h2 e2 ∧ w1 h 1 ≤ w1 (h + 1) 1) :=
  ⟨⟨w2_mono_left (by omega) (by omega) he2 (by omega), w1_mono (by omega) (by omega) (by omega)⟩,
   ⟨w2_mono_left (by omega) (by omega) he2 (by omega), w1_mono (by omega) (by omega) (by omega)⟩,
   ⟨w2_mono_left (by omega) (by omega) he2 (by omega), w1_mono (by omega) (by omega) (by omega)⟩⟩

/-! FALSE variants of Lemma 7, refuted.
(F7a) With `3p + 2` in place of `3p + 1` in the second line of the table the weight is NOT
monotone: heights (0, 1) with flags (0, 1) have weight 3; raising the higher out-neighbour to
(2, 0) gives weight 2. -/
def w2sBad (p ep q eq : Nat) : Int :=
  if q = p then 3 * (p : Int) - 1 + ep + eq
  else if q = p + 1 then 3 * (p : Int) + 2 + ep + eq
  else 3 * (p : Int) + 2 + ep
example : ¬ (∀ p ep q eq q' eq' : Nat, p ≤ q → eq ≤ 1 → eq' ≤ 1 → 2 * q + eq ≤ 2 * q' + eq' →
    w2sBad p ep q eq ≤ w2sBad p ep q' eq') := by
  intro h; have := h 0 0 1 1 2 0 (by omega) (by omega) (by omega) (by omega); revert this; decide
/-- (F7b) "The weight strictly increases when a slot increases" is false (step S3 with the
other out-neighbour one higher: old `3p + 2 + ε`, new `3p + 2 + ε`). -/
example : ¬ (∀ h e h' e' h2 e2 : Nat, e ≤ 1 → e' ≤ 1 → e2 ≤ 1 → 2 * h + e < 2 * h' + e' →
    w2 h e h2 e2 < w2 h' e' h2 e2) := by
  intro h; have := h 0 1 1 0 1 0 (by omega) (by omega) (by omega) (by omega); revert this; decide
/-- (F7c) The weight is NOT monotone in the heights alone (flags ignored), i.e. the order
`2h + ε` matters: `(1, 1) → (1, 0)` lowers the weight. -/
example : ¬ (∀ h e h' e' h2 e2 : Nat, e ≤ 1 → e' ≤ 1 → e2 ≤ 1 → h ≤ h' →
    w2 h e h2 e2 ≤ w2 h' e' h2 e2) := by
  intro h; have := h 1 1 1 0 1 0 (by omega) (by omega) (by omega) (by omega); revert this; decide

/-! ### case lemmas (so that `omega` can treat the table as linear constraints) -/

theorem w2s_cases (p ep q eq : Nat) :
    (q = p ∧ w2s p ep q eq = 3 * (p : Int) - 1 + ep + eq) ∨
    (q = p + 1 ∧ w2s p ep q eq = 3 * (p : Int) + 1 + ep + eq) ∨
    (q ≠ p ∧ q ≠ p + 1 ∧ w2s p ep q eq = 3 * (p : Int) + 2 + ep) := by
  simp only [w2s]; ifs_omega

theorem w2_of_le {h1 h2 : Nat} (e1 e2 : Nat) (h : h1 ≤ h2) : w2 h1 e1 h2 e2 = w2s h1 e1 h2 e2 := by
  simp only [w2]; ifs_omega

theorem w2_of_ge {h1 h2 : Nat} (e1 e2 : Nat) (h : h2 ≤ h1) : w2 h1 e1 h2 e2 = w2s h2 e2 h1 e1 := by
  rw [w2_comm]; exact w2_of_le e2 e1 h

theorem w1_cases (h e : Nat) : (h = 0 ∧ w1 h e = e) ∨ (h ≠ 0 ∧ w1 h e = 1) := by
  simp only [w1]; ifs_omega

theorem flagNe_cases (a b : Nat) : (a = b ∧ flagNe a b = 0) ∨ (a ≠ b ∧ flagNe a b = 1) := by
  simp only [flagNe]; ifs_omega

/-! ### Lemma 8: the carry and the four path inequalities

Path `x = w₀ → w₁ → … → w_k`, `h(w_i) = k − i`.  For `i < k` the other out-neighbour of `w_i`
is `c_i`, of height `h(w_{i+1}) + α_i` and flag `ce`.  `y` has height `k + β` and flag `ey`.
State after `w_i`: `(o, e, f)`, `o = h'(w_i) − h(w_i)`, `e` = flag of `w_i` AFTER the round,
`f` = flag of `w_{i+1}` BEFORE the round.  (The draft's `e`, `f` are the Booleans "slots
equal", i.e. flag = 0.) -/

/-- The carry `π(o, e, f)`: −1 if the next vertex had unequal slots and `(o, new slots equal?)`
is one of (0, no), (1, yes), (1, no), (2, yes); 0 otherwise. -/
def pi (o e f : Nat) : Int :=
  if f = 0 then 0 else if (o = 0 ∧ e ≠ 0) ∨ o = 1 ∨ (o = 2 ∧ e = 0) then -1 else 0

theorem pi_cases (o e f : Nat) :
    (f = 0 ∧ pi o e f = 0) ∨
    (f ≠ 0 ∧ ((o = 0 ∧ e ≠ 0) ∨ o = 1 ∨ (o = 2 ∧ e = 0)) ∧ pi o e f = -1) ∨
    (f ≠ 0 ∧ ¬ ((o = 0 ∧ e ≠ 0) ∨ o = 1 ∨ (o = 2 ∧ e = 0)) ∧ pi o e f = 0) := by
  simp only [pi]; ifs_omega

/-- (a) START.  `l = h(w₁)`; old out-neighbours of `x`: `w₁` (height `l`, flag `en`) and `c₀`
(height `l + al`, flag `ce`); new: `c₀` and `y` (height `l + 1 + be`, flag `ey`).
Rule L enters as `hL`: if `h(y) = h(x)` and the slots of `y` are equal then `α₀ = 0`. -/
theorem path_start (l al ce be ey en : Nat) (hce : ce ≤ 1) (hey : ey ≤ 1) (hen : en ≤ 1)
    (hL : be = 0 → ey = 0 → al = 0) :
    2 + pi (min al (be + 1)) (flagNe al (be + 1)) en
      ≤ w2 (l + al) ce (l + 1 + be) ey - w2 l en (l + al) ce := by
  rw [w2_of_le en ce (Nat.le_add_right l al)]
  have c1 := w2s_cases l en (l + al) ce
  have c2 := flagNe_cases al (be + 1)
  have c3 := pi_cases (min al (be + 1)) (flagNe al (be + 1)) en
  by_cases hd : al ≤ be + 1
  · rw [w2_of_le ce ey (by omega : l + al ≤ l + 1 + be)]
    have c4 := w2s_cases (l + al) ce (l + 1 + be) ey
    omega
  · rw [w2_of_ge ce ey (by omega : l + 1 + be ≤ l + al)]
    have c4 := w2s_cases (l + 1 + be) ey (l + al) ce
    omega

/-- (b) INTERIOR.  `l = h(w_{i+1})`; old out-neighbours of `w_i`: `w_{i+1}` (height `l`, flag
`en`) and `c_i` (height `l + al`, flag `ce`); new: `c_i` and `w_{i−1}` (new height `l + 2 + o`,
new flag `e`).  New state: `(min al (o + 2), flagNe al (o + 2), en)`; the old state was
`(o, e, flagNe al 0)` because the old slots of `w_i` are equal exactly when `α_i = 0`. -/
theorem path_interior (l o e al ce en : Nat) (he : e ≤ 1) (hce : ce ≤ 1) (hen : en ≤ 1) :
    3 + pi (min al (o + 2)) (flagNe al (o + 2)) en - pi o e (flagNe al 0)
      ≤ w2 (l + al) ce (l + 2 + o) e - w2 l en (l + al) ce := by
  rw [w2_of_le en ce (Nat.le_add_right l al)]
  have c1 := w2s_cases l en (l + al) ce
  have c2 := flagNe_cases al (o + 2)
  have c3 := pi_cases (min al (o + 2)) (flagNe al (o + 2)) en
  have c5 := flagNe_cases al 0
  have c6 := pi_cases o e (flagNe al 0)
  by_cases hd : al ≤ o + 2
  · rw [w2_of_le ce e (by omega : l + al ≤ l + 2 + o)]
    have c4 := w2s_cases (l + al) ce (l + 2 + o) e
    omega
  · rw [w2_of_ge ce e (by omega : l + 2 + o ≤ l + al)]
    have c4 := w2s_cases (l + 2 + o) e (l + al) ce
    omega

/-- (c) END, `w_k` had no out-neighbour (its old flag is 0): weight 0 → `w1`. -/
theorem path_end_none (o e : Nat) : 1 - pi o e 0 ≤ w1 (1 + o) e - 0 := by
  have c1 := w1_cases (1 + o) e
  have c2 := pi_cases o e 0
  omega

/-- (c) END, `w_k` had one out-neighbour `z` (height `zh`, flag `ze`; old flag of `w_k` is 1);
its new out-neighbour `w_{k−1}` has new height `1 + o` and new flag `e`. -/
theorem path_end_some (o e zh ze : Nat) (he : e ≤ 1) (hze : ze ≤ 1) :
    1 - pi o e 1 ≤ w2 zh ze (1 + o) e - w1 zh ze := by
  have c1 := w1_cases zh ze
  have c2 := pi_cases o e 1
  by_cases hd : zh ≤ 1 + o
  · rw [w2_of_le ze e hd]
    have c4 := w2s_cases zh ze (1 + o) e
    omega
  · rw [w2_of_ge ze e (by omega : 1 + o ≤ zh)]
    have c4 := w2s_cases (1 + o) e zh ze
    omega

/-- (d) `k = 0`, `x` had no out-neighbour. -/
theorem free_none (be ey : Nat) : 0 ≤ w1 be ey - 0 := by simp only [w1]; ifs_omega

/-- (d) `k = 0`, `x` had one out-neighbour `z`.  Rule L enters as `hL`: `y` is not a vertex of
height 0 with equal slots (i.e. without out-neighbours), since then `y` has smaller slots. -/
theorem free_some (be ey zh ze : Nat) (hey : ey ≤ 1) (hze : ze ≤ 1) (hL : ¬ (be = 0 ∧ ey = 0)) :
    0 ≤ w2 zh ze be ey - w1 zh ze := by
  simp only [w2, w2s, w1]; ifs_omega

/-! FALSE variants of Lemma 8, refuted.
(F8a) (a) without the rule-L hypothesis: `l = 0, α = 1, β = 0`, all flags 0: gain 1, needed 2. -/
example : ¬ (∀ l al ce be ey en : Nat, ce ≤ 1 → ey ≤ 1 → en ≤ 1 →
    2 + pi (min al (be + 1)) (flagNe al (be + 1)) en
      ≤ w2 (l + al) ce (l + 1 + be) ey - w2 l en (l + al) ce) := by
  intro h; have := h 0 1 0 0 0 0 (by omega) (by omega) (by omega); revert this; decide
/-- (F8b) (b) with the carry `π ≡ 0`, i.e. "every interior vertex gains 3": false at
`l = o = e = α = ce = 0`, `en = 1` (gain 2). -/
example : ¬ (∀ l o e al ce en : Nat, e ≤ 1 → ce ≤ 1 → en ≤ 1 →
    3 ≤ w2 (l + al) ce (l + 2 + o) e - w2 l en (l + al) ce) := by
  intro h; have := h 0 0 0 0 0 1 (by omega) (by omega) (by omega); revert this; decide
/-- (F8c) (d) without the rule-L hypothesis: joining a leaf `y` to `x` with one out-neighbour
that is a leaf makes a cherry, weight 0 → −1. -/
example : ¬ (∀ be ey zh ze : Nat, ey ≤ 1 → ze ≤ 1 → 0 ≤ w2 zh ze be ey - w1 zh ze) := by
  intro h; have := h 0 0 0 0 (by omega) (by omega); revert this; decide

/-! ### telescoping

A PROFILE of the part of the path after `w_{i−1}`: the list `rest = [(α_i, ε(c_i)), …,
(α_{k−1}, ε(c_{k−1}))]` and the end `z : Option (height × flag)` (the out-neighbour of `w_k`,
if any).  `h(w_j) = ` number of entries of `rest` after `w_j`'s own. -/

/-- Flag (before the round) of the end vertex `w_k`. -/
def endFlag : Option (Nat × Nat) → Nat
  | none => 0
  | some _ => 1

/-- Flag (before the round) of the first vertex of the remaining path. -/
def headFlag : List (Nat × Nat) → Option (Nat × Nat) → Nat
  | [], z => endFlag z
  | c :: _, _ => flagNe c.1 0

/-- Sum of the OLD weights of the vertices of the remaining path. -/
def oldRest : List (Nat × Nat) → Option (Nat × Nat) → Int
  | [], none => 0
  | [], some z => w1 z.1 z.2
  | c :: rest, z => w2 rest.length (headFlag rest z) (rest.length + c.1) c.2 + oldRest rest z

/-- Sum of the NEW weights of the vertices of the remaining path, when the previous path
vertex has risen by `o` and has new flag `e`. -/
def newRest (o e : Nat) : List (Nat × Nat) → Option (Nat × Nat) → Int
  | [], none => w1 (1 + o) e
  | [], some z => w2 z.1 z.2 (1 + o) e
  | c :: rest, z =>
      w2 (rest.length + c.1) c.2 (rest.length + 2 + o) e
        + newRest (min c.1 (o + 2)) (flagNe c.1 (o + 2)) rest z

theorem rest_gain : ∀ (rest : List (Nat × Nat)) (z : Option (Nat × Nat)) (o e : Nat),
    e ≤ 1 → (∀ c, c ∈ rest → c.2 ≤ 1) → (∀ c, z = some c → c.2 ≤ 1) →
    3 * (rest.length : Int) + 1 - pi o e (headFlag rest z) ≤ newRest o e rest z - oldRest rest z
  | [], none, o, e, _, _, _ => by
    have := path_end_none o e
    simp only [newRest, oldRest, headFlag, endFlag, List.length_nil] at *; omega
  | [], some z, o, e, he, _, hz => by
    have := path_end_some o e z.1 z.2 he (hz z rfl)
    simp only [newRest, oldRest, headFlag, endFlag, List.length_nil] at *; omega
  | c :: rest, z, o, e, he, hr, hz => by
    have hf : headFlag rest z ≤ 1 := by
      cases rest with
      | nil => cases z <;> simp [headFlag, endFlag]
      | cons d _ => exact flagNe_le _ _
    have h1 := path_interior rest.length o e c.1 c.2 (headFlag rest z) he
      (hr c (List.mem_cons_self ..)) hf
    have h2 := rest_gain rest z (min c.1 (o + 2)) (flagNe c.1 (o + 2)) (flagNe_le _ _)
      (fun d hd => hr d (List.mem_cons_of_mem _ hd)) hz
    simp only [newRest, oldRest, headFlag, List.length_cons] at *
    omega

/-- LEMMA 8, `k ≥ 1`.  Profile of the whole path: `x = w₀` has the other out-neighbour `c₀`
(`al`, `ce`), `y` has height `k + be` and flag `ey`, the rest of the path is `rest`, `z`;
`k = rest.length + 1`.  Old weights: `w(x) + oldRest`; new: `w'(x) + newRest`.
The vertices of the path together gain at least `3k`. -/
theorem path_gain (al ce be ey : Nat) (rest : List (Nat × Nat)) (z : Option (Nat × Nat))
    (hce : ce ≤ 1) (hey : ey ≤ 1) (hr : ∀ c, c ∈ rest → c.2 ≤ 1) (hz : ∀ c, z = some c → c.2 ≤ 1)
    (hL : be = 0 → ey = 0 → al = 0) :
    3 * ((rest.length : Int) + 1)
      ≤ (w2 (rest.length + al) ce (rest.length + 1 + be) ey
          + newRest (min al (be + 1)) (flagNe al (be + 1)) rest z)
        - (w2 rest.length (headFlag rest z) (rest.length + al) ce + oldRest rest z) := by
  have hf : headFlag rest z ≤ 1 := by
    cases rest with
    | nil => cases z <;> simp [headFlag, endFlag]
    | cons d _ => exact flagNe_le _ _
  have h1 := path_start rest.length al ce be ey (headFlag rest z) hce hey hf hL
  have h2 := rest_gain rest z (min al (be + 1)) (flagNe al (be + 1)) (flagNe_le _ _) hr hz
  omega

/-- (F8d) FALSE variant of `path_gain`, refuted: without the rule-L hypothesis the path need
not pay.  `k = 1`: `x` has out-neighbours `w₁` (no out-neighbours) and `c₀` (height 1, flag 0),
`y` has height 1 and flag 0.  Old weights 1 + 0, new weights 2 + 1: gain 2 < 3. -/
example : ¬ (∀ (al ce be ey : Nat) (rest : List (Nat × Nat)) (z : Option (Nat × Nat)),
    ce ≤ 1 → ey ≤ 1 → (∀ c, c ∈ rest → c.2 ≤ 1) → (∀ c, z = some c → c.2 ≤ 1) →
    3 * ((rest.length : Int) + 1)
      ≤ (w2 (rest.length + al) ce (rest.length + 1 + be) ey
          + newRest (min al (be + 1)) (flagNe al (be + 1)) rest z)
        - (w2 rest.length (headFlag rest z) (rest.length + al) ce + oldRest rest z)) := by
  intro h
  have := h 1 0 0 0 [] none (by omega) (by omega) (by intro c hc; cases hc) (by intro c hc; cases hc)
  revert this; decide

/-!
WHAT THE GRAPH PART (U3) MUST SUPPLY to use `path_gain` (and `free_none`/`free_some` for
`k = 0`), for a round of rule L on a valid forest position, path `x = w₀, …, w_k`:
 1. (old side) for `i < k`: `h(w_{i+1}) = k − i − 1`; the out-neighbours of `w_i` are `w_{i+1}`
    and one other vertex `c_i` with `h(c_i) = h(w_{i+1}) + α_i`; the flag of `w_i` is
    `flagNe α_i 0`; `w_k` has no out-neighbour (flag 0) or exactly one, `z` (flag 1).
    Hence `Σ_i w(w_i) = w2 … + oldRest rest z`.
 2. (new side) `c_i`, `z`, `y` keep their height and flag; after the round the out-neighbours
    of `w₀` are `c₀, y`, of `w_i` are `c_i, w_{i−1}`, of `w_k` are `z` (if any) and `w_{k−1}`;
    hence `h'(w₀) = h(w₀) + min α₀ (β + 1)` with new flag `flagNe α₀ (β + 1)` and
    `h'(w_i) = h(w_i) + min α_i (o_{i−1} + 2)` with new flag `flagNe α_i (o_{i−1} + 2)`.
    Hence `Σ_i w'(w_i) = w2 … + newRest … rest z`.
 3. (rule L) `h(y) = k + β` with `β ≥ 0`, and if `β = 0` and the flag of `y` is 0 then `α₀ = 0`;
    for `k = 0`: if `x` has an out-neighbour then not (`h(y) = 0` and flag of `y` = 0).
 4. all flags are 0 or 1.
For the vertices off the path (U3): same out-neighbours, and `2h + ε` of every vertex does not
decrease; then `w1_mono` / `w2_mono` apply.
-/

end FO.Upper
