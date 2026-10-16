---
id: T-17104
title: Wave 3 package w3-rpg-bat-p04
status: Done
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the one listed RPG_BAT function of `src/ovl/RPG_BAT/801468B0.c`: `func_80146C30` (2,992 bytes of code, a 36-case switch of the Stampede / hermit battle script). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best draft `drafts/T-14500/RPG_BAT/func_80146C30` (score 329), typed variant `drafts/T-17100/RPG_BAT/func_80146C30` (329).

## Acceptance criteria

- [x] Matched function is plain C (FAKE statements commented), unit sha1 OK, committed
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17104 comment, others restored to `INCLUDE_ASM`
- [x] Notes in [[matching-notes]] (section T-17104); compiler-gap rows in [[data/t0018-cases]] if any
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p04`, branch `w3-rpg-bat-p04`, base `w2-good`. Owns `src/ovl/RPG_BAT/801468B0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 19:19 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80146C30` | 2,992 | draft T-14500 / T-17100 (score 329, 764 against 745 instructions) | matched in plain C (one FAKE: `(&D_8015ECEC)[i]` first-symbol view in a rolled 3-trip loop): `RPG_BAT_OVERRIDE_func_8013E7C0` and `_func_8013E97C` implicit `s32` views move the `D_8015EDE0` web to `$v1`; m2c temporaries replaced by `D_8015EDE0 += n;` |

1 of 1 matched (2,992 bytes of code, about 15 minutes). Side result: the guarded `func_80145CC0` (T-12300) matches under `funcdiff --proto` with the same two overrides; `80145CC0.c` was outside this package and is not edited. Notes: [[matching-notes]] (section T-17104), [[register-cheatsheet]] (two rows), [[data/t0018-cases]] (update line), [[rpg-bat-family-results]].

## Comments

### Code review (Opus, 2026-10-16)

Checked the diff `w2-good...w3-rpg-bat-p04` against `CODING_STANDARDS.md`. Scope: only `src/ovl/RPG_BAT/801468B0.c` changed among the C files (no header, config or `drafts/` change); no inline asm, `.word` hacks, game data or binaries. `func_80146C30` is plain C, C89 (declaration at block top, `/* */` ASCII comments, Japanese text only in string literals as in the neighbouring RPG_BAT files). The one fakematch (the `(&D_8015ECEC)[i]` first-symbol view in the rolled 3-trip loop) carries `FAKE`, the reason and T-17104. The two `RPG_BAT_OVERRIDE_*` defines sit before the includes with a reason and the `s32 f();` declarations follow, as section 8a asks (same shape as `801454F0.c` and `80156870.c`). No `NON_MATCHING` block in the file, so none to check. Commits carry the ticket id and the trailer.

No findings. Verified after a forced rebuild of the unit: `tools/docker.sh ninja` unit sha1 OK, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK (0 new, 15 known). Result: 1 of 1 matched (2,992 bytes). Done.

Follow-ups for other packages (not blocking, files outside T-17104): (1) the guarded `func_80145CC0` (T-12300, `80145CC0.c`) matches under the same two overrides per `funcdiff --proto`; it needs the two `RPG_BAT_OVERRIDE_*` defines, the two `s32 f();` declarations and the guard removed. (2) The FAKE first-symbol view can go once `D_8015ECEC..D_8015ECF8` become an `s32` array in `include/ovl/RPG_BAT.h` and the clears in `8014A660.c` are rewritten.
