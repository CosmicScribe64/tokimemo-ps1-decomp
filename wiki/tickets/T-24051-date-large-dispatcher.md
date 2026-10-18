---
id: T-24051
title: Recover DATE large table dispatcher with new source evidence
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[date-family-results]]", "[[tickets/T-24011-date-dispatch-family]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover func_80144E20 (approximately 3,200 instruction bytes) in exclusively owned src/ovl/DATE/80141EE0.c. No other DATE target is assigned; preserve all existing bodies and the smaller dispatcher fallback.

## Acceptance criteria
- Read current wiki and all T-17207/T-24011 source/negative evidence before new probes.
- Reuse built /private/tmp/tokimemo-complete-etc on codex/complete-date-large-followup from frozen T-24043 6be3243b1; retain all ancestry.
- One GPT-6.1 Sol medium compiler total, Docker only, ninja -j1; bounded novel source/semantic-structure experiments rather than blind sweeps.
- Report any necessary shared-header/config scope before edits. TT and DATE2 remain excluded.
- Pin useful source/reproducer corrections and actual C wins promptly. Actual C must remove guard/INCLUDE_ASM and pass explicit default linked bytes, checksum and header/global/prototype guards; root owns independent review/integration/push.

## Prior evidence and novelty gate
T-17207 improved the original table-copy draft; T-24011 fixed all GameState reads to use g and reached exact frame 0xE38/all 21 table offsets, with 72 full-unit added/removed rows. Source: drafts/T-24011/func_80144E20.c. Remaining redundant case-0/case-20 branch layout, branch displacements, symmetric compare order and padding are documented. Exhausted forms include explicit first/last defaults, goto joins, flipped comparisons, selector/byte views, initial pointer changes and overall s32 return view. Do not repeat them without new evidence.

New hypotheses to evaluate: callback pointer return/ABI view (distinct from overall dispatcher return, informed by recent return-liveness matches), actual C table/subobject pointer expressions and their source/control-flow implications, and fresh verification that table-selection branches have the recovered semantics. Treat any ABI/codegen-only construction as FAKE if promoted, with explanation; no compiler/assembler exception.

## Comments
New source checkpoint `drafts/T-24051/func_80144E20-4rows.c` fixes twelve of fourteen reversed pairs using the exact EVENT assignment-in-comparison idiom. The real comparison byte is declared after base, preserving frame 0xE38, all 21 table offsets and base home. Four diagnostic rows remain at source selector !=1 / selector ==1 tests (rows 411/495); all other instruction words agree. Captures are tagged FAKE. Safe replay is `probe.py --variant capture-below`. The earlier 28-row body and all production source remain unchanged while focused work continues.

Latest user steering keeps this ticket In Progress after the transfer checkpoint: continue only func_80144E20 with reduced usage and bounded evidence-driven source hypotheses. ETC work is deferred; no additional agents or compiler-pass patches. The failed transfer controls are frozen separately before further DATE work, and the 28-row source remains unchanged.

Bounded EVENT recipe transfer complete without gain. Selector neighbor D_800CA2F4 + 4 preserves u8 width and exact frame/table offsets but gives 44 rows: original 14 reversed pairs plus eight new selector pairs. BF44 + 4, the movie-work byte at +0x1C, and their grouped control retain 28. Byte-shade captures with ordinary/const g give 132, retain frame 0xE38, shift all table offsets and base spill -4, and do not fix the original pairs. Six named controls are retained in `drafts/T-24051/probe.py`; unknown neighboring aggregate ownership is FAKE. The 28-row body is unchanged, production is untouched, and ticket/card return to In Review before ETC work resumes.

Reopened for a bounded recipe transfer from the parent Astra EVENT investigation. Parent reports that `_constant_first` distinguishes direct global-backed comparisons from constant-only ranges; a neighboring-base byte read fixed remaining EVENT selector pairs without a compiler patch. Proposed DATE selector view is `((u8 *)&D_800CA2F4)[4]` (0x800CA2F4 + 4 = 0x800CA2F8), preserving its existing unsigned-byte load. This is an explicitly FAKE address view of unknown aggregate ownership. If needed, grouped controls use BF48 from BF44 + 4 and the existing movie-work fade byte at D_800B5920 + 0x1C. No header/compiler/config edits are authorized or proposed. Preserve the exact-frame 28-row source checkpoint.

Source checkpoint improves 72 to 28 validated full-TU rows with zero new verified C bytes. Remove the pre-switch initializer and assign table 0 in an explicit default arm, keeping cases 0 and 20. This restores both redundant switch branches, all displacements and terminal padding. Earlier empty-default controls kept the preinitializer, so this form was not exhausted. `drafts/T-24051/` retains the improved body, safe target-only full-TU reproducer and table audit. Only 14 symmetric branch-operand words remain: two selector comparisons, one BF48 comparison and eleven fade comparisons. No new FAKE is introduced beyond the preserved T-24011 pointer-home tag.

The audit verifies 21 contiguous source tables totaling 0xE00 bytes, stack interval +0x34..+0xE34, all source-copy extents, and exact selector 0..20 mappings. No semantic mapping correction was found. Callback return and aggregate/word-table representation controls retain old 72; selector-derived offsets worsen to 720. Against new 28, unsigned casts/constants and the real movie subobject view do not improve; signed counter/BF48 views, XOR-zero and real constant-one homes worsen allocation/frame. README records precise controls. Both direct-cc and padded/trimmed diagnostics confirm the new 28-row checkpoint.

Production src/include/config are unchanged from setup 824c6fddc; no necessary header or config scope arose. Any C promotion would still require the previously documented BF48/callback declarations and actual default linked proof. T-24043 archive is now published at cec8f5843; prior T-24038 matches remain published at f55a2d7e9. Archive delta awaits independent review.

Metric context: the early private search helper reported 71/71 for the unchanged old baseline because it supplied terminal padding; validated direct-cc full-TU replay reports the historical 72 rows. The assigning-default checkpoint is 28 rows in both contexts. README distinguishes the older representation table's direct-cc totals from private helper cost/row counts.
Archived small dispatcher and libgte work are reference-only. T-24043 semantic/ownership archive remains frozen; T-24038 wins are published on main f55a2d7e9. Root reports current main f089f1f89 at 81.1% (6,500 functions / 1,849,648 bytes), including published TEL 2,128-byte gain; this lane does not re-count those as its own progress.

## Root archive replay

Independent Docker replay on integrated current main: audit.py validates all21 table entries, contiguous0xE00-byte source extent and stack+0x34..+0xE34; probe.py --variant assigned-default reproduces28 resolved +/- rows and confirms production unchanged. This validates the retained handoff, not a default C match. Target remains ASM pending branch-order closure.
Closure candidate: consistent selector byte view D_800CA2E8 + 0x10 and captures of 17/16 reach zero resolved-word differences with exact frame/table layout. Both source idioms carry FAKE provenance. Canonical callbacks are FD6C/40788 void(void), 407D0/40D38 s32(void), supported by actual definitions or original argument/return paths. Literal replay exposed missing linker export D_800CA2E8; parent approved its single absolute export and narrow DATE.h declarations. Production promotion and default literal proof remain pending.

Actual C checkpoint: target fallback replaced unconditionally; 3,200 linked bytes MATCH under explicit default/resolve/no-build. Whole DATE checksum passes after canonical headers rebuild all callers. Header/global guards PASS; prototype branch check zero new, fifteen known. Only target body changed in its TU; smaller fallback preserved. Approved header declarations and one absolute existing-symbol export are included. All three FAKE rationales are visible in production. Ticket/card move to In Review for root's independent two-axis review; no push or full completion claim.

## Root independent completion review

Reviewed full source against CODING_STANDARDS: C89, existing table types/fields, actual unguarded target, explicit FAKE selector/capture/home notes, narrow canonical declarations and absolute symbol export without allocation. No inline ASM or compiler changes. Independent Docker literal default proof3200bytes, all27 SHA1 checks, headers/globals/prototype guards pass. Earlier diagnostic/archive notes are historical; target is now matched C. No unresolved findings; Done.
