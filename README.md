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
| 1 | [01-late-thermostat](01-late-thermostat/) | regulating a level from a delayed one-bit signal | a blind clock is within 1 of a controller with full memory | partly machine-checked, one part open |

## License

MIT, copyright ModelMiser. See [LICENSE](LICENSE).
