---
id: T-0014
title: Find the exact MIPS ucode compiler (frame +16, address CSE)
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[toolchain]]", "[[tickets/T-0013-identify-original-compiler-pipeline]]"]
---

## Goal

IDO 5.3 matches leaf game functions, but the original compiler allocates 16 more stack bytes in every non-leaf frame and CSEs global addresses less (`func_80042400`). Find the compiler (or option) that reproduces both, so non-leaf functions become matchable.

## Acceptance criteria

- [x] Candidates tested (older MIPS/SGI ucode compilers such as IRIX 4 IDO, Ultrix 4.x or NEWS-OS `cc`; IDO 5.3 ugen/uopt options) with results recorded in [[matching-notes]]. All cfe/uopt/ugen options of IDO 5.3 (and the main ones on 7.1) tried; older compilers are not available as static recompilations, emulation moved to [[tickets/T-0100-older-mips-compiler-emulation]].
- [x] Either `func_80042400` and one non-leaf function (e.g. `func_80041584`) byte-match, or the evidence that no available compiler does. Split result: `func_80042400` matches (`-Wo,-no_const_in_reg`); for non-leaf frames, the evidence that no available compiler matches is in [[matching-notes]] ("Frame size").

## Notes

Evidence and hypotheses: [[matching-notes]] ("Global-address CSE", "Frame size"). Do not fake the frame size.

## Comments

- 2026-10-09: CSE solved. uopt's undocumented `-no_const_in_reg` (from its option table) reproduces the original's per-access `%hi/%lo` reloads. Added `-Wo,-no_const_in_reg` to `IDO_CFLAGS` in `tools/cc.py`; matched `func_80042400` (`s32 t = D + 0x377; D = t; return t;`). Verified in Docker: `ninja` sha1 OK; `funcdiff.py` MATCH for all 23 C functions (the 22 earlier ones plus `func_80042400`).
- 2026-10-09: Frame not solved. Measured layout rules (non-leaf: +16 between saves and locals; leaf with a frame: +16 below the saves, as if the leaf kept a 16-byte arg build area; frameless leaves unchanged; the bytes are never accessed). No cfe/uopt/ugen option of IDO 5.3 or 7.1 changes it (list in [[matching-notes]]). Diagnostic: adding 16 to the ucode `DEF Mmt` length between uopt and ugen makes `func_80041584` and `func_8004111C` byte-match (rest of the codegen is plain IDO 5.3), but not leaf functions; documented as a fakematch fallback only, not adopted. No static recompilation of an older MIPS/IDO compiler exists; emulating one needs an OS image download, which needs a user decision: [[tickets/T-0100-older-mips-compiler-emulation]]. No non-leaf function matched, so none added to `src/`.
- 2026-10-09: Found that the first `splat split` in a fresh worktree overwrites `include/include_asm.h`; workaround in [[build-system]], fix in [[tickets/T-0101-splat-overwrites-include-asm-h]].
