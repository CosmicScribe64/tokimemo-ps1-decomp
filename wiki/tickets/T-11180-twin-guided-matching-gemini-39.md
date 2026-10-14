---
id: T-11180
title: Twin-guided matching (Gemini) 39
status: In Progress
assignee: gemini
created: 2026-10-14
updated: 2026-10-14
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match functions from twin-guided batch list 39 (`g39-list.txt`): 24 twin rows with a matched sibling and 11 twin groups without a matched member yet (50 target functions total across BUNKAKEN, BUNKASAI, EVENT, NAME_ENT, TACO, BUNKA_SD, KANGEI, DATE, GYOZI, SHOUGATU, GEKO).

## Acceptance criteria

- [ ] Copy matched twins and fix differing constants/symbols for `twin A <- matched B` rows
- [ ] Decompile the first member of each `group` row and propagate to its twins
- [ ] Guard any near misses with `#ifdef NON_MATCHING` and `INCLUDE_ASM` fallback
- [ ] Commit after every match
- [ ] `tools/docker.sh ninja` passes 27/27 OK and headers OK

## Notes

## Comments
