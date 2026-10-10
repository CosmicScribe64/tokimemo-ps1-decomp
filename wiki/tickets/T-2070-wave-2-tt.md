---
id: T-2070
title: "Wave 2: TT"
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal
Match as many remaining `INCLUDE_ASM` functions of the TT overlay (`src/ovl/TT.c`, `include/ovl/TT.h`) as possible.

## Acceptance criteria

- [ ] At least 60 functions matched (or the queue is exhausted): not reached, 49 matched; the rest hit register-order and frame-layout gaps (see Notes)
- [x] 27 of 27 sha1 OK after a clean rebuild
- [x] Inline code review against CODING_STANDARDS.md, findings resolved

## Notes
Queue: `tools/queue.py --next 40 --files TT`.

Result: 49 of 277 `INCLUDE_ASM` functions in `src/ovl/TT.c` matched (277 -> 228), 4 commits `T-2070: match ...`. `ninja progress`: TT 75/303 functions, grand total 950/6962 in this worktree. Patterns, blockers and the m2c ambiguity bug are in [[matching-notes]] (section "Wave 2, TT"). Six T-0018 `regorder` rows appended to [[data/t0018-cases]].

Added to `include/ovl/TT.h`: externs for the tables and globals the new C reads (`D_80155A34` ... `D_80155A74`, `D_801559F0`...`D_80155A30`, `D_80152CA4`/`CB0`/`CC4`, `D_80151A50`, `D_80150978`, `D_80155B54`, `D_80156780`, `D_80158A68`/`AAC`/`AB0`), `u8 func_800460DC(void)` and `void func_8013AE3C(void)`.

## Comments
- 2026-10-09 inline review against CODING_STANDARDS.md (sections 1-12 and the checklist): matches verified with `funcdiff.py` per function and the overlay sha1 (an unmatched C body that had been left in place was caught by the sha1 and reverted to `INCLUDE_ASM` before the last commit); no `NON_MATCHING` blocks added; C89 only (declarations first, `/* */` comments, scripted check for late declarations: none); no SDK structs redefined; placeholders kept; six functions carry a `FAKE` comment (slot-spawn reset stores written relative to the advanced pointer, all in the same shape: `func_8014BA54`, `func_8014BC74`, `func_8014BE60`, `func_8014B4A4`, `func_8014C0B0`, `func_8014B67C`); unused externs added during attempts were removed from `TT.h`, `tools/check_headers.py` passes (`build/headers.ok`); no game data staged. No open findings.
