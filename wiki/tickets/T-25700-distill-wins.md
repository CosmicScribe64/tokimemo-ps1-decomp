---
id: T-25700
title: Distill the wave 8-11 wins into playbook recipes
status: In Review
assignee: claude
created: 2026-10-23
updated: 2026-10-23
links: ["[[notes/wins-2026-10]]", "[[playbook]]", "[[register-cheatsheet]]", "[[matching-notes]]", "[[index]]"]
---

## Goal

Make every match of waves 8-11, the wave 9 closers, the stubborn-function attack and diman8's PRs #68 to #70 findable by later matching agents: one page with each match and its deciding change, and the reusable levers written as playbook recipes.

## Acceptance criteria

- [x] [[notes/wins-2026-10]]: all 93 functions (unit, function, bytes, ticket, symptom -> fix, recipe or one-off, link to the note), grouped by recipe, with a ranked summary at the top.
- [x] [[playbook]]: recurring recipes extended with the new examples; new recipes where none existed; FAKE constructs marked as FAKE.
- [x] [[register-cheatsheet]]: rows only for register symptoms that had no row.
- [x] [[index]] line under Matching.
- [ ] Review against CODING_STANDARDS.md (wiki-only change; the FAKE marking rule of section 7 is the one that applies).

## Notes

Sources: the extraction list handed to the synthesis agent (35 functions in full), the `## T-NNNN` sections of [[matching-notes]] and [[notes/matching-08]] for the rest, `git log` match commits since 2026-10-22 for the list itself, and ELF symbol sizes (`mips-linux-gnu-nm -S`) for the bytes. 93 functions, 125,820 bytes; GEKO `func_80132B24` was matched twice (PR #70, T-25401) and counted once.

Playbook, new entries (11): section 1 "The original passes more arguments than the callee reads"; section 2 "Split a sum to fix ugen's free-list order"; section 3 "The access path decides reloads, CSE and ranges", "A loop store that may alias stops a hoist", "A load sunk to its first use: a volatile member"; section 4 "Address sums", "A byte-global compare built constant-first by cvt_pass", "A pointer compare against a fresh `lui/addiu` of an array base is an index test"; section 5 "Packet builders: write the libgpu macros first"; new section 13 "Never-true arms and empty tests" (a table of the forms); new section 14 "Loop shape: index loops, counters, `for`". The access-path entry and sections 13 and 14 also gather examples that were spread over older entries.

Playbook, extended entries (19): sections 1 (implicit int, value-returning library views), 2 (ops as1 deletes, and the reverse), 4 (sum and compare orders), 5 (line grouping, volatile caveat narrowed, one symbol, data island, `#line`), 6 (zero chain), 7 (frame), 8 (pre-offset base), 9 (switch spelling), 10 (sticky state F), 11 (`s32` with no return), 12 (bit-field tests), and the T-25102 `.noalias`, T-25115 block-count, T-25201 equal-priority and T-25301 copy-propagation entries.

Register cheat sheet: 11 new rows (extra arguments, pre-offset base, priority by blocks, value moved past a loop guard, LFTR bound register, typed field pointer, load sunk to first use, per-block interference, input-pass constant, split sum, `for` against do/while).

Not changed: [[kanban]] and [[log]] (the orchestrator updates them).

## Comments

- 2026-10-23, review corrections (checked against the match commits, sources and notes): main `func_8004A1A0` uses indexed map stores, not a `map` pointer (the pointer after the split reached 4 rows); FAKE labels added for the ENDING `func_801387B0` identity spellings, the main `func_80067F04` and TEL `func_8013D88C` two-path splits, and the TAIIKU `func_80137784` T-25106 FAKEs that stay (its deciding change is plain C); a scan of every "plain C" row against its source also found FAKE marks in main `func_8004FC10`, `get_near_menu`, `sndi` and GEKO `func_80133024` (per-arm locals), now labelled; GYOZI `func_801368B8` moved out of the as1 one-symbol list into its own uopt-promotion sentence; the TEL example under "Two expressions" now names the F64 flag word; the strcpy sentence names the value-returning prototype as the lever; ETC `func_8013D570` cost and the `make_color_bar` offsets corrected; the wins page counts are stated as of a018bb0e9, and main `func_8007132C` (T-25318) and GEKO `func_80142C9C` (T-25403) are listed under "Matched after this page".
