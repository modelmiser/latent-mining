# latent-mining

Experiments in getting a language model to produce a small result it could not
recall: a question is invented by a chain of model instances, then solved and
attacked by other instances that share no context.

Everything here was produced by instances of one model family (Claude). Read
each experiment's status table before relying on anything: "proved by hand"
means a model wrote the proof and other instances of the same model checked it.

## Method

1. **Seed.** One hard, open-ended question.
2. **Chain.** Three instances in turn. Each sees only the previous question and
   must hand on a successor that is fully formal, has small cases a program can
   settle exactly, and has a general form whose answer is a formula or theorem.
   The last link reduces it to one headline question.
3. **Solve.** Two instances work independently: exact computation on small
   cases, then conjectures, then proofs, each tagged computed / proved / guessed.
4. **Attack.** A referee instance reads the proofs line by line, re-implements
   the computation from the definition, and runs checks that are able to fail.
5. **Prior art.** A separate instance searches the literature.
6. **Follow-up.** Open ends go to further instances, and short proofs into Lean.

The checkability rule in step 2 is what makes the rest possible. Without it the
chain produces prose that nothing can test.

## Experiments

| # | Folder | Subject | Headline | Status |
|---|---|---|---|---|
| 1 | [01-late-thermostat](01-late-thermostat/) | regulating a level from a delayed one-bit signal | a blind clock is within 1 of a controller with full memory | headline proved by hand for every delay, not machine-checked; one supporting bound is in Lean; the exact clock value for odd delays is open past 13 |
| 2 | [02-move-on-free](02-move-on-free/) | a memory allocator that may move blocks only when something is freed | how much that right is worth | open. Proved by hand: an allocator that packs its first blocks together cannot use the right to get below 7/4 of live memory asymptotically, the best lower bound known with no moves at all; with only the smallest blocks freed the cost is exactly 5/3 asymptotically. Nothing machine-checked |

## License

MIT, copyright ModelMiser. See [LICENSE](LICENSE).
