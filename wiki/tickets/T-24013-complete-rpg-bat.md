---
id: T-24013
title: Complete RPG_BAT battle family package
status: In Review
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[rpg-bat-family-results]]"]
---

## Goal

Recover verified C for RPG_BAT func_80138324, func_80152F30 and func_80139510 in 80137280.c / 801525B0.c.

## Acceptance criteria

- [ ] Default literal linked proof and RPG_BAT checksum for recovered C.
- [ ] Header/global/prototype checks and independent review.

## Notes

Bounded types/sibling/control-flow pass from unstarted w4-p40. Reuse clean worktree; one compiler and ninja -j1. Preserve earlier package commits and scopes.

## Comments

2026-10-17: Read wiki index, RPG_BAT family type/idiom results and related notes before probing.

2026-10-17: func_80139510 promoted: literal default proof confirms 2,424 bytes (worklist 2,616 includes rodata). Reconstructed the T-9960 four RECT recipe with paired comma expressions for x/y and w/h, direct existing counter field updates, and the current file's existing int callee view. FAKE two unused s32 locals preserve eight original frame bytes above RECTs; comment explains unknown real source. Three existing callback forward declarations added. Docker ninja -j1 build/ovl/RPG_BAT.ok passes; funcdiff.py --unit RPG_BAT --resolve --version default --no-build func_80139510 reports MATCH (linked bytes equal target, 2,424 bytes).

2026-10-17 independent root integration review: func_80139510: reviewed C89 source, SDK RECT fields, shared aggregates and callback declarations. Two unused frame words have precise FAKE explanation. Independent default literal MATCH 2424 bytes; all 27 hashes and guards pass; published 12e1afca5. No unresolved findings in this match; remaining family work is separate.
2026-10-17 final: Package result 1 of 3 matched, 2,424 literal linked bytes. Other two remain INCLUDE_ASM with selected source-only drafts and concrete blockers in drafts/T-24013/README.md. All loose experimental candidates, scripts, input files and instruction diagnostics are preserved under ignored build/T-24013-probes. Header/global checks pass; branch prototype check passes (0 new disagreements, 15 known). FAKE frame words are explicit. Self-review found no unresolved C89/type/guard/raw-data issues; parent independently reviewed the source/header delta and is verifying integration. Ticket/board moved In Review pending that verification.
