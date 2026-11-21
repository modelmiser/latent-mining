/-
Reduction of `IsPartialAnd` for three bits to finite checks, and the certificate (one
permutation of the 36 shifts per input) used for the 36-card layout.
-/
import CutPeek.PartialSpec
import Mathlib.Data.Fintype.EquivFin

namespace CutPeek

/-- The input with bits `a`, `b`, `c`. -/
def bits (a b c : Bool) : Fin 3 → Bool :=
  fun i => if i.val = 0 then a else if i.val = 1 then b else c

theorem eq_bits (x : Fin 3 → Bool) : x = bits (x 0) (x 1) (x 2) := by
  funext i
  match i with
  | 0 => rfl
  | 1 => rfl
  | 2 => rfl

variable {m : ℕ} [NeZero m]

/-- If a permutation `σ` of the shifts carries the views of `w'` to those of `w`, the two
multisets of views are equal. -/
theorem views_eq_of_perm (R : Finset (ZMod m)) (w w' : Row m) (σ : ZMod m → ZMod m)
    (hσ : Function.Injective σ) (h : ∀ j : ZMod m, ∀ p ∈ R, w (p - σ j) = w' (p - j)) :
    views R w = views R w' := by
  let E : ZMod m ≃ ZMod m := Equiv.ofBijective σ ⟨hσ, Finite.injective_iff_surjective.mp hσ⟩
  have hf : seen R w' = seen R w ∘ E := by
    funext j p
    exact (h j p.1 p.2).symm
  unfold views
  rw [hf, ← Multiset.map_map, Multiset.map_univ_val_equiv]

/-- `IsPartialAnd` for three bits from finitely many checks: for each input with AND `false` an
injective map of shifts matching its views with those of the all-`false` input, and for every
pair of shifts an opened position where the all-`true` and all-`false` rows differ. -/
theorem partial3_of_checks (e : Encoding 3 m) (R : Finset (ZMod m)) (hR : R.Nonempty)
    (L : Bool → Bool → Bool → ZMod m → ZMod m)
    (hinj : ∀ a b c, Function.Injective (L a b c))
    (hpriv : ∀ a b c, (a && b && c) = false → ∀ j : ZMod m, ∀ p ∈ R,
      e.row (bits a b c) (p - L a b c j) = e.row (fun _ => false) (p - j))
    (hcorr : ∀ j k : ZMod m, ∃ p ∈ R,
      e.row (fun _ => true) (p - j) ≠ e.row (fun _ => false) (p - k)) :
    IsPartialAnd e R := by
  refine ⟨hR, ?_, ?_⟩
  · have key : ∀ x, andFn 3 x = false →
        views R (e.row x) = views R (e.row fun _ => false) := by
      intro x hx
      obtain ⟨a, b, c, rfl⟩ : ∃ a b c, x = bits a b c := ⟨x 0, x 1, x 2, eq_bits x⟩
      have habc : (a && b && c) = false := by
        revert hx
        cases a <;> cases b <;> cases c <;> decide
      exact views_eq_of_perm R _ _ (L a b c) (hinj a b c) (hpriv a b c habc)
    intro x y hx hy
    rw [key x hx, key y hy]
  · intro w hw hw'
    obtain ⟨j, -, rfl⟩ := Multiset.mem_map.mp hw
    obtain ⟨k, -, hk⟩ := Multiset.mem_map.mp hw'
    obtain ⟨p, hp, hne⟩ := hcorr j k
    exact hne (congrFun hk ⟨p, hp⟩).symm

/-- For the input `(a, b, c)` of the 36-card layout, the shift whose view equals the view of the
all-`false` input at shift `j` is the `j`-th entry. Found by a search outside Lean; Lean checks
it. -/
def cert36 (a b c : Bool) : List ℕ :=
  match a, b, c with
  | false, false, true => [35, 8, 31, 16, 9, 6, 5, 14, 1, 22, 15, 12, 11, 20, 7, 28, 3, 18, 17, 26, 13, 34, 27, 24, 23, 32, 19, 4, 33, 30, 29, 2, 25, 10, 21, 0]
  | false, true, false => [3, 22, 32, 0, 5, 25, 9, 28, 2, 6, 11, 31, 15, 34, 8, 12, 17, 1, 21, 4, 14, 18, 23, 7, 27, 10, 20, 24, 29, 13, 33, 16, 26, 30, 35, 19]
  | false, true, true => [19, 32, 22, 35, 6, 9, 25, 2, 28, 5, 12, 15, 31, 8, 34, 11, 0, 21, 1, 14, 4, 17, 24, 27, 7, 20, 10, 23, 30, 33, 13, 26, 16, 29, 18, 3]
  | true, false, false => [28, 5, 12, 27, 8, 1, 34, 11, 18, 33, 14, 7, 4, 17, 24, 3, 2, 13, 10, 23, 30, 9, 26, 19, 16, 29, 0, 15, 32, 25, 22, 35, 6, 21, 20, 31]
  | true, false, true => [21, 18, 5, 28, 1, 8, 27, 24, 11, 34, 7, 14, 33, 30, 17, 4, 13, 20, 3, 0, 23, 10, 19, 26, 9, 6, 29, 16, 25, 32, 15, 12, 35, 22, 31, 2]
  | true, true, false => [14, 10, 35, 1, 15, 0, 20, 16, 5, 7, 3, 6, 26, 22, 11, 13, 9, 12, 32, 28, 17, 19, 33, 18, 2, 34, 23, 25, 21, 24, 8, 4, 29, 31, 27, 30]
  | _, _, _ => List.range 36

/-- `cert36` as a map of shifts. -/
def perm36 (a b c : Bool) : ZMod 36 → ZMod 36 :=
  fun j => ((cert36 a b c).getD j.val 0 : ℕ)

end CutPeek
