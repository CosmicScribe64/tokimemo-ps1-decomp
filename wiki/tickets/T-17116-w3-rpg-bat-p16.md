---
id: T-17116
title: Wave 3 package w3-rpg-bat-p16
status: In Review
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the three listed RPG_BAT functions, best first: `func_80156064` (856 B, `80155CC0.c`, T-17100 draft score 228), `func_80134E78` (1,268 B, `80134D30.c`, score 254) and `func_80135358` (1,700 B, `80134D30.c`, score 521). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17116 comment, others restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17116)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p16`, branch `w3-rpg-bat-p16`, base `w2-good`. Owns `src/ovl/RPG_BAT/80155CC0.c` and `src/ovl/RPG_BAT/80134D30.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 21:12 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80156064` | 828 | `INCLUDE_ASM` (T-17100 draft, score 228) | matched, plain C: `RpgObjectRecord44[147]` fields, index loops over records `i + 96` and the words `(&D_8015EBAC)[i + 50]` (FAKE first-symbol view), compound assignment on the word, `RPG_BAT_OVERRIDE_func_8013F0F4` |
| `func_80135358` | 1,700 | `INCLUDE_ASM` (T-17100 draft, score 521) | matched by the closer pass: plain C with `RPG_BAT_OVERRIDE_func_8013E97C` (implicit int view); FAKE `h + -h` and first-symbol words |
| `func_80134E78` | 1,268 | `INCLUDE_ASM` (T-17100 draft, score 254) | restored to `INCLUDE_ASM`; best draft in `drafts/T-17116/RPG_BAT/func_80134E78` (about 128 rows; constant stored and passed gap) |

2 of 3 matched (2,528 bytes; the second by the Opus closer pass). Method and findings: [[matching-notes]] (section T-17116), six new rows in [[register-cheatsheet]], two rows in [[data/t0018-cases]], [[rpg-bat-family-results]]. Header changes: `RPG_BAT.h` (`#ifndef RPG_BAT_OVERRIDE_func_8013F0F4` wrapper; externs `D_8015ECB8`, `D_8015ECE0`, `D_8015ED08`).

## Comments

### 2026-10-16 start and finish (claude-opus, branch `w3-rpg-bat-p16`)

Started 21:12 UTC. `func_80156064` matched at 21:35 UTC (commit "T-17116: match RPG_BAT:func_80156064"). `func_80135358` went from 521 rows to 10 differing lines (5 instructions) and stays guarded; `func_80134E78` has two early blockers and is back to `INCLUDE_ASM`. Moved to In Review; code review against `CODING_STANDARDS.md` pending.

### 2026-10-16 Closer pass (claude-opus, 22:32-22:45 UTC)

`func_80135358` matched (commit "T-17116: match RPG_BAT:func_80135358", guard removed). `regdiff` said that in the original an interfering range holds `$v0` at the `sra t3,v0,7` / `sra t0,v0,6` reads of the `func_800A0070` results, in the arm blocks where the range `D_8015EC68 - (h - D_8015EC6C) * 2` is recomputed right after `jal func_8013E97C`. That is [[register-cheatsheet]] row "a counter or stored value right after a call sits in `$v0`": `funcdiff --proto 's32 func_8013E97C();'` gave MATCH at once (`s32 func_8013E810();` and `s32 func_8013F0F4();` did not). `80134D30.c` now has `#define RPG_BAT_OVERRIDE_func_8013E97C` and the K&R `s32 func_8013E97C();`; no other function in the file calls it. The FAKE `h + -h`, the first-symbol words and the three declared locals stay as before. `void func_80135358` does not build (`func_80134D30` calls it earlier as an implicit int). `func_80134E78` was not in the closer's list and stays `INCLUDE_ASM`; a later attempt has to test its draft with the new file-local `func_8013E97C` view. Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` OK. Ticket stays In Review.
