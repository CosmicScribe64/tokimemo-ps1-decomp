---
id: T-26003
title: Model IDO 4.1's value-branch F and dropped-arm stores in tools/ugen_pass.py
status: Done
assignee:
created: 2026-10-24
updated: 2026-10-26
links: [[[tickets/T-26000-ugen-block-copy-41-rule]], [[tickets/T-26002-fake-sweep-41-rule]], [[toolchain]]]
---

## Goal

The T-26000 rule leaves two measured differences from IDO 4.1's ugen: 4.1 sets F at a branch on a value, and it ignores an indirect store inside an arm ugen drops as never entered, while the pass counts it. Four matches (MASTER func_801344E0, func_801363F4, main strCallback, func_8005938C) depend on a never-true FAKE arm because of that (T-26002 census). Measure the remaining shapes in 4.1 (value branches, stored compares, Uxjp, Uijp, func_8006B0C8's 4-byte compaction copy), and if a uniform rule fits every matched procedure, adopt it and remove those arms.

## Acceptance criteria

- [ ] 4.1 measurements for the open shapes, as synthetic cases in tools/ido_eval_cases/ugen_copy_state/.
- [ ] A rule change only if all 27 sha1 stay unchanged across the corpus, with the four FAKE arms removed or a written reason why they stay.

## Notes

Low priority: the build is already byte-identical; this only removes four FAKEs.

## Comments

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
