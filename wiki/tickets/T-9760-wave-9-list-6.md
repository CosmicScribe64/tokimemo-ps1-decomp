---
id: T-9760
title: "Wave 9: list 6"
status: Done
assignee: w9-6
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Match as many of the 162 assigned functions (99,552 bytes) in 36 owned files as possible, following the wave-9 shared brief. Work for at least four hours, starting at 14:59 UTC on 2026-10-11.

## Acceptance criteria

- [x] Quick first pass followed by ranked deep attempts and aggregate repair.
- [x] Retained C verified by resolved function diffs and all 27 unit SHA1 checks.
- [x] Header/prototype and GameState migration checks pass.
- [x] Inline standards/spec review; all findings resolved.
- [x] No commits, tooling changes, or edits outside assigned scope.
- [x] At least four hours elapsed from 14:59:44 UTC.

## Notes

Work list: scratchpad/wave9-list-6.txt in the shared task directory. Scratch: scratchpad/w9-6/. Deadline for the requested minimum: 18:59:44 UTC. No reviewer subagents, per brief.

## Comments

2026-10-11 16:26 UTC checkpoint: five retained functions / 1,344 bytes. Full ninja completed successfully with all overlay and main SHA1 checks after the TT actor header append. TtActor68 now resides only at the end of TT.h; the existing header prefix is preserved. Continued aggregate repair produced a resolved match for TACO func_80152C60 (340 bytes), awaiting its unit build verification. Declared the assembly-proven 16-word menu color array D_800E3680 through sync_protos.py --add-global for the next main-file trials.

2026-10-11: Read wiki index, shared brief, local instructions, coding standards, decompilation workflow, T-1321/T-3330 notes, and register-allocation wave-7 evidence. Initial worktree is clean; build/ and asm/ are absent. Starting Docker baseline configuration/build.

2026-10-11 baseline: Docker configure plus ninja completed all 27 binary SHA1 checks. build/headers.ok and build/globals.ok exist. Scratch harness collects m2c drafts for all assigned functions, applies existing symbol renames and GameState migration mappings, then builds/diffs one draft at a time and restores failures. It retains only resolved matches whose whole unit SHA1 also passes.

2026-10-11 deep pass: first retained match is TT func_80133970, 240 bytes. The assembly compares old counter values; the generated draft tested the incremented ones. Natural post-increment do-loop conditions match, with resolved funcdiff and TT SHA1 verified. TACO func_801522B0 is six differing lines away after control-flow cleanup. Its first permutation setup failed because the host scratch path was not writable inside Docker; use an ephemeral /tmp mirror and export results to the approved host scratch directory.

2026-10-11 16:09 UTC: four retained matches, 1,120 bytes: TT func_80133970 (240), TACO func_801522B0 (288), BUNKA_SD func_80138CAC (156), RPG_BAT func_80138170 (436). The TACO byte mask and RPG unsigned lvalue view are marked FAKE. All four pass resolved diffs and individual unit SHA1. Genuine permutation work uses stdlib-preloaded worker invocation to avoid tools/queue.py shadowing Python queue; tooling remains untouched.

2026-10-11 17:06 UTC: TACO func_80152C60 passed its real-file resolved diff and whole-overlay SHA1. Six retained functions / 1,684 bytes. Subsequent typed table, palette and matrix trials are temporary and retained only after real build verification.

2026-10-11 17:15 UTC: seventh match, BUNKAKEN func_80145EBC (996 bytes), verified by resolved diff and BUNKAKEN SHA1. Natural GameState-pointer rewrite; batch total 2,680 bytes. No additional FAKE.

2026-10-11 18:19 UTC: eighth match TT func_80148460 (292 bytes), resolved function diff and overlay SHA1 verified. Batch total 2,972 bytes. Native DR_MODE cursor and appended 0x14/0x80B8/0x0C game layouts; two unused frame words explicitly marked FAKE. Existing headers preserved as prefixes.

2026-10-11 18:21 UTC: ninth match GYOZI func_80142D40 (176 bytes), real resolved diff and GYOZI SHA1 verified. Batch total 3,148 bytes. Native callback-table aggregate appended to GYOZI.h. FAKE selector read, xor-zero operand-order control and unsigned byte view are explicitly marked; remaining redundant xor cleanup follows.

