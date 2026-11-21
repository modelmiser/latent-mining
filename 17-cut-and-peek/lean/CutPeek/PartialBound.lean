/-
Proof of Proposition 3 of the note: the card bound `2^n - 1 ≤ m` for partial opening.
Cards are signs (`true` = -1); `corr R S w` is the note's `C_S`.
-/
import CutPeek.PartialSpec
import Mathlib.Algebra.BigOperators.Ring.Finset
import Mathlib.Algebra.BigOperators.Fin
import Mathlib.Data.Fintype.Powerset
import Mathlib.Data.Fintype.BigOperators
import Mathlib.Algebra.Ring.Parity
import Mathlib.Tactic.Ring
import Mathlib.Tactic.Push

namespace CutPeek

/-- A card as a sign. -/
def sg (b : Bool) : ℤ := if b then -1 else 1

theorem sg_xor (a b : Bool) : sg (xor a b) = sg a * sg b := by cases a <;> cases b <;> rfl

variable {n m : ℕ} [NeZero m]

/-- The note's `C_S(x)`, for `S` inside `R`, as a function of the row. -/
def corr (R : Finset (ZMod m)) (S : Finset R) (w : Row m) : ℤ :=
  ∑ j : ZMod m, ∏ p ∈ S, sg (w (p.1 - j))

theorem corr_eq_views (R : Finset (ZMod m)) (S : Finset R) (w : Row m) :
    corr R S w = ((views R w).map fun t => ∏ p ∈ S, sg (t p)).sum := by
  unfold corr views
  rw [Multiset.map_map]
  rfl

