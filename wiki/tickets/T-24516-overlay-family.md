---
id: T-24516
title: Recover remaining BUNKA_SD EN_NICHI KANGEI functions
status: In Review
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[playbook]]", "[[notes/functions]]"]
---

## Goal
Recover literal matching default C for the two BUNKA_SD, two EN_NICHI and three KANGEI remaining functions, with bounded source-backed reconstruction and durable negative evidence where blocked.

## Acceptance criteria
- Verify any promoted literal C through linked bytes, full 27 checksums and declaration guards.
- Preserve known blocker boundaries; no blind grids, compiler patches or inline assembly.
- Review standards and scope before handoff; no integration, push or shared-root writes.

## Notes
Fresh worktree /private/tmp/tokimemo-t24516, branch codex/t24516-overlay-family, exact base eaf7c6e709cb85435c313ac2fbfde5c7db14cd93. Existing Docker image, original root disc mounted read-only. Own only BUNKA_SD, EN_NICHI and KANGEI source and overlay headers. Read wiki index/guidance/status/playbook and function histories before experiments.

## Comments
- 2026-10-20: Claimed seven remaining functions. ETC source-only archive remains separately In Review.

### Source reconstruction findings
- Three-overlay fresh Docker build passes. KANGEI func_80134724 and func_8013852C retain their documented two-row scheduler blockers; no repeated grids or unsupported new controls.
- KANGEI func_801327DC: repaired obsolete GameState fields, record-flag accesses and renamed background loader. Current default full-TU target274/candidate270 words,270 printed rows,264 aligned cost. Int return neutral; consistent GameState pointer260 cost. Still broad load/range/control differences.
- BUNKA_SD func_80139290: current guard380 words/123 rows/cost versus379 target; explicit frame word local gives378/87/87 and restores shared0x70000000 constant. Signed/unsigned elapsed locals89 cost; signed frame with explicit unsigned guards/divisions419 cost, rejected. Remaining copied switch range and register allocation differ.
- BUNKA_SD func_80134D94: original0x4C2 test skips the action when selector==3; current guard uses opposite polarity. Original first draw coordinates(-160,-120) are constants, not uninitialized q/b. All three original draw calls put priority2 before color on the stack; the guard swaps those arguments. Corrected semantic source restores original243-word length, frame0x48 and frame registert0; aligned cost284 to144 (printed rows368 due equal-length diff metric). Sibling promoted-byte recipe worsens274; int-return146. No promotion or byte-gain claim.
- EN_NICHI func_8013735C: real actor records98..102, sentinel table and20-record fade replace obsolete offsets/phantom timer-init arguments. Three-scalar draft552 words/453 rows/cost versus575 target. Broad loop/arithmetic/promotion gaps remain; no equivalence proof yet.
- EN_NICHI func_80136DEC: corrected wrong alpha source(record96 byte7, not record0 byte0), wrong inner dispatcher field(110D, not unk0), current asset pointer types and copied spill aliases. Typed pointer draft354/472/466 versus348 target. Matched menu-setup indexed-table recipe improves347/319/317. Original table stack positions agree; candidate lacks one persistent saved byte cursor and has frame0xF0 versus original0xF8. One source-backed byte-cursor control follows.
- Final source-backed EN_NICHI byte-cursor control worsens317 to453 cost, rejected. KANGEI actual bitfield views worsen264 to280; a real halfword club local is neutral280. All bounded hypotheses stopped. Retain five reconstructed bodies and exact replay; two known scheduler guards unchanged. No old block-copy pass experiments or impossibility claims.

### Self-review and validation
- Standards: no findings against CODING_STANDARDS.md and local code-review skill. Private drafts use C89/fixed-width types, current arrays/records, no invented padding/FAKE shaping, no SDK duplicate/inline asm, no game data or generated compiler output. No production guard/header/compiler changes.
- Scope: no function matched. Five retained corrected source bodies, rejected controls and known scheduler boundaries remain durable; no coverage or impossibility claim. No push, integration/shared-root writes or other units touched. In Review for coordinator review.
- Exact audit: `git diff eaf7c6e709cb85435c313ac2fbfde5c7db14cd93 -- src include config tools` is empty.
- Fresh full633-step Docker ninja-j2 completes all27 checksum targets. Independent built-binary hashes pass27/27. Header/global guards pass; prototype branch0 new/15 known. README deslop weak technical-word hit reviewed and retained.
- Final retained-body replay: all five compile as actual default full-TU C and validate explicit-object DIFF with exact README metrics; production unchanged. No compiler failure or missing verdict is counted as a result.
