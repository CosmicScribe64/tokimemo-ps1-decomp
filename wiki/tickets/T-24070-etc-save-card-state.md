---
id: T-24070
title: Match ETC save-card state machine
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-20900-shared-type-unification]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80144F74 (608 bytes) in src/ovl/ETC/80143240.c. Preserve every other body, including archived T-24055 helpers. Read NAME_ENT func_8013F758 solely as a matched reference; do not edit contributor-owned NAME_ENT.

## Acceptance criteria
- Read current wiki, prior T-17300/T-20900 and target negatives before edits. Establish a fresh valid current-type full-TU baseline.
- Same sole Sol medium worker/compiler, existing built checkout on codex/complete-etc-card-state from f76548701, Docker only and ninja -j1; no additional agents or broad sweep.
- Confirm ownership: T-24055 is a frozen source-only In Review archive, no active TU worker found; current app inventory has only parent and this matching chat active. Parent assigns function-level scope.
- Use target assembly and actual matched sibling's structure/types, one concrete source change at a time. Coordinate exact interface/config scope before editing shared files.
- Actual C needs unguarded default literal proof, ETC checksum/interface guards and exact unchanged-neighbor audit; parent independently reviews/all27 before integration.

## Notes
Matched NAME_ENT func_8013F758 (556 bytes, sibling ratio0.770) shares save-card status cases1/3 and known GameState110D/1118/1120/1115 fields. T-20900 plain46 rows improves old115, but retained scalar aliases require current-type normalization. Original target uses v1 step-byte allocation and two capped byte-array updates in case0. Verify target-specific case conditions rather than assume full sibling identity. T-24055 closed source-only scope concerns other functions4354C/43758/43968 in this TU; preserve their production ASM and all other functions. TT/DATE2/NAME_ENT writes remain excluded.

## Comments
Selection and rationale sent to parent before edits. T-24069 published BFFC820-byte C and provenance remain preserved; T-24067/68 unresolved archives receive no probes. No target compile or source edit before this ticket setup.

Fresh current-type full-TU baseline117 rows (historical T-20900 scalar-alias46 is a different context). Consistent GameState pointer improves47, with cases1–3 exact; case0 two capped-byte updates remain. One shared-byte counter is49; promoting that same local to word is neutral49, both rejected without a width sweep. Production and every neighbor remain unchanged. Next source-led form models the first array element as an actual pointer, matching the original retained address; no guessed aliases/header/config changes.

Follow-up explicit element pointer47 is neutral. Byte-index111, raw-index captures49, reused pointer103 and required signed call-argument capture49 are rejected. Ordinary direct array cap/increment code, removing redundant byte-capture locals, improves39 and fixes second cap's a0/call schedule. Best source-only body/replay/provenance retained under drafts/T-24070; replay independently reproduces39 and production equals pre-task source exactly. Remaining first-cap index/address/counter color rotation and masked-index copy require causal evidence, not a broader source sweep. Existing unchanged tracer is a proposed next observation with mandatory same-input fidelity; no new diagnostic build or actual C proof. In Progress.

Parent-approved unchanged trace on39 input passes exact full-object/four-section fidelity before interpretation (input73c73dcb..., objectd597f00b..., audit helper retained). Synthetic mask local bit16 priority3 takesv0 before retained index22 priority1.5/v1; first address76 and counter77 priority1.5 takea0/a1. These are observed best colors, not per-range masks. One ordinary inline-mask source105 restores scratch/copy but index remainsv1; retaining actual element pointer110 changes +53C access to offset0/extraaddiu, both rejected. Thus priority alone does not establish index availability. Best39 remains; production unchanged. Retained source controls and trace_fidelity.py document exact hashes/commands. Parent next scope is unchanged-tracer comparison on already-tested105 input after this pin, mandatory same-source fidelity; no new binary or width/color probe.

Approved unchanged-tracer105 comparison passes same-input entire-object/four-section equality (input7c5a2f24..., object446a2ed1...). Common global mask expression becomes bit80 priority3/v0; retained index23 priority2/v1. Removing named mask local therefore leaves a competing expression range. No per-color availability inference or new binary. Next ONE proposed source form expresses only conditional reload as unsigned modulo16 instead of &15, same byte/index domain, to test separate pre-allocation expression ranges; pending parent scope, not yet compiled. Best39 and production unchanged.

Parent approves exactly that conditional reload modulo16 control after this fidelity pin; first mask and all other choices fixed. Reject if neutral/worse or added operations. Mark FAKE if retained solely for expression-web codegen. No width/color/base sweep or new diagnostic binary.
