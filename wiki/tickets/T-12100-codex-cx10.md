---
id: T-12100
title: Match DATE movie and effect functions
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[build-system]]"]
---

## Goal

Byte-matching pass on the twelve assigned DATE functions, prioritizing the large movie/effect family and reusable type/data ownership repairs. Owned C files: src/ovl/DATE/801461B0.c, 8014A1B0.c, and 8014CC40.c.

## Acceptance criteria

- [ ] Every promotion has literal linked-byte and DATE SHA1 verification.
- [ ] All assigned functions attempted; only ticketed near misses below about 30 rows retained.
- [ ] Final configure/full build passes 27 checksums, headers, globals and prototype checks.
- [ ] Standards review before Done.

## Notes

Baseline Docker configure and ninja -j2 pass 27/27 checksums, headers and globals. Evidence: build/scratch/baseline.log. Worktree cx10 only; cx3 and main are frozen/read-only. No toolchain changes, host dependencies, merge or push.

The existing 489D0 draft has a documented data-ownership/shared-LUI blocker. Unlike its earlier batch, this package permits changes to config; investigate the established 8015CF80..8015CFD0 data range first. External matching research was read for scratch-only AS1 diagnostics and equal-score/full-file candidate verification; production proof remains unchanged.

## Comments

- 2026-10-15: package claimed after successful baseline.

- Matched `DATE:func_801489D0`: 2,068 literal linked bytes, production DATE SHA1 and header/global checks pass. Added the established 8015CF80..8015CFD0 data island, first verifying its assembly pieces unchanged, then defined the four known s16[3] coordinate/velocity arrays in the owning C file. Their zero initializers and aligned padding reproduce the original bytes. Shared LUI initialization now matches without any function-body trick. Existing DateMovieScene50 layout and array declarations were reused. Logs: build/scratch/data-island.log, island-build.log, 489d0-before.log, 489d0-owned.log, 489d0-build.log and 489d0-final.log.
