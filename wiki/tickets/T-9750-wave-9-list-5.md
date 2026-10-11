---
id: T-9750
title: "Wave 9: list 5"
status: Done
assignee: w9-5
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Match list 5 in the 36 assigned original objects, following the wave-9 brief.

## Acceptance criteria

- [x] First pass and deep attempts recorded; at least four hours of matching work.
- [x] Matching C verified by resolved function diff and 27/27 binary checksums.
- [x] Header/global checks and inline standards review pass.
- [x] Changes left uncommitted for the orchestrator.

## Notes

Started 2026-10-11 around 15:00 UTC. Work list: scratchpad/wave9-list-5.txt.
Scratch confined to scratchpad/w9-5. No tool/config edits or reviewer agents.

## Comments

- 2026-10-11: Read shared brief, wiki index, coding standards, workflow,
  register-allocation clusters and relevant earlier-wave matching notes.

- First-pass preparation: raw m2c output needs GameState migration and typed
  pointer-field repairs. Early drafts made before their assembly split exists
  are discarded and regenerated after baseline initialization. Historical rows
  alone do not count as wave-9 deep attempts.

- First pass complete: 123 drafts, 0 immediate matches, 16 usable DIFFs,
  70 compile/type errors, 37 malformed aggregate/table drafts. Detailed C and
  diagnostics in scratchpad/w9-5/firstpass.jsonl. Failed bodies restored.
  Deep pass starts with type repairs and the closest resolved diffs.

- ETC func_8013EEFC (344 bytes), repaired types and aggregates; resolved
function diff MATCH and unit SHA1 OK. Source src/ovl/ETC/8013BEF0.c.

- Type repair: all 70 error drafts recompiled with available aggregate views,
  fixed primitive pointer accesses, and overlay scalar declarations where the
  target load/store width is known. One verified match; rejects restored.
  Closest new candidates: ETC func_8013F498 (38 differing lines), RPG_BAT
  func_8013FF28 (40). Scratch captures each candidate and fresh resolved diff.

- ETC func_8013F498 (740 bytes), selector halfword u16; resolved
function diff MATCH and unit SHA1 OK. Source src/ovl/ETC/8013BEF0.c.

- Tool bug: permute.py strip_functions regex (line 200) hangs in setup on the
  SHOUGATU 80134120 preprocessed source; --time does not bound setup. Captured
  traceback by interrupting our own child process after over eight minutes.
  Scratch helper now bounds whole calls; repository tools unchanged.

- RPG_BAT func_8013FF28 (680 bytes), permuter candidate; review pending; resolved
function diff MATCH and unit SHA1 OK. Source src/ovl/RPG_BAT/8013F4F0.c.

- TACO func_80156244 (416 bytes), base pointer with targeted operand mask 6; resolved
function diff MATCH and unit SHA1 OK. Source src/ovl/TACO/80154960.c.

- ETC func_8013EB90 (332 bytes), result s32 index s32 cached 0; resolved
function diff MATCH and unit SHA1 OK. Source src/ovl/ETC/8013BEF0.c.

- ETC func_8014B0D0 (228 bytes), init and do grouped; resolved
function diff MATCH and unit SHA1 OK. Source src/ovl/ETC/8014ABD0.c.

- ETC func_8013F77C (516 bytes), loop body on header line; resolved
function diff MATCH and unit SHA1 OK. Source src/ovl/ETC/8013BEF0.c.


## Final results

Seven functions, 3,256 bytes. Matching work ran approximately 15:00–19:49 UTC
on 2026-10-11 (4 hours 49 minutes); review/checks finished around 20:00 UTC.
First pass covered 123 drafts; all 70 compile-error drafts received type
repairs. Deep passes prioritized fresh resolved diffs and iterated control
flow, aggregate views, widths, homes, operand order and scheduling. Remaining
bodies are restored to INCLUDE_ASM; no claim that every function is solved.

| Unit | Function | Bytes |
|---|---|---:|
| ETC | func_8013EEFC | 344 |
| ETC | func_8013F498 | 740 |
| RPG_BAT | func_8013FF28 | 680 |
| TACO | func_80156244 | 416 |
| ETC | func_8013EB90 | 332 |
| ETC | func_8014B0D0 | 228 |
| ETC | func_8013F77C | 516 |

Nine new rows in [[data/t0018-cases]]. Closest remaining scheduling cases:
SHOUGATU func_801419D0 (7), RPG_BAT func_801495DC (12), MouseState (14).
Top non-T-0018 blocker: InitMouse's extra LUI/shared-$at ownership gap;
config/objects/main.txt gives object 80059710 no data range. Another blocked
case is ETC func_80140BC4's pre-existing void declaration, which cannot be
changed under the append-only overlay-header scope.

Tool bug: tools/permute.py strip_functions regex at line 200 can hang in
setup on the SHOUGATU source; --time bounds search only. Captured traceback,
bounded whole calls in scratch helper, no repository tooling changes.
The corrected accidental unscoped duplicate scan is documented in
[[matching-notes]]; the copy and unrelated generated declarations were
fully restored and excluded from counts.

## Inline review: Standards

Reviewed the uncommitted diff against HEAD
b1e5c6df17f1bc24e3dd32b7f0acd09ce50ed524 with
CODING_STANDARDS.md, CONTRIBUTING.md and the local code-review skill.
Resolved findings: removed unused RPG local; conventional case 4/5 layout
and buffer loop; fixed indentation; documented unsigned halfword and
byte-stride views; pruned all rejected experimental declarations; generated
the missing main helper declaration through sync_protos.py.

C89, fixed-width types, documented game layout, original function order,
guarded header additions and canonical main declarations checked. No SDK
redefinitions, new overrides, compiler flags, generated-asm edits, tooling
changes or copyrighted assets. Matching requirements override generic
refactoring smells for the explicit repeated record stores.

Retained FAKE forms: ETC func_8013F498's u16 halfword view prevents constant
sharing; RPG_BAT func_8013FF28 groups case 6 for store scheduling; TACO
func_80156244 retains mixed pointer spelling and a byte-stride view for addu
operand order; ETC func_8013F77C uses a marked legacy main-BSS record view
and loop-header store scheduling. Each has its reason and T-9750 in source.
The buffer helper's assembly proves 16 byte/halfword output entries.
No unresolved Standards findings.

## Inline review: Spec

The wave-9 brief and supplied work list are the spec. Four-hour minimum
exceeded; typed deep attempts and bounded searches recorded. Final source
scope is four assigned C files, required ETC/RPG_BAT append blocks and one
generated main_api.h declaration, plus own wiki/ticket changes. All rejected
or accidental out-of-scope edits restored. No commits, stash, reset,
checkout, merge, push, external messages or reviewer agents.
No unresolved Spec findings.

## Final validation

- Fresh full ninja after the generated prototype: 27/27 SHA1 OK.
- All seven final funcdiff --resolve checks: MATCH.
- check_headers.py: headers OK; migrate_globals.py --check: globals OK.
- sync_protos.py --check-branch: 0 new disagreements, 17 known; branch check OK.
- Explicit validated 36-file duplicate scan: applied 0 of 0.
- git diff --check and scope audit pass; all changes remain uncommitted.
- ninja progress: grand total 4877/6958 779916/2279368 34.2%.

Scratch evidence is under scratchpad/w9-5: final-build-after-proto.log,
final-functions.log, final-protos.log, final-guards-progress.log,
final-dupes.log and the phase/cleanup JSONL logs.
