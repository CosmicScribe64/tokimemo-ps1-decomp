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

2026-10-15: BUNKASAI func_801438E4 matched: 852 inventory coverage bytes / 852 literal linked bytes. All 14 diff rows came from the final byte flag ORs. Replaced those operations with the existing character-record CharFlags.f.b12 and .b13 assignments; explicit byte read-modify-write syntax had not helped. This uses the established aggregate and bit-field model (T-9020/T-12300); no FAKE or headers changed. Original C preserved: only the target INCLUDE_ASM line removed. Docker default literal proof, BUNKASAI SHA1, headers/globals and branch prototype guard all pass (zero new / 15 known). Evidence: /private/tmp/t15300-bunkasai-{seed,rmw,bitfield}-diff.log and /private/tmp/t15300-bunkasai-final-proof.log. Continuing fresh TACO candidates under this ticket; incoming zeros retain priority.
