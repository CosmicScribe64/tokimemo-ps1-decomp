---
id: T-0013
title: Identify the original compiler pipeline
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[tickets/T-0011-game-code-file-boundaries-and-compiler]]"]
---

## Goal

Find a compiler/assembler pipeline that reproduces the original codegen. gcc 2.6 to 2.95 plus maspsx cannot match even trivial setters, see [[matching-notes]].

## Acceptance criteria

- [x] Hypothesis tested: original used something other than gcc+ASPSX as modelled by maspsx (`$t6`-first allocation, `$at` store expansion with delay-slot fill, `lh` copies, `or` for move).
- [x] A pipeline (compiler, flags, assembler step) found that byte-matches the unmatched examples in [[matching-notes]], or the evidence that none is available. Partial: IDO 5.3 + asm-processor matches 7 of 8; for `func_80042400` and non-leaf frames, the evidence that no available compiler matches is in [[matching-notes]], follow-up [[tickets/T-0014-find-exact-ucode-compiler]].
- [x] [[toolchain]] updated with the result.

## Notes

Look at more complex functions (loops, switch, saved registers, stack frames) for further compiler signatures before choosing candidates.

## Comments

- 2026-10-09: Verdict: IDO-family MIPS ucode compiler. IDO 5.3 (`-EL -O2 -mips1 -G 0 -non_shared`) reproduces `$t6`-first temps, `$at` stores with filled `jr` slot, `lh` copies, `or` move, IDO prologue order. Added IDO 5.3/7.1 (ido-static-recomp v1.2) and asm-processor f3b2f85 to `tools/Dockerfile`; `configure.py` picks the toolchain per C file, `tools/cc.py` gains an `ido` mode; `src/game.c` builds with IDO 5.3. Verified in Docker: `ninja` sha1 OK; `funcdiff.py` MATCH for `func_800438DC`, `func_800451E0`, `func_80046284`, `func_80047550`, `func_8004902C`, `func_8004E99C`, `func_8004EA98`, `func_8004E750`, `func_8004E93C` and the 13 earlier functions.
- Open: `func_80042400` (less address CSE in the original) and every non-leaf function (original frames 16 bytes larger than IDO's). Hypotheses and next experiments in [[matching-notes]]. Follow-up: [[tickets/T-0014-find-exact-ucode-compiler]].
- 2026-10-09: code-review (Standards + Spec, since fa2f49e). Fixed: configure.py docstring line length; AC2 reworded as partial with evidence; follow-up ticket T-0014 created. Accepted: matched functions committed as two small batches (as in T-0011); IDO 7.1 kept in the image for comparison; per-file (not per-segment) toolchain map, equivalent while game is one C file; `-Xcpluscomm` is harmless (no `//` in `src/`). No unresolved findings.
