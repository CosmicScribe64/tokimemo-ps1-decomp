---
id: T-24523
title: Match seasonal overlay tails using new source hypotheses
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: []
---

## Goal

Match remaining GYOZI, VALEN, and SHOUGATU functions from baseline dba33c4f5 in an isolated worktree. Avoid exhausted scheduler/type grids and compiler patches.

## Acceptance criteria

- Literal default C matches verified with private linked byte proof, 27 binary checksums and declaration guards.
- Standards review resolves findings before Done.

## Notes

Scope: 16 remaining functions. Source/loop/aggregate hypotheses take priority over repeated broad grids. Docker image tokimemo-decomp-arm64, one compiler worker.

## Comments

- Started with wiki/playbook, prior function notes and standards review. No new matches yet.

### SHOUGATU func_80133B44 candidate

A signed-halfword cast of the first string-width product lowers full-unit resolved score from 82 to zero. The conversion is redundant before the surrounding signed-halfword narrowing, so as1 deletes its instructions while ugen spends two temporary slots. All later ring assignments match as a consequence. Neither u8/u16/u32/s32 variants nor s8 variants match; s8 gives 7/8 rather than zero. The two inherited unused scalar slots are now explicitly documented FAKE frame padding. Default fallback removed only for this function. Literal linked proof and full 27 binary checks underway.

GYOZI func_80140E0C: a bounded 24-variant alias-store grid (plain/byte/signed rereads; zero through byte or signed store, redundant store/re-read chains, xor-self, &=0, source from the earlier flag) leaves the base resolved score 9 unchanged or worse. No source change retained for this target.

### Verified match and review

SHOUGATU `func_80133B44`: **408 bytes matched** by literal default C with no INCLUDE_ASM fallback. Docker `python3 tools/funcdiff.py --resolve --version default SHOUGATU:func_80133B44` reports `MATCH (linked function bytes equal target (408 bytes))`. Private `ninja -j1 build/ovl/SHOUGATU.ok` passes SHA1 `d956b1df6fbf9d1f8261d7e1f98ea301f7eb66a9`.

All 27 binary hashes pass against committed config hashes: the modified SHOUGATU binary is privately rebuilt here; the other 26 use unchanged verified binaries from the root checkout mounted read-only at `/baseline`. This is explicit checksum provenance, not a claim of recompiling all 27 in this lane. A full fresh build was stopped at the coordinator's request to prioritize the private target proof. The root integration independently repeats full verification.

`check_headers.py`: headers OK. `migrate_globals.py --check`: globals OK; four TACO generated original-asm proof files use individual symlinks to the read-only baseline, DATE's proof was generated locally. `sync_protos.py --check-branch`: 0 new disagreements, 15 known, branch check OK. `git diff --check` passes.

Parent standards/source review: no blockers. C89 declarations, unchanged existing strings, no assembly/config/header/compiler edits, FAKE padding and redundant narrowing documented. Product narrowing retained as the modulo-equivalent form. Self-review agrees. Remaining 15 seasonal targets continue under this ticket.

### GYOZI func_80140F90 verified candidate and review

Recovered a three-local typed loop from the older fifteen-local decompiler foundation. Full-unit resolved costs: initial typed 145; `b1 >= 1` 140; redundant unsigned-byte status conversion 20; sum-operand preservation 18; common `GyoziWork` state-byte accesses 10; integer debug-call view 0. Explicit flag temporaries, signed narrowing, return width variants and empty status tests did not solve the flag register difference and were discarded. No uninitialized-variable tricks retained.

The shared declaration is exactly `void func_800BCDF0();` before and after, now inside `#ifndef MAIN_API_OVERRIDE_func_800BCDF0`. This translation unit defines that guard with its result-lifetime reason and declares `s32 func_800BCDF0();`. Other callers retain the original canonical void view. This patch does not change any other main-exe prototype. GYOZI's existing `GyoziWork` gains offset-documented fields for the directly observed bytes; `func_80141550(void)` is declared in the owning overlay header. Source contains no scratch externs or raw main-address pointer arithmetic.

