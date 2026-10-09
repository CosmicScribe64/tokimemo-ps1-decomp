---
id: T-0016
title: Frame-layout emulation pass
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[toolchain]]", "[[compiler-mismatch-research]]", "[[tickets/T-0014-find-exact-ucode-compiler]]", "[[tickets/T-0100-older-mips-compiler-emulation]]", "[[tickets/T-0015-research-compiler-mismatch-handling]]"]
---

## Goal

The original's stack frames are 16 bytes larger than IDO 5.3's (non-leaf: unused 16 bytes between register saves and locals; leaf with a frame: below the saves). Model this as one documented, uniform build step in the toolchain (a toolchain emulation pass, in the spirit of maspsx), with no per-function switches, so framed functions become matchable with ordinary C. User chose option 1 of [[compiler-mismatch-research]].

## Acceptance criteria

- [x] Rule derived from the original code: 3312 framed functions (main segment and 26 overlays), 33-row sample table, exceptions investigated; the one exception (`func_801488F0`) is a gcc-compiled function, explained by its prologue/epilogue shape (see [[matching-notes]]).
- [x] Pass in its own module (`tools/frame_pass.py`) wired into `tools/cc.py`; deterministic, fails loudly; 19 unit tests in Docker (synthetic binasm plus snippets compiled by the real IDO).
- [~] Proof: all 63 earlier matches still match, 12 non-leaf functions newly matched (need 5), 27/27 sha1 OK after a clean rebuild, `ninja progress` 75/834. Framed leaf functions (need 2): 3 match byte for byte in a diagnostic compile without `-Wo,-no_const_in_reg` (`func_8013815C`, `func_801446A0`, `func_80146FA0`) but not under the project flag, so they are `NON_MATCHING` in `src/ovl/TAIIKU.c`; the flag decision is [[tickets/T-0017-const-in-reg-loop-hoisting]]. Not met in the default build.
- [x] [[toolchain]], [[matching-notes]], `CODING_STANDARDS.md` (section 7a), README updated.
- [x] code-review gate (recorded in Comments); [[tickets/T-0100-older-mips-compiler-emulation]] stays open with a note.

## Notes

Constraint: one rule for every function; anything per-function is a fakematch.

Design (decided while working): the layer is IDO's binasm stream between ugen and as1, via an `as1` shim under `USR_LIB`. Rejected: `cc -S` text (reassembly changes the code), ucode `DEF Mmt` +16 (cannot do leaves; hole misplaced with spill temporaries). Findings that are not the frame: [[tickets/T-0017-const-in-reg-loop-hoisting]], [[tickets/T-0018-ugen-temp-register-order]].

## Comments
- 2026-10-09 review (standards agent): FAKE items tracked here: `v[6]` in the three TAIIKU leaf functions (`src/ovl/TAIIKU.c`), `u8 buf[0x20]` in `func_800462C8`/`func_80046318` (size from the original frame). Standards findings otherwise: none hard besides these markers. The spec-axis review had not returned when this was handed back; ticket stays In Review, not Done (framed-leaf criterion unmet in the default build, see T-0017).
- 2026-10-09 review (spec agent): framed-leaf shortfall honestly recorded. Fixed: a tainted `$sp`-derived register stored as a value now fails (test added). Open, accepted: unknown binasm itypes other than ifile/ioption pass through; leaf evidence is 24 original functions; IdoIntegration tests skip without IDO. Ticket stays In Review pending the T-0017 decision.
