---
id: T-4080
title: "Wave 3: list 8"
status: Done
assignee: agent (w3-8)
created: 2026-10-09
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal

Match the INCLUDE_ASM functions of wave-3 work list 8 (127 functions, 26068 bytes) in the files: 800451D0, 8007C030, BUNKAKEN/80145E40, BUNKA_SD/80134540 and 80136270, DATE/8015A7E0, ETC/8013FF80 and 80146400, EVENT/800FDC20, GEKO (5 files), GYOZI (4 files), NAME_ENT/80139740, SHOUGATU (2), TACO (3), TAIIKU/80133C80, TT (2).

## Acceptance criteria

- [x] Functions matched in list order, time-boxed; T-0018 cases recorded in [[data/t0018-cases]] (32 functions, 4908 bytes; 19 rows added)
- [x] `ninja` ends with 27 of 27 OK and `build/headers.ok` (clean `rm -rf asm build; configure.py; ninja`, no `-k`)
- [x] Inline review against CODING_STANDARDS.md recorded below

## Notes

Result: 32 of 127 functions matched (4908 of 26068 bytes), grand total 2837 of 6958 functions, 309952 bytes. Patterns and left-overs: [[matching-notes]], section "Wave 3, list 8 (T-4080)". T-0018 rows (regorder/promo) appended to [[data/t0018-cases]].

## Comments

Inline review against CODING_STANDARDS.md (no findings open):
- Section 2 (C89): locals declared at block top, `/* */` comments only, no mixed declarations; checked in every new body.
- Section 1/7: no `NON_MATCHING` blocks added. Two fakematches, both marked `FAKE` with ticket id: unused `s32 pad` in TAIIKU `func_80133C80` (two-word case of T-3330) and EVENT `func_800FE090` (frame 4 bytes larger than the dead struct copy gives).
- Section 4/6: names are placeholders (`func_XXXXXXXX`, `D_XXXXXXXX`); `INCLUDE_ASM` lines replaced in place, the rest untouched. Temporaries are named for what they hold (`g`, `cur`, `idx`, `slot`, `key`).
- Section 8a: new main-exe symbols went to `include/main_api.h` (`D_800E74D0`, `D_800F5638`, `func_8004435C`, `func_80078950`; `func_80082764` now returns `s32`); overlay symbols to `include/ovl/GEKO.h` (36 pointer words, one prototype), `TAIIKU.h`, `SHOUGATU.h`. No `MAIN_API_OVERRIDE` added. The local `typedef struct { void (*f[N])(); } FnTblN` and the `extern FnTblN D_xxxxxxxx;` of the table copies sit in the `.c` next to the function, the same way the T-3330 matches do (the table type is used by one function).
- Strings are written as literals (`"グランド"`, `"s%d\n"`), never as `extern`.
- `tools/check_headers.py` passes (`build/headers.ok`); 27 of 27 sha1 OK.
- Tooling note for the orchestrator: `funcdiff.py func_X` does not select the overlay (it printed `MATCH` for functions whose overlay failed sha1), see matching-notes.
