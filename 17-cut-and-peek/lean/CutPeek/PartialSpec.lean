/-
Specification for opening only some of the cards (Section 4 of the note), for the AND function.
Written before the proofs. Everything proved in `PartialMain.lean` is a statement about these
definitions and those of `CutPeek/Spec.lean`.
-/
import CutPeek.Spec
import Mathlib.Data.ZMod.Basic
import Mathlib.Data.Fintype.Basic
import Mathlib.Data.Multiset.Basic

namespace CutPeek

/-- What an observer sees when the row `w` is cut by `j` places and only the cards at the
positions in `R` are opened: the cards of `rot j w` at the positions of `R`. -/
def seen {m : ℕ} (R : Finset (ZMod m)) (w : Row m) (j : ZMod m) : R → Bool :=
  fun p => rot j w p.1

/-- The multiset of the `m` words seen at `R` as the cut runs over all `m` shifts. A uniformly
random cut shows a uniformly random element of this multiset. -/
def views {m : ℕ} [NeZero m] (R : Finset (ZMod m)) (w : Row m) : Multiset (R → Bool) :=
  (Finset.univ : Finset (ZMod m)).val.map (seen R w)

/-- `e` with opened set `R` is a valid single-cut partial-open protocol for the AND of its `n`
bits: `R` is not empty; any two inputs whose AND is `false` give the same multiset of views
(privacy); and no view of the all-`true` input is a view of the all-`false` input
(correctness, in the form used in the note). -/
def IsPartialAnd {n m : ℕ} [NeZero m] (e : Encoding n m) (R : Finset (ZMod m)) : Prop :=
  R.Nonempty ∧
  (∀ x y : Fin n → Bool, andFn n x = false → andFn n y = false →
      views R (e.row x) = views R (e.row y)) ∧
  (∀ w, w ∈ views R (e.row fun _ => true) → w ∉ views R (e.row fun _ => false))

end CutPeek
