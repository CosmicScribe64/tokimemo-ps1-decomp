---
id: T-26002
title: Sweep the FAKEs that steered the old block-copy detector
status: Done
assignee: w20-fake-sweep (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: `[tickets/T-26000-ugen-block-copy-41-rule`, [[toolchain]], [[matching-notes]]]
---

## Goal

T-26000 replaced the block-copy state rule of `tools/ugen_pass.py` with IDO 4.1's own: a procedure starts in B, an indirect store sets F, a compare of two registers sets B, everything else keeps the state. Earlier matches added FAKE constructs only to steer the old detector (never-true arms holding a store, empty signed tests, dead loops, extra tests against constants). Remove every one that the new rule no longer needs, restoring the plain spelling, so the C is more natural for the Godot port. Keep (with marker, ticket and reason, CODING_STANDARDS section 7) every one that is still needed, and fix its comment where the new rule changes the reason.

## Acceptance criteria

- [x] Every candidate FAKE of the grep list (58 hits) classified: related or not related to the copy state.
- [x] Each related FAKE removed (funcdiff MATCH for it and for the file's other functions) or kept with an updated comment. (None removable; all eight kept, comments updated.)
- [x] Clean build 27/27; check_headers, migrate_globals --check, sync_protos --check-branch, unit tests OK.
- [x] Removed and kept FAKEs recorded below and in a short matching-notes section.

## Notes

Method: remove the construct, rebuild the unit's object, `funcdiff --resolve --version default` for the function, and compare the whole object (all functions of the file) with the original.

## Comments

### 2026-10-24, w20-fake-sweep (Sonnet): result

Census first, then one rebuild per candidate. Scripts and method: `drafts/T-26002/README.md`; summary and numbers: [[matching-notes]] "T-26002".

- Method. The pass only changes procedures with a `Umov` (566 in the build); under the 4.1 rule only an indirect store (F) and a compare of two registers (B) move the state. A trace of every ugen input (a throwaway clone with a logging shim, full build) gave, per procedure, the copy states, the events that set them (with source lines from `Uloc`) and the FAKE comments of its body. For each FAKE the events within seven lines of its comment were dropped and the states recomputed.
- Result of the census. 25 procedures have a copy in F, 13 of them contain a FAKE. The state depends on a FAKE in exactly four, all known (MASTER `func_801344E0`, `func_801363F4`, main `strCallback`, `func_8005938C`); a fifth hit, main `func_80062948`, is a real store after a FAKE spelling. Four procedures are B because a register compare followed a store (`func_8005557C`, `func_80055AFC`, TAIIKU `func_8013D2B4`, `func_8006B0C8`); none of those compares is in a FAKE.
- Candidates (58 grep hits): 35 sit in functions without a block copy (the pass does not touch them: TEL `func_80136724`, EN_NICHI `func_8013735C`, ETC `func_80132C3C`, main `func_80065B0C`, BUNKAKEN `func_8014D65C`, ...), 15 sit in functions with a copy but document another mechanism and change no state in the census, 8 were removed and rebuilt (below). not_related = 50.

Removed: none. The two empty signed tests that only fed the old lock (main `func_80054FC0`, TACO `func_80145650`) were removed in T-26000.

Kept (removal rebuilt, overlay or main sha1 FAILED, `funcdiff --resolve --version default` DIFF):

| Function | File | Construct | Why it stays |
| --- | --- | --- | --- |
| MASTER `func_801344E0` | `src/ovl/MASTER/80133680.c` | never-true arm, T-25107 | its indirect store is the only store before the two record copies; without it both are B, the original has F |
| MASTER `func_801363F4` | `src/ovl/MASTER/80133680.c` | never-true arm, T-25107 | same |
| main `strCallback` | `src/main/800563F0.c` | never-true arm, T-26000 | RECT copy is F in the original; 4.1 gives F through a branch on a value that the pass does not model |
| main `func_8005938C` | `src/main/80058D20.c` | never-true arm, T-26000 | same, two MATRIX copies |
| GYOZI `func_8014494C` | `src/ovl/GYOZI/80143AD0.c` | repeated copy line, T-24092 | the original has two identical copy loops (both B); one line gives one loop. Not a state lever; the comment said so wrongly and is rewritten |
| SHOUGATU `func_8014265C` | `src/ovl/SHOUGATU/80141780.c` | repeated copy line, T-24092 | same |
| GEKO `func_801406CC` | `src/ovl/GEKO/8013F360.c` | `s32 pad`, T-24092 | frame padding (136 against 128 bytes), unrelated to the copy state |
| EVENT `func_80109404` | `src/ovl/EVENT/80107FC0.c` | `s32 pad[2]`, T-24092 | same |

Comments updated (reason and T-26002): GYOZI and SHOUGATU repeated copy lines; the four never-true arms now say the pass counts a store in an arm ugen drops, which 4.1 ignores (`copy_dropped_arm_store`), and that they were checked again here (follow-up 4 of the T-26000 review). Comment-only edits.

Open: when the pass models F at a branch on a value (T-26000 follow-up, needs the 4.1 measurements of `func_8006B0C8`'s 4-byte copy), the four never-true arms can go; this sweep does not change that.

Proof: clean rebuild (`rm -rf build && configure.py && ninja -j2`) 27/27 sha1 OK with the comment edits; `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK, `python3 -m unittest discover -s tools -p 'test_*.py'`: 1580 tests OK (3 skipped). Review: see below.

### 2026-10-24, review: approved

Standards and spec review (CODING_STANDARDS section 7) approved with no blocking findings. The source diff is comment-only in 5 files; no FAKE was removed, so no new trick replaces one, and every kept FAKE comment has its marker, ticket and reason. `funcdiff --resolve --version default` gives MATCH for all six changed functions (main `strCallback` and `func_8005938C`, GYOZI `func_8014494C`, SHOUGATU `func_8014265C`, MASTER `func_801344E0` and `func_801363F4`). The corrected GYOZI and SHOUGATU comments are right: the original holds two separate block-copy loops, so one repeated copy line produces them and does not steer the state.

Non-blocking findings, both resolved at merge: the kanban card was missing (added under Done), and the bare `copy_dropped_arm_store` in the `strCallback` and `func_8005938C` comments now carries its path `tools/ido_eval_cases/ugen_copy_state/copy_dropped_arm_store.c`.
