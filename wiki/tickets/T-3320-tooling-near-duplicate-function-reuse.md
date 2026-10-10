---
id: T-3320
title: Tooling: near-duplicate function reuse
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[build-system]]", "[[overlays]]", "[[tickets/T-1300-reuse-c-across-identical-functions]]", "[[tickets/T-1321-register-promotion-build-step]]"]
---

## Goal

`tools/dupes.py` reuses C only for byte-identical functions (relocations masked). The overlays are scenes built from shared templates, so many functions have the same instruction shape and other constants (event ids, text ids, coordinates, counts). Build `tools/neardupes.py`: fingerprint with ALU immediates masked too, group by shape, and copy the matched member's C into unmatched members with each differing constant substituted by value, only when unambiguous.

## Acceptance criteria

- [x] `--report` lists the groups, how many unmatched members can be filled, and why the others cannot.
- [x] `--apply --check` writes the C into the per-object files, builds each object and reverts failures.
- [x] Unit tests on synthetic asm/C: `tools/test_neardupes.py`.
- [x] Documented in [[decompile-workflow]] and [[build-system]].
- [x] In this branch: report on the current tree, and `--apply --check` on 2-3 small overlays (not ETC or TACO) as proof. Bulk application waits for T-1321 to merge (it changes ETC/TACO C).

## Notes

Scope rule from the orchestrator: no edits to `include/`, `src/` (except what the proof applies and then reverts or reports), `tools/cc.py`, `tools/queue.py`, `config/*.yaml`.

## Comments

Inline review against CODING_STANDARDS (sections 7a, 8a, 11): the tool only writes C that the per-object build then verifies (no fakematch, no per-function switches); declarations come through dupes.py's 8a logic; Python 3, stdlib only, docstring and 19 unit tests. Review found unused code (a helper and two parameters), removed. Proof: `--apply --check` on EN_NICHI, KANGEI, SHUGAKU kept 7 of 7 (3408 bytes), progress 2734 -> 2741 functions; clean build 27/27 OK, headers OK. Report numbers: 178 near groups, 19 with a matched source, 66 functions / 34592 bytes fillable (59 / 31184 left after the proof), 59 groups (193 functions, 41972 bytes) without a matched member. Bulk application after T-1321 merges.
