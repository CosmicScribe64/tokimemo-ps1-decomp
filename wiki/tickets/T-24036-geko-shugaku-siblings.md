---
id: T-24036
title: GEKO and SHUGAKU sibling completion
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[scenes-family-results]]", "[[shugaku-family-results]]"]
---

## Goal

Recover GEKO func_80140040 and SHUGAKU func_80134C98 from matched siblings, preserving the integrated GEKO func_8013FD88 body. No other function/header changes without parent notice.

## Acceptance criteria

- [ ] Actual unguarded C plus default literal linked proof and unit checksum for claimed wins.
- [ ] Useful failed source forms preserved.
- [ ] Header/global/prototype checks and independent review.

## Notes

Merged latest main from a clean tree (db89e0ce); wiki-only conflicts preserve both histories, no C conflict. Read scenes/shugaku families, T-17700/T-18000 and older GEKO7-row reload/scheduling negatives before reconstruction. One Docker compiler/ninja-j1; bounded20min.

## Verified batch1

SHUGAKU func_80134C98 matches244 default linked bytes using direct matched func_80133F6C structure, canonical GameState fields and its required extra state/flag stores. No FAKE or header change. Target ASM removed; Docker ninja-j1 SHUGAKU checksum passes, --resolve --version default --no-build reports linked bytes equal target244. Headers/globals pass; prototype branch check0 new/15 known. GEKO's matched func_8013FD88 remains identical to main.
