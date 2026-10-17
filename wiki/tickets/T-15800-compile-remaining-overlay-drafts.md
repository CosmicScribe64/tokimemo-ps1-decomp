---
id: T-15800
title: Compile remaining assigned overlay drafts
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[tickets/T-14501-remaining-draft-proof]]", "[[tickets/T-14500-exhaustive-drafts]]"]
---

## Goal

Produce complete compiling C alternatives for all 23 assigned nonreserved failed scopes from the frozen T-14500 best catalog: GYOZI 5, OPTION 5, SHOUGATU 6, SHUGAKU 3, MASTER 1, NAME_ENT 3. Matching optimization is out of scope.

## Acceptance criteria

- [x] All 23 scopes compile as complete full-TU C alternatives, or have a precise documented blocker.
- [x] Archive body, declarations, exact TU/context, diagnostics, hashes, score and provenance under drafts/T-15800 with catalog.jsonl.
- [x] No empty stubs, inline assembly, or target ASM fallback counted as compiling C.
- [x] Original archives and production matched C unchanged; Docker only, one serial compiler.

## Comments

2026-10-15: Claimed renewed user request through root. Use frozen 48362009 source/header snapshots and retained/historical alternatives where available; reconstruct genuine C/types otherwise. Reserved unaligned repairs remain in other lanes.

2026-10-15: All 23 assigned scopes compile as complete full-TU C alternatives: GYOZI 5, OPTION 5, SHOUGATU 6, SHUGAKU 3, MASTER 1, NAME_ENT 3. Final serial Docker replay saves exact preprocessed input, complete compiler diagnostics, full-TU resolved scores, source/header/compiler hashes and provenance. `drafts/T-15800/audit_archive.py` passes 23/23: final body/context/preprocessed hashes agree, every target has a C definition and compiled instructions, and no target ASM or M2C_ERROR placeholder remains. No match or coverage gain is claimed; production sources are unchanged.

Repairs: measured copied callback structs replace integer arrays called as functions; existing return views resolve K&R declaration conflicts; real four-by-eight coordinate and seven-halfword threshold arrays replace incomplete locals; byte pointers replace illegal void-pointer arithmetic; known Word4 copy views replace scalar/struct assignment mismatches. Caller result-register views are explicit and documented where retained helper declarations use void. Root's generation lane reconstructed the two NAME_ENT phantom-argument failures after inspecting func_80054E6C: it initializes all incoming registers before use and takes no arguments. Those two compile with scores 847 and 501; the original M2C_ERROR attempts remain immutable.

Archive: drafts/T-15800/catalog.jsonl and per-scope body.c/declarations.h/context.c/preprocessed.c/diagnostics.txt/result.json/provenance.json, plus earlier attempts, repair scripts, preparation and audit drivers. Frozen common headers remain referenced at drafts/T-14500/baseline/include. No objects, game binaries or assembly files are included. Final evidence: /private/tmp/t15800-final-replay.log and the archived catalog. Status In Review for root's archive/completeness review.

2026-10-17: Closed after review with no findings. All 23 assigned scopes compile as archived full-TU alternatives with hashes and diagnostics, production C is unchanged, and nine of them are now plain C on main through later matching waves; the rest were redrafted by the types-first passes. The archive carries no instruction rows or assembly, so nothing needs scrubbing.
