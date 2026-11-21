/-
Specification for experiment 17 (single-cut full-open card protocols).
Everything a reader has to trust, apart from Lean and Mathlib, is in this file and in the
statements of the four theorems at the end of `CutPeek/Main.lean`.
-/
import Mathlib.Data.ZMod.Basic

namespace CutPeek

/-- A row of `m` cards. For `m ≥ 1` the positions are the integers modulo `m` (for `m = 0`,
`ZMod 0` is the integers; every theorem assumes `m ≠ 0`). A card is a `Bool` (`true` = heart,
`false` = club). -/
abbrev Row (m : ℕ) := ZMod m → Bool

/-- `rot s w` is the row `w` rotated by `s` places: the card at position `p` moves to `p + s`. -/
def rot {m : ℕ} (s : ZMod m) (w : Row m) : Row m := fun p => w (p - s)

/-- Two rows are cyclically equal if one is a rotation of the other. After one random cut and
opening every card, an observer learns exactly the class of the row under this relation. -/
def CycEq {m : ℕ} (v w : Row m) : Prop := ∃ s : ZMod m, v = rot s w

/-- An encoding of `n` input bits on `m` cards. Position `p` is an additional card when
`owner p = none`: its card is `base p` for every input. Otherwise `owner p = some i` and the card
is controlled by bit `i`: it is `base p` when bit `i` is `false` and the other card when it is
`true`. So every card depends on at most one input bit. -/
structure Encoding (n m : ℕ) where
  owner : ZMod m → Option (Fin n)
  base : Row m

/-- The row laid down for the input `x`. -/
def Encoding.row {n m : ℕ} (e : Encoding n m) (x : Fin n → Bool) : Row m :=
  fun p => match e.owner p with
    | none => e.base p
    | some i => xor (e.base p) (x i)

/-- `e` is a single-cut full-open protocol for `f`: two inputs give cyclically equal rows
exactly when `f` takes the same value on them. -/
def IsSCFO {n m : ℕ} (e : Encoding n m) (f : (Fin n → Bool) → Bool) : Prop :=
  ∀ x y : Fin n → Bool, CycEq (e.row x) (e.row y) ↔ f x = f y

/-- The input that agrees with `z` except at the three positions `i 0`, `i 1`, `i 2`, where it
takes the values `x 0`, `x 1`, `x 2`. This description assumes `i` injective, as the theorem that
uses `patch` does; if two of the positions coincide, the later update wins. -/
def patch {n : ℕ} (z : Fin n → Bool) (i : Fin 3 → Fin n) (x : Fin 3 → Bool) : Fin n → Bool :=
  Function.update (Function.update (Function.update z (i 0) (x 0)) (i 1) (x 1)) (i 2) (x 2)

/-- The AND of `n` bits. -/
def andFn (n : ℕ) : (Fin n → Bool) → Bool := fun x => decide (∀ j, x j = true)

/-- Equality of `n` bits: `true` exactly when all bits are equal. -/
def eqFn (n : ℕ) : (Fin n → Bool) → Bool := fun x => decide (∀ j k, x j = x k)

end CutPeek
