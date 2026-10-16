---
id: T-16930
title: "Read aggregate fields through a local pointer: kept copies and constant-first tricks"
status: In Progress
assignee: claude-opus (pensive-montalcini-7151d4)
created: 2026-10-15
updated: 2026-10-15
links: [[[tickets/T-16920-gamestate-1093-type]], [[tickets/T-16910-cpw-census]], [[register-cheatsheet]], [[matching-notes]], [[game-state]], [[data-types]]]
---

## Goal

[[tickets/T-16920-gamestate-1093-type]] matched the NAME_ENT selector-copy family by reading GameState through a local
`GameState *g = &D_800E6280;`: an indirect load is not a plain variable for uopt, so it keeps the original's
constant-first `==`/`!=` and the `move` copy of the field. Apply the same reading to:

1. The other 23 unmatched functions of the T-16910 census whose original keeps a used `move` copy of a load of an
   aggregate member (GameState 0x800E6280..0x800E7D10, the `D_8011ECD0` record table 0x8011ECD0..0x80121750, indexed
   arrays). Read the field through `GameState *g`, or a record pointer for the table.
2. The five matched constant-first compares that still need a trick: ENDING `func_8013855C` (`FAKE` `s16 one`),
   NAME_ENT `func_80148C20` (`FAKE` byte constant), BUNKA_SD `func_8013261C` (`FAKE` `^ 0`), EVENT `func_80117004`
   (`FAKE` `(u32)`), RPG_BAT `func_80150CD8` (`(u32)` cast). Replace a trick only if the function stays byte-identical.

## Acceptance criteria

- [ ] The 23 functions listed with the census scripts; each tried with a pointer read; matches committed as plain C
      (no new `FAKE` unless marked and ticketed).
- [ ] The five tricks tried; each replaced only when the function stays byte-identical.
- [ ] Unit sha1 and 27/27 sha1, headers and globals OK.
- [ ] Results and misses here and in [[matching-notes]]; log entry; ticket In Review.

## Notes

## Comments
