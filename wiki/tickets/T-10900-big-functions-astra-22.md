---
id: T-10900
title: Big functions (astra) 22
status: Done
assignee: Codex a22
created: 2026-10-13
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Match the assigned large functions in DATE/80132000, main/8007C030,
and EVENT/8010FE10; publish reusable types and source idioms.

## Acceptance criteria

- [ ] Largest functions receive sustained attempts; keep only verified C or ticketed near misses.
- [ ] Recovered layouts and idioms documented in headers and workflow.
- [ ] Docker build, header/prototype/global checks, and inline standards review complete.

## Notes

Started 2026-10-13 14:50 UTC. User requests at least four hours. Follow the
wave11 brief: only assigned C files, append-only overlay header additions,
generated main declarations, no Git mutations or other agents' files.
Scratch: external scratchpad/a22. Work list: astra22-list.txt (46 functions,
67,672 bytes). Work largest-first across the three objects.

## Comments

- Read wiki index, brief, standards, register cheatsheet, workflow, and prior DATE/EVENT findings. Fresh worktree has no generated asm/build; preparing Docker baseline.

- Baseline: 27/27 Docker SHA1 checks; progress 5333/6958 functions,
  1094140/2279368 bytes (48.0%). Dialogue baseline: 2168 instructions,
  eight positions; coordinate locals, argument line layouts, and 96 string
  lifetime/layout combinations do not improve it. Standard permuter's
  stripped/prettyprinted seed is much farther away, so a scratch-only
  full-file candidate compiler is being tested (no repository tooling edits).
- EVENT 103A0: recovered-state seed remains broad; conditional place reloads
  reduce 1714 instructions to 1712 against 1711, but allocation/control-flow
  differences exceed retention threshold. Alternate common-tail month views
  and control-flow forms tested with regdiff; scratch only.
- `select_girl_init`: MATCH, 656 linked bytes (`funcdiff --resolve`). Added
  three 12-element geometry declarations; existing width declaration reused.
  Signed strlen local and shared s16 cursor recover the exact frame/loops.
- `func_8007C8D8`: 683/683 instructions, two frame-size constants remain
  after inlining the default-case row coordinate. All register words agree.
- `func_80082764`: inlining the line cursor at the dialogue call reduces
  the inherited five differing positions to one commutative addu.
- `func_8007C8D8`: MATCH, 2,732 linked bytes (`funcdiff --resolve`). Inlining
  both half-width and right edge removes two locals and fixes the final
  frame constants. Total confirmed gain: 2 functions, 3,388 bytes.
- `func_8007D8AC`: MATCH, 2,788 linked bytes (`funcdiff --resolve`). Removing
  the case-5 right-edge local alone fixes both spill offsets. Preserved the
  inherited documented raw-state view. Total: 3 functions, 6,176 bytes.
- Full Docker build after the first three matches: 27/27 checksums OK.
- DATE `func_80133EA0`: MATCH, 972 linked bytes. Added flat park/shop table
  declarations and stride notes to DATE.h. Correct one-argument dialogue
  call resolves the final scheduling differences. Total: 4 / 7,148 bytes.
- Scratch permutation now resolves symbol aliases before scoring; confirmed
  baseline scores 120 (dialogue) and 10 (advance) against regdiff results.
- DATE `func_80136340`: MATCH, 868 linked bytes. Structured January–February
  branch replaces m2c's over-shared goto. Added 36-byte concert table rows
  to DATE.h. Total confirmed gain: 5 functions / 8,016 bytes.

2026-10-17 cleanup: Codex stopped at its usage limit; the work merged as 66d5a55c4. Seven functions are plain C on main: select_girl_init, func_8007C8D8, func_8007D8AC, func_80133EA0, func_80135E7C, func_80136340, func_80113CA0 (the last two were committed after the last comment above). Main builds 27/27 (T-20900 clean rebuild). Code review against CODING_STANDARDS.md found nothing blocking; the unsigned char casts are tracked in the cleanup followup. The largest leftover, the dialogue function, has its own ticket T-12500.
