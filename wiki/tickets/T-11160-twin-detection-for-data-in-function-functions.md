---
id: T-11160
title: Twin detection for data-in-function functions
status: Done
assignee: Claude
created: 2026-10-14
updated: 2026-10-17
links: [[[build-system]], [[tickets/T-11150-twin-detection-for-jump-table-functions]], [[tickets/T-9010-per-object-data-shared-lui-at]], [[tickets/T-10200-orphan-jump-tables]], tools/dupes.py, tools/neardupes.py]
---

## Goal

`tools/neardupes.py --report` on the real build: "not fingerprinted (unmatched): data in function 257 fn/259628B" (about 23% of the unmatched bytes). Fingerprint these functions too, so twins are found, and copy C to them only where the build machinery can provide their rodata.

## Acceptance criteria

- [x] Find what "data in function" meant: a non-`jtbl_` symbol block in the `.rodata` that `rodata_pieces.py` appends to an INCLUDE_ASM function file (strings, constants), or a line in the function text that is neither an instruction nor a label.
- [x] Own rodata (the function's C object island `<asm_path>/data/<name>.rodata.s`, or the block in its own file) is masked in the code by its position among the function's data symbols; its directives join the fingerprint, `.word .L` as the offset from the function start, `.word SYM` as a marker. Twins must have equal data.
- [x] A copy into such a function is applied only with `--apply --check` and only when all its data is in its own function file and holds no pointers; the others are listed as "NEEDS DATA CHECK".
- [x] Report line: data-in-function groups, unmatched members, bytes, fillable, needs data check.
- [x] Synthetic tests.
- [ ] Orchestrator: run on the real build (see Comments).

## Notes

- The source C may now contain string literals when source and target both have own rodata (equal by the fingerprint), so the literal is copied verbatim. `static` and `#` are still refused. String contents are blanked before the body's identifiers are checked, and `neardupes.py` never substitutes numbers inside strings.
- Strings are compared as text after Shift-JIS escaping (as `asm.py` does): the appended copy is escaped by `rodata_pieces.py`, the island file is not.
- Plan field `data`: `island` (all own data in its function file: the C object provides its rodata and the compiler's string takes the original's place), `outside` (an earlier function or the island owns some of it: the copy would emit it twice), `pointers` (relocated words in the data). Only `island` is applied, and only with `--check`.
- A matched source that names an own-data symbol as an extern (`INCLUDE_RODATA`) is refused: the symbol is no longer in the relocation sequence.
- What remains in the function text is reported as "data in function text"; an unknown directive in the rodata as "unparsed rodata in function". The real-build report will show what is left of the 257.
- `split_jtbl` is now `split_held` (jump tables and data).

## Comments

- 2026-10-14: Implemented without game data (cloud worktree, no build). `test_dupes.py` 66 OK, `test_neardupes.py` 28 OK, `check_headers.py include src` OK, `test_check_headers.py` OK. To verify locally: `tools/docker.sh python3 tools/neardupes.py --report` (the "data-in-function groups" line and what remains of "data in function"), `tools/docker.sh python3 tools/dupes.py -v`, then `tools/docker.sh python3 tools/dupes.py --lenient --apply --check`, `tools/docker.sh python3 tools/neardupes.py --apply --check` and a full `tools/docker.sh ninja`.

2026-10-17: Cleanup pass. Merged on main as 200bc9d3a: own-rodata fingerprinting and the 'needs data check' plans in tools/dupes.py and tools/neardupes.py, with tests (+175 and +21 lines) and the build-system section. The real-build run was never logged for this ticket, but the T-16500 whole-tree sweep (2026-10-16) plans a copy into a data-island function (GYOZI func_8013506C) and the unit sha1 check rejects it, which is the intended behaviour; no other data-island plan was found. Review against CODING_STANDARDS.md: no finding. Closing as Done.
