---
id: T-6030
title: Wave 4: list 3
status: Done
assignee: agent (w4-3)
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[game-state]]", "[[data-types]]"]
---

## Goal

Match the functions of wave-4 work list 3 (144 functions, 40244 bytes, 33 files: main 80042A00 .. 800737A0 and 20 overlays) with plain C where possible.

## Acceptance criteria

- [x] Work list time-boxed out; every match verified by ninja and funcdiff (19 functions).
- [x] T-0018 blocked cases recorded in [[data/t0018-cases]] (5 rows).
- [x] Clean build 27 of 27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK.
- [x] Inline review against CODING_STANDARDS.md recorded below.

## Notes

Branch w4-3, not merged. Result: 19 of 144 functions, 3384 bytes; progress 3539 -> 3558 of 6958 (447768 of 2279368 bytes after the branch). Idioms and unsolved shapes: [[matching-notes]], section "Wave 4, list 3 (T-6030)".

Matched: main `vacation_day_init`, `func_8006C934`; EVENT `func_800FD360`; KANGEI `func_80136810`, `func_80137638`, `func_80138470`, `func_8013614C`, `func_80138944`; OPTION `func_801389A0`; RPG_BAT `func_8013D780`, `func_80132CE4`, `func_80132DE8`, `func_80143C28`, `func_80143CE8`, `func_80143DA8`, `func_8014E28C`; SHOUGATU `func_80137940`; TACO `func_80142DE0`, `func_80142F34`.

Lever for the merge: EVENT `func_800FD360` is the source of a neardupes group whose unmatched members are in GEKO (`func_80139A8C`, `func_8013B9F0`, `func_8013F4C0`, `func_80140C50`), outside this list.

## Comments
- 2026-10-10 inline review against CODING_STANDARDS.md (sections 1 to 13). Matches: each verified with `funcdiff.py --resolve` and a full `ninja` (sha1 OK for the main exe and all overlays) before its commit; the final clean rebuild is 27 of 27 OK. No `NON_MATCHING` added; non-matching attempts were returned to `INCLUDE_ASM`. C89: declarations at block top, `/* */` comments, no `inline`/`bool`; local bit-field views (`TcBits`, `KgFlags`) follow the existing `EtcSlot` pattern. Types: fixed-width; `GameState` fields only through `D_800E6280.unk_XXXX` (`tools/migrate_globals.py --check` OK); one cast view of a `GameState` byte array as `s32` (TACO `func_80142DE0`: `*(s32 *)&D_800E6280.unk_1095[3]`, no word field exists at +0x1098). Headers: new main-exe symbols declared once in `include/main_api.h`, overlay symbols in `include/ovl/<NAME>.h` (appended); one override (`MAIN_API_OVERRIDE_D_800EECB0` with reason, symbol wrapped in `main_api.h`); `tools/check_headers.py` and `sync_protos.py --check-branch` OK. Fakematches: `s32 unused; /* FAKE ... */` in KANGEI `func_80136810` (frame slot, same as the T-3330 idiom). Other tricks are plain C and commented where non-obvious (the missing return of the `u8` switch functions is described in matching-notes). No game data or `asm/` staged. Findings: none open.
- Tooling notes: `tools/permute.py --time 60` over a queue of seven functions was still on the first one (`func_801421AC`) after 25 minutes while builds ran in the same worktree (killed; its best candidates scored 20 and worse, none 0). `tools/m2c.py` prints wrong call arguments when `$a0` is a leftover (KANGEI `func_80138470`) and reverses the order of a post-incremented counter (`func_80134024`, `func_80137940`).
