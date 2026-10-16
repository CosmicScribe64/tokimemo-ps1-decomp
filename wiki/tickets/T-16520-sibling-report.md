---
id: T-16520
title: Matched-sibling report for work packages
status: Done   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee: claude-tooling
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16200-matching-wave-2]], [[decompile-workflow]]
---

## Goal

For every unmatched function (INCLUDE_ASM, guarded drafts included), find the closest matched functions anywhere in
the tree (all overlays and main; TT may be read) by register-erased opcode-sequence similarity, and cluster
unmatched functions that look alike. A matching agent then starts from a matched sibling's C instead of from nothing.
New tool `tools/siblings.py`; branch `t16520-siblings`. No file under `src/` is edited.

## Acceptance criteria

- [x] `tools/siblings.py` writes JSON (target -> top 3 siblings with ratio, size, C file, function) and a text summary by tier (>= 0.90, 0.80-0.90, 0.70-0.80) with counts and bytes, plus unmatched-unmatched clusters at ratio >= 0.90
- [x] `--package` writes a short note file per target: the sibling's C excerpt and both asm listings side by side
- [x] Whole tree in a few minutes (opcode n-gram index before difflib)
- [x] Unit tests in `tools/test_siblings.py`; full tool suite passes
- [x] Short section in `wiki/decompile-workflow.md`: what it does, how to run it, limits
- [x] Tier counts on the current tree recorded; 5 pairs spot-checked by eye

## Notes

Tool: `tools/siblings.py` (docstring has the usage), tests `tools/test_siblings.py` (45 tests), docs: section "Matched siblings for work packages" and step 3a in [[decompile-workflow]]. One refactor outside the new file: `dupes.asm_sources` (stale-file and matched/guarded rules of `load_funcs`, now shared; `load_funcs` returns the same list, `tools/test_dupes.py` has a test for it). No file under `src/` or `drafts/` touched.

Design decisions
- Score = difflib ratio of register/immediate/branch-offset/jump-target-erased opcode sequences (own `op_token`; `regdiff.key_shape` keeps branch offsets, jump targets and shift amounts, `neardupes`/`queue.compute_groups` are hash groups).
- Prefilter: opcode 4-gram index; a target probes its 256 rarest grams, the 800 best-sharing functions go on. Then the length bound and an exact bit-parallel LCS ratio (an upper bound of the difflib ratio; 0.5 s against 11.6 s for 11,863 candidate pairs) drop all but about 0.6 percent of the pairs before difflib runs. The LCS alone is not the score: 12 of its 70 pairs at >= 0.70 are scattered matches of generic code (difflib 0.30-0.65, largest block 11 of 199 instructions).
- First attempt (32 rarest grams, 40 candidates, difflib on every candidate) found only 52 of 79 siblings at >= 0.60 on 120 targets; the final setting finds the best sibling and tier for every target.
- Known weakness, by design of difflib: a repeating structure can be aligned with a shifted copy (test: 0.76 instead of 0.95); `tools/siblings.py` documents it. On the tree the effect is small at the top tiers (1 pair each at >= 0.90 and >= 0.80 in a 120-target sample).
- `build/siblings-cache.json` (size/mtime per asm file) makes later runs skip the reading; notes go to `build/siblings/` because they contain game asm.

Numbers on the tree (main `ae8a31e2` + w2-good state, 6,958 functions: 5,953 matched, 1,005 unmatched = 821,736 bytes, 128 of them guarded drafts)
- Time: cold (no cache) 30 s on a quiet machine, up to about 80 s under load; warm 2-25 s (the matching itself is about 15 s); `--package` on 12 targets 2.6 s.
- Best matched sibling per target:

| tier | functions | bytes | clones |
|---|---|---|---|
| >= 0.90 | 46 | 24,616 | 12 |
| 0.80-0.90 | 36 | 19,676 | 0 |
| 0.70-0.80 | 68 | 28,556 | 0 |
| < 0.70 or none | 855 | 748,888 | 0 |

  Without TT (`--skip-unit TT`, 954 targets, 783,036 bytes): 45 / 24,232, 36 / 19,676, 67 / 28,332, 806 / 710,796. 25 of the guarded drafts have a sibling >= 0.70. Clones (same opcodes and registers, only constants differ) are neardupes territory: DATE/func_8014AD9C, ETC/func_80144AC0, EVENT/func_800FF320, EVENT/func_80104C3C, GEKO/func_8013E22C, GYOZI/func_8013506C, GYOZI/func_8013E308, KANGEI/func_801370A0, OLH/func_80135B8C, TACO/func_801347F4, TACO/func_80147488, TAIIKU/func_801406A8 (follow-up: why neardupes does not fill them).
- Unmatched clusters at >= 0.90: 59 clusters, 139 functions, 102,340 bytes (no TT member); 42 of them have no matched sibling >= 0.70: matching one member of each opens 55 functions, 45,348 bytes. Largest: DATE/func_801462B0 + DATE2/func_80133AF0 + EVENT/func_8011C750 (11,088 bytes, weakest link 0.943), DATE/func_80147958 + EVENT/func_8011DDB8 (8,036 bytes).
- Index recall (`--check-recall 300`, all sizes): 50 of 50 targets with a sibling >= 0.70 get the same best sibling, tier and top-3 ratios as a brute-force search over all 5,953 matched functions; at >= 0.60, 94 of 94 best siblings, 93 of 94 top-3 lists.

