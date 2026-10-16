---
id: T-17900
title: "Types-first: NAME_ENT"
status: In Review
assignee: claude (w3-name-ent-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[name-ent-family-results]]"]
---

## Goal

Types-first pass for the NAME_ENT overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover NAME_ENT's shared
data types into `include/ovl/NAME_ENT.h`, redraft every unmatched NAME_ENT function with them (drivers adapted from
T-16550, T-16560 and T-17700 into `drafts/T-17900/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit NAME_ENT`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/NAME_ENT.h`; every matched NAME_ENT function still matches.
- [x] Every unmatched NAME_ENT function redrafted and scored full-TU; drafts in `drafts/T-17900/NAME_ENT/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-17900)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/NAME_ENT/*.c`, `include/ovl/NAME_ENT.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 32 unmatched NAME_ENT functions (30,504 bytes) in 4 files, 9 of them guarded drafts.

## Comments

### 2026-10-16 claude (w3-name-ent-types)

- Types (`include/ovl/NAME_ENT.h`): flag counters `u8 D_8014D060[16]`, `u8 D_8014D158[16]`, digit sprite (u, v)
  `u8 D_8014D0F4[10][2]`, name-entry buffers `D_8014CC74[8]`/`D_8014CC7C[8]` sized. The main-exe records
  (`NameMetricWork` at `D_800E7D10`, `NameRecordF00[]` at `D_80123120`, `DisplayActor44[]` at `D_8011ECD0`) are
  typed only in the m2c context and read through casts, as the matched code does. NAME_ENT sha1 OK.
- Redraft: `drafts/T-17900/redraft.py` (from T-17700) and `hand.py`; 21 unguarded functions not matched here:
  14 better than the census (19,372 bytes), 7 the same, 0 worse; sum 7,056 to 6,107 rows, median 271 to 195.
  `func_80140F90` and `func_80141820` compile without repair (no-argument `func_80054E6C` in the m2c context).
  No draft scored 0.
- Matched: `func_80139740` (port of main `func_8004B19C`) and `func_8013EE7C` (sibling `func_8013EBEC`), 904 bytes.
- Packages: T-17901..T-17904 (17 functions, 11,316 bytes), table in [[name-ent-family-results]].
- `func_80142A80`'s drafts write its song lines as rodata labels, so the song text is not copied into the repo.
