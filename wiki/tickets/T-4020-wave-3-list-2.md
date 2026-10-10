---
id: T-4020
title: Wave 3 list 2
status: Done
assignee: agent (w3-2)
created: 2026-10-09
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal

Match the 137 functions (25632 bytes) of wave 3 work list 2 in 25 files: 80042540, BUNKA_SD, DATE, ENDING, EVENT, GEKO, GYOZI, SHUGAKU, TACO, TT.

## Acceptance criteria

- [x] Work list processed in order, time-boxed per function
- [x] Clean rebuild (`rm -rf asm build`, configure, ninja): 27 of 27 sha1 OK and `build/headers.ok`
- [x] Inline review against CODING_STANDARDS.md recorded in Comments
- [x] T-0018 cases appended to `wiki/data/t0018-cases.md` (45 rows)

## Notes

Worktree w3-2, branch w3-2. Not merged or pushed. Patterns and tooling notes: [[matching-notes]], section "Wave 3, list 2 (T-4020)".

Result: 60 functions matched, 8988 bytes: 59 from the list (8924 of 25632 bytes) plus `func_8013C624` (DATE, 64 bytes, not on the list: twin of `func_8013934C`, found with `queue.py`). By file: DATE 80132000 (27), 80145AA0 (1), 801461B0 (1); SHUGAKU (7); EVENT (13); GEKO (1); GYOZI (2); ENDING (2); TACO (5); TT (1). Grand total 2805 -> 2865 of 6958 (`ninja progress`). 45 rows added to [[data/t0018-cases]]. Left as `INCLUDE_ASM`: the rest of the list; top non-T-0018 blocker: functions whose original uses `multu` with the factor 0x38/0x24 in a register, and `return const` where the original puts the last store in the `jr` delay slot.

## Comments

### Inline review (CODING_STANDARDS 13)
- [x] Matches verified: every function by `funcdiff.py --resolve` against the first clean build's objects, string and rodata users (`S` flag) by the overlay sha1; the final clean rebuild gives 27 of 27 `OK` and `build/headers.ok`.
- [x] No `NON_MATCHING`, no fakematch: nothing needed a dummy variable, forced register or unused local. Odd-looking but plain C, each with a source comment: the T-3330 index local before the table, `2U`/`1U` on one of two equal constants (keeps IDO from sharing the constant), the single table symbol in SHUGAKU `func_80135AF4`.
- [x] C89: declarations at block top, `/* */` comments only (grep for `//` over the diff: none), no inline/stdbool.
- [x] SDK headers and types used; no SDK struct redefined. Local structs (`FnTbl*`, `Blk44`) are game-specific and sit next to their only user.
- [x] Placeholders kept (`func_`/`D_` names); no renames. New externs are in `include/main_api.h` (main-exe symbols, with the type of the accesses) or `include/ovl/<NAME>.h` (overlay symbols); EVENT's own data at 0x8012xxxx stays out of `main_api.h`. `func_80082764` now returns `s32` in `main_api.h` (callers that ignore it compile the same).
- [x] Standards 8a: no override needed; `tools/check_headers.py` passes.
- [x] Commits: one per file group (DATE, SHUGAKU/EVENT/GEKO/GYOZI/ENDING, TACO/TT/DATE leftovers, docs), each after a full ninja with 27 of 27 `OK`.
