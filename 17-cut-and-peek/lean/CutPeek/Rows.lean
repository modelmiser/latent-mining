/-
From the Fourier core to Lemma 1 on Boolean rows.
-/
import CutPeek.Spec
import CutPeek.Fourier

namespace CutPeek

variable {m : ℕ}

/-- A card as a complex sign. -/
noncomputable def sgn (b : Bool) : ℂ := if b then 1 else -1

theorem sgn_injective : Function.Injective sgn := by
  intro a b h
  cases a <;> cases b <;> simp [sgn] at h ⊢
  · have h2 : (2 : ℂ) = 0 := by linear_combination -h
    norm_num at h2
  · have h2 : (2 : ℂ) = 0 := by linear_combination h
    norm_num at h2

theorem CycEq.symm' {v w : Row m} (h : CycEq v w) : CycEq w v := by
  obtain ⟨s, rfl⟩ := h
  exact ⟨-s, by funext p; simp [rot]⟩

theorem CycEq.trans' {u v w : Row m} (h1 : CycEq u v) (h2 : CycEq v w) : CycEq u w := by
  obtain ⟨s, rfl⟩ := h1
  obtain ⟨t, rfl⟩ := h2
  exact ⟨s + t, by funext p; simp [rot, sub_sub]⟩

/-- The four pointwise sign identities between the rows of a 3-bit encoding: the three pair
equations and the affine formula for the corner where all three bits are flipped. -/
theorem row_identities (e : Encoding 3 m) (c0 c1 c2 : Bool) (p : ZMod m) :
    (sgn (e.row ![c0, !c1, !c2] p) + sgn (e.row ![!c0, c1, !c2] p)
        - sgn (e.row ![!c0, !c1, !c2] p) = sgn (e.row ![c0, c1, !c2] p)) ∧
    (sgn (e.row ![c0, !c1, !c2] p) + sgn (e.row ![!c0, !c1, c2] p)
        - sgn (e.row ![!c0, !c1, !c2] p) = sgn (e.row ![c0, !c1, c2] p)) ∧
    (sgn (e.row ![!c0, c1, !c2] p) + sgn (e.row ![!c0, !c1, c2] p)
        - sgn (e.row ![!c0, !c1, !c2] p) = sgn (e.row ![!c0, c1, c2] p)) ∧
    (sgn (e.row ![c0, !c1, !c2] p) + sgn (e.row ![!c0, c1, !c2] p)
        + sgn (e.row ![!c0, !c1, c2] p) - 2 * sgn (e.row ![!c0, !c1, !c2] p)
        = sgn (e.row ![c0, c1, c2] p)) := by
  simp only [Encoding.row]
  generalize e.base p = β
  generalize e.owner p = o
  rcases o with _ | j
  · cases β <;> simp [sgn] <;> norm_num
  · fin_cases j <;> cases β <;> cases c0 <;> cases c1 <;> cases c2 <;>
      simp [sgn] <;> norm_num

/-- Lemma 1 with the odd corner written out in coordinates. -/
theorem seven_aux [NeZero m] (e : Encoding 3 m) (c0 c1 c2 : Bool)
    (h : ∀ x, x ≠ ![c0, c1, c2] → ∀ y, y ≠ ![c0, c1, c2] → CycEq (e.row x) (e.row y)) :
    CycEq (e.row ![c0, c1, c2]) (e.row ![!c0, !c1, !c2]) := by
  have hb : (![!c0, !c1, !c2] : Fin 3 → Bool) ≠ ![c0, c1, c2] :=
    fun h => by have := congrFun h 0; simp at this
  have n1 : (![c0, !c1, !c2] : Fin 3 → Bool) ≠ ![c0, c1, c2] :=
    fun h => by have := congrFun h 1; simp at this
  have n2 : (![!c0, c1, !c2] : Fin 3 → Bool) ≠ ![c0, c1, c2] :=
    fun h => by have := congrFun h 0; simp at this
  have n3 : (![!c0, !c1, c2] : Fin 3 → Bool) ≠ ![c0, c1, c2] :=
    fun h => by have := congrFun h 0; simp at this
  have n12 : (![c0, c1, !c2] : Fin 3 → Bool) ≠ ![c0, c1, c2] :=
    fun h => by have := congrFun h 2; simp at this
  have n13 : (![c0, !c1, c2] : Fin 3 → Bool) ≠ ![c0, c1, c2] :=
    fun h => by have := congrFun h 1; simp at this
  have n23 : (![!c0, c1, c2] : Fin 3 → Bool) ≠ ![c0, c1, c2] :=
    fun h => by have := congrFun h 0; simp at this
  obtain ⟨s1, e1⟩ := h _ n1 _ hb
  obtain ⟨s2, e2⟩ := h _ n2 _ hb
  obtain ⟨s3, e3⟩ := h _ n3 _ hb
  obtain ⟨s12, e12⟩ := h _ n12 _ hb
  obtain ⟨s13, e13⟩ := h _ n13 _ hb
  obtain ⟨s23, e23⟩ := h _ n23 _ hb
  have hc := core (fun p => sgn (e.row ![!c0, !c1, !c2] p)) s1 s2 s3 s12 s13 s23
    (fun p => by
      have t := (row_identities e c0 c1 c2 p).1
      rw [e1, e2, e12] at t
      exact t)
    (fun p => by
      have t := (row_identities e c0 c1 c2 p).2.1
      rw [e1, e3, e13] at t
      exact t)
    (fun p => by
      have t := (row_identities e c0 c1 c2 p).2.2.1
      rw [e2, e3, e23] at t
      exact t)
  refine ⟨s12 + s13 + s23 - s1 - s2 - s3, funext fun p => sgn_injective ?_⟩
  have t := (row_identities e c0 c1 c2 p).2.2.2
  rw [e1, e2, e3] at t
  exact t.symm.trans (hc p)

/-- Lemma 1, for use in `CutPeek/Main.lean`. -/
theorem seven_force_eighth_aux [NeZero m] (e : Encoding 3 m) (c : Fin 3 → Bool)
    (h : ∀ x, x ≠ c → ∀ y, y ≠ c → CycEq (e.row x) (e.row y)) :
    ∀ x, x ≠ c → CycEq (e.row c) (e.row x) := by
  obtain ⟨c0, c1, c2, rfl⟩ : ∃ c0 c1 c2, c = ![c0, c1, c2] :=
    ⟨c 0, c 1, c 2, by funext j; fin_cases j <;> rfl⟩
  intro x hx
  have hb : (![!c0, !c1, !c2] : Fin 3 → Bool) ≠ ![c0, c1, c2] :=
    fun h => by have := congrFun h 0; simp at this
  exact (seven_aux e c0 c1 c2 h).trans' (h _ hb x hx)

end CutPeek
