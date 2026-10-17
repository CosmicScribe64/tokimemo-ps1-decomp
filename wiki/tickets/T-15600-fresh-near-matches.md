---
id: T-15600
title: Resolve fresh RENSYU and RPG_BAT near matches
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[tickets/T-14900-rpg-near-matches]]", "[[game-state]]"]
---

## Goal

Investigate RENSYU func_801320E0 (688 code bytes) and RPG_BAT func_80155A70 (588), with optional SHOUGATU func_80137144 (568). Prioritize genuine types and control flow and preserve original census attempts.

## Acceptance criteria

- [x] Classify actual full-TU discrepancies; avoid exhausted T-14900 targets.
- [x] Promote only explicit default linked-byte and affected SHA1 verified C.
- [x] Existing matching C preserved, guards pass.
- [ ] Independent standards review before Done.

## Notes

Wiki has no retained target-specific experiment for the first two; ancient RENSYU note about unlinked rodata is obsolete under the current build system. RENSYU case 5 accesses byte +0x66E at stride four, represented canonically by GameState.unk_66C[].unk_02. Initial inferred s32 declaration and pointer scaling are suspect. RPG and SHOUGATU will be assessed from exact baseline differences before structural probes. No overlap with root BUNKASAI or proof-lane BUNKAKEN/main targets.

## Comments

2026-10-17: Review closed. Both matches (RENSYU func_801320E0 and SHOUGATU func_80137144, 1,256 linked bytes) are on the integration tree as plain C, and the RPG_BAT func_80155A70 gap was closed by T-16210 (s32 return with no value). One small cleanup remains: use the RensyuClubCount view in func_801320E0 case 5; filed as a followup. Moved to Done.

Followup opened: [[tickets/T-21012-rensyu-801320e0-club-count-view|T-21012]].

## Results

Two accepted matches, **1,256 literal linked bytes**:

- RENSYU func_801320E0: 688 bytes. Replacing the inferred word/scaled-pointer view of D_800E68EE with the existing four-byte GameState.unk_66C record reduces 16 to two diagnostic rows. The target uses lhu at record offset +2, spanning the existing +2/+3 bytes; an explicit u16 view reduces two to zero. All other main globals use canonical GameState members. No invented layout, padding, FAKE shaping or main-header addition.
- SHOUGATU func_80137144: 568 bytes. The target compares the old unsigned counter against 0x401 and increments before branching. The census postincrement rewrite instead compared the incremented value. Natural `if (counter++ >= 0x401U)` preserves the actual old-value test and reduces fourteen rows to zero. Canonical GameState fields and three existing void callee declarations preserve the zero result.

Explicit default linked function proofs pass for both functions, and RENSYU/SHOUGATU SHA1 checks pass. Headers OK, globals OK, prototype branch guard 0 new / 15 known disagreements. Exact header scope: three void forward declarations in include/ovl/SHOUGATU.h only. Prior C bodies are unchanged; only two INCLUDE_ASM lines are replaced.

RPG_BAT func_80155A70 remains nine rows: target has a NOP after the sign-bit branch, while the draft schedules the following LUI into that delay slot. The missing instruction changes four branch displacements; otherwise the diagnostic instruction stream agrees. Real state-pointer and nested-condition forms remain nine; equivalent signed-status comparison worsens to 53. No dummy NOP/register forcing or additional experiments retained. This is a concrete source/code-scheduling gap, not evidence of a compiler defect.

All exploratory C bodies, source/preprocessed contexts, comparisons, provenance and reproduction drivers are archived in drafts/T-15600; no binaries/ASM/game data. Original T-14500 attempts are untouched. Stop further experiments at the user's usage-budget instruction.

## Review

C89, fixed-width types, canonical existing GameState storage, target-evidenced halfword width and old-counter semantics. Generated ASM trees were detached from shared hardlinks before splat regeneration. No prior matching C edits or artificial match shaping. Independent standards/spec review pending; In Review.
