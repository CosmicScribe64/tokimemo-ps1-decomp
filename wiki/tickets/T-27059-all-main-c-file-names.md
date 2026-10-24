---
id: T-27059
title: Name every main executable C file
status: Done
assignee: parallel-main-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Review and prepare logical role names for all remaining main C files, reusing the reviewed34-object map and original object boundaries. Every old address-only basename must be covered by an executable file row or a concrete unresolved blocker; no numeric fallback names.

## Acceptance criteria

- [x] Complete source-file census and role-backed proposed mappings, unique case-insensitively per folder.
- [x] Separate supported file-only tables, no mixed symbol/header-extraction/literal edits.
- [x] Independent review and dry plans pass.
- [x] Parent applies with declaration-view and clean matching-hash proof.
- [x] All34main C files have logical basenames; the all-overlay gate is tracked by T-27060 and remains open.

## Comments

2026-10-24: User explicitly requires understandable names for all game-code C files and asked to start now. Root is sole source applicator; worker prepares/reviews new filename tables only. Existing reviewed symbol batches remain frozen.

2026-10-24 census/preparation: current YAML C entries, original object map and filesystem agree on 34 main C objects; applied main-files ledger accounts for scheduler.c and school_day.c, leaving exactly 32 address basenames. Three disjoint Luna passes checked 10 early, 13 middle and 9 late objects against actual bodies. New exhaustive supported file-only table: config/renames/proposals/layout-main-all-files.tsv (32 rows), full 34-object evidence mapping in [[spec/file-naming-main]]. All 34 final stems are case-insensitively unique; no address/numeric fallback remains in the planned layout. The prior seven reviewed filename stems are reused unchanged and both applied filenames omitted from the new table. Concrete corrections include background_images rather than opaque dec_bg, debug_display at 80059A20, boot_assets at 80059B40, reload_executable at 80056300 and sound_driver at 80079B10. Mixed-object notes record non-dominant helpers rather than implying a split. Independent Sol review and exhaustive Docker plan are running; this lane has not changed sources, headers, YAML, symbols, ledgers, shared wiki files or earlier proposal/spec outputs.

2026-10-24 final reviewed delivery: independent Sol 6.1 review PASS for the exact layout-main-all-files.tsv 32-row table and [[spec/file-naming-main]] 34-object mapping; no remaining naming findings. Read-only coverage audit confirms 32/32 remaining address stems, no extras/omissions, 34 unique final basenames, both applied names and all 7 previous subset mappings preserved. Current definition inventory is 830 total / 755 in moving files. Exhaustive Docker tokimemo-decomp-arm64 --cpus=2 rename.py plan with ledger main-files exited0, 32 rows, 69 affected files, no PROBLEM; ignored log .claude/orchestrator/state/T-27059-main-plan.txt. New-page/table notes retain body evidence for mixed objects, CD-level helpers, TT/system-file work and non-display girl-state helpers. No tracked source/header/YAML/symbol/ledger/raw-data changes or apply/build/clean/commit occurred in this lane. Root now owns application/view/hash proof, source review and shared wiki index/status/board/log updates; ticket remains In Progress until that execution is complete. Table SHA256: ad5732c7550d82505e9873996f229622b08ab59d06d88129a98ca31a87dc6198.

2026-10-24 root closeout: applied32moves via rename.py; all34main C objects now have logical names. Full437declaration payloads are exact after mapping only32path keys in a copy of the original snapshot:0/0/0. Clean Dockerbuild exits0:27/27checks and6952/6952functions,2276920bytes; independent hash reread27/27. Independent post-apply Standards and Spec reviews confirm unchanged bodies/ranges/config boundaries and only approved path references. The sole stale-doc finding is corrected. Main object/prefix README is separately reviewed and prepared for the documentation commit. Root closes this main-only ticket; broaderphase2/T27060 remain open.
