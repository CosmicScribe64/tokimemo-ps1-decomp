---
id: T-24605
title: Match GEKO top-six ranking initialization
status: Done
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24600-finish-matching]]"]
---

## Goal

Complete GEKO `func_801420E4` using the existing game-state record and six-element ranking types.

## Acceptance criteria

- [x] Default C matches all 1,256 literal linked bytes.
- [x] Full Docker build, executable hashes and declaration guards pass.
- [x] Independent standards and behavior reviews have no unresolved findings.

## Notes

The private reconstruction matches 314 words. Repeating the score expression preserves the original reloads at each insertion. A discarded girls[2] test resolves an allocation tie; unused 0x420-byte storage reproduces the original 0x480 frame. These mechanisms require FAKE annotations and minimization evidence before promotion.

## Comments

- 2026-10-20: Created from the T-24600 ranking lane before production promotion. Private literal proof passes; cleaning and integration review remain pending.
- 2026-10-20: In Progress -> In Review. Cleaned default production proof matches1,256 literal bytes; full ninja, fresh27 hashes and declarations pass (zero new disagreements,15 known). Independent standards/spec reviews report no findings. Actor stores use existing DisplayActor44 fields. Omitting padding changes only frame offsets; omitting girls[2] test exchanges two registers; splitting girl initialization from do changes scheduling. All three retained mechanisms have FAKE reasons.
- 2026-10-20: In Review -> Done after both independent reviews and every integration gate passed. Adds1,256 verified bytes; total6,821 functions /2,095,956 bytes,131 functions remain.
