---
id: T-0013
title: Identify the original compiler pipeline
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[tickets/T-0011-game-code-file-boundaries-and-compiler]]"]
---

## Goal

Find a compiler/assembler pipeline that reproduces the original codegen. gcc 2.6 to 2.95 plus maspsx cannot match even trivial setters, see [[matching-notes]].

## Acceptance criteria

- [x] Hypothesis tested: original used something other than gcc+ASPSX as modelled by maspsx (`$t6`-first allocation, `$at` store expansion with delay-slot fill, `lh` copies, `or` for move).
- [x] A pipeline (compiler, flags, assembler step) found that byte-matches the unmatched examples in [[matching-notes]], or the evidence that none is available. (IDO 5.3 + asm-processor: 7 of 8; the 8th and all non-leaf frames need a compiler we do not have, evidence in [[matching-notes]].)
- [x] [[toolchain]] updated with the result.

## Notes

Look at more complex functions (loops, switch, saved registers, stack frames) for further compiler signatures before choosing candidates.

## Comments

- 2026-10-09: Verdict: IDO-family MIPS ucode compiler. IDO 5.3 (`-EL -O2 -mips1 -G 0 -non_shared`) reproduces `$t6`-first temps, `$at` stores with filled `jr` slot, `lh` copies, `or` move, IDO prologue order. Added IDO 5.3/7.1 (ido-static-recomp v1.2) and asm-processor f3b2f85 to `tools/Dockerfile`; `configure.py` picks the toolchain per C file, `tools/cc.py` gains an `ido` mode; `src/game.c` builds with IDO 5.3. Verified in Docker: `ninja` sha1 OK; `funcdiff.py` MATCH for `func_800438DC`, `func_800451E0`, `func_80046284`, `func_80047550`, `func_8004902C`, `func_8004E99C`, `func_8004EA98`, `func_8004E750`, `func_8004E93C` and the 13 earlier functions.
- Open: `func_80042400` (less address CSE in the original) and every non-leaf function (original frames 16 bytes larger than IDO's). Hypotheses and next experiments in [[matching-notes]]. Follow-up ticket suggested for finding the exact (older) compiler.
