---
id: T-1330
title: Tooling: m2c context and decomp-permuter
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[toolchain]]", "[[matching-notes]]"]
---

## Goal
Two helpers that make hard functions cheaper: `tools/m2c.py` (m2c with project context and IDO flags, ready-to-paste C draft) and `tools/permute.py` (decomp-permuter set up with our exact IDO 5.3 pipeline and the original bytes).

## Acceptance criteria
- [x] `tools/m2c.py` runs m2c on one function with a context built from `include/`, the right target and the function's rodata/jtbl; draft quality compared against plain m2c on ~10 matched functions; result in the wiki.
- [x] decomp-permuter in the Docker image at a pinned commit; `tools/permute.py` sets up a permuter dir for one function (compile pipeline = `tools/cc.py`) and runs it with a time limit; proven on 2-3 near-miss functions.
- [x] Both documented in `wiki/decompile-workflow.md` and `wiki/toolchain.md`; unit tests for the setup logic.
- [x] Clean build with the new image: 27/27 OK.
- [x] Inline code review against CODING_STANDARDS.md.

## Notes
Permuter results must be real C; a result that needs a trick is a fakematch and gets a `FAKE` comment (CODING_STANDARDS section 7). Register-promotion cases (T-0018) are out of reach for the permuter.

## Comments

- 2026-10-09: `tools/m2c.py`, `tools/permute.py`, tests `tools/test_m2c.py` / `tools/test_permute.py` (9 + 9 pass). Dockerfile: decomp-permuter 8556c81 + toml 0.10.2 + Levenshtein 0.27.1. Clean build with `IMG=tokimemo-decomp-m2c`: 27/27 sha1 OK. Results (m2c comparison, permuter on `strSync`, `func_80044700`, `func_8004111C`, `LoadSquare`) in [[matching-notes]]. Two verified matches (`strSync`, `func_80044700`) are not applied here; they are left for T-0950.

- Inline code review (CODING_STANDARDS section 13), 2026-10-09: checked scripts (Python 3, Docker-run, docstrings, nonzero exit via `sys.exit`, pinned versions in the Dockerfile), no game data staged (`build/permute/`, `build/scratch/` are gitignored), no `src/` or `include/` change committed, permuter output rule (FAKE marker) stated in the docs, tests pass. Findings: (1) the first permuter runs ignored stack offsets and produced a false score 0 (`int pad;`), fixed by making `--stack-diffs` the default; (2) a debug file pair written by `permuter.py --debug` into the repo root was deleted before commit. No open findings.
