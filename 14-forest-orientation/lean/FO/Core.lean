/-
M1. ABSTRACT CORE of Theorem 1 (open-limit/proofs.txt, section 4).
Self-contained, core Lean 4.  No graph theory: a tree is represented by its assigned
index `i` (index `2k` = type `A_k`, index `2k+1` = type `R_k`).

Everything is scaled by 3 to stay in `Nat`:  `F3 i = 3 * F(i)`, i.e.
`F3 (2k) = 3k + 1`  (F(A_k) = k + 1/3)   and   `F3 (2k+1) = 3k + 2`  (F(R_k) = k + 2/3).
-/
namespace FO.Core

/-- Three times the potential `F` of the paper. -/
def F3 (i : Nat) : Nat := 3 * (i / 2) + 1 + i % 2

/-- Conclusion of the Type Lemma for a merge by rule `M(i)` that costs `c` and produces a
tree of index `j`:  `j ∈ {i, i+1}` and `cost + F(j) ≥ 2 F(i)`   (the inequality (*)). -/
def TL (i j c : Nat) : Prop := (j = i ∨ j = i + 1) ∧ 2 * F3 i ≤ 3 * c + F3 j

/-- Sum of `F3` over the trees. -/
def Phi : List Nat → Nat
  | [] => 0
  | i :: l => F3 i + Phi l

section Game
variable {σ μ : Type}
/- An abstract game.  `Legal s m`: `m` is a legal Adversary move in state `s`.
`Reply s m c s'`: Algorithm may answer `m` by going to `s'` and paying `c`. -/
variable (Legal : σ → μ → Prop) (Reply : σ → μ → Nat → σ → Prop)

/-- `Forces k s g`: in the next `k` rounds Adversary can force a total payment of at least `g`
(for `g = 0` there is nothing to force). -/
def Forces : Nat → σ → Nat → Prop
  | 0, _, g => g = 0
  | k + 1, s, g => g = 0 ∨ ∃ m, Legal s m ∧ ∀ c s', Reply s m c s' → Forces k s' (g - c)

theorem forces_zero : ∀ (k : Nat) (s : σ), Forces Legal Reply k s 0
  | 0, _ => rfl
  | _ + 1, _ => Or.inl rfl

theorem phi_perm {l l' : List Nat} (h : l.Perm l') : Phi l = Phi l' := by
  induction h with
  | nil => rfl
  | cons a _ ih => simp [Phi, ih]
  | swap a b l => simp [Phi]; omega
  | trans _ _ ih1 ih2 => exact ih1.trans ih2

/-- Strictly decreasing list (the trees left for the cleanup, largest index first). -/
def Desc (l : List Nat) : Prop := l.Pairwise (fun x y => y < x)

theorem desc_length : ∀ (l : List Nat) (a : Nat), Desc (a :: l) → l.length ≤ a := by
  intro l
  induction l with
  | nil => intro a _; exact Nat.zero_le _
  | cons b t ih =>
    intro a h
    have h1 := List.pairwise_cons.mp h
    have h2 := ih b h1.2
    have h3 : b < a := h1.1 b (List.mem_cons_self ..)
    simp only [List.length_cons]; omega

