---
id: T-24007
title: Complete GEKO scene package
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[scenes-family-results]]"]
---

## Goal

Recover verified C in GEKO 801323E0.c for func_80133024, func_80133598, func_80133308, func_80132D04, and func_80132B24.

## Acceptance criteria

- [ ] Verify recovered C by default literal linked proof and affected overlay checksum.
- [ ] Pass header/global/prototype checks and independent review.

## Notes

Bounded types-first follow-up of T-17700 and source-family pass T-13900. Reuse existing clean worktree; one compiler/ninja job. No changes to previous package scopes.

## Comments

2026-10-17: Read wiki index, scenes-family results and unstarted w4-p35 list. Earlier func_80133598 four-row scratch is unavailable under the referenced build/scenes13900 path, so reconstruct from documented field types and source.

2026-10-17: Recovered func_80133598 in plain C (452 bytes). Reconstructed prior four-row recipe with one saved status byte, correct full-word Rec38 bit test, and direct signed amount field update. A genuine s32 call-result local before the byte fixes the stack slot (0x2F to original 0x2B). No FAKE or padding. Docker ninja -j1 build/ovl/GEKO.ok passes; funcdiff.py --unit GEKO --resolve --version default --no-build func_80133598 confirms linked target-byte equality (452 bytes).