Docker `python3 tools/funcdiff.py --resolve --version default GYOZI:func_80140F90`: MATCH, privately linked function equals all 788 target bytes. Private `ninja -j1 build/ovl/GYOZI.ok` passes SHA1 `43f92523116e7b6cd772d4397712a52eeda89703`. `check_headers.py`, `migrate_globals.py --check`, and `sync_protos.py --check-branch` pass (0 new/15 known prototype disagreements). Parent standards/source review has no blockers: override convention, field offsets/padding, C89, documented FAKE conversion and operand-order identity, owned prototype, unchanged existing debug text. Self-review agrees. Final 27-hash refresh uses private GYOZI and SHOUGATU plus 25 unchanged baseline binaries.

### Adjacent SHOUGATU source-only checkpoint

Applied the verified GYOZI selection-loop template to `func_8013E6E4`. Initial full-unit resolved cost218; scalar-before-table layout214; sequential retry/continue conditions187. Integer debug-call view improves the 214 form only to210. Bounded stride/explicit flag/status reuse/dead-count-as-stride experiments do not close the saved-constant, repeated-selector-load and downstream temporary differences. Source-only checkpoint in drafts/T-24523/SHOUGATU/func_8013E6E4; production remains INCLUDE_ASM and earns no coverage. No instruction listings archived.

### Further triage

The corrected typed sibling port for SHOUGATU `func_8013EA98` remains broad (resolved cost284), with copy/frame differences; no additional source archive or production change retained. The newly verified integer debug-call view does not close GYOZI `func_80138C9C`'s documented saved-register/rotation-loop tail. VALEN `func_80133088`'s current guarded draft is structurally stale versus the location/background target; the correct foundation already lives in drafts/T-19016 and its roughly190-row dispatch-layout tail is documented in [[notes/matching-05]]. No repeated broad grids run on those known scheduler/layout tails.

- Source-only E6 checkpoint review: parent and self review found no blockers. Clearly labeled as unmatched, inherited matching controls carry FAKE/ticket comments, no assembly listings or game assets, and no production code change.

### Stale VALEN draft cleanup and final handoff

Removed the incorrect NON_MATCHING `func_80133088` body only; retained its matching assembly fallback in original address order, with a concise pointer to the existing correct T-19016 source. Provenance: immutable target VALEN offset0x1088 length0x454, VRAM0x80133088..0x801334DB; next function0x801334DC. It classifies seed bits into location choices, calls func_800AE0F0 and selects text/background ids. Removed body computed a girl-stat sum and changed stats, and contained no such location calls. This was a structural stale-body error, not an unresolved register delta.

Private `ninja -j1 build/ovl/VALEN.ok` passes SHA1 `356aed2481987108f908d7013cc0392cea9dcc8c`. Header/global/prototype guards pass; final all27 hashes pass with private GYOZI, SHOUGATU and VALEN plus24 unchanged read-only baseline binaries. Parent and self standards/spec review found no blockers: assembly fallback retained, no header or compiler edit in this cleanup, no unverified coverage claim.

Batch outcome: **2 verified functions / 1,196 bytes** (`SHOUGATU:func_80133B44`408B and `GYOZI:func_80140F90`788B), one unmatched source-only checkpoint, one stale guarded-body cleanup. Fourteen seasonal functions remain unmatched. In Review for integration handoff; no additional searches queued.

### Focused new-donor follow-up

At the coordinator's request, compared immutable original opcode sequences for the two newly verified donors against only the fourteen remaining seasonal functions. Tokens use tools/siblings.py op_token (registers/immediates/targets erased), with donor-to-target SequenceMatcher ratio and no autojunk. No broad historical sweep.

| Remaining target | Best new donor | Opcode ratio | Outcome |
| --- | --- | ---: | --- |
| SHOUGATU func_8013E6E4 | GYOZI func_80140F90 | 0.770 | Already tested source-only cost187; excluded from repeat work |
| GYOZI func_801412A4 | GYOZI func_80140F90 | 0.745 | New direct transplant218; explicit flag219; neither matches |
| SHOUGATU func_8013EA98 | GYOZI func_80140F90 | 0.707 | Already tested broad typed port284; excluded from repeat work |
| All other remaining targets | GYOZI func_80140F90 | <=0.347 | No fresh close structural relative |
| GYOZI func_80140E0C / SHOUGATU func_8013E50C | SHOUGATU func_80133B44 | 0.291 each | Best remaining relatives still structurally distant; no transplant |

