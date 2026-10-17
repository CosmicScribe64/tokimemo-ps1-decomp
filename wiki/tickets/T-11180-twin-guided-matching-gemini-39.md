---
id: T-11180
title: Twin-guided matching (Gemini) 39
status: Done
assignee: gemini
created: 2026-10-14
updated: 2026-10-17
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

2026-10-17: Cleanup pass. 25 of the 50 listed functions were matched and merged on 2026-10-14 (BUNKAKEN 12, BUNKASAI 9, NAME_ENT 2, TACO 1, EVENT 1) and are still plain C on main. The other targets got no T-11180 commit; the family waves and the dupes/neardupes fills took them (T-11150 filled 10 jump-table twins), and the T-16500 whole-tree sweep of 2026-10-16 found only 4 twin plans left, so the BUNKA_SD group func_801336AC, func_80136B3C, func_80137CAC is plain C now. Review against CODING_STANDARDS.md: copies of matched donors, no FAKE, NON_MATCHING or asm lines, no finding. Closing as Done.
