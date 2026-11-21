/-
Restriction of an encoding to a 3-dimensional subcube.
-/
import CutPeek.Spec
import CutPeek.Rows

namespace CutPeek

variable {n m : ℕ}

theorem patch_i (z : Fin n → Bool) (i : Fin 3 → Fin n) (hi : Function.Injective i)
    (x : Fin 3 → Bool) (k : Fin 3) : patch z i x (i k) = x k := by
  have h01 : i 0 ≠ i 1 := fun h => by simpa using hi h
  have h02 : i 0 ≠ i 2 := fun h => by simpa using hi h
  have h12 : i 1 ≠ i 2 := fun h => by simpa using hi h
  match k with
  | 0 => simp [patch, h01, h02]
  | 1 => simp [patch, h12]
  | 2 => simp [patch]

theorem patch_other (z : Fin n → Bool) (i : Fin 3 → Fin n) (x : Fin 3 → Bool) (j : Fin n)
    (hj : ∀ k, j ≠ i k) : patch z i x j = z j := by
  simp [patch, hj 0, hj 1, hj 2]

theorem patch_const (b : Bool) (i : Fin 3 → Fin n) :
    patch (fun _ => b) i (fun _ => b) = fun _ => b := by
  funext j
  simp [patch, Function.update_apply]

/-- The 3-bit encoding with the same rows as `e` on the cube through `z` in directions `i`. -/
noncomputable def restrict (e : Encoding n m) (z : Fin n → Bool) (i : Fin 3 → Fin n) : Encoding 3 m where
  owner p := match e.owner p with
    | none => none
    | some j =>
      if j = i 0 then some 0 else if j = i 1 then some 1 else if j = i 2 then some 2 else none
  base p := match e.owner p with
    | none => e.base p
    | some j => if j = i 0 ∨ j = i 1 ∨ j = i 2 then e.base p else xor (e.base p) (z j)

theorem restrict_row (e : Encoding n m) (z : Fin n → Bool) (i : Fin 3 → Fin n)
    (hi : Function.Injective i) (x : Fin 3 → Bool) :
    (restrict e z i).row x = e.row (patch z i x) := by
  funext p
  simp only [Encoding.row, restrict]
  rcases e.owner p with _ | j
  · rfl
  · simp only
    by_cases h0 : j = i 0
    · subst h0; simp [patch_i z i hi]
    by_cases h1 : j = i 1
    · subst h1; simp [patch_i z i hi, h0]
    by_cases h2 : j = i 2
    · subst h2; simp [patch_i z i hi, h0, h1]
    · have hj : ∀ k, j ≠ i k := fun k => by
        match k with
        | 0 => exact h0
        | 1 => exact h1
        | 2 => exact h2
      simp [h0, h1, h2, patch_other z i x j hj]

end CutPeek
