---
id: T-0100
title: Run an older MIPS ucode compiler to reproduce the +16 stack frame
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[toolchain]]", "[[tickets/T-0014-find-exact-ucode-compiler]]"]
---

## Goal

Every stack frame in the original is 16 bytes larger than IDO 5.3 produces (rules in [[matching-notes]], "Frame size"), and no IDO 5.3/7.1 option changes it. Test the older MIPS ucode compilers that may have built the game, so non-leaf functions become matchable without fakes.

## Acceptance criteria

- [ ] Decision from the user on obtaining an OS image or compiler binaries (Ultrix 4.x for gxemul, IRIX 5.x IDO for qemu-irix, Sony NEWS-OS), since this needs a download of third-party software.
- [ ] Chosen compiler runs in Docker (pinned emulator version) and compiles `func_80041584`, `SD_DetectCDPeak`, `strSync`, `func_80042400` with `-EL -O2 -G 0`; results recorded in [[matching-notes]].
- [ ] If one matches all four: integrate in `tools/cc.py`/`configure.py`/`tools/Dockerfile`, re-check every matched function and the sha1.

## Notes

Ranked candidates and the diagnostic ucode experiment: [[matching-notes]]. decompals/ido-static-recomp ships only IDO 5.3 and 7.1; no other static recompilation was found (2026-10-09). The leaf-function pattern (16 bytes below the register saves) looks like an ugen without IDO's leaf-frame optimization, which points at an older ugen.

## Comments
- 2026-10-09 (T-0016): the frame difference is now reproduced by `tools/frame_pass.py` ([[tickets/T-0016-frame-layout-emulation-pass]], [[toolchain]]), so framed functions are matchable. This ticket stays open: a copy of the real compiler would replace the pass without changing any C, and would also settle the other differences listed in [[matching-notes]] (constant hoisting in loops, ugen temporary order). The four named functions are no longer the test: use the evidence table in [[matching-notes]].
