/-
Theorem 2 (odd corner) and its two corollaries (AND, equality).
-/
import CutPeek.Spec
import CutPeek.Cube

namespace CutPeek

variable {n m : ℕ}

theorem no_scfo_of_odd_corner_aux [NeZero m] (f : (Fin n → Bool) → Bool)
    (i : Fin 3 → Fin n) (hi : Function.Injective i) (z : Fin n → Bool) (c : Fin 3 → Bool)
    (h : ∀ x : Fin 3 → Bool, x ≠ c → f (patch z i x) ≠ f (patch z i c)) :
    ¬ ∃ e : Encoding n m, IsSCFO e f := by
  rintro ⟨e, he⟩
  have hrow := restrict_row e z i hi
  have h7 : ∀ x, x ≠ c → ∀ y, y ≠ c →
      CycEq ((restrict e z i).row x) ((restrict e z i).row y) := by
    intro x hx y hy
    rw [hrow, hrow, he]
    have a := h x hx
    have b := h y hy
    generalize f (patch z i x) = fx at a ⊢
    generalize f (patch z i y) = fy at b ⊢
    generalize f (patch z i c) = fc at a b
    cases fx <;> cases fy <;> cases fc <;> simp at a b ⊢
  have hx0 : (fun j => !c j) ≠ c := fun h => by have := congrFun h 0; simp at this
  have key := seven_force_eighth_aux (restrict e z i) c h7 _ hx0
  rw [hrow, hrow, he] at key
  exact h _ hx0 key.symm

theorem no_scfo_and_aux [NeZero m] (hn : 3 ≤ n) :
    ¬ ∃ e : Encoding n m, IsSCFO e (andFn n) := by
  apply no_scfo_of_odd_corner_aux (andFn n) (Fin.castLE hn) (Fin.castLE_injective hn)
    (fun _ => true) (fun _ => true)
  intro x hx
  have hc : andFn n (patch (fun _ => true) (Fin.castLE hn) (fun _ => true)) = true := by
    rw [patch_const]; simp [andFn]
  have hx' : andFn n (patch (fun _ => true) (Fin.castLE hn) x) = false := by
    obtain ⟨k, hk⟩ : ∃ k, x k = false := by
      by_contra hcon
      apply hx
      funext k
      simpa using fun hk => hcon ⟨k, hk⟩
    simp only [andFn, decide_eq_false_iff_not]
    intro hall
    have := hall (Fin.castLE hn k)
    rw [patch_i _ _ (Fin.castLE_injective hn)] at this
    simp [hk] at this
  rw [hc, hx']; simp

theorem no_scfo_eq_aux [NeZero m] (hn : 4 ≤ n) :
    ¬ ∃ e : Encoding n m, IsSCFO e (eqFn n) := by
  have h3 : 3 ≤ n := by omega
  apply no_scfo_of_odd_corner_aux (eqFn n) (Fin.castLE h3) (Fin.castLE_injective h3)
    (fun _ => false) (fun _ => false)
  intro x hx
  have hc : eqFn n (patch (fun _ => false) (Fin.castLE h3) (fun _ => false)) = true := by
    rw [patch_const]; simp [eqFn]
  have hx' : eqFn n (patch (fun _ => false) (Fin.castLE h3) x) = false := by
    obtain ⟨k, hk⟩ : ∃ k, x k = true := by
      by_contra hcon
      apply hx
      funext k
      simpa using fun hk => hcon ⟨k, hk⟩
    simp only [eqFn, decide_eq_false_iff_not]
    intro hall
    have := hall (Fin.castLE h3 k) ⟨3, by omega⟩
    rw [patch_i _ _ (Fin.castLE_injective h3), patch_other _ _ _ _ (fun k' hk' => by
      have := congrArg Fin.val hk'
      simp at this
      omega)] at this
    simp [hk] at this
  rw [hc, hx']; simp

end CutPeek
