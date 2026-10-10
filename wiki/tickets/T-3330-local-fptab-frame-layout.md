---
id: T-3330
title: Local function-pointer table frame layout
status: Done
assignee: agent (o-fptab)
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[toolchain]]", "[[ido-52-evaluation]]", "[[data/t3330-fptab-proof.patch]]"]
---

## Goal

About 236 overlay functions copy a function-pointer table from data into a local and call through it. With IDO 5.3 and the frame pass, the local lands 4 bytes lower and the frame 8 bytes smaller than in the original. Find the exact rule. If it is a toolchain difference, implement a pass. If it is a source difference, document the C idiom.

## Acceptance criteria

- [x] Rule determined with scripts over all affected functions (236) and checked against IDO experiments
- [x] Pass or C idiom decided: C idiom (source difference), no tooling, no `tools/cc.py` change
- [x] Proof on at least 10 affected functions: 18 functions matched in an uncommitted tree, 27 of 27 sha1 OK; C saved as `wiki/data/t3330-fptab-proof.patch`
- [x] No matched function changes (no tool changed)
- [x] Findings in [[matching-notes]] and [[toolchain]]

## Notes

- Rule: if any local is in memory, IDO gives every declared local a stack slot. The slots run from the top of the frame down in declaration order. The original source declared one scalar (the index) before the table.
- IDO 5.2 and 4.1 behave like 5.3 here (T-3110), which is consistent with a source difference.
- `wiki/data/t3330-fptab-proof.patch` touches `src/` and `include/ovl/ENDING.h`. Apply it after T-1321 merges.
- Left: SHOUGATU `func_80134120` and GYOZI `func_80137798` have the right layout, but their registers differ (T-1321). The TAIIKU two-word cases have no C without a `FAKE` local.

## Comments

- 2026-10-09: inline review against CODING_STANDARDS (7a, 8a, 9, 11) done. There are no committed tool or source changes. The proof C has a comment on each non-obvious declaration order. `include/ovl/ENDING.h` gets one extern (`D_800E738A`, the same `u8` type as `game.h`, which ENDING.h does not include). The GEKO callees `func_8007C8A4`, `func_8007E81C`, `func_8007EC68` and `func_8007EC9C` have no prototype in the GEKO header closure, so they are called through implicit declarations. Adding `void` prototypes when the patch is applied is a follow-up for the orchestrator, not a finding against this ticket.