Spot check by eye (notes written with `--package`, both listings read)
1. DATE/func_80150460 <- EVENT/func_800FC56C, 0.964: the same function with other globals: copies three globals to three others, adds 2 and subtracts 10 on two halfwords, then the same call sequence (28 identical lines, 53 differing only in constants or symbols, 0 in registers). Plausible; the strongest kind of sibling.
2. SHUGAKU/func_80139054 (guarded draft) <- SHUGAKU/func_8013A42C, 0.940: same switch over `D_800CA2CC` calling `func_8007ED84` and `func_800AE0F0` per case; the sibling also clears `D_8013CB04` (extra store). Plausible; the draft's structure is the sibling's.
3. OPTION/func_801342A8 <- OPTION/func_80136A50, 0.863: same `D_800E6280.unk_110D` state machine with the 3-record loop on `D_8011ECD0`; the `!` rows are register-allocation differences around the loop. Plausible.
4. TACO/func_80135638 <- KANGEI/func_801354AC, 0.748: both are a jump-table switch (`sltiu`, `sll 2`, `jr`) with one call per case; the target has an outer range guard and `func_80042908` instead of `func_80062CD0`. Same control flow only, as the tier says.
5. BUNKAKEN/func_80140F60 <- BUNKASAI/func_8013B190, 0.704: both read the `D_80122EB8` mode struct, store 2 to its second word and branch on its first; the target has more statements around it. Weakest tier, still the right family.

## Comments

### Code review, 2026-10-16 (against CODING_STANDARDS.md and the ticket)

Result: no blocking findings; 2 documentation fixes and 1 test added, all committed (`T-16520: review fixes ...`).

Checked
- Diff `w2-good...t16520-siblings`: only `tools/dupes.py`, `tools/siblings.py`, `tools/test_dupes.py`, `tools/test_siblings.py` and wiki files. Nothing under `src/` or `drafts/`; no asm, game data or binaries staged (`build/` and `asm/` are gitignored; the notes the tool writes go to `build/siblings/`).
- Tests in Docker: `tools/test_siblings.py` 45 OK (44 plus the new one), whole suite `unittest discover -s tools` 1062 tests OK (3 skipped; 1061 before the new test).
- `dupes.asm_sources` refactor: `load_funcs` old (w2-good) against new on the real tree, with and without `near`, and with `units=['TACO']`: same functions, same keys, paths, relocs and order (6,958 / 6,958 / 406).
- Tool run: whole tree (warm cache 19 s, `--no-cache` 28 s, JSON of both byte-identical); tier table, clone counts and cluster figures equal the ticket (46 / 24,616 / 12, 36 / 19,676, 68 / 28,556, 855 / 748,888; 59 clusters, 139 functions, 102,340 bytes, 42 without a sibling, 55 functions / 45,348 bytes). `--check-recall 300 --recall-max-insns 100000`: 50 of 50 best sibling and top 3 agree. `--package` on named functions, a `file:` entry, a bare name, a missing name (reported, exit 1 when nothing resolves) and the JSON of `queue.py --plan 2 --json`: notes and INDEX.md as documented. Spot-check numbers of pairs 1 to 5 reproduced (28 / 53 / 0 for pair 1, the note listings match the prose).
- Standards: Python 3, stdlib only, docstring with usage, paths as arguments, non-zero exit on `--check-recall` mismatch or an empty `--package`; tests use synthetic asm and C only; commits small with ticket id and the Co-Authored-By trailer; wiki: ticket, kanban, index and log consistent.

Findings fixed
1. The `--package` examples in the `tools/siblings.py` docstring and in `wiki/decompile-workflow.md` named `TACO/func_8013C0A8` (no such function in TACO) and `file:src/ovl/TACO.c` (the tree has `src/ovl/TACO/<address>.c`), so neither resolved. Replaced by `TACO/func_80135638`, `BUNKAKEN/func_80140F60`, `file:src/ovl/TACO/80134930.c` (also `file:TACO/80134930`, `file:80134930`), run and checked. New test `test_resolve_file_entries_in_the_per_object_layout` covers the real layout (the older tests used a flat `src/ovl/<NAME>.c`).
2. Docs: a cluster's `min_ratio` is the lowest ratio of any member pair found, not the weakest chain link; `INDEX.md` lists the last `--package` run only and older notes stay in the folder.

Not blocking, left as is
- Branch base is an older `w2-good` (7f081d79); a trial merge into the current `w2-good` (9a960bdf) conflicts only in `wiki/log.md` (both sides append): keep both entries.
- `--out-dir` accepts any path; notes contain game asm, so it should stay under `build/` (documented).
- `--unit` and `--skip-unit` are ignored with `--package` (documented in `--help`).

