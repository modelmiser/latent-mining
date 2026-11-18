/-
SPEC.  Online orientation of a forest with out-degree at most 2.
This file contains ONLY definitions and the statement of the final theorem.
Core Lean 4, no Mathlib.  Every definition is a computable function on lists.
See SPEC.md for the sentence of the problem statement that each definition encodes.
-/
namespace FO

/-- A directed edge `(t, h)`: the edge `{t, h}` of the forest, directed from its tail `t`
to its head `h`.  The vertex set is `Fin n`. -/
abbrev Arc (n : Nat) := Fin n × Fin n

/-- A position `(G, D)`: one arc per edge of `G`.  The newest edge is the head of the list.
(That `G` is a forest is not a field: the game starts with no edges and every round adds
an edge between two different components, see `moves`.) -/
abbrev Pos (n : Nat) := List (Arc n)

/-- Number of edges of which `v` is the tail. -/
def outdeg {n : Nat} (D : Pos n) (v : Fin n) : Nat := D.countP (fun a => a.1 == v)

/-- `D` is valid: every vertex is the tail of at most 2 edges. -/
def valid {n : Nat} (D : Pos n) : Bool := (List.finRange n).all (fun v => outdeg D v ≤ 2)

/-- `conn D a b`: `a` and `b` are in the same connected component of the undirected graph.
Recursion on the edge list: `a`,`b` are connected in `G + uv` iff they are connected in `G`,
or `a` is connected to one end of `uv` and the other end to `b`.
(`FO.conn_iff` in SpecCheck.lean proves this equal to the reflexive-transitive closure of
adjacency.) -/
def conn {n : Nat} : Pos n → Fin n → Fin n → Bool
  | [], a, b => a == b
  | (u, v) :: D, a, b =>
      conn D a b || (conn D a u && conn D v b) || (conn D a v && conn D u b)

/-- Adversary's legal moves: all pairs `(u, v)` of vertices in different components. -/
def moves {n : Nat} (D : Pos n) : List (Fin n × Fin n) :=
  (List.finRange n).flatMap fun u =>
    ((List.finRange n).filter fun v => !conn D u v).map fun v => (u, v)

/-- All ways to choose a direction for every edge of `D` (each arc kept or reversed, in place). -/
def reorient {n : Nat} : Pos n → List (Pos n)
  | [] => [[]]
  | (a, b) :: D => (reorient D).flatMap fun D' => [(a, b) :: D', (b, a) :: D']

/-- Algorithm's legal replies to the move `(u, v)`: every valid assignment of directions to
the edges of `G + uv`.  The new edge is the head of the list. -/
def replies {n : Nat} (D : Pos n) (u v : Fin n) : List (Pos n) :=
  (reorient ((u, v) :: D)).filter valid

/-- Number of places where two edge-aligned lists carry different arcs. -/
def flips {n : Nat} : Pos n → Pos n → Nat
  | a :: D, a' :: D' => (if a = a' then 0 else 1) + flips D D'
  | _, _ => 0

/-- Payment for the reply `D'` (a list for `G + uv`, new edge first) from the position `D`:
the number of OLD edges whose direction changed. -/
def cost {n : Nat} (D D' : Pos n) : Nat := flips D D'.tail

/-- Maximum of a list (0 for the empty list). -/
def maxL : List Nat → Nat
  | [] => 0
  | a :: l => max a (maxL l)

/-- Minimum of a list (0 for the empty list). -/
def minL : List Nat → Nat
  | [] => 0
  | [a] => a
  | a :: l => min a (minL l)

/-- Value of the game with `k` rounds left: Adversary maximises over its legal moves,
Algorithm minimises over its legal replies, payment of the round plus value of the rest. -/
def value {n : Nat} : Nat → Pos n → Nat
  | 0, _ => 0
  | k + 1, D =>
      maxL ((moves D).map fun m =>
        minL ((replies D m.1 m.2).map fun D' => cost D D' + value k D'))

/-- `T n`: the value of the `n - 1` round game from the position without edges. -/
def T (n : Nat) : Nat := value (n - 1) ([] : Pos n)

/-- The statement of the final theorem, in integers:
    `n + 3 ≤ 3 T(n) + 7 ⌊log₂(n+1)⌋`.
It implies the real inequality `T(n) ≥ n/3 - (7/3) log₂(n+1) + 5/6`:
divide by 3 to get `T(n) ≥ n/3 + 1 - (7/3)⌊log₂(n+1)⌋`, and use `⌊log₂(n+1)⌋ ≤ log₂(n+1)`
and `1 ≥ 5/6`.  Nothing is lost; the integer form is slightly stronger.
(`Nat.log2 m` is `⌊log₂ m⌋` for `m ≠ 0`: `Nat.le_log2 : n ≠ 0 → (k ≤ n.log2 ↔ 2 ^ k ≤ n)`.) -/
def MainStatement : Prop :=
  ∀ n : Nat, 1 ≤ n → n + 3 ≤ 3 * T n + 7 * Nat.log2 (n + 1)

end FO
