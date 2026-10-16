---
id: T-16554
title: "Types-first pilot package pt-taiiku-p4"
status: Done
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of package pt-taiiku-p4 starting from the T-16550 types-first redrafts (`drafts/T-16550/TAIIKU/<func>/`, best draft per function): `func_80140A90`, `func_80132970`, `func_8014357C`, `func_8014280C`, `func_80143F50`, `func_80142FB8`, `func_80143DD0`. Owned C files: `src/ovl/TAIIKU/80140810.c`, `80132000.c`, `80142240.c`.

## Acceptance criteria

- [x] Each function matched (unit sha1 OK, no guard) or kept under `#ifdef NON_MATCHING` (< about 30 differing lines) with a ticket comment, else plain `INCLUDE_ASM`.
- [x] Types extended in `include/ovl/TAIIKU.h` instead of byte-offset casts where the build allows.
- [x] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [x] Short T-16554 section in [[matching-notes]]; compiler-gap rows in `wiki/data/t0018-cases.md`.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p4`, branch `pt-taiiku-p4` from `w2-good`. Time box: stop starting new functions after about 2.5 hours. Pilot stage B (measured).

## Results

Four of seven matched (3,208 bytes), unit sha1 OK after each: `func_80132970`, `func_80143DD0`, `func_80143F50`, `func_8014280C`. Guarded drafts: `func_80140A90` (33 differing instructions, ugen temporary numbering after a block copy; the two callee prototype overrides and one FAKE selector view are in the tree), `func_8014357C` (20 instructions, flags and rating register allocation). `func_80142FB8` stays `INCLUDE_ASM` (loop variables in caller-saved registers with spills in the original, `$s1..$s3` here). FAKE comments: `*(u8 *)&D_8014A110` views (`func_80143F50`, `func_8014280C`), `u8 pad[4]` (`func_8014280C`), `*(s32 *)&D_80122CDC` selector (`func_80140A90`, guarded). Notes: [[matching-notes]] "T-16554"; rows in [[data/t0018-cases]]; cheat sheet rows added.

## Comments

- 2026-10-16: agent time about 83 minutes (02:25-03:48 UTC). Clean rebuild 27/27 (`rm -rf build && configure.py && ninja`), `check_headers.py` OK, `migrate_globals.py --check` OK, `sync_protos.py --check-branch` OK (0 new, 15 known). Code review against `CODING_STANDARDS.md` still to do before Done: FAKE comments in `80142240.c` (`func_80143F50`, `func_8014280C`) and `80140810.c` (guarded `func_80140A90`).
- 2026-10-16: code review against `CODING_STANDARDS.md` (checklist, plus owned-file, drafts, inline-asm, guard and header checks). Findings and fixes:
  1. `func_8014280C`: `*(s32 *)&...[96].pad_28` made the compiler warn "'&' before array or function; ignored" (`pad_28` is an array). The `&` was dropped (same bytes) and the cast now carries a `FAKE` comment (s32 view of the 16.16 y position at 0x28, high half `unk_2A`, ticket id).
  2. `func_80143DD0`: `*(s16 *)&D_8014A1E8[unk_00 * 2]` reached an s16 table through a `u8` array (section 8). `D_8014A1E8` is now `extern s16 D_8014A1E8[]` in `include/ovl/TAIIKU.h` and the access is `D_8014A1E8[unk_00]`; unit sha1 still OK.
  3. `MAIN_API_OVERRIDE_func_800853FC` reason said only "same"; now states the reason itself. `func_80143F50` FAKE comment split from the `bits` explanation.
  No finding: C89, no inline asm or `.word`, only the three owned C files changed, no `drafts/` change, no game data; both `NON_MATCHING` blocks and the `FAKE` selector view in the guarded `func_80140A90` carry T-16554 and the line counts; header edits consistent (`check_headers.py` OK); `MAIN_API_OVERRIDE_func_80085368` and `_800853FC` are explained and declared next to the file's other declarations (they are used only by the guarded draft; `sync_protos.py --prune` would drop them while the draft is guarded, re-add them when it is enabled).
  Observation, not changed: `D_8014A208[8]` and `D_8014A228[11]` are contiguous; `type_recovery.py` proposes one array `D_8014A204` (ambiguous, `func_80142400` still `INCLUDE_ASM`); revisit when that function is decompiled.
  After fixes: clean build, headers OK, globals OK, `sync_protos.py --check-branch` OK (0 new, 15 known), TAIIKU sha1 OK. Moved to Done.