/-- Inserting one element into a strictly decreasing list: either an equal pair appears or the
result can be arranged strictly decreasing. -/
theorem insert_desc : ∀ (l : List Nat), Desc l → ∀ a : Nat,
    (∃ i rest, (a :: l).Perm (i :: i :: rest)) ∨ (∃ l', (a :: l).Perm l' ∧ Desc l') := by
  intro l
  induction l with
  | nil => intro _ a; exact Or.inr ⟨[a], List.Perm.refl _, List.pairwise_singleton _ _⟩
  | cons b t ih =>
    intro h a
    have hb := List.pairwise_cons.mp h
    rcases Nat.lt_trichotomy a b with hab | hab | hab
    · rcases ih hb.2 a with ⟨i, rest, hp⟩ | ⟨l', hp, hd⟩
      · refine Or.inl ⟨i, b :: rest, ?_⟩
        have h1 : (a :: b :: t).Perm (b :: a :: t) := List.Perm.swap b a t
        have h2 : (b :: a :: t).Perm (b :: i :: i :: rest) := hp.cons b
        have h3 : (b :: i :: i :: rest).Perm (i :: b :: i :: rest) := List.Perm.swap i b _
        have h4 : (i :: b :: i :: rest).Perm (i :: i :: b :: rest) := (List.Perm.swap i b _).cons i
        exact h1.trans (h2.trans (h3.trans h4))
      · refine Or.inr ⟨b :: l', (List.Perm.swap b a t).trans (hp.cons b), ?_⟩
        refine List.pairwise_cons.mpr ⟨?_, hd⟩
        intro x hx
        have hx' : x ∈ a :: t := hp.mem_iff.mpr hx
        rcases List.mem_cons.mp hx' with rfl | hx''
        · exact hab
        · exact hb.1 x hx''
    · subst hab; exact Or.inl ⟨a, t, List.Perm.refl _⟩
    · refine Or.inr ⟨a :: b :: t, List.Perm.refl _, List.pairwise_cons.mpr ⟨?_, h⟩⟩
      intro x hx
      rcases List.mem_cons.mp hx with rfl | hx'
      · exact hab
      · exact Nat.lt_trans (hb.1 x hx') hab

/-- Every list has two equal entries or is a permutation of a strictly decreasing list. -/
theorem dichotomy : ∀ l : List Nat,
    (∃ i rest, l.Perm (i :: i :: rest)) ∨ (∃ l', l.Perm l' ∧ Desc l') := by
  intro l
  induction l with
  | nil => exact Or.inr ⟨[], List.Perm.refl _, List.Pairwise.nil⟩
  | cons a t ih =>
    rcases ih with ⟨i, rest, hp⟩ | ⟨l', hp, hd⟩
    · refine Or.inl ⟨i, a :: rest, ?_⟩
      have h2 : (a :: t).Perm (a :: i :: i :: rest) := hp.cons a
      have h3 : (a :: i :: i :: rest).Perm (i :: a :: i :: rest) := List.Perm.swap i a _
      have h4 : (i :: a :: i :: rest).Perm (i :: i :: a :: rest) := (List.Perm.swap i a _).cons i
      exact h2.trans (h3.trans h4)
    · rcases insert_desc l' hd a with ⟨i, rest, hp'⟩ | ⟨l'', hp', hd'⟩
      · exact Or.inl ⟨i, rest, (hp.cons a).trans hp'⟩
      · exact Or.inr ⟨l'', (hp.cons a).trans hp', hd'⟩

/- What the graph-theoretic part must supply: a relation `Rep s l` ("the trees of state `s`
can be listed so that the t-th tree has a witness of index `l[t]`") with the properties
`hperm` (the order of the list is irrelevant) and `hmerge` (rule M(i) applied to a tree of
index `i` and a tree of index `i' ≥ i`, the latter downgraded to `i` by nesting of types:
there is a legal move such that EVERY reply costs `c` and yields one tree of index `j` in
place of the two, with `TL i j c`). -/
variable (Rep : σ → List Nat → Prop)

/-- CLEANUP phase: the indices are strictly decreasing `a > b > ...`; Adversary repeatedly
merges the two largest.  It forces `3 * paid ≥ Phi - F3 a - 2 * (number of merges)`.
(`paid` is an integer, so a goal `g` with `3 g ≤ bound + 2` is still forced: the `+ 2`.) -/
theorem cleanup
    (hmerge : ∀ s i i' rest, Rep s (i' :: i :: rest) → i ≤ i' →
      ∃ m, Legal s m ∧ ∀ c s', Reply s m c s' → ∃ j, TL i j c ∧ Rep s' (j :: rest)) :
    ∀ (rest : List Nat) (a : Nat) (s : σ), Desc (a :: rest) → Rep s (a :: rest) →
      ∀ g, 3 * g + F3 a + 2 * rest.length ≤ Phi (a :: rest) + 2 →
      Forces Legal Reply rest.length s g := by
  intro rest
  induction rest with
  | nil =>
    intro a s _ _ g hg
    simp only [Phi, List.length_nil] at hg
    show g = 0
    omega
  | cons b t ih =>
    intro a s hd hr g hg
    have hd1 := List.pairwise_cons.mp hd
    have hd2 := List.pairwise_cons.mp hd1.2
    have hba : b < a := hd1.1 b (List.mem_cons_self ..)
    obtain ⟨m, hm, hrep⟩ := hmerge s b a t hr (Nat.le_of_lt hba)
    refine Or.inr ⟨m, hm, ?_⟩
    intro c s' hc
    obtain ⟨j, ⟨hj, hin⟩, hr'⟩ := hrep c s' hc
    have hdj : Desc (j :: t) := by
      refine List.pairwise_cons.mpr ⟨?_, hd2.2⟩
      intro x hx
      have := hd2.1 x hx
      rcases hj with rfl | rfl <;> omega
    by_cases hgc : g ≤ c
    · rw [Nat.sub_eq_zero_of_le hgc]; exact forces_zero Legal Reply _ _
    apply ih j s' hdj hr'
    simp only [Phi, List.length_cons] at hg ⊢
    have hF : F3 j ≤ F3 b + 2 := by
      unfold F3; rcases hj with rfl | rfl <;> omega
    omega

/-- MAIN phase followed by CLEANUP, from any represented state with `m + 1` trees whose indices
never exceed `K`: Adversary forces `3 * paid ≥ Phi - F3 K - 2 K`. -/
theorem main_phase
    (hperm : ∀ s l l', Rep s l → l.Perm l' → Rep s l')
    (hmerge : ∀ s i i' rest, Rep s (i' :: i :: rest) → i ≤ i' →
      ∃ m, Legal s m ∧ ∀ c s', Reply s m c s' → ∃ j, TL i j c ∧ Rep s' (j :: rest))
    (K : Nat) (hK : ∀ s l, Rep s l → ∀ i, i ∈ l → i ≤ K) :
    ∀ (m : Nat) (s : σ) (l : List Nat), l.length = m + 1 → Rep s l →
      ∀ g, 3 * g + F3 K + 2 * K ≤ Phi l + 2 → Forces Legal Reply m s g := by
  intro m
  induction m with
  | zero =>
    intro s l hl _ g hg
    match l, hl with
    | [a], _ =>
      have ha : a ≤ K := hK s [a] ‹_› a (List.mem_cons_self ..)
      simp only [Phi] at hg
      show g = 0
      unfold F3 at hg; omega
  | succ m ih =>
    intro s l hl hr g hg
    rcases dichotomy l with ⟨i, rest, hp⟩ | ⟨l', hp, hd⟩
    · have hr' := hperm s l _ hr hp
      obtain ⟨mv, hmv, hrep⟩ := hmerge s i i rest hr' (Nat.le_refl i)
      refine Or.inr ⟨mv, hmv, ?_⟩
      intro c s' hc
      obtain ⟨j, ⟨_, hin⟩, hr''⟩ := hrep c s' hc
      have hlen : (j :: rest).length = m + 1 := by
        have := hp.length_eq
        simp only [List.length_cons] at this ⊢; omega
      by_cases hgc : g ≤ c
      · rw [Nat.sub_eq_zero_of_le hgc]; exact forces_zero Legal Reply _ _
      apply ih s' (j :: rest) hlen hr''
      have := phi_perm hp
      simp only [Phi] at this ⊢
      omega
    · have hr' := hperm s l _ hr hp
      match l', hp, hd, hr' with
      | [], hp, _, _ => have := hp.length_eq; simp only [List.length_nil] at this; omega
      | a :: rest, hp, hd, hr' =>
        have hlen : rest.length = m + 1 := by
          have := hp.length_eq
          simp only [List.length_cons] at this; omega
        have ha : a ≤ K := hK s _ hr' a (List.mem_cons_self ..)
        have hla := desc_length rest a hd
        have h := cleanup Legal Reply Rep hmerge rest a s hd hr' g (by
          have := phi_perm hp
          have hF : F3 a ≤ F3 K := by unfold F3; omega
          omega)
        rw [hlen] at h; exact h

/-- THEOREM 1, abstract form.  Hypotheses to be supplied by the graph-theoretic part:
* `hperm`, `hmerge`: see above (Type Lemma + nesting of types);
* `hsize`: SIZE BOUND, a tree with a witness of index `i` forces `2^(⌊i/2⌋+1) - 1 ≤ n`;
* `h0`: the start state is represented by `n` trees of index 0.
Conclusion: in `n - 1` rounds Adversary forces every `g` with `3 g + 7 ⌊log₂(n+1)⌋ ≤ n + 5`,
i.e. the total payment `p` satisfies `3 p + 7 ⌊log₂(n+1)⌋ ≥ n + 3`. -/
theorem core_bound
    (hperm : ∀ s l l', Rep s l → l.Perm l' → Rep s l')
    (hmerge : ∀ s i i' rest, Rep s (i' :: i :: rest) → i ≤ i' →
      ∃ m, Legal s m ∧ ∀ c s', Reply s m c s' → ∃ j, TL i j c ∧ Rep s' (j :: rest))
    (n : Nat) (hn : 1 ≤ n)
    (hsize : ∀ s l, Rep s l → ∀ i, i ∈ l → 2 ^ (i / 2 + 1) ≤ n + 1)
    (s₀ : σ) (h0 : Rep s₀ (List.replicate n 0))
    (g : Nat) (hg : 3 * g + 7 * Nat.log2 (n + 1) ≤ n + 5) :
    Forces Legal Reply (n - 1) s₀ g := by
  have hne : n + 1 ≠ 0 := by omega
  have hl1 : 1 ≤ Nat.log2 (n + 1) := (Nat.le_log2 hne).mpr (by simp; omega)
  have hK : ∀ s l, Rep s l → ∀ i, i ∈ l → i ≤ 2 * Nat.log2 (n + 1) - 1 := by
    intro s l hr i hi
    have := (Nat.le_log2 hne).mpr (hsize s l hr i hi)
    omega
  have hphi : ∀ k : Nat, Phi (List.replicate k 0) = k := by
    intro k
    induction k with
    | zero => rfl
    | succ k ih => simp only [List.replicate_succ, Phi, ih, F3]; omega
  apply main_phase Legal Reply Rep hperm hmerge _ hK (n - 1) s₀ (List.replicate n 0)
    (by simp; omega) h0
  rw [hphi]
  unfold F3
  omega

end Game
end FO.Core
