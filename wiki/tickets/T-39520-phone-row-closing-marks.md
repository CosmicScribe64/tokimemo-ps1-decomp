---
id: T-39520
title: Close direct-row telephone dialogue consistently
status: Ready
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[tickets/T-39506-english-script-runtime]]"]
---

## Goal

Give complete telephone dialogue pages balanced quotation/parenthetical marks, including callers that render individual rows directly.

## Acceptance criteria

- [ ] Direct-row pages receive one appropriate closing mark after their final row; sound effects stay unquoted and existing closings are not duplicated.
- [ ] Wrapping reserves the closing mark and remains safe for full names; row/page boundaries preserved.
- [ ] Actual phone-route captures show complete marks without clipping or overlap.
- [ ] Production tests, original build matching and standards review pass.

## Evidence

Visual review found missing closing quotes in `build/emu/route-en-ending-stream-full/108_phone_call_06.png` and `114_phone_answer_02.png`, among others. Source confirms `english_sndisp` appends a closing mark, whereas direct `english_sndi` adds the opening mark only. `src/ovl/telephone/call_and_invitation_flow.c` frequently calls `sndi` once per row, then `k_sub_disp_start`; these callers bypass the block-level close logic. This is a separate path from the already fixed classroom screenshot. Investigate the display-start boundary before choosing a safe implementation; do not close every row independently.
