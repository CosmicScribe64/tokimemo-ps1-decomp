---
id: T-24051
title: Recover DATE large table dispatcher with new source evidence
status: In Progress
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
Archived small dispatcher and libgte work are reference-only. T-24043 semantic/ownership archive remains frozen; T-24038 wins are published on main f55a2d7e9. Root reports current main f089f1f89 at 81.1% (6,500 functions / 1,849,648 bytes), including published TEL 2,128-byte gain; this lane does not re-count those as its own progress.
