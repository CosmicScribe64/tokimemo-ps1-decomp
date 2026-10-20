---
id: T-24514
title: Reconstruct remaining MASTER functions
status: In Review
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[families/master-family-results]]"]
---

## Goal

Investigate remaining MASTER functions with bounded causal source reconstructions.

## Acceptance criteria

- Literal default C and exact linked bytes for any match claim.
- All27 checksums and interface guards; code-review standards/spec self-review before Done.
- Preserve a narrow source-only handoff for unresolved targets.

## Notes

Base dba33c4f5, isolated /private/tmp/tokimemo-t24514, branch codex/t24514-master. Ownership src/ovl/MASTER and include/ovl/MASTER.h. Avoid main_api.h edits while root MAIN worker owns shared prototype work. Docker only, immutable disc mount; j2 maximum and no duplicate compiler workload.

## Comments

- 2026-10-20: Read wiki index first, local guidance, status/playbook, standards, function catalog and MASTER family histories. Existing344E0/363F4 large copy/scheduler histories inform target selection; start with matched-twin35FB8 and table-heavy326B4 source evidence.

- Docker MASTER baseline SHA1 OK. Existing35FB8 hand body, updated to current DisplayFixActor44/ScheduleClubFlags, scores58 aligned. Separate first-loop end marker adds a precondition guard and scores148; do-loop marker removes it but returns58. Absolute initializer62; absolute endpoints150. These controls do not separate the early s2 base lifetime. Production remains unchanged.

-363F4 reconstruction uses its own cases0..3 and late check_end_k conditions/actions; T-24048 donor344E0 is an archived nonmatching draft, not a matched function. Its typed shared case4 loop source gives aligned420. Indexed clone loop363, indexed point loop424. Frame remains72 versus target64, and first copy setup begins with one rotated temporary; source is still nonmatching. Final35FB8 explicit advancing pointer reused as shuffle base remains58.

- Removing the single-use raw RNG result local after indexed-clone reconstruction recovers the original64-byte frame and improves363F4 aligned363 ->342 (pointer clone equivalent433). First213 instructions align exactly; first miss is the aligned-copy end temporary t2 versus t3, then copy/shuffle/late-loop ring differences. Clone loop uses bne instead of the original pointer sltu/branch, and live spill homes differ. Byte/integer address roundtrip of the pre-copy byte store is a bounded alias-classification control, not a toolchain modification.
- Old correctly-framed326B4 source now scores538; its known raw-halfword versus shifted-selector promotion blocker remains. No new lead justifies repeating its earlier45-callee sweep or source grids.

## Source handoff

`drafts/T-24514/` retains only two valid narrow function bodies, one replay driver and a source/probe README. No original assembly dumps, full translation-unit context, invalid scratch variants or compiler changes. Final copy-state controls are byte view342/no gain and integer-address view439/worse. Default production source remains unchanged. Per root instruction, no repeat full27 build is needed for archive-only work: base dba33c4f5 carries parent-verified production provenance, and this lane verifies its MASTER unit and interface guards.

## Self-review

Applied the local code-review skill standards/spec axes directly, preserving the explicit worker cap and self-review instruction. No reviewer agents spawned.

Standards: retained bodies compile with current shared headers;363F4 only needs its valid overlay forward declaration in replay context. No C99, inline assembly, SDK redeclarations, dead experiment source, invalid C or generated whole-TU context retained.35FB8 inherited scalar-byte FAKE view is marked with reason/ticket. Only source/draft/wiki material staged; production diff empty. No original instruction dumps, game data or build products retained.

Spec: bounded function-by-function causal hypotheses with measured negative controls. Original363F4 conditions/actions retained;344E0 donor explicitly nonmatching. No claim of a C match or compiler impossibility. Remaining objective unresolved, research checkpoint will sit In Review.

## Verification

- Narrow retained-body replay compiles both sources:35FB8 aligned58,363F4 aligned342; no match.
- MASTER default binary checksum passes against original disc. Parent verified production provenance at baseline dba33c4f5; no broad checksum rebuild repeated for unchanged production, per root instruction.
- Header guard passes. Global guard initially lacked five existing DATE/TACO proof references because only MASTER was split; generated those two reference splits without compiling their C, then globals OK.
- Prototype branch guard:0 new disagreements,12 known. Three stale baseline entries are noted absent because their other reference units were not generated; no config change made.
- replay.py passes Docker py_compile. `git diff --check` passes; src/include/tools/config diff is empty. Prose review read README fully and removed two weak colon-title patterns.
