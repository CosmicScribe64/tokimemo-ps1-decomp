---
id: T-12090
title: Match cx9 BUNKA_SD functions
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal

Deep matching pass over ten assigned BUNKA_SD functions, prioritizing verified byte gains.

## Acceptance criteria

- [ ] Exact linked and overlay checksum proof for every match.
- [ ] Retain only measured guards within about thirty differing rows.
- [ ] Full configure/build and prototype verification; independent standards review before Done.

## Notes

Owned files: src/ovl/BUNKA_SD/80132000.c, 80134260.c and 801388D0.c. No main or other worktree edits. Deadline 21:45 UTC.

## Comments

2026-10-15: Read wiki index, project/local instructions, coding standards, workflow and register cheat sheet. Searched historical evidence for all ten functions. Baseline Docker configure and ninja -j2 pass 27/27 SHA1 checks, headers and globals. Prior small fade loops have known allocation gaps; prioritize the 1164/1604-byte functions and their setup family after checking the old score-zero dispatcher.

### func_80132428: MATCH, 448 text bytes

Canonical GameState selector/timer access and six missing overlay forward declarations make the old score-zero dispatcher compile in its complete translation unit. Plain C, no FAKE. Exact production linked comparison and BUNKA_SD SHA1 pass; headers/globals pass. Worklist size512 includes the64-byte jump table.

### func_80132FF0: MATCH, 532 text bytes

Restored explicit case4/default from the five-entry jump table; canonical player-name arrays and strcat declaration replace old draft aliases. Full linked text MATCH and BUNKA_SD SHA1 pass, with headers/globals OK. FAKE: 24-byte name-buffer capacity was chosen to reproduce observed stack homes; the original source capacity is unknown. Worklist552 includes20-byte jump table. Cumulative980 matched text bytes plus84 table bytes. Added overlay prototypes and typed display-data declarations used by ongoing larger-family reconstruction.

### func_80132940: MATCH, 1144 text bytes

Reused the palette/setup loop index, restored case4/default from the five-entry jump table, and represented the intermediate frame number as u16 before subtracting2 into the record halfword. The narrow unsigned temporary preserves the original arithmetic/update order without extra instructions. Cleaned raw offsets to DisplayActor44 and canonical calls. No FAKE. Exact production linked comparison and BUNKA_SD checksum pass, with headers/globals OK. Cumulative2124 text bytes plus104 table bytes.

### Larger-target scheduling diagnostic

func_80138D48 reaches6 rows with typed DisplayActor44 access and the texture assignment before counter/index updates; all238 instructions otherwise agree. A private copy of frame_pass applies the unchanged transform, then invokes native as1 with -R -xbbdbg8. Stock and diagnostic full object files compare byte-identical, and the captured native log contains8958 lines including the reached loop DAG. Direct -Wa flags failed because frame_pass interpreted the numeric8 argument as another input; no production tool was edited. At the remaining discrepancy, loop-bound constant6, index halfword store and -120 materialization all have two-cycle priorities; current as1 picks original emission order. Bounded line grouping, condition width and simple loop forms did not yet close it. Automated wider stores improved raw scores but changed semantics and were rejected.

### func_80138D48: MATCH, 952 bytes

Typed DisplayActor44 records, a32-bit frame induction variable and unsigned byte stores repair the saved draft. Assigning the image metadata before the counter/index updates fixes the large scheduling gap; putting the index increment in the for clause fixes the final constant scheduling. Keep i=2 before frame=15, outside the for initializer, to preserve load order. Removed unused m2c locals and raw offsets. No FAKE. Exact linked comparison and BUNKA_SD checksum pass; headers/globals OK. Cumulative3076 verified text bytes plus104 jump-table bytes.

### func_80133204: MATCH, 820 text bytes

Restored the fifth switch entry, replaced the shared m2c goto with separate if/else assignments, and used typed word-table indexing for voice records. Natural string-table indexing preserves the match after removing the synthetic byte-offset temporary. Exact production linked comparison and BUNKA_SD checksum pass, headers/globals OK; no FAKE. Cumulative3896 text bytes plus132 table bytes (worklist848 includes28 rodata bytes).
