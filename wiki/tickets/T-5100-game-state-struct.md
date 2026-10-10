---
id: T-5100
title: Recover the main game-state struct
status: In Progress
assignee: agent (worktree o-gamestate)
created: 2026-10-10
updated: 2026-10-10
links: ["[[game-state]]", "[[data-types]]", "[[tickets/T-5000-type-recovery-arrays-structs]]", "tools/migrate_globals.py", "config/migrate_globals.txt"]
---

## Goal

T-5000 found that the main-exe bss block around 0x800E6248..0x800E7400 behaves as one object that almost every overlay touches. Recover it as one struct: field offsets, types, sub-arrays and records from the original's access patterns, declared once in `include/main_api.h` with the base symbol the original used. Write `tools/migrate_globals.py` to rewrite every C use of an absorbed `D_` global into a field access (re-runnable for later waves) and a ninja check that rejects the old names. Apply it to the whole tree byte-identically, retry the load-hoist and unit-private functions that touch the block, and document the struct in [[game-state]].

## Acceptance criteria

- [ ] `GameState` declared in `include/main_api.h`, base symbol from the original's relocations, offsets checked by the tool
- [ ] `tools/migrate_globals.py` (apply, check, layout) with unit tests; ninja step `build/globals.ok`
- [ ] all C uses migrated; clean build 27/27 sha1 OK, headers OK, `sync_protos.py --check-branch` OK
- [ ] every use kept on an old view listed with its reason (`config/migrate_globals.txt`)
- [ ] load-hoist and unit-private functions touching the block retried; result recorded
- [ ] [[game-state]] page; [[data-types]] and [[decompile-workflow]] updated

## Notes

## Comments
