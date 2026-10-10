---
id: T-7000
title: Shared constants, lui $at sharing and parameter copies
status: Done
assignee: r4-consts
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[toolchain]]", "[[data/shared-at-groups]]", "[[data/t0018-cases]]", "[[tickets/T-3001-shared-constant-registers]]", "[[tickets/T-5020-loop-unrolling-and-lui-sharing]]", "[[tickets/T-5010-t0018-register-order-second-attempt]]"]
---

## Goal

Find what decides three differences that blocked most of wave 4, and fix them with a uniform pass (CODING_STANDARDS 7a) or a documented C idiom:
- (a) a constant reused from a register across stores (or a `case` label and a store, or a store and a call argument) against a re-load per use;
- (b) one `lui $at` shared by adjacent global stores, re-measured now that T-5000/T-5100 recovered the aggregates;
- (c) parameter copies at entry: the original copies a `u8`/`arg0` parameter into another register or keeps it in its home slot, IDO does not.

## Acceptance criteria

- [x] A verdict per pattern, with evidence over all instances (matched controls and blocked functions).
- [x] Any pass: uniform, documented, unit-tested, changes no matched function (3721); any idiom: in [[decompile-workflow]], proven on 15+ blocked functions.
- [x] At least 15 functions matched across the patterns; rule logged in [[matching-notes]].

## Notes

Results in [[matching-notes]], "K&R promotion rules (T-7000)"; idioms in [[decompile-workflow]], "K&R promotion: types, constants and parameters"; build in [[toolchain]], "K&R mode".
- Finding: the original compiled with K&R promotion (unsigned char/short promote to unsigned int): on `lbu`/`lhu` values `sltiu` 1459 against `slti` 3, `srl` 556 against `sra` 0, `divu` 101 against `div` 4. IDO's `-cckr` gives this for plain C. IDO's K&R front end drops the widening `CVT` that the original kept for narrow globals (compare-operand order: narrow globals constant first 364 against 84, word globals and stack-slot locals variable first), so `tools/cvt_pass.py` puts it back before its other rules.
- Build change: `-cckr` in `tools/cc.py` and the widening step in `tools/cvt_pass.py` (tests in `tools/test_cc.py`, `tools/test_cvt_pass.py`). 14 matched functions written for ANSI promotion were adjusted with plain C, two callee declarations changed; every other C function builds as before.
- (c) parameter copies: K&R narrow parameter passed to a callee without prototype; solved (declare the callee `()`).
- (a) shared constants: K&R constant types (`li at,k` per compare across `u8`/`s32`) and chain assignment to aggregate elements; constants kept in `$t6`-style temporaries (41 functions) open.
- (b) shared `lui $at`: the original shares only for data defined in the sharing file; 221 matched functions (758 places) reach neighbours through one symbol where the original re-emits; needs per-object data (T-3052) before an as1 pass; none adopted.
- Matched 16 functions: ETC `func_80132048`, TACO `func_80136E70`, `func_8014EDCC`, TT `func_8013C764`, main `func_8007B5EC`, `SD_DetectCDPeak`, `set_kanji_string`, RPG_BAT `func_8014F1D4`, EVENT `func_80102C0C`, `func_8011A2C4`, `func_8011A4A4`, `func_80119600`, `func_80118764`, `func_80116360`, DATE `func_8013E5B0`, `func_8013BDB4`. Progress 3721 -> 3737 of 6958.

## Comments

- 2026-10-10 review (inline, CODING_STANDARDS checklist, diff 7c36126..HEAD): matches verified by the 27 sha1 checks of a clean build (`rm -rf asm build`, configure, ninja: 27/27 OK, headers OK) and a per-function object compare with the pre-change build (3524 C functions, the only differences are the 5 new C functions whose relocations name the struct base instead of splat's label); progress 3737/6958. No `NON_MATCHING`. C89, `/* */` comments, fixed-width types. Fakematches: the GYOZI `func_80135D64` `FAKE` keeps its comment (updated to the new local copy) and main `func_80057640` keeps its `FAKE` cast; no new tricks besides those. `set_kanji_string` writes the record array (`((Entry8 *)D_800B3DC0)[i].unk_07`), not the `D_800B3DC7` field label. Main-exe declarations: `set_kanji_string` prototype updated to the definition, `func_80042878()` K&R (ETC `func_80132048` forwards a `u8` to it, which the original compiled without prototype), `s32 func_8009AD70();`, `D_80126080` with a comment; `sync_protos.py --check-branch` OK, `--write` leaves the file unchanged; `migrate_globals.py --check` OK. Tooling (7a): the widening step is uniform (every narrow unsigned global load, no per-function control), documented (module docstring, [[toolchain]], [[matching-notes]]), evidence-backed (compare-order statistics and the 440-file ucode comparison), cannot produce invalid ucode (it inserts a fixed record; unknown ucode still aborts in `parse`), tested (`Widen` class, an IDO integration test), and replaceable (the C is plain). `-cckr` is a build-wide flag, documented with its evidence. All tool tests pass. Findings: `tools/cc.py` module docstring did not mention `-cckr` (fixed). No open findings.
