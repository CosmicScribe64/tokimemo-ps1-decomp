---
id: T-24003
title: Complete BUNKAKEN selector and date-loader remainder
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[bunka-family-results]]", "[[matching-notes]]"]
---

## Goal

Match func_80140F60, func_8013BD84 and func_8013B510 in the two assigned BUNKAKEN objects using bounded source hypotheses and existing typed family forms.

## Acceptance criteria

- Literal default linked funcdiff proof and BUNKAKEN checksum for accepted matches.
- Preserve useful unsuccessful drafts with provenance.
- Headers, globals and prototypes checked; review before Done.

## Notes

Isolated branch codex/complete-bunkaken from w2-good. Starting archived scores: 37, 87 and 297 rows respectively. Existing wave-4 package was not started.

## Comments

- Selector func_80140F60: literal default linked MATCH, 296 bytes (`build/t24003/selector-proof.txt`). The existing BunkaDispatch8 view of D_80122EB8/D_80122EBC supplies the original v1 selector allocation and permits the nested calendar load to occupy the original delay slot. Existing CharFlags b2 supplies the original sign-bit test. No FAKE needed.
- Heroine-selection loop func_8013BD84: literal default linked MATCH, 540 bytes (`build/t24003/bd-proof.txt`); BUNKAKEN SHA1 OK (`build/t24003/checksum.txt`). Archived hand draft 87 -> 38 -> 0 search cost. Explicit `u8 flags` for the fixed heroine-0 tests supplies the original unshifted byte in v1; the indexed loop keeps its inline byte expression, and its sum is written unk_06 + unk_02. Conditional-local and reload variants scored worse (65 and 91). No FAKE or header changes.
