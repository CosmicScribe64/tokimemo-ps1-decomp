---
id: T-15500
title: Match BUNKASAI character flag continuation
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-15300-fresh-structural-near-matches]]"]
---

## Goal
Transfer the verified CharFlags representation to fresh func_8013FEB4 (820 coverage bytes, six diagnostic rows).

## Acceptance criteria
- [x] Default linked literal proof and affected checksum pass.
- [x] Preserve prior matching C and original draft archive.

## Notes
D_800E6679 is GameState +0x3F9: record ten at 0x1BC +10*0x38, flags byte +0x0D. Mask 0x20 corresponds to existing bit13.

## Results
The existing record-ten CharFlags bit13 representation eliminates all six differing rows. Default linked proof matches 820 bytes; affected BUNKASAI checksum, header/global guards and branch prototype check pass. No header changes, artificial code or edits to prior matching functions. Evidence: build/scratch/goal-bunkasai-feb4.log and build/scratch/t15500/.
