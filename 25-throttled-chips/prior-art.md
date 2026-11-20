# Prior-art report

**Nothing found that states this game, the constant 5, C(h) = 5h-10, or the small values. Only the potential is standard.**

## CLOSEST PRIOR WORK

1. **Disks, balls, and walls: analysis of a combinatorial game** — Anderson, Lovász, Shor, Spencer, Tardos, Winograd, 1989. Primary not obtained; read via Kelley's survey, https://math.mit.edu/~apost/courses/18.204-2016/18.204_Anne_Kelley_final_paper.pdf . Same firing rule on Z ("we fire only a single pile and from this pile we move only a single pair of chips"), with the sum-of-squares potential ("for every move that is made, the sum of the squared distances increases by two"). Differs: all 2n+1 chips start at the origin, no chips arrive during play, no height threshold, no opponent.
2. **Laplacian growth, sandpiles and scaling limits** — Levine, Peres, 2016, https://arxiv.org/pdf/1611.00411 . States the quadratic weight Q = sum (σ_k(x) − σ_0(x))|x|², "Q increases by h every time we topple mass h". Differs: used for stabilisation, with unlimited toppling.
3. **Toppling numbers of complete and random graphs** — Bonato, Kinnersley, Prałat, 2013, https://arxiv.org/pdf/1305.1677 . Two players "alternate turns; on each turn, a player adds one chip"; "after each turn … vertices are fired until a stable configuration arises". Differs: both players add, firing is automatic and unlimited, finite graphs, no height bound.
4. **Buffer Size for Routing Limited-Rate Adversarial Traffic** — Miller, Patt-Shamir, 2017, https://arxiv.org/pdf/1707.03856 . Adversarial injections on a line/tree, centralized forwarding; "the number of packets in v's buffer never exceeds σ + 2c". Differs: packets move one way to a sink and leave; every link may forward each round, not one move in total.
5. **The Buffer Minimization Problem for Scheduling Flow Jobs with Conflicts** — Haas, Schmitt, van Stee, 2025, https://arxiv.org/pdf/2511.19690 (cites Höhne–van Stee on paths). "The goal is to minimize the maximum load that is ever stored in a single buffer", online against offline. Differs: running a machine destroys load; nothing passes to neighbours.
6. **Chip-firing games on graphs** — Björner, Lovász, Shor, 1991. Known only as cited in item 3 (firing order does not matter); primary not obtained.

## VERDICTS

- **Q potential, +2 per firing** (base of Theorem 1 / P3): FOUND as a tool (items 1, 2).
- **C(h) ≤ 5h-5 / 5h-6 via the weights 9,8,8,5,5 on five vertices**: NOT FOUND as a statement; in method, a short application of the known potential.
- **L = 5; Theorem 2 (lag strategy, G(h) ≥ 5h − O(√h))**: NOT FOUND.
- **G(h) ≥ 2h-1, Lemmas C and Z**: NOT FOUND.
- **C(h) = 5h-10 for 4 ≤ h ≤ 14, G(3)=5, G(4)=10, G(5)=15, G = C from h=4, adaptive values 4 and 7**: NOT FOUND.

## SEQUENCES (OEIS queried directly)

- C: 1,2,7,10,15,20,25,30,35,40 and 2,7,10,15,20,25,30: no match.
- Other reading: 1,4,7,12,17,22,27,32 and without the leading 1: no match.
- G (windowed): 1,2,5,10,15,20 matches A135042 (a(n) = 5(n−1), a(0)=1, a(1)=2), A215098, A109666; none mentions chip-firing.

## QUERIES (21, paraphrased)

ALSSTW title (2); chip-firing on the line, one firing per step, height bound; adder versus firer; toppling number on paths; Cranston–West toppling game; 1D sandpile, one toppling per time step; sandpile finite driving rate; packet routing on a line, buffer size; throttled chip-firing, online arrivals; Spencer balancing chip game; dynamic load balancing on a path; two-player chip-firing on Z; Björner–Lovász–Shor quadratic potential; committed sequence, one firing between additions; buffer minimisation with conflicts (2); odometer second moment on Z; sandpile game height "5h"; OEIS (1 web, 5 direct).

## WHO TO ASK

A chip-firing / abelian-sandpile combinatorialist; failing that, an adversarial-queueing theorist.

## NOT CHECKED

- ALSSTW 1989 and Björner–Lovász–Shor 1991 primaries were not obtained; a remark there on chips arriving during play would be missed.
- Höhne–van Stee 2021 and the physics "finite driving rate" papers: snippets only.
- MathSciNet / zbMATH not searched.
- Sources read in full: 0. Five downloads were read in the relevant sections (Kelley sections 1–3; abstract, model and main theorem of the others).
