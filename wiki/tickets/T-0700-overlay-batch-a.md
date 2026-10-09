---
id: T-0700
title: Overlay batch A: RENSYU, OMIMAI, VALEN, MASTER
status: Done   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal
Decompile functions in `src/ovl/RENSYU.c`, `OMIMAI.c`, `VALEN.c`, `MASTER.c` (IDO 5.3 + frame pass), smallest first, about 50 matches. Branch ovl-batch-a.

## Acceptance criteria
- [x] About 50 functions matched (56), all 27 sha1 checks OK
- [x] Hard patterns noted in [[matching-notes]] (section Overlay batch A)
- [x] Code review gate passed

## Notes
Per-overlay headers in `include/ovl/<NAME>.h`. Main-exe symbols declared with old names where needed to link (list below, for rename at merge).

## Results
Matched 56 (verified by byte comparison of the linked `.bin` against the original and by the 27 sha1 checks): MASTER 22, VALEN 15, OMIMAI 13, RENSYU 6. Left as `INCLUDE_ASM`, with reasons, in [[matching-notes]] (section Overlay batch A): jump-table and string functions (rodata is an asm blob), function-pointer table copy, global counter post-increment, `|=` address register, `goto` state machines.

Build notes: mid-overlay object padding is reproduced by committed stubs `src/ovl/pad/pad_<NAME>_<addr>.s` (nops only); `include/ovl/*.h` is not a ninja input (touch the `.c` after header edits).

Old names used in the new C (they are still the names the build links against; rename at merge with `config/obin_renames.txt`): func_800438DC draw2d3d, func_800438F0 back_clear_switch, func_80048390 tpage_buf_clear, func_8004E9F4 k_reset, func_80057390 set_dec_bri, func_800649D4 hizuke_init, func_80064DEC hizuke_show, func_80064E84 message_window_init, func_80064F48 message_window_show, func_8007C740 addr_init_bustup, func_8007E390 normal_date_girl_in, func_8007ED84 bg_read_sub2, func_8007EDF8 check_k_scroll, func_800846C0 k_disp_inc2, func_80085958 normal_date_speak_1line, func_80086424 Default_Disp. Applies to `src/ovl/{MASTER,VALEN,OMIMAI,RENSYU}.c` and `include/ovl/*.h`.

## Comments
- Code review (inline, CODING_STANDARDS checklist, 2026-10-09): matches verified by byte compare and sha1; no NON_MATCHING and no fakematch in the new C; C89 and `/* */` only, declarations at block top; no SDK structs redefined; placeholders kept; `INCLUDE_ASM` used per section 6 plus the pad stubs (explained in the C comment and matching-notes); fixed-width types; no game data staged (only `src/`, `include/ovl/`, `wiki/`); commits carry T-0700. No findings.
