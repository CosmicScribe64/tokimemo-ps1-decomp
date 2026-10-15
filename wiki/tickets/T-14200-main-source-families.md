---
id: T-14200
title: Recover MAIN source families
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-breakthroughs]]", "[[main-family-results]]"]
---

## Goal
Recover verified game-code bytes in MAIN from 0f117230, excluding 8004F870.c and 8007C030.c. No overlay edits.

## Acceptance criteria
- Literal full-TU default linked proof for accepted functions and MAIN SHA1.
- Headers, globals, prototype guards; preserve existing C and generated assembly.
- Standards review before Done. Failed probes remain ignored.

## Notes
Existing image only, Docker analysis/build, one compiler. Prior broad default sweeps will not be repeated.

## Comments

### Initializer checkpoint
func_800412E0: 676 default full-TU literal linked bytes, MAIN SHA1 OK. Evidence and rejected focused probes in [[main-family-results]]. Only added shared prototype is the existing void func_80041840 definition. No generated assembly or existing C bodies modified.
Header/global guards pass; prototype guard: 0 new / 15 known disagreements, branch check OK (build/main14200/412-guards.log). MAIN SHA1 independently checked by ninja. Review pending; work continues.

### Character initializer checkpoint
func_80041C2C: 796 default linked bytes, MAIN SHA1, headers/globals pass; prior 676-byte initializer reverified. Existing GameState pointer and CharFlags suffice; no new type layout. Cumulative 1472 bytes. Evidence in [[main-family-results]].
