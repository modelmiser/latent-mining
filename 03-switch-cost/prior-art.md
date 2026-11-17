## Prior-art report

Method: 22 web searches. I read two sources in full text (the Lenstra–Shmoys book and the Dvořák et al. arXiv paper). Everything else is judged from search snippets or abstract listings only.

### Closest prior work

1. **Elements of Scheduling**, Lenstra & Shmoys (eds.), 2020, https://arxiv.org/pdf/2001.06005. Lemma 3.14(ii) shows that for 1|r_j|Lmax, Lmax(non-preemptive EDD) − L*max < p_b, where b is the "interference job"; the notes credit the rule to Schrage [1971] and the improved heuristic to Potts [1980]. It differs in that it is stated as an offline approximation bound against the non-preemptive optimum, though the proof only uses the set bound r(S)+p(S)+q(S) (Cor. 3.13).
2. **Parameterized Complexity of Scheduling Problems in Robotic Process Automation**, Dvořák, Novák, Šucha, Knop, Hanen, 2026, https://arxiv.org/pdf/2601.11984. It states L^EDD_max ≤ L^OPT_max + p_max − 1 on a single machine, improving the 2(p_max − 1) of Carlier & Pinson (2004, Discrete Applied Mathematics 145), in a passage that assumes r_i ≤ r_j implies d_i ≤ d_j. It differs in being offline, with no switch cost and no game.
3. **Online Flow Time Scheduling in the Presence of Preemption Overhead**, Chan, Lam, Li, APPROX 2012, https://unpaywall.org/10.1007%2F978-3-642-32512-0_8. It shows no online algorithm is competitive for total flow time once resuming a preempted job costs extra work, with a (1+ε)-speed, (1+1/ε)-competitive remedy. It differs in objective (total flow, not max lateness), has no feasibility promise, and I could not see whether its offline optimum pays the overhead.
4. **The Optimal Online Algorithms for Minimizing Maximum Lateness**, Uthaisombut, SWAT 2004 (Algorithmica 2008), https://rd.springer.com/chapter/10.1007/978-3-540-27810-8_36. It characterises all optimal online algorithms for preemptive max lateness (EDF and LLF generalised). It differs in that preemption is free, so the game value is 0.
5. **Online deadline scheduling with preemption penalties**, Zheng, Xu, Poon, Zhang, Wu, 2011, https://www.iaorifors.com/paper/70665. It gives competitive ratios in Δ (the length ratio) when each preemption costs ρ times the job's weight. It differs in being throughput/profit with a monetary penalty, not a time cost, and not lateness.
6. **Online scheduling with release times and set-ups**, Divakaran & Saks, 2001 (Algorithmica 2011), https://archive.dimacs.rutgers.edu/TechnicalReports/abstracts/2001/2001-50.html. It gives an O(1)-competitive algorithm for max flow time with setup times between job types. It differs in that the setup is per job family, not per preemption, and the result is a ratio, not an exact additive value.

Also seen: Jeffay–Stanat–Martel 1991 (https://techreports.cs.unc.edu/papers/90-019.pdf) and Baruah ECRTS 2005 (https://profiles.wustl.edu/en/publications/the-limited-preemption-uniprocessor-scheduling-of-sporadic-task-s/). Both are schedulability tests for sporadic tasks, not lateness games.

### Verdicts

- **Claim 1: EASY COROLLARY** of item 1's Lemma 3.14(ii). The lower bound used in its proof also holds for preemptive schedules, so Lmax(NEDD) < L_pmtn + p_b ≤ P on a preemptively feasible instance, and integrality gives P − 1. Item 2 prints the "p_max − 1" form explicitly, under an extra assumption. I did not find the statement phrased against the preemptive optimum verbatim. The Jeffay et al. paper is not the right citation for this.
- **Claim 2: NOT FOUND.** No paper found computes exact game values for max lateness with a one-tick preemption cost, or shows that non-preemption is optimal under such a cost. Chan–Lam–Li is the nearest in spirit.
- **Claim 3: NOT FOUND.** The upper bound F(P) ≤ P − 1 is claim 1; the matching lower bound and the S(P) version did not appear.
- **Claim 4: NOT FOUND.** I found nothing comparing an online algorithm against a same-cost versus a free-switching benchmark, and nothing on adversary horizon lengths.

Queries for claims 2–4 covered: preemption penalties (Zheng/Xu/Zhang, Fung-style lower bounds); context-switch and preemption overhead competitive lower bounds; online max lateness with an adversary; price/benefit of preemption; limited-preemption EDF and floating non-preemptive regions; setup/changeover online max flow time; polling systems; switching cost with an offline optimum that also pays; costly preemption flow time; and computer-aided game-tree lower bounds.

### Who to ask

An online-scheduling theorist in the competitive-analysis community (the Lam / Chan / Pruhs / Megow circle) for claims 2–4. A real-time systems person working on limited-preemption EDF (Baruah, Bertogna, Buttazzo) to confirm that claim 1 is folklore there.

### Not checked

- Full texts of Chan–Lam–Li, Uthaisombut, Carlier 1982, Carlier–Pinson 2004, Zheng et al., Divakaran–Saks and Jeffay et al. (Springer and HAL blocked the fetches).
- "Lower bounds on online deadline scheduling with preemption penalties" (https://portal.mardi4nfdi.de/wiki/Publication:975450): title only.
- An ESA 2026 paper that surfaced in a costly-preemption search (https://drops.dagstuhl.de/storage/00lipics/lipics-vol388-esa2026/LIPIcs.ESA.2026.128/LIPIcs.ESA.2026.128.pdf): not opened.
- Google Scholar citation trails, and any non-English or pre-1990 OR literature.

Absence here is weak evidence: the model (one idle tick per abandoned job, additive lateness, feasibility promise) is unusual enough that keyword search could miss a paper using different vocabulary.
