# T-20820: keep lines with a necessity proof

Measurements behind `wiki/keep-lines.md`, produced by `tools/keepproof.py` on the tree at the branch point (`w2-good`). Reports only: no game data, no assembly, no C drafts.

- `results.txt`: the census summary and candidate list, the three `check --from-draft` runs for TACO `func_8014616C`, `func_80149538` and `func_801484E4`, and the `rows` table for the four controls and six candidates.
- `census.json`: every function whose original asm has the counter-tail shape (371 of 6,958): unit, name, matched or not, size, tags of the matched C, and the hits (address, positions, uses, held).

Reproduce (Docker, after `configure.py` and a build):

    tools/docker.sh python3 tools/keepproof.py census --json census.json
    tools/docker.sh python3 tools/keepproof.py check TACO:func_8014616C --symbol D_800E7384 --from-draft
    tools/docker.sh python3 tools/keepproof.py rows --controls

An archived result is not a match. The proofs hold for the draft text that `--from-draft` builds from the guarded drafts of `src/`; nothing was committed to `src/`.
