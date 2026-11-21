import CutPeek.Spec
import CutPeek.Odd

namespace CutPeek

/-- Lemma 1. Three input bits, any row length `m ≥ 1`, any encoding in which each card depends on
at most one bit. If the seven rows other than the one at `c` are cyclically equal to one another,
then the row at `c` is cyclically equal to them too. -/
theorem seven_force_eighth {m : ℕ} [NeZero m] (e : Encoding 3 m) (c : Fin 3 → Bool)
    (h : ∀ x, x ≠ c → ∀ y, y ≠ c → CycEq (e.row x) (e.row y)) :
    ∀ x, x ≠ c → CycEq (e.row c) (e.row x) :=
  seven_force_eighth_aux e c h

/-- Theorem 2. If some three variables `i 0, i 1, i 2` (distinct) and some values `z` of the other
variables give a cube on which `f` takes, at seven of the eight corners, a value different from
its value at the eighth corner `c`, then `f` has no single-cut full-open protocol on any number
`m ≥ 1` of cards. -/
theorem no_scfo_of_odd_corner {n m : ℕ} [NeZero m] (f : (Fin n → Bool) → Bool)
    (i : Fin 3 → Fin n) (hi : Function.Injective i) (z : Fin n → Bool) (c : Fin 3 → Bool)
    (h : ∀ x : Fin 3 → Bool, x ≠ c → f (patch z i x) ≠ f (patch z i c)) :
    ¬ ∃ e : Encoding n m, IsSCFO e f :=
  no_scfo_of_odd_corner_aux f i hi z c h

/-- No single-cut full-open protocol for the AND of three or more bits. -/
theorem no_scfo_and {n m : ℕ} [NeZero m] (hn : 3 ≤ n) :
    ¬ ∃ e : Encoding n m, IsSCFO e (andFn n) :=
  no_scfo_and_aux hn

/-- No single-cut full-open protocol for the equality of four or more bits. -/
theorem no_scfo_eq {n m : ℕ} [NeZero m] (hn : 4 ≤ n) :
    ¬ ∃ e : Encoding n m, IsSCFO e (eqFn n) :=
  no_scfo_eq_aux hn

end CutPeek
