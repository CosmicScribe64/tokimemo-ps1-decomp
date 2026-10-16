---
id: T-17117
title: Wave 3 package w3-rpg-bat-p17
status: Done
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the one listed RPG_BAT function, `func_8013CC90` (928 B, `8013CC90.c`, T-17100 typed draft, score 157). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] A matched function is plain C (FAKE statements commented), unit sha1 OK, committed at once
- [x] An unmatched draft under 30 differing lines stays under `#ifdef NON_MATCHING` with a T-17117 comment, otherwise restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17117)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p17`, branch `w3-rpg-bat-p17`, base `w2-good`. Owns `src/ovl/RPG_BAT/8013CC90.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 21:19 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_8013CC90` | 928 | `INCLUDE_ASM` (T-17100 typed draft, 157 rows) | matched, plain C with one FAKE (scrutinee view): `D_8015EBFC.unk_34` updated in place (promoted into `$v1`), `switch (sel = D_8015ED98 & 0x1FF)`, `u32` file view of `D_8015ED98` (`RPG_BAT_OVERRIDE_D_8015ED98`), scalar-view scrutinee `*(u32 *)&D_8015ED98 & 0xFF000000` (autotweak) |

1 of 1 matched, no guard left (about 15 minutes). Method and findings: [[matching-notes]] (section T-17117), four new rows in [[register-cheatsheet]], [[rpg-bat-family-results]], update and row in [[data/t0018-cases]]. Header changes: `RPG_BAT.h` (prototype `func_8013D0D4`, `#ifndef` wrapper for `D_8015ED98`).

## Comments

### 2026-10-16 start and finish (claude-opus, branch `w3-rpg-bat-p17`)

Started 21:19 UTC, `func_8013CC90` matched by 21:31 UTC (commit `T-17117: match RPG_BAT:func_8013CC90`). Clean rebuild 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK (see the log). Moved to In Review; code review against `CODING_STANDARDS.md` pending.

### 2026-10-16 code review (claude-opus, against `CODING_STANDARDS.md` section 13)

Diff `w2-good...w3-rpg-bat-p17` checked: only `src/ovl/RPG_BAT/8013CC90.c` changed among C files, no `drafts/` archive touched, no inline asm or `.word`, no game data, no `NON_MATCHING` guard left, C89 (declaration at block top, `/* */` comments), header changes consistent (`func_8013D0D4(void)` matches its definition and has no other declaration; `RPG_BAT_OVERRIDE_D_8015ED98` follows section 8a, reason on the `#define`, own type declared in the file, `#ifndef` wrapper in `RPG_BAT.h`).

One finding, fixed: the scrutinee `switch (*(u32 *)&D_8015ED98 & 0xFF000000)` is an address-taken view with no purpose except moving the `lui` constant to `$v1`. The project treats the same form as a fakematch (`func_80141280` T-19032, `func_80151C10` T-17102, [[register-cheatsheet]]) but the matcher left it unmarked ("plain C, no FAKE"). It now carries a `FAKE: ... T-17117` comment, and the ticket, [[matching-notes]], [[rpg-bat-family-results]], [[data/t0018-cases]] and [[register-cheatsheet]] say "one FAKE". Revisit if a plausible source for the `$v1` constant turns up.

Outside this ticket (not touched, owned by another file): `func_80155E80` in `src/ovl/RPG_BAT/80155CC0.c` uses the same `switch (*(u32 *)&D_8015EBAC)` view without a `FAKE` comment.

Rebuild after the fix: `tools/docker.sh ninja` OK (globals OK, headers OK, `RPG_BAT.bin` sha1 OK). Matched: 1 of 1 (`func_8013CC90`, 928 bytes). Ticket Done, card moved to Done.
