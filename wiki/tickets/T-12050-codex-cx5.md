---
id: T-12050
title: Match cx5 main menu and text functions
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal

Deep matching pass over fourteen listed functions in src/main/8004F870.c.

## Acceptance criteria

- [ ] Exact linked and unit checksum proof for every match.
- [ ] Guard only measured near misses within about thirty differing rows.
- [ ] Full build and prototype verification; independent standards review before Done.

## Notes

Owned source: src/main/8004F870.c. Main and other worktrees remain untouched. Cutoff 21:45 UTC.

## Comments

2026-10-15: Read index, project/local instructions, standards, workflow and register cheat sheet. Initial history search covers all fourteen names. Baseline configure plus ninja -j2 passes 27/27 SHA1, headers OK and globals OK. Known cases: menu_bar_color lacks same-object data ownership for shared lui; menu_set masked argument allocation; menu_check_1 record/base registers; menu_bar_show packed-word/record repair previously reached eight rows.

### menu_bar_show: MATCH, 208 bytes

Reproduced the prior T-10010 seed from its retained scratch declaration-order-probes/best.c, then replaced scalar-neighbor selection reads with MenuSelection5 and flat record indexing with the interleaved GsRecF90 pair view. Direct packed-global extraction into mode/page/flag locals avoids the scheduling and frame changes of an extra cached color or base local. Exact linked comparison and main executable SHA1 both pass. No FAKE.