2026-10-11 18:40 UTC inline review preparation: reviewed the retained C/header diff against CODING_STANDARDS.md, CONTRIBUTING.md, and the shared wave-9 brief, using the local code-review skill's Standards and Spec axes inline (the brief explicitly forbids reviewer subagents). Fixed TACO brace indentation and stray trial blank lines. SDK DR_MODE/TtOtTag are reused; game layouts have offsets/sizes; header additions preserve original prefixes. Unused generated declarations were removed, retaining only the needed code-address symbol. Final function diffs, full unit checks and four-hour minimum are still pending.

Tracked FAKEs: TACO func_801522B0 redundant byte mask; RPG_BAT func_80138170 unsigned index lvalue view; TT func_80134ADC empty duplicate flag comparison; TT func_80148460 two unused frame words; GYOZI func_80142D40 empty selector read, xor-zero callback operand, and unsigned byte view. Each has a reason and T-9760 in source. GYOZI's redundant condition-level xor was removed and formatting/name cleanup still passes its resolved diff.

2026-10-11 18:51 UTC verification: fresh resolved diffs pass for all nine retained functions (3,148 bytes), and full ninja completes 517 jobs with all 27 binary SHA1 checks OK. Fresh sync_protos --check-branch reports zero new disagreements (17 baseline-known); migrate_globals --check and check_headers report OK. build/headers.ok and build/globals.ok are present. Exact-twin apply adds zero; read-only current-C donor audit also yields zero plans. Progress: grand total 4879/6958, 779808/2279368, 34.2%.

| Unit | Function | Bytes |
| --- | --- | ---: |
| TT | func_80133970 | 240 |
| TACO | func_801522B0 | 288 |
| BUNKA_SD | func_80138CAC | 156 |
| RPG_BAT | func_80138170 | 436 |
| TT | func_80134ADC | 224 |
| TACO | func_80152C60 | 340 |
| BUNKAKEN | func_80145EBC | 996 |
| TT | func_80148460 | 292 |
| GYOZI | func_80142D40 | 176 |

Twelve T-0018 followup rows added after register_shapes audits and source iteration. Top other blocker: TAIIKU func_801339C4 needs its live a0 index parameter, but the existing void(void) overlay prototype cannot be changed under this batch's append-only header scope. Main menu_bar_color still needs a shared-$at BSS definition outside the object's permitted data range. Tool reports: permute's worker imports tools/queue.py instead of stdlib queue; strip_functions' broad regex can hang before timed search. Both were bypassed only in scratch launchers, with genuine 300-second searches logged. dupes identifies donors by asm folder placement, so a scratch read-only audit also checked the new C donors.

2026-10-11 18:55 UTC final inline review: Standards axis has no unresolved findings; every retained trick is marked/tracked, native SDK types are reused, recovered layouts are documented, and declaration guards pass. Spec axis passes the 36-file scope, all-162 first pass, ranked iterative/aggregate work, genuine timed near-miss searches, restored failures, wiki tracking, and uncommitted handoff. Only the explicit elapsed-time gate remains before Done. The final extra argument bit-field view fails (eight lines) and is discarded; SHOUGATU is byte-for-byte unchanged from HEAD and its post-restoration checksum passes.

Tool handoff details: multiprocessing fails importing Empty/Full from /work/tools/queue.py; the permutation wrapper can still return success/no-improvement. Preload stdlib queue before executing the worker as a scratch workaround. A faulthandler trace places the pre-search hang in tools/permute.py:200 re.findall inside strip_functions; it reproduces with SHOUGATU, GYOZI and BUNKA_SD full/isolated preprocessed sources. Scratch scanning with srcscan's ANSI/K&R definition regexes plus the existing function_range avoids that hang. Repository tools were not modified.

2026-10-11 19:00 UTC completion: the requested four-hour minimum has elapsed (started 14:59:44 UTC). Inline Standards and Spec review has no unresolved finding; nine functions / 3,148 bytes, all 27 binary checks, fresh resolved diffs and declaration guards pass. Twelve T-0018 rows added. Ticket, board and index move together from In Review to Done. All changes remain uncommitted for the coordinator; no outstanding batch-owned process or temporary source/declaration remains.
