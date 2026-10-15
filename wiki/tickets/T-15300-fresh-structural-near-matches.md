---
id: T-15300
title: Resolve fresh structural near matches
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-notes]]", "[[tickets/T-14501-remaining-draft-proof]]"]
---

## Goal

Resolve fresh non-guarded near matches from completed T-14500 units through bounded C structure/type changes. Start with BUNKASAI func_801438E4 (852 inventory bytes, score 14). Incoming zero promotion remains priority.

## Acceptance criteria

- [ ] Only default literal matches and affected binary SHA1 passes are promoted.
- [ ] Existing C preserved; ordinary C or any FAKE marked.
- [ ] Header/global/prototype guards pass.

## Notes

Avoid main, EVENT, RPG_BAT, and separately reserved repair functions. Historical wiki/catalog contains no earlier attempt for BUNKASAI func_801438E4.

## Comments

2026-10-15: Claimed at root's request. Inspect original control flow before bounded experiments; do not change the frozen census context.
