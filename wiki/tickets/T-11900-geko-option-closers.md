---
id: T-11900
title: "GEKO and OPTION: match the remaining 14 functions (outside-contributor claim #65)"
status: In Progress
assignee: claude (diman8 fork)
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24140-geko-option-tel-omimai-wave-3-closers]]", "[[playbook]]", "[[build-step-gaps]]"]
---

## Goal
Outside-contributor claim CosmicScribe64/tokimemo-ps1-decomp#65: the 11 GEKO and 3 OPTION functions left at
`ff6c770b2` (14,152 bytes). Taken off [[tickets/T-24140-geko-option-tel-omimai-wave-3-closers]] for the claim; TEL and
OMIMAI stay with T-24140. Method as in [[tickets/T-11880-date2-remainder]]: one Opus agent per group with a long
budget, two at a time in separate worktrees, never two on the same C file.

1. GEKO `801323E0.c`: `func_80133024` (280), `func_80132B24` (480), `func_80133308` (656)
2. OPTION: `func_801326BC` (`80132000.c`, 516), `func_80139A30` (`801389A0.c`, 1036), `func_8013A7EC` (`8013A550.c`, 1092)
3. GEKO `80134D10.c`: `func_80136C20` (604), `func_801367EC` (832), `func_80135DC0` (2260)
4. GEKO `80142760.c`: `func_80144198` (216), `func_80142C9C` (3656)
5. GEKO `80134040.c`, `801338F0.c`, `80141AF0.c`: `func_80134808` (432), `func_80133C50` (836), `func_801420E4` (1256)

## Acceptance criteria

- [ ] All groups run; matches committed one per commit, near misses guarded with `T-11900` (under about 30 rows)
- [ ] Inline review against `CODING_STANDARDS.md` (FAKE comment on every fake trick)
- [ ] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
