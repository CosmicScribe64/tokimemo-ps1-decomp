---
id: T-1030
title: "Batch G: overlays BUNKA_SD, DATE2"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/BUNKA_SD.c` and `src/ovl/DATE2.c` (IDO 5.3 with the frame pass), smallest first, at least 40 matches. Per-overlay externs go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [x] At least 40 functions matched (funcdiff MATCH), all 27 sha1 checks OK. Result: 63 (BUNKA_SD 28, DATE2 35)
- [x] New failure patterns noted in [[matching-notes]]
- [x] Code-review gate run inline, findings resolved

## Notes

`ninja progress` after the batch: grand total 267/6962 functions, 19124/2279368 bytes; BUNKA_SD 28/98, DATE2 35/102. Headers: `include/ovl/BUNKA_SD.h`, `include/ovl/DATE2.h`. Main-exe names used are the ones in `build/main_names.ld` (`bg_read_sub2`, `dec_bg_show_switch`, `dec_bg_cd_read`, `get_g_zyotai_s`, `k_reset`, `get_k_speed`, `hizuke_disp_switch`, `draw2d3d`, `back_clear_switch`, `tpage_buf_clear`, `hizuke_init`, `hizuke_show`, `message_window_init`, `message_window_show`, `check_k_scroll`, `k_disp_inc2`, `read_bustup`, `get_p_name`, `memcpy`). Patterns and failures in [[matching-notes]] (section Overlay batch G).

Reuse C from other `src/ovl/*.c` where an identical function is already matched.

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): matches verified with `funcdiff.py` (relocation-name-only and trailing-pad-only DIFFs explained in [[matching-notes]]) and `ninja` (all overlays sha1 OK); no `NON_MATCHING`, no fakematch, no `FAKE` needed; C89 only (grep for `//`, `inline`, `bool` clean); headers have include guards and fixed-width types; placeholders kept for every name not in the applied rename list; pad stubs in `src/ovl/pad/` hold only `nop` (no game data); no generated asm, `build/` or `expected/` staged. Findings: none open. Spec axis: goal of at least 40 matches met with 63.
