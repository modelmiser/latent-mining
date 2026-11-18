/-
Graph-theoretic foundation: components, forests built edge by edge, and the two facts about
orientations of a forest that the paper uses silently:
  * `dag`      : if `x → o` is an arc, `x` is not reachable from `o` by a directed path;
  * `disjoint` : the sets reachable from two different out-neighbours of `x` are disjoint.
Both come from `nb`: two different neighbours of `x` are not connected in `G - x`.
-/
import FO.Spec
namespace FO

/-- Reflexive-transitive closure. -/
inductive RT {α : Type} (r : α → α → Prop) : α → α → Prop
  | refl (a : α) : RT r a a
  | step {a b c : α} : r a b → RT r b c → RT r a c

namespace RT
variable {α : Type} {r r' : α → α → Prop} {a b c : α}

theorem trans (h1 : RT r a b) (h2 : RT r b c) : RT r a c := by
  induction h1 with
  | refl => exact h2
  | step hab _ ih => exact .step hab (ih h2)

theorem single (h : r a b) : RT r a b := .step h (.refl _)

theorem tail (h1 : RT r a b) (h2 : r b c) : RT r a c := h1.trans (single h2)

theorem mono (hm : ∀ a b, r a b → r' a b) (h : RT r a b) : RT r' a b := by
  induction h with
  | refl => exact .refl _
  | step hab _ ih => exact .step (hm _ _ hab) ih

theorem symm (hs : ∀ a b, r a b → r b a) (h : RT r a b) : RT r b a := by
  induction h with
  | refl => exact .refl _
  | step hab _ ih => exact ih.tail (hs _ _ hab)

theorem preserve {Q : α → Prop} (hstep : ∀ a b, Q a → r a b → Q b) (h : RT r a b) :
    Q a → Q b := by
  induction h with
  | refl => exact id
  | step hab _ ih => exact fun hq => ih (hstep _ _ hq hab)

/-- Adding one undirected edge `uv` to a symmetric relation: a new connection crosses it at
most once. -/
theorem add (hs : ∀ a b, r a b → r b a) (u v : α)
    (h : RT (fun a b => r a b ∨ (a = u ∧ b = v) ∨ (a = v ∧ b = u)) a b) :
    RT r a b ∨ (RT r a u ∧ RT r v b) ∨ (RT r a v ∧ RT r u b) := by
  induction h with
  | refl a => exact Or.inl (.refl a)
  | step hab _ ih =>
    rcases hab with hab | ⟨h1, h2⟩ | ⟨h1, h2⟩
    · rcases ih with h | ⟨h1, h2⟩ | ⟨h1, h2⟩
      · exact Or.inl (.step hab h)
      · exact Or.inr (Or.inl ⟨.step hab h1, h2⟩)
      · exact Or.inr (Or.inr ⟨.step hab h1, h2⟩)
    · subst h1; subst h2
      rcases ih with h | ⟨h1, h2⟩ | ⟨_, h2⟩
      · exact Or.inr (Or.inl ⟨.refl _, h⟩)
      · exact Or.inl ((h1.symm hs).trans h2)
      · exact Or.inl h2
    · subst h1; subst h2
      rcases ih with h | ⟨_, h2⟩ | ⟨h1, h2⟩
      · exact Or.inr (Or.inr ⟨.refl _, h⟩)
      · exact Or.inl h2
      · exact Or.inl ((h1.symm hs).trans h2)
end RT

variable {n : Nat}

/-- `a` and `b` are joined by an edge. -/
def adj (D : Pos n) (a b : Fin n) : Prop := (a, b) ∈ D ∨ (b, a) ∈ D

/-- Same component: reflexive-transitive closure of adjacency. -/
def Conn (D : Pos n) : Fin n → Fin n → Prop := RT (adj D)

/-- Adjacent, and neither end is `x`. -/
def adjA (D : Pos n) (x a b : Fin n) : Prop := adj D a b ∧ a ≠ x ∧ b ≠ x

/-- Connected in the graph with the vertex `x` deleted. -/
def CA (D : Pos n) (x : Fin n) : Fin n → Fin n → Prop := RT (adjA D x)

theorem adj_symm {D : Pos n} {a b : Fin n} (h : adj D a b) : adj D b a := Or.symm h

theorem adjA_symm {D : Pos n} {x a b : Fin n} (h : adjA D x a b) : adjA D x b a :=
  ⟨adj_symm h.1, h.2.2, h.2.1⟩

theorem adj_cons {D : Pos n} {u v a b : Fin n} :
    adj ((u, v) :: D) a b ↔ adj D a b ∨ (a = u ∧ b = v) ∨ (a = v ∧ b = u) := by
  simp only [adj, List.mem_cons, Prod.mk.injEq]
  constructor
  · rintro ((h | h) | (h | h))
    · exact Or.inr (Or.inl h)
    · exact Or.inl (Or.inl h)
    · exact Or.inr (Or.inr ⟨h.2, h.1⟩)
    · exact Or.inl (Or.inr h)
  · rintro ((h | h) | h | h)
    · exact Or.inl (Or.inr h)
    · exact Or.inr (Or.inr h)
    · exact Or.inl (Or.inl h)
    · exact Or.inr (Or.inl ⟨h.2, h.1⟩)

theorem Conn.symm {D : Pos n} {a b : Fin n} (h : Conn D a b) : Conn D b a :=
  RT.symm (fun _ _ => adj_symm) h

theorem CA.symm {D : Pos n} {x a b : Fin n} (h : CA D x a b) : CA D x b a :=
  RT.symm (fun _ _ => adjA_symm) h

theorem CA.conn {D : Pos n} {x a b : Fin n} (h : CA D x a b) : Conn D a b :=
  RT.mono (fun _ _ h => h.1) h

theorem Conn_cons {D : Pos n} {u v a b : Fin n} :
    Conn ((u, v) :: D) a b ↔
      Conn D a b ∨ (Conn D a u ∧ Conn D v b) ∨ (Conn D a v ∧ Conn D u b) := by
  have up : ∀ {a b}, Conn D a b → Conn ((u, v) :: D) a b :=
    fun h => RT.mono (fun _ _ h => adj_cons.mpr (Or.inl h)) h
  have e1 : adj ((u, v) :: D) u v := adj_cons.mpr (Or.inr (Or.inl ⟨rfl, rfl⟩))
  constructor
  · intro h
    exact RT.add (fun _ _ => adj_symm) u v (RT.mono (fun _ _ h => adj_cons.mp h) h)
  · rintro (h | ⟨h1, h2⟩ | ⟨h1, h2⟩)
    · exact up h
    · exact (up h1).trans (RT.step e1 (up h2))
    · exact (up h1).trans (RT.step (adj_symm e1) (up h2))

/-- The computable `conn` of the Spec is the reflexive-transitive closure of adjacency. -/
theorem conn_iff : ∀ (D : Pos n) (a b : Fin n), conn D a b = true ↔ Conn D a b
  | [], a, b => by
    simp only [conn, beq_iff_eq]
    constructor
    · rintro rfl; exact RT.refl _
    · intro h
      cases h with
      | refl => rfl
      | step h _ => simp [adj] at h
  | (u, v) :: D, a, b => by
    simp only [conn, Bool.or_eq_true, Bool.and_eq_true, conn_iff D, Conn_cons, or_assoc]

theorem CA_cons_of {D : Pos n} {u v x a b : Fin n} (h : CA ((u, v) :: D) x a b) :
    CA D x a b ∨ (u ≠ x ∧ v ≠ x ∧
      ((CA D x a u ∧ CA D x v b) ∨ (CA D x a v ∧ CA D x u b))) := by
  by_cases hu : u = x
  · refine Or.inl (RT.mono ?_ h)
    rintro a b ⟨h1, h2, h3⟩
    rcases adj_cons.mp h1 with h | ⟨h4, _⟩ | ⟨_, h4⟩
    · exact ⟨h, h2, h3⟩
    · exact absurd (h4.trans hu) h2
    · exact absurd (h4.trans hu) h3
  by_cases hv : v = x
  · refine Or.inl (RT.mono ?_ h)
    rintro a b ⟨h1, h2, h3⟩
    rcases adj_cons.mp h1 with h | ⟨_, h4⟩ | ⟨h4, _⟩
    · exact ⟨h, h2, h3⟩
    · exact absurd (h4.trans hv) h3
    · exact absurd (h4.trans hv) h2
  have h' : RT (fun a b => adjA D x a b ∨ (a = u ∧ b = v) ∨ (a = v ∧ b = u)) a b := by
    refine RT.mono ?_ h
    rintro a b ⟨h1, h2, h3⟩
    rcases adj_cons.mp h1 with h | h | h
    · exact Or.inl ⟨h, h2, h3⟩
    · exact Or.inr (Or.inl h)
    · exact Or.inr (Or.inr h)
  rcases RT.add (fun _ _ => adjA_symm) u v h' with h | h | h
  · exact Or.inl h
  · exact Or.inr ⟨hu, hv, Or.inl h⟩
  · exact Or.inr ⟨hu, hv, Or.inr h⟩

/-- `D` was built edge by edge, every new edge joining two different components.
(The underlying graph is then a forest; this is the form in which positions of the game
arise.) -/
def Forest : Pos n → Prop
  | [] => True
  | (u, v) :: D => ¬ Conn D u v ∧ Forest D

theorem forest_noloop : ∀ (D : Pos n), Forest D → ∀ a, (a, a) ∉ D
  | [], _ => by intro a h; cases h
  | (u, v) :: D, hF => by
    intro a h
    rcases List.mem_cons.mp h with h | h
    · simp only [Prod.mk.injEq] at h
      obtain ⟨h1, h2⟩ := h
      subst h1; subst h2
      exact hF.1 (RT.refl _)
    · exact forest_noloop D hF.2 a h

theorem forest_antisymm : ∀ (D : Pos n), Forest D → ∀ a b, (a, b) ∈ D → (b, a) ∉ D
  | [], _ => by intro a b h; cases h
  | (u, v) :: D, hF => by
    intro a b h h'
    have hadj : ¬ adj D u v := fun h => hF.1 (RT.single h)
    rcases List.mem_cons.mp h with e | k
    · simp only [Prod.mk.injEq] at e
      obtain ⟨h1, h2⟩ := e
      subst h1; subst h2
      rcases List.mem_cons.mp h' with e' | k'
      · simp only [Prod.mk.injEq] at e'
        obtain ⟨h1, _⟩ := e'
        subst h1
        exact hF.1 (RT.refl _)
      · exact hadj (Or.inr k')
    · rcases List.mem_cons.mp h' with e' | k'
      · simp only [Prod.mk.injEq] at e'
        obtain ⟨h1, h2⟩ := e'
        subst h1; subst h2
        exact hadj (Or.inr k)
      · exact forest_antisymm D hF.2 a b k k'

theorem forest_nodup : ∀ (D : Pos n), Forest D → D.Nodup
  | [], _ => List.nodup_nil
  | _ :: D, hF =>
    List.nodup_cons.mpr ⟨fun h => hF.1 (RT.single (Or.inl h)), forest_nodup D hF.2⟩

/-- In a forest two different neighbours of `x` are not connected in `G - x`. -/
theorem nb : ∀ (D : Pos n), Forest D → ∀ x o o', adj D x o → adj D x o' → o ≠ o' →
    ¬ CA D x o o'
  | [], _ => by intro x o o' h; simp [adj] at h
  | (u, v) :: D, hF => by
    intro x o o' ho ho' hne hca
    obtain ⟨hnc, hF'⟩ := hF
    have ih := nb D hF'
    have cadj : ∀ {a b}, adj D a b → Conn D a b := fun h => RT.single h
    rw [adj_cons] at ho ho'
    rcases ho with ho | ⟨hxu, hov⟩ | ⟨hxv, hou⟩
    · rcases ho' with ho' | ⟨hxu, hov'⟩ | ⟨hxv, hou'⟩
      · rcases CA_cons_of hca with h | ⟨_, _, ⟨h1, h2⟩ | ⟨h1, h2⟩⟩
        · exact ih x o o' ho ho' hne h
        · exact hnc (h1.conn.symm.trans ((cadj ho).symm.trans ((cadj ho').trans h2.conn.symm)))
        · exact hnc (h2.conn.trans ((cadj ho').symm.trans ((cadj ho).trans h1.conn)))
      · subst hxu; subst hov'
        rcases CA_cons_of hca with h | ⟨hux, _, _⟩
        · exact hnc ((cadj ho).trans h.conn)
        · exact hux rfl
      · subst hxv; subst hou'
        rcases CA_cons_of hca with h | ⟨_, hvx, _⟩
        · exact hnc (Conn.symm ((cadj ho).trans h.conn))
        · exact hvx rfl
    · subst hxu; subst hov
      rcases CA_cons_of hca with h | ⟨hux, _, _⟩
      · rcases ho' with ho' | ⟨_, hov'⟩ | ⟨hxv, _⟩
        · exact hnc ((cadj ho').trans h.conn.symm)
        · exact hne hov'.symm
        · subst hxv; exact hnc (RT.refl _)
      · exact hux rfl
    · subst hxv; subst hou
      rcases CA_cons_of hca with h | ⟨_, hvx, _⟩
      · rcases ho' with ho' | ⟨hxu, _⟩ | ⟨_, hou'⟩
        · exact hnc (h.conn.trans (cadj ho').symm)
        · subst hxu; exact hnc (RT.refl _)
        · exact hne hou'.symm
      · exact hvx rfl

/-- Reachable by a directed path. -/
def Reach (D : Pos n) : Fin n → Fin n → Prop := RT (fun a b => (a, b) ∈ D)

theorem Reach.conn {D : Pos n} {a b : Fin n} (h : Reach D a b) : Conn D a b :=
  RT.mono (fun _ _ h => Or.inl h) h

/-- A directed path that leaves `x` by the arc `x → o` never comes back to `x` and stays in
the component of `o` in `G - x`. -/
theorem reach_avoid {D : Pos n} (hF : Forest D) {x o c : Fin n} (hxo : (x, o) ∈ D)
    (h : Reach D o c) : CA D x o c ∧ c ≠ x := by
  have hox : o ≠ x := fun e => forest_noloop D hF x (e ▸ hxo)
  refine RT.preserve (Q := fun c => CA D x o c ∧ c ≠ x) ?_ h ⟨RT.refl _, hox⟩
  rintro a b ⟨h1, h2⟩ hab
  have hbx : b ≠ x := by
    intro e
    subst e
    have hao : a ≠ o := by
      intro e; subst e
      exact forest_antisymm D hF _ _ hxo hab
    exact nb D hF b o a (Or.inl hxo) (Or.inr hab) (Ne.symm hao) h1
  exact ⟨h1.tail ⟨Or.inl hab, h2, hbx⟩, hbx⟩

theorem dag {D : Pos n} (hF : Forest D) {x o : Fin n} (hxo : (x, o) ∈ D) : ¬ Reach D o x :=
  fun h => (reach_avoid hF hxo h).2 rfl

theorem disjoint {D : Pos n} (hF : Forest D) {x o o' c : Fin n} (h1 : (x, o) ∈ D)
    (h2 : (x, o') ∈ D) (hne : o ≠ o') (hc : Reach D o c) : ¬ Reach D o' c := fun hc' =>
  nb D hF x o o' (Or.inl h1) (Or.inl h2) hne
    ((reach_avoid hF h1 hc).1.trans (reach_avoid hF h2 hc').1.symm)

/-- No directed cycle: nothing is reachable from itself by a path with at least one arc. -/
theorem no_cycle {D : Pos n} (hF : Forest D) {a b : Fin n} (hab : (a, b) ∈ D) :
    ¬ Reach D b a := dag hF hab

end FO