The GYOZI func_801412A4 experiment transferred the newly verified counted-bitfield spelling, named state fields and true bounded unknown byte-array accesses, removed the debug/status block and used the aggregate random-word field. Two causal forms only: direct reused flag expression and explicit u32 flag local. Both remain broad: work base shifts from original a3 to t0, the count-loop temporary sequence differs, and the random word hoists ahead of the retry loop instead of being reloaded each iteration. The donor's decisive integer debug return view and narrowed status-call result are absent from this target, so those wins do not transfer. Its older T-24301 source-only foundation (~100 reported rows) remains the better starting point; scores from different contexts are not claimed as a direct metric comparison. Worse transplant bodies discarded, no additional archive or production source edits.

Follow-up outcome: no additional matches or coverage. Parent notes-only review found no blockers (ratios and source-level descriptions, no instruction listings). Returned to In Review; no further compiler searches queued.

### Fresh VALEN location/background reconstruction

Read T-19016 and T-16930 before starting. In the current full-unit context, the correct T-19016 foundation scores189. Explicit forward case labels173, player-only unsigned-byte seed narrowing127, region-limited GameState pointer54. Removing the old single-use cur local does not improve54. Separate scopes restore frame0x30 but retain id spill+0x2C (50). A legal, explicitly FAKE union shares the disjoint selector/pointer phases and restores id spill+0x28 (14). The exact annotated archived text recompiles at14. Production remains matching INCLUDE_ASM; no header edits or coverage claimed.

Source-only checkpoint: drafts/T-24523/VALEN/func_80133088/body.c and README.md. Remaining differences are the third dispatch equality/inequality orientation (0x801330F8/0x80133100, target blocks0x8013314C/0x80133164) and argument/id-spill scheduling for the two copy calls at0x801331F8 and0x8013329C. README records precise source-level order, measured causal steps and negative bounded probes without instruction listings. No compiler-limit claim.

Negative follow-ups: nested one-case dispatch unchanged; explicit case3/default alias worse21; two physical id/call line groups, destination comma assignments, and named string-pointer ranges all remain14. A previously started13-single semantic-view pass found no gain; the final requested func_800AE0F0-only integer/pointer return checks also remain14. No callee views retained. No broad historical scheduler grids repeated.

- Fourteen-edit checkpoint review: parent and self found no blockers. C89; union active member written before use in disjoint phases; redundant byte override remains equivalent under the following &1; forward-label topology and FAKE controls are documented; scratch extern canonical-declaration warning explicit. Exact annotated body recompiled at14. No production/header changes, bulk instruction listings, target words or assets. Returned to In Review for this focused source/evidence handoff; verified coverage remains two functions/1,196 bytes.

### Candidate-list ownership and new MAIN donors

Read T-24116 before reconstructing GYOZI `func_80138C9C` (992 bytes). Original local accesses establish a thirteen-halfword list at sp+0x4C through sp+0x64, with final pointer spill at sp+0x44. They do not establish metadata at sp+0x48/0x4A or an encompassing local record. No local record was invented from unused stack space. The original keeps its shared signed-halfword index in s0 across gather, print, rotation and reorder stages, including the no-call rotation loop. Gather uses both a local count and independently loaded/stored global count around opaque callbacks.

Current full-unit baseline resolved cost103 remains best. Four bounded source forms: natural C89 stage scopes with separate index locals112 (frame0x70, list+0x54, rotation index v0); reuse the real gather-count scalar for the later disjoint rotation offset103 (unchanged); direct global candidate-work aggregate111; local typed global-work pointer150 (s6/s7 and frame0x78). The latter two were scratch proof only. No production or header edits retained, no new archive of worse bodies, no coverage.

The global aggregate hypothesis had address/width evidence: signed-halfword current at D_80147220+0, values[12] at+4 (24 bytes), signed-halfword count at+0x1C, unsigned-byte special at+0x20 and mode at+0x24; next known table begins+0x28. Unknown gaps remain byte padding. Original reload of count after each rotation values store motivated testing shared aggregate alias ownership. Direct aggregate still reloads that count but does not fix the s3/s4 swap, v0 rotation offset, early bound guard or hoisted second format pointer; it adds reloads in reorder branches. Local pointer adds saved-register/frame pressure. The scratch cast from the first old scalar is not proposed as a production declaration: any future successful aggregate must be owned by the overlay header and migrate all overlapping aliases/users.

