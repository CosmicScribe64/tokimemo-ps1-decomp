---
id: T-17108
title: Wave 3 package w3-rpg-bat-p08
status: Done
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the six listed RPG_BAT functions, best first: `func_8014E994` (452 B, guarded draft, score 8), `func_8014AB84` (240 B, guarded draft, score 12), `func_8014AC74` (348 B, guarded draft, score 36), `func_8014F278` (216 B, draft score 67), `func_8014EF8C` (244 B, draft score 69), `func_8014EBEC` (344 B, draft score 61). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17108 comment, others restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17108)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p08`, branch `w3-rpg-bat-p08`, base `w2-good`. Owns `src/ovl/RPG_BAT/8014E780.c` and `src/ovl/RPG_BAT/8014A660.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 19:36 UTC, finished about 20:25 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_8014E994` | 452 | guarded draft (T-10850, 8 rows: two swapped `lui` pairs) | matched; one shared counter for the three clear loops, third loop on one line, element-index views for the `$at` groups (FAKE comment) |
| `func_8014EBEC` | 344 | `INCLUDE_ASM` (draft score 61) | matched; same loops, `for (i = 40; i < 64; i++) rec[i + 96].unk_01 = 5;` on one line, `(&D_8015ED98)[1]` (FAKE comment) |
| `func_8014EF8C` | 244 | `INCLUDE_ASM` (draft score 69) | matched; same loops and record loop, `(&D_8015EDC4)[3] = 0;` keeps the first store ahead of the load (FAKE comment) |
| `func_8014F278` | 216 | `INCLUDE_ASM` (draft score 67) | matched at the first compile; `(&D_8015EDF0)[3]`, `[0]`, `[1]` (FAKE comment) |
| `func_8014AC74` | 348 | guarded draft (T-10320, 16 listing lines; 36 rows by `funcdiff`) | matched; typed `DisplayActor44` records, slot words inline as element indexes, no `ent`/`rec`/`p` locals, `for (i = 0; i < 4; i++)` (FAKE comment) |
| `func_8014AB84` | 240 | guarded draft (T-10320, 12 rows) | matched; element-index stores `(&D_8015EBAC)[arg0 + 0x32]` instead of byte-offset casts (FAKE comment) |

6 of 6 matched (1,844 bytes; about 45 minutes), no guard left in the two files. Method and findings: [[matching-notes]] (section T-17108), [[register-cheatsheet]] (five rows), [[data/t0018-cases]] (update paragraph, seven old rows solved), [[rpg-bat-family-results]]. No header change. Siblings: none above ratio 0.70 for any of the six; the matched `func_8014F540` supplied the shared-counter idea.

## Comments

### 2026-10-16 start and finish (claude-opus, branch `w3-rpg-bat-p08`)

- Matched in order: `func_8014E994`, `func_8014EBEC`, `func_8014EF8C`, `func_8014F278`, `func_8014AC74`, `func_8014AB84`; one commit per function.
- The two register-only functions fell to a one-run variant tester (many edits of one function compiled in a single container run, 0.5 s each) after `autotweak` (202 variants) and `regdiff` had not moved `func_8014AB84`.
- Verified in the worktree: clean `rm -rf build && configure.py && ninja` 27/27 sha1 OK, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK (0 new, 15 known).
- Awaiting code review against `CODING_STANDARDS.md`.

### 2026-10-16 code review (claude-opus, reviewer)

Reviewed `w2-good...w3-rpg-bat-p08` against `CODING_STANDARDS.md`: no findings.

- Only `src/ovl/RPG_BAT/8014E780.c` and `src/ovl/RPG_BAT/8014A660.c` changed in `src`; no header, config, `drafts/` or asm change. No `NON_MATCHING`, `INCLUDE_ASM`, inline asm or `.word` left in the two files; all six functions are plain C.
- The six FAKE comments state what each trick does and name T-17108 (shared loop counter, flat word views, first-symbol element stores, one-line loop bodies). The unrecovered aggregates (`D_8015EBAC` slot table, `D_8015EDC4`, `D_8015EDF0`) are called out as such, per section 8.
- C89 (declarations at block top, `/* */` only), ASCII, fixed-width types, typed `DisplayActor44` fields instead of byte-offset casts.
- Rebuild in the worktree: `ninja` up to date, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK (0 new, 15 known). Clean 27/27 sha1 OK reported by the matcher; the merge script repeats a clean configure and full ninja.
- Ticket status matches the kanban column (Done).
