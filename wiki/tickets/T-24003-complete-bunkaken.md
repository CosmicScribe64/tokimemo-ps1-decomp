---
id: T-24003
title: Complete BUNKAKEN selector and date-loader remainder
status: In Review
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
- Loader func_8013B510 remains INCLUDE_ASM. Bounded current-header tests: typed/GameState-pointer 335; old-style calls and retained seed self-store 318; dispatch view, s32 return view and local heroine copies 318; artificial fourth-parameter diagnostic 326, discarded. The target consistently uses a3 for repeated heroine loads while current C uses v1; a temporary-numbering shift follows case 17. Current typed candidate and provenance archived at drafts/T-24003/. The seed self-store is explicitly FAKE in the unaccepted archive. Historical 297 was not reproduced with current headers.
- Final validation: complete default `ninja -j2` succeeds (`build/t24003/final-build.txt`). Combined `funcdiff.py --unit BUNKAKEN --resolve --version default func_80140F60 func_8013BD84` reports literal linked MATCH for 296 and 540 bytes (`build/t24003/final-proof.txt`). Headers and globals pass; prototype branch check reports 0 new / 15 known disagreements (`build/t24003/guards-final.txt`). Ticket remains In Review for independent review against CODING_STANDARDS.md.