Also compared immutable original normalized opcodes from new MAIN `func_80076000` (1196 bytes) and `func_800767A4` (932 bytes) against the fourteen remaining seasonal targets, using the same op_token/SequenceMatcher method as the prior donor pass. MAIN76000's maximum ratio is0.240 to VALEN33088 (next GYOZI412A4,0.238). MAIN767A4's maximum is0.230 to SHOUGATU3E6E4 (next GYOZI38C9C,0.216). All28 pairs are structurally distant; no source transplant compiled. MAIN76000's available source manages packed menu icons rather than the candidate gather/rotation dataflow. No broad historic sweep or exhausted scheduler grid repeated.

- Candidate-list/donor notes review: parent found no blockers in the precise deltas and raw-source scope. Self-review agrees; notes only, production unchanged, no instruction listings or coverage claim. Ticket stays In Progress for the newly assigned bounded VALEN32834 factoring hypothesis.

### VALEN ranking identity probe exposes stale guard

Read T-24101/T-24138 and indexed history before one assignment-only identity experiment. Current guarded VALEN func_80132834 compiles at resolved cost484; multiplying the first operand by1 in its three assignment sums (comparisons untouched) gives491. No source retained. The failure is structurally broader than the guard's historical CSE claim: immutable target_span confirms address0x80132834, length0x4D8 (1240), end0x80132D0B, padding0, with next function0x80132D0C. Target clears current/count/mode/special and tests girl9 state before a strict top-three ranking pass, then gathers, rotates and reorders a candidate list. Guarded body instead starts an eleven-entry0xFF list initialization, invokes func_8013387C before ranking, uses >= comparisons and a different final counting loop. Its historical near metric must not be used for the actual target.

Correct structural decompiler checkpoints already exist at drafts/T-14500/VALEN/func_80132834 and drafts/T-16100/batch-1/VALEN/func_80132834 (historical509, not current rerun). Target signed-halfword top scores live at sp+0x50/0x52/0x54; candidate list at sp+0x5C uses halfword stores. Original normalized VALEN32834-to-GYOZI38C9C opcode ratio is0.60215. Their gather/rotate/reorder stages share real structure, but VALEN has the initial ranking stage, lacks GYOZI's debug-print loops and retains reorder traversal instead of breaking. No additional compiler run or production cleanup yet; precise sibling-port hypothesis is being reviewed.

The single correct typed sibling reconstruction gives381 resolved edits. Exact annotated archive text in drafts/T-24523/VALEN/func_80132834/body.c independently reproduces381 after cleanup (scratch fallback replacement, literal C). Frame0x70 differs from target0x78 and ranking values/register lifetimes remain broad. List capacity13 is explicitly FAKE/uncertain; no invented local aggregate, declaration grid or coverage claim. Named GameState fields and CharFlags replace old scratch extern accesses without header changes.

Removed only the structurally incorrect guard, retaining original INCLUDE_ASM in address order with a concise source-checkpoint reference. Private `ninja -j1 build/ovl/VALEN.ok` passes SHA1 `356aed2481987108f908d7013cc0392cea9dcc8c`. All27 checksums pass (private GYOZI/SHOUGATU/VALEN, other24 unchanged binaries mounted read-only from root); headers OK, globals OK, 0 new/15 known prototype disagreements, branch check OK, git diff --check passes. Separate source/archive and cleanup commits await parent standards/source review. Ticket remains In Progress for active lane coordination; verified coverage remains1,196 bytes.

- Parent and self review of the correct381 source checkpoint and wrong-guard removal found no blockers: real CharFlags/strict signed-halfword semantics, explicit FAKE capacity uncertainty, precise target bounds, no header/compiler changes and unchanged full-unit/checksum/guard proof. Approved separate archive and cleanup pins. Next permitted probe is a real three-element s16 score array replacing only the independent score scalars, based on adjacent observed score homes.
