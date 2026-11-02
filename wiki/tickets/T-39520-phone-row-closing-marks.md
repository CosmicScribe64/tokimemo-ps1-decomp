---
id: T-39520
title: Close direct-row telephone dialogue consistently
status: In Review
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[tickets/T-39506-english-script-runtime]]"]
---

## Goal

Give complete telephone dialogue pages balanced quotation/parenthetical marks, including callers that render individual rows directly.

## Acceptance criteria

- [x] Direct-row pages receive one appropriate closing mark after their final row; sound effects stay unquoted and existing closings are not duplicated.
- [x] Wrapping reserves the closing mark and remains safe for full names; row/page boundaries preserved.
- [x] Actual phone-route captures show complete marks without clipping or overlap.
- [x] Production tests, original build matching and standards review pass.

## Evidence

Visual review found missing closing quotes in `build/emu/route-en-ending-stream-full/108_phone_call_06.png` and `114_phone_answer_02.png`, among others. `english_sndisp` appends a closing mark, whereas direct `english_sndi` adds the opening mark only. `src/ovl/telephone/call_and_invitation_flow.c` frequently calls `sndi` once per row, then `k_sub_disp_start`. These callers bypass the block-level closing logic. This is a separate path from the already fixed classroom screenshot.

The English hooks collect consecutive rows and render the complete page at `k_sub_disp_start`, retaining the speaker entry and its measured mark position. Block rendering suppresses row capture to avoid recording the same text twice. A reset or interleaved text invalidates the pending batch. Pages of up to three rows keep the original voice and typewriter behavior; longer pages use the existing scrolling path.

The C89 test runs production batching, wrapping and closing code. It covers existing quotes, thoughts, sound effects, resets, interleaved text, repeated display starts and wrapped pages. Combined runtime and name tests pass 15/15.

The build completed in 182.5 seconds and produced a 6,565,120-byte patch in `build/phone-row-final-proof/disc`.

## Review

The fresh route completed 34,745 frames in 796 seconds with 146 captures and zero original-font calls, unknown glyphs or overflows. All captures were inspected. Phone pages 109–111 and 114–120 show complete closing marks, with no overlap or clipping. Sound effects remain unquoted. Full player names survive save/load in captures 142–146. The separate short-row issue is tracked in T-39511.

All 27 original executable hashes, headers, globals and 14,449 rename records pass. Standards review checked C89 declarations, bounded row storage, reset/interleaving invalidation, block-renderer recursion suppression and English-only hooks. No unresolved code issue remains. The main executable is 692,224 bytes and ends at 0x80131280, below the 0x80132000 limit. The ticket remains In Review for an additional run with voice playback enabled.
