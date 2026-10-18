---
id: T-24072
title: Match ETC model initializer with verified cursor recipe
status: In Review
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-24055-etc-short-helpers]]", "[[tickets/T-24064-etc-count-trio]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8014354C (248 bytes) in src/ovl/ETC/80143240.c using the subsequently verified func_8013F980 recipe. Preserve every other function and the unresolved T-24070/71 ASM bodies.

## Acceptance criteria
- Unique ticket and parent ownership release checked before edits. Parent confirms prior T-24055 worker completed clean/no compiler; current single-target lane owns TU.
- Read current wiki and prior13-row T-24055 controls. Fresh faithful baseline first, then target-specific adaptation one source change at a time.
- Same sole Sol medium worker/compiler and built checkout from5b221cb12 on codex/complete-etc-model-init, Docker only/ninja -j1. No additional agents or broad source-line sweeps.
- Preserve actual map801F9004, calls/no NOBJ print, record0x50/object0x10 strides and SVECTOR tail vz/vx/vy. No assumed header changes; coordinate exact shared scope if needed.
- Actual C needs default unguarded literal248-byte proof, ETC hash/interfaces and exact neighbor audit; parent independently reviews/all27 before integration.

## Notes
T-24055 archived13/13 pointer/for model draft; literal argument108/12, byte pointer13, vector chain15 and array view13 did not match. New evidence postdates that archive: matched T-24064 func_8013F980 uses integer TMD cursor, real TaiikuBig/u32 do-loop and measured initializer/do grouping. Target shares this loop/address split but differs in map address/no count print. Transfer semantic recipe, not arbitrary padding or source grouping; explicitly annotate genuine codegen scaffolding if required.

## Comments
Parent approved exclusive target after read-only rationale and releases its old archive ownership. Primary ce538a96a includes final T-24071 archive; original target remains ASM. Earlier unresolved packets remain intact with no new probes. No edit/compile before this setup.

Fresh valid current full-TU baseline reproduces13 resolved rows. One adaptation of verified3F980 integer cursor/TaiikuBig/u32 do-loop improves to5; prologue, map cursor and loop agree. Residual is SVECTOR vz/vx/vy tail base/store grouping. Production/neighbors/interfaces unchanged. No old chained-zero/array-view controls repeated; known aggregate rule precludes silently restoring scalar50140 alias.

Source-only checkpoint under drafts/T-24072 retains baseline13/new integer-recipe5 plus neutral vz*=0 and exact two-field vy=vx=0 controls. Tail is3 AT/LUI groups versus original2 sharing vx/vy, with ra reload schedule affected. Original248B; draft adds one setup instruction. Known SVECTOR preserved; no unsupported scalar alias/keep exception. No new faithful source form established or impossibility claim; In Progress pending focused coordination.

Parent-approved single post-vz real SVECTOR*rotation control is neutral5, with complete full-TU object identical to integer-recipe. Source/replay retained as rotation-pointer.c. Read-only review suggested this one form with limited confidence; no further pointer sweep/instrumentation. Production unchanged, best5.

## Final handoff
Unresolved/In Review, not Done. Best source5 is preserved with baseline13 and all neutral controls; production remains ASM. The new integer-cursor recipe closes prologue/model-loop gaps, but the SDK vector tail still adds an address setup. No supported untried source control is established; no further4354C probes/instrumentation. No compiler-impossibility claim or verified-byte gain.