theorem corr_congr {R : Finset (ZMod m)} (S : Finset R) {w w' : Row m}
    (h : views R w = views R w') : corr R S w = corr R S w' := by
  rw [corr_eq_views, corr_eq_views, h]

omit [NeZero m] in
theorem sum_subsets (R : Finset (ZMod m)) (t u : R → Bool) :
    ∑ S : Finset R, ∏ p ∈ S, sg (t p) * sg (u p) = if t = u then (2 : ℤ) ^ R.card else 0 := by
  have h := Finset.prod_one_add (f := fun p : R => sg (t p) * sg (u p)) Finset.univ
  rw [Finset.powerset_univ] at h
  rw [← h]
  split_ifs with htu
  · subst htu
    have : ∀ p : R, (1 : ℤ) + sg (t p) * sg (t p) = 2 := fun p => by
      cases t p <;> rfl
    simp only [this, Finset.prod_const, Finset.card_univ, Fintype.card_coe]
  · obtain ⟨p, hp⟩ := Function.ne_iff.mp htu
    refine Finset.prod_eq_zero (Finset.mem_univ p) ?_
    revert hp
    cases t p <;> cases u p <;> simp [sg]

theorem count_views (R : Finset (ZMod m)) (w : Row m) (t : R → Bool) :
    (2 : ℤ) ^ R.card * ((views R w).count t : ℤ)
      = ∑ S : Finset R, (∏ p ∈ S, sg (t p)) * corr R S w := by
  have hc : ((views R w).count t : ℤ) = ∑ j : ZMod m, if t = seen R w j then 1 else 0 := by
    unfold views
    rw [Multiset.count_map]
    rw [show Multiset.card (Multiset.filter (fun a => t = seen R w a) Finset.univ.val)
        = (Finset.univ.filter (fun a => t = seen R w a)).card from rfl, Finset.card_filter]
    push_cast
    rfl
  rw [hc, Finset.mul_sum]
  have h2 : ∀ j : ZMod m, (2 : ℤ) ^ R.card * (if t = seen R w j then 1 else 0)
      = ∑ S : Finset R, (∏ p ∈ S, sg (t p)) * ∏ p ∈ S, sg (w (p.1 - j)) := fun j => by
    rw [mul_ite, mul_one, mul_zero, ← sum_subsets R t (seen R w j)]
    refine Finset.sum_congr rfl fun S _ => ?_
    rw [Finset.prod_mul_distrib]
    rfl
  simp only [h2]
  rw [Finset.sum_comm]
  refine Finset.sum_congr rfl fun S _ => ?_
  rw [corr, Finset.mul_sum]

theorem views_eq_of_corr {R : Finset (ZMod m)} {w w' : Row m}
    (h : ∀ S : Finset R, corr R S w = corr R S w') : views R w = views R w' := by
  refine Multiset.ext.mpr fun t => ?_
  have h1 := count_views R w t
  have h2 := count_views R w' t
  simp only [h] at h1
  rw [← h2] at h1
  have h3 := mul_left_cancel₀ (pow_ne_zero _ (two_ne_zero : (2 : ℤ) ≠ 0)) h1
  exact_mod_cast h3

/-- The number of positions of `S`, shifted by `j`, that bit `i` controls. -/
def cnt (e : Encoding n m) (R : Finset (ZMod m)) (S : Finset R) (j : ZMod m) (i : Fin n) : ℕ :=
  (S.filter fun p => e.owner (p.1 - j) = some i).card

omit [NeZero m] in
theorem sg_row (e : Encoding n m) (x : Fin n → Bool) (q : ZMod m) :
    sg (e.row x q) = sg (e.base q) * ∏ i : Fin n, if e.owner q = some i then sg (x i) else 1 := by
  unfold Encoding.row
  cases e.owner q with
  | none => simp
  | some i => simp [sg_xor, Finset.prod_ite_eq]

omit [NeZero m] in
theorem prod_row (e : Encoding n m) (R : Finset (ZMod m)) (S : Finset R) (j : ZMod m)
    (x : Fin n → Bool) :
    ∏ p ∈ S, sg (e.row x (p.1 - j))
      = (∏ p ∈ S, sg (e.base (p.1 - j))) * ∏ i : Fin n, sg (x i) ^ cnt e R S j i := by
  simp only [sg_row, Finset.prod_mul_distrib]
  congr 1
  rw [Finset.prod_comm]
  refine Finset.prod_congr rfl fun i _ => ?_
  rw [Finset.prod_ite, Finset.prod_const, Finset.prod_const_one, mul_one]
  rfl

theorem sum_char (k : Fin n → ℕ) (v : Fin n → Bool) (h : ∃ i, decide (Odd (k i)) ≠ v i) :
    ∑ x : Fin n → Bool, (∏ i, sg (x i) ^ k i) * ∏ i, (if v i then sg (x i) else 1) = 0 := by
  simp only [← Finset.prod_mul_distrib]
  rw [← Fintype.prod_sum (fun i b => sg b ^ k i * if v i then sg b else 1)]
  obtain ⟨i, hi⟩ := h
  refine Finset.prod_eq_zero (Finset.mem_univ i) ?_
  rw [Fintype.sum_bool]
  by_cases hk : Odd (k i)
  · have hv : v i = false := by simpa [hk] using hi
    simp [sg, hv, hk.neg_one_pow]
  · have hv : v i = true := by simpa [hk] using hi
    simp [sg, hv, (Nat.not_odd_iff_even.mp hk).neg_one_pow]

theorem sum_psi (v : Fin n → Bool) (i0 : Fin n) (h : v i0 = true) :
    ∑ x : Fin n → Bool, ∏ i, (if v i then sg (x i) else 1) = 0 := by
  have := sum_char (fun _ => 0) v ⟨i0, by simp [h]⟩
  simpa using this

theorem exists_shift (e : Encoding n m) (R : Finset (ZMod m)) (h : IsPartialAnd e R) :
    ∃ S : Finset R, ∀ v : Fin n → Bool, v ≠ (fun _ => false) →
      ∃ j : ZMod m, (fun i => decide (Odd (cnt e R S j i))) = v := by
  obtain ⟨-, hpriv, hcorr⟩ := h
  have hex : ∃ S, corr R S (e.row fun _ => true) ≠ corr R S (e.row fun _ => false) := by
    by_contra hc
    push Not at hc
    have hv := views_eq_of_corr hc
    have hm : seen R (e.row fun _ => true) 0 ∈ views R (e.row fun _ => true) :=
      Multiset.mem_map_of_mem _ (Finset.mem_univ_val 0)
    exact hcorr _ hm (hv ▸ hm)
  obtain ⟨S, hS⟩ := hex
  refine ⟨S, fun v hv => ?_⟩
  by_contra hno
  push Not at hno
  obtain ⟨i0, hi0⟩ : ∃ i, v i = true := by
    by_contra hh
    push Not at hh
    exact hv (funext fun i => by simpa using hh i)
  have h0 : andFn n (fun _ => false) = false := by
    simp only [andFn, decide_eq_false_iff_not]
    exact fun hall => Bool.false_ne_true (hall i0)
  have hc : ∀ x : Fin n → Bool, x ≠ (fun _ => true) →
      corr R S (e.row x) = corr R S (e.row fun _ => false) := fun x hx => by
    refine corr_congr S (hpriv x _ ?_ h0)
    simp only [andFn, decide_eq_false_iff_not]
    exact fun hall => hx (funext hall)
  have A : ∑ x : Fin n → Bool,
      corr R S (e.row x) * ∏ i, (if v i then sg (x i) else 1) = 0 := by
    unfold corr
    simp only [Finset.sum_mul]
    rw [Finset.sum_comm]
    refine Finset.sum_eq_zero fun j _ => ?_
    simp only [prod_row, mul_assoc]
    rw [← Finset.mul_sum, sum_char _ v (Function.ne_iff.mp (hno j)), mul_zero]
  have B : ∑ x : Fin n → Bool, corr R S (e.row x) * ∏ i, (if v i then sg (x i) else 1)
      = (corr R S (e.row fun _ => true) - corr R S (e.row fun _ => false))
        * ∏ i : Fin n, (if v i then sg true else 1) := by
    have hx : ∀ x : Fin n → Bool, corr R S (e.row x) * ∏ i, (if v i then sg (x i) else 1)
        = corr R S (e.row fun _ => false) * ∏ i, (if v i then sg (x i) else 1)
          + if x = (fun _ => true) then
              (corr R S (e.row fun _ => true) - corr R S (e.row fun _ => false))
                * ∏ i : Fin n, (if v i then sg true else 1) else 0 := fun x => by
      split_ifs with hxt
      · subst hxt; ring
      · rw [hc x hxt, add_zero]
    simp only [hx]
    rw [Finset.sum_add_distrib, ← Finset.mul_sum, sum_psi v i0 hi0, mul_zero, zero_add,
      Finset.sum_ite_eq' Finset.univ (fun _ => true), if_pos (Finset.mem_univ _)]
  rw [A] at B
  rcases mul_eq_zero.mp B.symm with hz | hz
  · exact hS (sub_eq_zero.mp hz)
  · rw [Finset.prod_eq_zero_iff] at hz
    obtain ⟨i, -, hi⟩ := hz
    revert hi
    cases v i <;> simp [sg]

theorem partial_and_card_bound_aux (e : Encoding n m) (R : Finset (ZMod m))
    (h : IsPartialAnd e R) : 2 ^ n - 1 ≤ m := by
  obtain ⟨S, hS⟩ := exists_shift e R h
  have hsub : (Finset.univ.erase (fun _ => false) : Finset (Fin n → Bool))
      ⊆ Finset.univ.image fun j : ZMod m => fun i => decide (Odd (cnt e R S j i)) := fun v hv => by
    obtain ⟨j, hj⟩ := hS v (Finset.ne_of_mem_erase hv)
    exact Finset.mem_image.mpr ⟨j, Finset.mem_univ _, hj⟩
  have hle := (Finset.card_le_card hsub).trans Finset.card_image_le
  rw [Finset.card_erase_of_mem (Finset.mem_univ _), Finset.card_univ, Finset.card_univ,
    ZMod.card] at hle
  simpa using hle

end CutPeek
