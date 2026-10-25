---
id: T-27061
title: Prepare next bounded main topic-header extraction packet
status: Done
assignee: parallel-header-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[spec/header-split-plan]]", "[[tickets/T-27054-topic-header-tooling]]"]
---

## Goal

Produce an executable, independently reviewed next extraction patch and exact declaration/include
manifest in ignored scratch, using the integrated owner-aware tooling and current logical source paths.
Root owns tracked application, matching proofs, shared log and board registration.

## Acceptance criteria

- [x] Bounded topic ownership established from current definitions, declarations and actual type needs.
- [x] Extraction patch, manifest and separately documented registration-only tooling change if needed.
- [x] No new prototypes, changed types/guards, lost overrides, or altered original object boundaries.
- [x] Current 437-unit strict snapshot equality and Docker guard/tests pass on the scratch packet.
- [x] Independent Sol review resolved before primary integration; no live implementation edits/builds.

## Comments

2026-10-24: Claimed on root's explicit continuation request. Source filenames are being migrated by
root; scratch copying must validate a stable source/config/header snapshot and avoid old basename
assumptions. Existing 34 prototype omissions and six GYOZI compatibility views remain invariant.

2026-10-24 inventory decision: next bounded owners are main/game_mode.h, main/pad.h,
main/holiday.h and main/ovl.h. Selected set is 77 existing declarations: 58 function prototypes,
18 initialized-data declarations and the separately evidenced main-bss `ovl_entry` declaration.
The entry pointer remains a main-only symbol at 0x80123110; the loader assigns it and the game-mode
loop invokes it. No symbol/config rename is part of extraction. Defer func_80079800 (the mixed loader
object's memory-card/system-file routine) and 13 old rodata compatibility labels pending provenance
review; absence from current owning C references is not evidence of unused status. All selected
signatures need only common.h, with no typedef/enum move or new prototype. Source direct includes
will follow actual symbol references in current main C files; overlay C paths remain untouched.

2026-10-24 copy guard: rejected two unstable snapshots while parallel proposal authors wrote drafts.
Unapplied config/renames/proposals tables are excluded from subsequent proof-copy inputs because
the header/symbol/aggregate view tooling does not consume them. All source/header files, live build
configs/symbols/object maps/ledgers and relevant tooling remain guarded by before/copy/after digest
equality. No partial packet from rejected snapshots is accepted.

2026-10-24 concrete packet: `01-register-topic-owners.patch`, `02-extract-main-topics.patch`,
`manifest.json` and `manifest.md` now exist under ignored T-27061 scratch. Stable 808-input snapshot
at HEAD `c3053b47caab9689cb9fabfd898eb049bdcdb289`; digest
`930f11212a9f41ec6a79aa279e05ee2a8046cfab3f6a5993089b79197fd680c5` matches before/copy/after.
Exactly 77 declarations are selected (owner counts 6/15/43/13), with 28 direct include edges added to
19 current logical main C files and no overlay C changes. Existing API minus 77 and the exact new
leaf sets are asserted; all raw types and non-include C text remain unchanged. Independent Sol review
found no implementation issue so far; manifest writer evidence was corrected to include
game_init.c:func_80041840 for ovl_entry. Final proof and review remain pending.

2026-10-24 resource steering: root requested one heavy Docker job per chat. Stopped only this lane's
redundant 355-suite test container and the reviewer's two duplicate read-only verification containers,
after confirming mounts/commands. The packet author's single sequential proof/test container continues;
no result is claimed for the intentionally cancelled duplicate suites. Root's live plan/progress/build
containers were not touched. Tools/assets is also excluded from snapshot copying because its separate
asset work is not a header-proof input. File reading and manifest/body review continue without extra
Docker jobs.

2026-10-24 final reviewed handoff: both split patches are complete under
`.claude/orchestrator/state/T-27061/`, alongside exact manifest.json/manifest.md and proof.json/proof.log.
Registration SHA256 `86b3dbc7bc99c9918b1eec64690b219fab62deb3781900d35a516b916ac8caf4`;
extraction SHA256 `98c97d21729b8abf74adff3de0104324bfca75ad99c5ed3920227bc948224239`.
All 437 source views remain exactly 0 benign/0 check/0 risky after extraction and after --fix;
--fix leaves all copied source/header bytes unchanged. All 77 owner assignments, 34 prior plus 18 new
prototype omissions, and 6 prior plus 6 new compatibility views are preserved. Of the new compatibility
views, pad func_800433D0 retains (s32 arg0) and the five holiday entries retain (). All 355 tests pass
(23 header, 95 sync, 78 rename, 3 srcscan, 24 migrate_globals, 67 dupes, 65 bulk_m2c), with header,
symbol, rename, aggregate guards and both patch consistency checks exit 0. No configure/clean/game
build was run in this lane.

Independent Sol 6.1 review inspected the exact patches, manifest, capture/proof scripts and author's
hash-validated Docker outputs, with no unresolved findings. Its one manifest-only correction added
game_init.c:func_80041840 to ovl_entry's writer evidence. Duplicate independent Docker jobs were
stopped under resource steering; the final proof results come from the single sequential author job,
not a claimed independent duplicate run. Root applies registration then extraction, takes a fresh
snapshot using current YAML paths after any overlay file moves, and runs strict views/guards and
27/27/6,952 matching proof at its actual integration revision. Root owns shared log/board transition;
worker leaves In Progress to avoid a kanban mismatch. Packet preparation is complete and reviewed;
live integration and the broader phase-2 header/type work remain root's next actions.

2026-10-24 actual strict integration complete: fresh r16 combined197-file packet67355f07 is independently approved by the original Astra reviewer and applied byte-for-byte to its prepared tree. Both umbrellas are absent; all437rawviews match before semantic fixes. The combined concrete Astra repairs pass437mappedviews/0changes,169tooltests and clean27/27matching6952functions2276920bytes; original reviewer accepts this bounded actual integration for local commit. This completes the extraction/ownership packet scope. The explicitly separate rink type correction, logical paths and broader phase2 semantic acceptance remain open under final-review gates. Evidence: ignored T-27061/live-integration/, T-27056/live-integration/clean-proof.json and T-27055/live-integration-acceptance.md.
